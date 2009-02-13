/*
  Ruby/SDL   Ruby extension library for SDL

  Copyright (C) 2001-2007 Ohbayashi Ippei
  
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

static VALUE WM_s_caption(VALUE mod)
{
  char *title, *icon;
  
  rb_secure(4);
  SDL_WM_GetCaption(&title, &icon);
#ifdef ENABLE_M17N
  return rb_ary_new3(2,
                     ENC_STR_NEW2(title, utf8_enc),
                     ENC_STR_NEW2(icon, utf8_enc));
#else
  return rb_ary_new3(2, rb_str_new2(title), rb_str_new2(icon));
#endif
}
static VALUE WM_s_setCaption(VALUE mod, VALUE title, VALUE icon)
{
  rb_secure(4);
  ExportStringValueToEnc(title, utf8_enc);
  ExportStringValueToEnc(icon, utf8_enc);
  SDL_WM_SetCaption(RSTRING_PTR(title), RSTRING_PTR(icon));
  return Qnil;
}
static VALUE WM_s_set_icon(VALUE mod, VALUE icon)
{
  rb_secure(4);
  SDL_WM_SetIcon(Get_SDL_Surface(icon), NULL);
  return Qnil;
}
static VALUE WM_s_iconify(VALUE mod)
{
  rb_secure(4);
  if( ! SDL_WM_IconifyWindow() )
    rb_raise( eSDLError,"iconify failed: %s",SDL_GetError() );
  return Qnil;
}

static VALUE WM_s_grabInput(VALUE mod, VALUE flag)
{
  rb_secure(4);
  return INT2FIX(SDL_WM_GrabInput(flag));
}

void rubysdl_init_WM(VALUE mSDL)
{
  VALUE mWM=rb_define_module_under(mSDL, "WM");
  rb_define_module_function(mWM, "caption", WM_s_caption, 0);
  rb_define_module_function(mWM, "setCaption", WM_s_setCaption, 2);
  rb_define_module_function(mWM, "icon=", WM_s_set_icon, 1);
  rb_define_module_function(mWM, "iconify", WM_s_iconify, 0);
  rb_define_module_function(mWM, "grabInput", WM_s_grabInput, 1);

  rb_define_const(mWM, "GRAB_QUERY", INT2NUM(SDL_GRAB_QUERY));
  rb_define_const(mWM, "GRAB_OFF", INT2NUM(SDL_GRAB_OFF));
  rb_define_const(mWM, "GRAB_ON", INT2NUM(SDL_GRAB_ON));

}
