#include "rubysdl.h"

static VALUE sdl_wm_getCaption(VALUE mod)
{
  char *title,*icon;
  SDL_WM_GetCaption( &title,&icon );
  return rb_ary_new3( 2,rb_str_new2(title),rb_str_new2(icon) );
}
static VALUE sdl_wm_setCaption(VALUE mod,VALUE title,VALUE icon)
{
  SDL_WM_SetCaption( STR2CSTR(title),STR2CSTR(icon) );
  return Qnil;
}
static VALUE sdl_wm_setIcon(VALUE mod,VALUE icon)
{
  SDL_Surface *surface;
  if( ! rb_obj_is_kind_of(icon,cSurface) )
    rb_raise(rb_eArgError,"type mismatch (expected Surface)");
  Data_Get_Struct(icon,SDL_Surface,surface);
  SDL_WM_SetIcon(surface,NULL);
  return Qnil;
}
static VALUE sdl_wm_iconifyWindow(VALUE mod)
{
  if( ! SDL_IconifyWindow() )
    rb_raise( eSDLError,"iconify failed: %s",SDL_GetError() );
  return Qnil;
}

void init_wm()
{
  mWM=rb_define_module_under(mSDL,"WM");
  rb_define_module_function(mWM,"caption",sdl_wm_getCaption,0);
  rb_define_module_function(mWM,"setCaption",sdl_wm_setCaption,2);
  rb_define_module_function(mWM,"icon=",sdl_wm_setIcon,1);
  rb_define_module_function(mWM,"iconify",sdl_wm_iconifyWindow,1);
}
