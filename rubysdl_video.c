#include "rubysdl.h"

#define SetRect(Rect,X,Y,W,H) \
do{ \
  Rect.x=NUM2INT(X); \
  Rect.y=NUM2INT(Y); \
  Rect.w=NUM2INT(W); \
  Rect.h=NUM2INT(H); \
}while(0) \

static VALUE sdl_warpMouse(VALUE mod,VALUE x,VALUE y)
{
  SDL_WarpMouse( NUM2UINT(x),NUM2UINT(y) );
  return Qnil;
}

static VALUE sdl_setVideoMode(VALUE class,VALUE w,VALUE h,VALUE bpp,
       VALUE flags)
{
  SDL_Surface *screen;
  screen=SDL_SetVideoMode(NUM2INT(w),NUM2INT(h),NUM2INT(bpp),
			  NUM2UINT(flags));
  if( screen==NULL ){
    rb_raise(eSDLError,"Cound't set %dx%d %d bpp video mode: %s",
	     NUM2INT(w),NUM2INT(h),NUM2INT(bpp),SDL_GetError());
  }
  return Data_Wrap_Struct(class,0,0,screen);
}

/* this method must be called after SDL::Surface::setVideoMode
   screen are given for getting information about mask and depth
   */
static VALUE sdl_createSurface(VALUE class,VALUE flags,VALUE w,VALUE h,
			       VALUE screen)
{
  SDL_Surface *scr,*newSurface;
  SDL_PixelFormat *pixFormat;
  if( !rb_obj_is_kind_of( screen,cScreen ) )
    rb_raise( rb_eArgError,"type mismatch(expect Screen)" );
  Data_Get_Struct(screen,SDL_Surface,scr);
  pixFormat = scr->format;
  newSurface = SDL_CreateRGBSurface( NUM2UINT(flags),NUM2INT(w),NUM2INT(h),
				     pixFormat->BitsPerPixel,
				     pixFormat->Rmask,pixFormat->Gmask,
				     pixFormat->Bmask,pixFormat->Amask );
  if( newSurface==NULL ){
    rb_raise( eSDLError,"Couldn't Create Surface: %s",SDL_GetError() );
  }
  return Data_Wrap_Struct(class,0,SDL_FreeSurface,newSurface);
}
				    
static VALUE sdl_loadBMP(VALUE class,VALUE filename)
{
  SDL_Surface *image;
  image=SDL_LoadBMP(STR2CSTR(filename));
  if( image==NULL ){
    rb_raise(eSDLError,"Couldn't Load BMP file %s : %s",
	     STR2CSTR(filename),SDL_GetError());
  }
  return Data_Wrap_Struct(class,0,SDL_FreeSurface,image);
}

static VALUE sdl_displayFormat(VALUE obj)
{
  SDL_Surface *srcImage,*destImage;
  Data_Get_Struct(obj,SDL_Surface,srcImage);
  destImage=SDL_DisplayFormat(srcImage);
  if( destImage==NULL ){
    rb_raise(eSDLError,"Couldn't convert surface format: %s",SDL_GetError());
  }
  return Data_Wrap_Struct(cSurface,0,SDL_FreeSurface,destImage);
}

static VALUE sdl_setColorKey(VALUE obj,VALUE flag,VALUE key)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  if( SDL_SetColorKey(surface,NUM2UINT(flag),NUM2UINT(key)) < 0 ){
    rb_raise(eSDLError,"setColorKey failed: %s",SDL_GetError());
  }
  return Qnil;
}

/* return 0 if succeed, return -2 video memory lost (on Direct X)*/
static VALUE sdl_blitSurface(VALUE obj,VALUE src,VALUE srcX,VALUE srcY,
			     VALUE srcW,VALUE srcH,VALUE dest,VALUE destX,
			     VALUE destY)
{
  SDL_Surface *srcSurface,*destSurface;
  SDL_Rect srcRect,destRect;
  int result;

  if( !rb_obj_is_kind_of(src,cSurface) ||
      !rb_obj_is_kind_of(dest,cSurface) ){
    rb_raise(rb_eArgError,"type mismatch");
  }
  Data_Get_Struct(src,SDL_Surface,srcSurface);
  Data_Get_Struct(dest,SDL_Surface,destSurface);
  SetRect(srcRect,srcX,srcY,srcW,srcH);
  SetRect(destRect,destX,destY,srcW,srcH);
  result = SDL_BlitSurface(srcSurface,&srcRect,destSurface,&destRect);
  if( result == -1 ){
    rb_raise(eSDLError,"SDL_BlitSurface fail: %s",SDL_GetError());
  }
  return INT2NUM(result);
}

static VALUE sdl_setAlpha(VALUE obj,VALUE flag,VALUE alpha)
{
  SDL_Surface *surface;

  Data_Get_Struct(obj,SDL_Surface,surface);
  SDL_SetAlpha(surface,NUM2UINT(flag),NUM2INT(alpha));
}
static VALUE sdl_setClipRect(VALUE obj,VALUE x,VALUE y,VALUE w,VALUE h)
{
  SDL_Surface *surface;
  SDL_Rect rect;

  Data_Get_Struct(obj,SDL_Surface,surface);
  SetRect(rect,x,y,w,h);
  SDL_SetClipRect(surface,&rect);
}
static VALUE sdl_updateRect(VALUE obj,VALUE x,VALUE y,VALUE w,VALUE h)
{
  SDL_Surface *screen;
  Data_Get_Struct(obj,SDL_Surface,screen);
  SDL_UpdateRect(screen,NUM2INT(x),NUM2INT(y),NUM2INT(w),NUM2INT(h));
  return Qnil;
}

static VALUE sdl_flip(VALUE obj)
{
  SDL_Surface *screen;
  Data_Get_Struct(obj,SDL_Surface,screen);
  if( SDL_Flip(screen) < 0 ){
    rb_raise( eSDLError,"flip fail : %s",SDL_GetError() );
  }
  return Qnil;
}

/* return ture if this fuction succeed, ohterwise return false */
static VALUE sdl_fillRect(VALUE obj,VALUE x,VALUE y,VALUE w,VALUE h,
		      VALUE color)
{
  SDL_Surface *surface;
  SDL_Rect rect;
  
  SetRect(rect,x,y,w,h);
  Data_Get_Struct(obj,SDL_Surface,surface);
  if( SDL_FillRect(surface,&rect,NUM2UINT(color)) < 0 ){
    rb_raise(eSDLError,"fillRect fail: %s",SDL_GetError());
  }
  return Qnil;
}

static VALUE sdl_surfaceH(VALUE obj)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  return INT2NUM( surface->h );
}
static VALUE sdl_surfaceW(VALUE obj)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  return INT2NUM( surface->w );
}
static void defineConstForVideo()
{
  /* Available for Screen.setVideoMode */
  rb_define_const(mSDL,"SWSURFACE",UINT2NUM(SDL_SWSURFACE));
  rb_define_const(mSDL,"HWSURFACW",UINT2NUM(SDL_HWSURFACE));
  rb_define_const(mSDL,"ASYNCBLIT",UINT2NUM(SDL_ASYNCBLIT));
  rb_define_const(mSDL,"ANYFORMAT",UINT2NUM(SDL_ANYFORMAT));
  rb_define_const(mSDL,"HWPALETTE",UINT2NUM(SDL_HWPALETTE));
  rb_define_const(mSDL,"DOUBLEBUF",UINT2NUM(SDL_DOUBLEBUF));
  rb_define_const(mSDL,"FULLSCREEN",UINT2NUM(SDL_FULLSCREEN));
  rb_define_const(mSDL,"OPENGL",UINT2NUM(SDL_OPENGL));
  rb_define_const(mSDL,"OPENGLBLIT",UINT2NUM(SDL_OPENGLBLIT));
  rb_define_const(mSDL,"RESIZABLE",UINT2NUM(SDL_RESIZABLE));
  rb_define_const(mSDL,"HWACCEL",UINT2NUM(SDL_HWACCEL));
  rb_define_const(mSDL,"SRCCOLORKEY",UINT2NUM(SDL_SRCCOLORKEY));
  rb_define_const(mSDL,"RLEACCELOK",UINT2NUM(SDL_RLEACCELOK));
  rb_define_const(mSDL,"RLEACCEL",UINT2NUM(SDL_RLEACCEL));
  rb_define_const(mSDL,"SRCALPHA",UINT2NUM(SDL_SRCALPHA));
  rb_define_const(mSDL,"PREALLOC",UINT2NUM(SDL_PREALLOC));
}

void init_video()
{
  rb_define_module_function(mSDL,"blitSurface",sdl_blitSurface,8);
  rb_define_module_function(mSDL,"warpMouse",sdl_warpMouse,2);
  
  cSurface = rb_define_class_under(mSDL,"Surface",rb_cObject);

  rb_define_singleton_method(cSurface,"new",sdl_createSurface,4);
  rb_define_singleton_method(cSurface,"loadBMP",sdl_loadBMP,1);
  rb_define_method(cSurface,"displayFormat",sdl_displayFormat,0);
  rb_define_method(cSurface,"setColorKey",sdl_setColorKey,2);
  rb_define_method(cSurface,"fillRect",sdl_fillRect,5);
  rb_define_method(cSurface,"setClipRect",sdl_setClipRect,4);
  rb_define_method(cSurface,"setAlpha",sdl_setAlpha,2);
  rb_define_method(cSurface,"h",sdl_surfaceH,0);
  rb_define_method(cSurface,"w",sdl_surfaceW,0);
  
  cScreen = rb_define_class_under(mSDL,"Screen",cSurface);
  rb_define_singleton_method(cScreen,"setVideoMode",sdl_setVideoMode,4);
  rb_define_method(cScreen,"updateRect",sdl_updateRect,4);
  rb_define_method(cScreen,"flip",sdl_flip,0);

  defineConstForVideo();
  return;
}
