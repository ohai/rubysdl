/*
  Ruby/SDL   Ruby extension library for SDL

  Copyright (C) 2001 Ohbayashi Ippei
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  */
#include "rubysdl.h"

Uint32 VALUE2COLOR(VALUE color,SDL_PixelFormat *format)
{
  if( rb_obj_is_kind_of( color, rb_cArray ) ){
    if( RARRAY(color)->len != 3 ){
      rb_raise(rb_eArgError,"type mismatch:color array needs 3 elements");
    }else{
      return SDL_MapRGB(format,
			NUM2UINT(rb_ary_entry(color,0)),
			NUM2UINT(rb_ary_entry(color,1)),
			NUM2UINT(rb_ary_entry(color,2)) );
    }
  }else{
    return NUM2UINT(color);
  }
}

static VALUE sdl_checkVideoMode(VALUE mod,VALUE w,VALUE h,VALUE bpp,
				VALUE flags)
{
  return INT2FIX( SDL_VideoModeOK(NUM2INT(w),NUM2INT(h),NUM2INT(bpp),
				  NUM2UINT(flags)) );
}
static VALUE sdl_getVideoInfo(VALUE mod)
{
  const SDL_VideoInfo *info;
  VALUE obj;
  info = SDL_GetVideoInfo();
  obj=rb_obj_alloc(cVideoInfo);
  rb_iv_set(obj,"@hw_available",BOOL(info->hw_available));
  rb_iv_set(obj,"@wm_available",BOOL(info->wm_available));
  rb_iv_set(obj,"@blit_hw",BOOL(info->blit_hw));
  rb_iv_set(obj,"@blit_hw_CC",BOOL(info->blit_hw_CC));
  rb_iv_set(obj,"@blit_hw_A",BOOL(info->blit_hw_A));
  rb_iv_set(obj,"@blit_sw",BOOL(info->blit_sw));
  rb_iv_set(obj,"@blit_sw_CC",BOOL(info->blit_sw_CC));
  rb_iv_set(obj,"@blit_sw_A",BOOL(info->blit_sw_A));
  rb_iv_set(obj,"@blit_fill",BOOL(info->blit_fill));
  rb_iv_set(obj,"@video_mem",UINT2NUM(info->video_mem));
  rb_iv_set(obj,"@vfmt",Data_Wrap_Struct(cPixelFormat,0,0,info->vfmt));
  return obj;
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

static VALUE sdl_setVideoMode(VALUE mod,VALUE w,VALUE h,VALUE bpp,
       VALUE flags)
{
  SDL_Surface *screen;
  VALUE screenObject;
  screen=SDL_SetVideoMode(NUM2INT(w),NUM2INT(h),NUM2INT(bpp),
			  NUM2UINT(flags));
  if( screen==NULL ){
    rb_raise(eSDLError,"Cound't set %dx%d %d bpp video mode: %s",
	     NUM2INT(w),NUM2INT(h),NUM2INT(bpp),SDL_GetError());
  }
  screenObject = Data_Wrap_Struct(cSurface,0,0,screen);
  rb_define_singleton_method(screenObject,"updateRect",sdl_updateRect,4);
  rb_define_singleton_method(screenObject,"flip",sdl_flip,0);
  return screenObject;
}


static VALUE sdl_createSurface(VALUE class,VALUE flags,VALUE w,VALUE h,
			       VALUE format)
{
  SDL_Surface *newSurface;
  SDL_PixelFormat *pixFormat;
  if( rb_obj_is_kind_of( format,cPixelFormat ) ){
    Data_Get_Struct(format,SDL_PixelFormat,pixFormat);
  }else{
    rb_raise( rb_eArgError,"type mismatch(expect PixelFormat)" );
  }
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
  if( SDL_SetColorKey(surface,NUM2UINT(flag),VALUE2COLOR(key,surface->format))
      < 0 ){
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

  if (NUM2INT(srcX)==0&&NUM2INT(srcY)==0&&NUM2INT(srcW)==0&&NUM2INT(srcH)==0){
    if (NUM2INT(destX)==0&&NUM2INT(destY)==0&&NUM2INT(srcW)==0&&NUM2INT(srcH)==0){
      result = SDL_BlitSurface(srcSurface,NULL,destSurface,NULL);
    }else{
      SetRect(destRect,destX,destY,srcW,srcH);
      result = SDL_BlitSurface(srcSurface,NULL,destSurface,&destRect);
    }
  }else{
    SetRect(srcRect,srcX,srcY,srcW,srcH);
    if (NUM2INT(destX)==0&&NUM2INT(destY)==0&&NUM2INT(srcW)==0&&NUM2INT(srcH)==0){
      result = SDL_BlitSurface(srcSurface,&srcRect,destSurface,NULL);
    }else{
      SetRect(destRect,destX,destY,srcW,srcH);
      result = SDL_BlitSurface(srcSurface,&srcRect,destSurface,&destRect);
    }
  }

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
static VALUE sdl_surface_format(VALUE obj)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  return Data_Wrap_Struct(cPixelFormat,0,0,surface->format);
}

/* return ture if this fuction succeed, ohterwise return false */
static VALUE sdl_fillRect(VALUE obj,VALUE x,VALUE y,VALUE w,VALUE h,
		      VALUE color)
{
  SDL_Surface *surface;
  SDL_Rect rect;
  
  SetRect(rect,x,y,w,h);
  Data_Get_Struct(obj,SDL_Surface,surface);
  if( SDL_FillRect(surface,&rect,VALUE2COLOR(color,surface->format)) < 0 ){
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

/* surface lock methods */
static VALUE sdl_mustlock(VALUE obj)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  return BOOL( SDL_MUSTLOCK(surface) );
}
static VALUE sdl_lockSurface(VALUE obj)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  return INT2FIX(SDL_LockSurface(surface));
}
static VALUE sdl_unlockSurface(VALUE obj)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  SDL_UnlockSurface(surface);
  return Qnil;
}
/* class PixelFormat methods */
static VALUE sdl_format_mapRGB(VALUE obj,VALUE r,VALUE g,VALUE b)
{
  SDL_PixelFormat *format;
  Data_Get_Struct(obj,SDL_PixelFormat,format);
  return UINT2NUM( SDL_MapRGB( format,NUM2INT(r),NUM2INT(g),NUM2INT(b) ) );
}
static VALUE sdl_format_mapRGBA(VALUE obj,VALUE r,VALUE g,VALUE b,VALUE a)
{
  SDL_PixelFormat *format;
  Data_Get_Struct(obj,SDL_PixelFormat,format);
  return UINT2NUM( SDL_MapRGBA( format,NUM2INT(r),NUM2INT(g),NUM2INT(b),
				NUM2INT(a) ) );
}
static VALUE sdl_format_getRGB(VALUE obj,VALUE pixel)
{
  SDL_PixelFormat *format;
  Uint8 r,g,b;
  Data_Get_Struct(obj,SDL_PixelFormat,format);
  SDL_GetRGB(NUM2UINT(pixel),format,&r,&g,&b);
  return rb_ary_new3( 3,UINT2NUM(r),UINT2NUM(g),UINT2NUM(b) );
}
static VALUE sdl_format_getRGBA(VALUE obj,VALUE pixel)
{
  SDL_PixelFormat *format;
  Uint8 r,g,b,a;
  Data_Get_Struct(obj,SDL_PixelFormat,format);
  SDL_GetRGBA(NUM2UINT(pixel),format,&r,&g,&b,&a);
  return rb_ary_new3( 4,UINT2NUM(r),UINT2NUM(g),UINT2NUM(b),UINT2NUM(a) );
}
static VALUE sdl_format_getBpp(VALUE obj)
{
  SDL_PixelFormat *format;
  Data_Get_Struct(obj,SDL_PixelFormat,format);
  return INT2FIX(format->BitsPerPixel);
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

  /*Transparency definitions: These define alpha as the opacity of a surface*/
  rb_define_const(mSDL,"ALPHA_OPAQUE",INT2NUM(SDL_ALPHA_OPAQUE));
  rb_define_const(mSDL,"ALPHA_TRANSPARENT",INT2NUM(SDL_ALPHA_TRANSPARENT));
}

void init_video()
{
  rb_define_module_function(mSDL,"blitSurface",sdl_blitSurface,8);
  rb_define_module_function(mSDL,"setVideoMode",sdl_setVideoMode,4);
  rb_define_module_function(mSDL,"checkVideoMode",sdl_checkVideoMode,4);
  cVideoInfo=rb_define_class_under(mSDL,"VideoInfo",rb_cObject);
  rb_define_attr(cVideoInfo,"hw_available",1,0);
  rb_define_attr(cVideoInfo,"wm_available",1,0);
  rb_define_attr(cVideoInfo,"blit_hw",1,0);
  rb_define_attr(cVideoInfo,"blit_hw_CC",1,0);
  rb_define_attr(cVideoInfo,"blit_hw_A",1,0);
  rb_define_attr(cVideoInfo,"blit_sw",1,0);
  rb_define_attr(cVideoInfo,"blit_sw_CC",1,0);
  rb_define_attr(cVideoInfo,"blit_sw_A",1,0);
  rb_define_attr(cVideoInfo,"blit_fill",1,0);
  rb_define_attr(cVideoInfo,"video_mem",1,0);
  rb_define_attr(cVideoInfo,"vfmt",1,0);

  rb_define_module_function(mSDL,"videoInfo",sdl_getVideoInfo,0);
  
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
  rb_define_method(cSurface,"format",sdl_surface_format,0);

  rb_define_method(cSurface,"mustLock?",sdl_mustlock,0);
  rb_define_method(cSurface,"lock",sdl_lockSurface,0);
  rb_define_method(cSurface,"unlock",sdl_unlockSurface,0);

  cPixelFormat = rb_define_class_under(mSDL,"PixelFormat",rb_cObject);
  rb_define_method(cPixelFormat,"mapRGB",sdl_format_mapRGB,3);
  rb_define_method(cPixelFormat,"mapRGBA",sdl_format_mapRGBA,4);
  rb_define_method(cPixelFormat,"getRGB",sdl_format_getRGB,3);
  rb_define_method(cPixelFormat,"getRGBA",sdl_format_getRGBA,4);
  rb_define_method(cPixelFormat,"bpp",sdl_format_getBpp,0);
  
  defineConstForVideo();
  return;
}
