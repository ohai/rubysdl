#ifdef HAVE_SGE

#include "rubysdl.h"
#include <sge.h>

#if 0
static VALUE sdl_get_autoLocking(VALUE mod);
stativ sdl_set_autoLocking(VALUE mod,VALUE bool);
#endif
     
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
  sge_PutPixel(surface,NUM2INT(x),NUM2INT(y),NUM2UINT(color));
  return Qnil;
}
static VALUE sdl_drawLine(VALUE obj,VALUE x1,VALUE y1,VALUE x2,VALUE y2,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_Line( surface,NUM2INT(x1),NUM2INT(y1),NUM2INT(x2),NUM2INT(x2),
	    NUM2UINT(color) );
  return Qnil;
}
static VALUE sdl_drawRect(VALUE obj,VALUE x,VALUE y,VALUE w,VALUE h,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_Rect( surface,NUM2INT(x),NUM2INT(y),NUM2INT(x)+NUM2INT(w),
	    NUM2INT(y)+NUM2INT(h),NUM2UINT(color) );
  return Qnil;
}
static VALUE sdl_drawCircle(VALUE obj,VALUE x,VALUE y,VALUE r,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_Circle( surface,NUM2INT(x),NUM2INT(y),NUM2INT(r),NUM2UINT(color) );
  return Qnil;
}
static VALUE sdl_drawFilledCircle(VALUE obj,VALUE x,VALUE y,VALUE r,VALUE color)
{
  SDL_Surface *surface;
  Data_Get_Struct(obj,SDL_Surface,surface);
  sge_FilledCircle( surface,NUM2INT(x),NUM2INT(y),NUM2INT(r),NUM2UINT(color) );
  return Qnil;
}

static VALUE sdl_rotateScaledSurface(VALUE obj,VALUE angle,VALUE scale,VALUE bgcolor)
{
  SDL_Surface *surface,*result;
  Data_Get_Struct(obj,SDL_Surface,surface);
  result=sge_rotate_scaled_surface(surface,NUM2INT(angle),NUM2DBL(scale),
			    NUM2UINT(bgcolor));
  return Data_Wrap_Struct(cSurface,0,SDL_FreeSurface,result);
}
static VALUE sdl_rotateXYScaledBlit(VALUE mod,VALUE src,VALUE dst,VALUE x,
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

void init_sge_video()
{
  sge_Update_OFF();
  sge_Lock_ON();

#if 0
  rb_define_module_function(mSDL,"autoLock?",sdl_get_autoLocking,0);
  rb_define_module_function(mSDL,"autoLock?=",sdl_set_autoLocking,1);
#endif
  rb_define_method(cSurface,"getPixel",sdl_getPixel,2);
  rb_define_method(cSurface,"putPixel",sdl_putPixel,3);
  rb_define_method(cSurface,"[]",sdl_getPixel,2);
  rb_define_method(cSurface,"[]=",sdl_putPixel,3);

  rb_define_method(cSurface,"drawLine",sdl_drawLine,5);
  rb_define_method(cSurface,"drawRect",sdl_drawRect,5);
  rb_define_method(cSurface,"drawCircle",sdl_drawCircle,4);
  rb_define_method(cSurface,"drawFilledCircle",sdl_drawFilledCircle,4);

  rb_define_method(cSurface,"rotateScaledSurface",sdl_rotateScaledSurface,3);

  rb_define_module_function(mSDL,"rotateXYScaledBlit",sdl_rotateXYScaledBlit,7);
}
#endif /* HAVE_SGE */
