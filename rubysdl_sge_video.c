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
#ifdef HAVE_SGE

#include "rubysdl.h"
#include <sge.h>

extern Uint8 _sge_lock;
static VALUE sdl_get_autoLocking(VALUE mod)
{
  return BOOL(_sge_lock);
}

static VALUE  sdl_set_autoLocking(VALUE mod,VALUE bool)
{
  if(RTEST(bool))
    sge_Lock_ON();
  else
    sge_Lock_OFF();
  return Qnil;
}

     
static VALUE sdl_getPixel(VALUE obj,VALUE x,VALUE y)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  return UINT2NUM( sge_GetPixel(surface,NUM2INT(x),NUM2INT(y)) );
}
static VALUE sdl_putPixel(VALUE obj,VALUE x,VALUE y,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_PutPixel(surface,NUM2INT(x),NUM2INT(y),VALUE2COLOR(color,surface->format));
  return Qnil;
}
static VALUE sdl_drawLine(VALUE obj,VALUE x1,VALUE y1,VALUE x2,VALUE y2,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_Line( surface,NUM2INT(x1),NUM2INT(y1),NUM2INT(x2),NUM2INT(y2),
	    VALUE2COLOR(color,surface->format) );
  return Qnil;
}
static VALUE sdl_drawRect(VALUE obj,VALUE x,VALUE y,VALUE w,VALUE h,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_Rect( surface,NUM2INT(x),NUM2INT(y),NUM2INT(x)+NUM2INT(w),
	    NUM2INT(y)+NUM2INT(h),VALUE2COLOR(color,surface->format) );
  return Qnil;
}
static VALUE sdl_drawCircle(VALUE obj,VALUE x,VALUE y,VALUE r,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_Circle( surface,NUM2INT(x),NUM2INT(y),NUM2INT(r),
	      VALUE2COLOR(color,surface->format) );
  return Qnil;
}
static VALUE sdl_drawFilledCircle(VALUE obj,VALUE x,VALUE y,VALUE r,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_FilledCircle( surface,NUM2INT(x),NUM2INT(y),NUM2INT(r),
		    VALUE2COLOR(color,surface->format) );
  return Qnil;
}
static VALUE sdl_drawEllipse(VALUE obj,VALUE x,VALUE y,VALUE rx,VALUE ry,
			     VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_Ellipse( surface,NUM2INT(x),NUM2INT(y),NUM2INT(rx),NUM2INT(ry),
	       VALUE2COLOR(color,surface->format) );
  return Qnil;
}
static VALUE sdl_drawFilledEllipse(VALUE obj,VALUE x,VALUE y,VALUE rx,VALUE ry,
				   VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_FilledEllipse( surface,NUM2INT(x),NUM2INT(y),NUM2INT(rx),NUM2INT(ry),
		     VALUE2COLOR(color,surface->format) );
  return Qnil;
}
static VALUE sdl_rotateScaledSurface(VALUE obj,VALUE angle,VALUE scale,VALUE bgcolor)
{
  SDL_Surface *surface,*result;
  Data_Get_Struct(obj,SDL_Surface,surface);
  result=sge_rotate_scaled_surface(surface,NUM2INT(angle),NUM2DBL(scale),
				   VALUE2COLOR(bgcolor,surface->format) );
  if( result==NULL )
    rb_raise( eSDLError,"Couldn't Create Surface: %s",SDL_GetError() );
  return Data_Wrap_Struct(cSurface,0,SDL_FreeSurface,result);
}
/* doesn't respect ColorKey */
static VALUE sdl_rotateXYScaled(VALUE mod,VALUE src,VALUE dst,VALUE x,
				VALUE y,VALUE angle,VALUE xscale,
				VALUE yscale)
{
  SDL_Surface *srcSurface,*dstSurface;
  if( !rb_obj_is_kind_of(src,cSurface) || !rb_obj_is_kind_of(dst,cSurface) )
    rb_raise(rb_eArgError,"type mismatch(expect Surface)");
  Data_Get_Struct(src,SDL_Surface,srcSurface);
  Data_Get_Struct(dst,SDL_Surface,dstSurface);
  sge_rotate_xyscaled(dstSurface,srcSurface,NUM2INT(x),NUM2INT(y),
		      NUM2INT(angle),NUM2DBL(xscale),NUM2DBL(yscale));
  return Qnil;
}
static VALUE sdl_rotateScaledBlit(VALUE mod,VALUE src,VALUE dst,VALUE x,
				  VALUE y,VALUE angle,VALUE scale)
{
  SDL_Surface *srcSurface,*dstSurface,*tmpSurface;
  SDL_Rect destRect;
  Uint32 colorkey;
  int result;
  
  if( !rb_obj_is_kind_of(src,cSurface) || !rb_obj_is_kind_of(dst,cSurface) )
    rb_raise(rb_eArgError,"type mismatch(expect Surface)");
  Data_Get_Struct(src,SDL_Surface,srcSurface);
  Data_Get_Struct(dst,SDL_Surface,dstSurface);
  colorkey=srcSurface->format->colorkey;
  tmpSurface = sge_rotate_scaled_surface(srcSurface,NUM2INT(angle),
					 NUM2DBL(scale),colorkey);
  if( tmpSurface==NULL )
    rb_raise(eSDLError,"SDL memory allocate failed :%s",SDL_GetError());
  SDL_SetColorKey(tmpSurface,SDL_SRCCOLORKEY|SDL_RLEACCEL,colorkey);
  destRect.x=NUM2INT(x)-tmpSurface->h/2;
  destRect.y=NUM2INT(y)-tmpSurface->w/2;
  result = SDL_BlitSurface(tmpSurface,NULL,dstSurface,&destRect);
  SDL_FreeSurface(tmpSurface);
  if( result == -1 ){
    rb_raise(eSDLError,"SDL_BlitSurface fail: %s",SDL_GetError());
  }
  return INT2NUM(result);
}
  
void init_sge_video()
{
  sge_Update_OFF();
#if 0
  sge_Lock_ON();
#endif

  rb_define_module_function(mSDL,"autoLock",sdl_get_autoLocking,0);
  rb_define_module_function(mSDL,"autoLock=",sdl_set_autoLocking,1);

  rb_define_method(cSurface,"getPixel",sdl_getPixel,2);
  rb_define_method(cSurface,"putPixel",sdl_putPixel,3);
  rb_define_method(cSurface,"[]",sdl_getPixel,2);
  rb_define_method(cSurface,"[]=",sdl_putPixel,3);

  rb_define_method(cSurface,"drawLine",sdl_drawLine,5);
  rb_define_method(cSurface,"drawRect",sdl_drawRect,5);
  rb_define_method(cSurface,"drawCircle",sdl_drawCircle,4);
  rb_define_method(cSurface,"drawFilledCircle",sdl_drawFilledCircle,4);
  rb_define_method(cSurface,"drawEllispe",sdl_drawEllipse,5);
  rb_define_method(cSurface,"drawFilledEllispe",sdl_drawFilledEllipse,5);
  
  rb_define_method(cSurface,"rotateScaledSurface",sdl_rotateScaledSurface,3);
  rb_define_module_function(mSDL,"rotateScaledBlit",sdl_rotateScaledBlit,6);
  rb_define_module_function(mSDL,"rotateXYScaled",sdl_rotateXYScaled,7);
}
#endif /* HAVE_SGE */
