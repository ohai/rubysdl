#include "rubysdl.h"

#define SetRect(Rect,X,Y,W,H) \
do{ \
  Rect.x=NUM2INT(X); \
  Rect.y=NUM2INT(Y); \
  Rect.w=NUM2INT(W); \
  Rect.h=NUM2INT(H); \
}while(0) \

static void dummy_free(void *p)
{
  return;
}
static VALUE sdl_setVideoMode(VALUE class,VALUE w,VALUE h,VALUE bpp,
       VALUE flags)
{
  SDL_Surface *screen;
  screen=SDL_SetVideoMode(NUM2INT(w),NUM2INT(h),NUM2INT(bpp),
			  NUM2INT(flags));
  return Data_Wrap_Struct(class,0,dummy_free,screen);
}

static VALUE sdl_loadBMP(VALUE class,VALUE filename)
{
  SDL_Surface *image;
  image=SDL_LoadBMP(STR2CSTR(filename));
  return Data_Wrap_Struct(class,0,SDL_FreeSurface,image);
}

static VALUE sdl_displayFormat(VALUE obj)
{
  SDL_Surface *srcImage,*destImage;
  Data_Get_Struct(obj,SDL_Surface,srcImage);
  destImage=SDL_DisplayFormat(srcImage);
  return Data_Wrap_Struct(gSurfaceClass,0,SDL_FreeSurface,destImage);
}

static VALUE sdl_setColorKey(VALUE obj,VALUE flag,VALUE key)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  return INT2FIX(SDL_SetColorKey(surface,NUM2INT(flag),NUM2INT(key)));
}

static VALUE sdl_blitSurface(VALUE obj,VALUE src,VALUE srcX,VALUE srcY,VALUE srcW,
			     VALUE srcH,VALUE dest,VALUE destX,
			     VALUE destY)
{
  SDL_Surface *srcSurface,*destSurface;
  SDL_Rect srcRect,destRect;
  
  Data_Get_Struct(src,SDL_Surface,srcSurface);
  Data_Get_Struct(dest,SDL_Surface,destSurface);
  SetRect(srcRect,srcX,srcY,srcW,srcH);
  SetRect(destRect,destX,destY,srcW,srcH);
  return INT2FIX(SDL_BlitSurface(srcSurface,&srcRect,destSurface,
				 &destRect));
}

static VALUE sdl_updateRect(VALUE obj,VALUE x,VALUE y,VALUE w,VALUE h)
{
  SDL_Surface *screen;
  Data_Get_Struct(obj,SDL_Surface,screen);
  SDL_UpdateRect(screen,NUM2INT(x),NUM2INT(y),NUM2INT(w),NUM2INT(h));
  return Qnil;
}

/* return ture if this fuction succeed, ohterwise return false */
static VALUE sdl_fillRect(VALUE obj,VALUE x,VALUE y,VALUE w,VALUE h,
		      VALUE color)
{
  SDL_Surface *surface;
  SDL_Rect rect;
  int result;
  
  SetRect(rect,x,y,w,h);
  Data_Get_Struct(obj,SDL_Surface,surface);
  result = SDL_FillRect(surface,&rect,NUM2INT(color));
  if( result == 0 )
    return Qtrue;
  else
    return Qfalse;
}

void defineConstForVideo()
{
  /* Available for Screen.setVideoMode */
  rb_define_const(gSDLModule,"SWSURFACE",INT2NUM(SDL_SWSURFACE));
  rb_define_const(gSDLModule,"HWSURFACW",INT2NUM(SDL_HWSURFACE));
  rb_define_const(gSDLModule,"ASYNCBLIT",INT2NUM(SDL_ASYNCBLIT));
  rb_define_const(gSDLModule,"ANYFORMAT",INT2NUM(SDL_ANYFORMAT));
  rb_define_const(gSDLModule,"HWPALETTE",INT2NUM(SDL_HWPALETTE));
  rb_define_const(gSDLModule,"DOUBLEBUF",INT2NUM(SDL_DOUBLEBUF));
  rb_define_const(gSDLModule,"FULLSCREEN",INT2NUM(SDL_FULLSCREEN));
  rb_define_const(gSDLModule,"OPENGL",INT2NUM(SDL_OPENGL));
  rb_define_const(gSDLModule,"OPENGLBLIT",INT2NUM(SDL_OPENGLBLIT));
  rb_define_const(gSDLModule,"RESIZABLE",INT2NUM(SDL_RESIZABLE));
  rb_define_const(gSDLModule,"HWACCEL",INT2NUM(SDL_HWACCEL));
  rb_define_const(gSDLModule,"SRCCOLORKEY",INT2NUM(SDL_SRCCOLORKEY));
  rb_define_const(gSDLModule,"RLEACCELOK",INT2NUM(SDL_RLEACCELOK));
  rb_define_const(gSDLModule,"RLEACCEL",INT2NUM(SDL_RLEACCEL));
  rb_define_const(gSDLModule,"SRCALPHA",INT2NUM(SDL_SRCALPHA));
  rb_define_const(gSDLModule,"PREALLOC",INT2NUM(SDL_PREALLOC));
}

void init_video()
{
  rb_define_module_function(gSDLModule,"blitSurface",sdl_blitSurface,8);

  gSurfaceClass = rb_define_class_under(gSDLModule,"Surface",rb_cObject);
  rb_define_singleton_method(gSurfaceClass,"loadBMP",sdl_loadBMP,1);
  rb_define_method(gSurfaceClass,"displayFormat",sdl_displayFormat,0);
  rb_define_method(gSurfaceClass,"setColorKey",sdl_setColorKey,2);
  rb_define_method(gSurfaceClass,"fillRect",sdl_fillRect,5);

  gScreenClass = rb_define_class_under(gSDLModule,"Screen",gSurfaceClass);
  rb_define_singleton_method(gScreenClass,"setVideoMode",
			     sdl_setVideoMode,4);
  rb_define_method(gScreenClass,"updateRect",sdl_updateRect,4);

  defineConstForVideo();
  return;
}
