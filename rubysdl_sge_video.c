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
  return UINT2NUM( sge_getPixel(surface,NUM2INT(x),NUM2INT(y)) );
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
}
#endif /* HAVE_SGE */
