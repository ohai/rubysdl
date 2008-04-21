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
#ifdef ENABLE_OPENGL
static VALUE GL_getAttr(VALUE class, VALUE attr)
{
  int val;
  if(SDL_GL_GetAttribute(NUM2INT(attr), &val)==-1)
    rb_raise(eSDLError, "GL get attribute failed: %s", SDL_GetError());
  return INT2NUM(val);
}
static VALUE GL_setAttr(VALUE mod, VALUE attr, VALUE val)
{
  if(SDL_GL_SetAttribute(NUM2INT(attr), NUM2INT(val))==-1)
    rb_raise(eSDLError, "GL set attribute failed: %s", SDL_GetError());
  return Qnil;
}
static VALUE GL_swapBuffers(VALUE mod)
{
  SDL_GL_SwapBuffers();
  return Qnil;
}
		  
void rubysdl_init_GL(VALUE mSDL)
{
  VALUE mGL;
  mGL = rb_define_module_under(mSDL, "GL");
  rb_define_module_function(mGL, "getAttr", GL_getAttr, 1);
  rb_define_module_function(mGL, "setAttr", GL_setAttr, 2);
  rb_define_module_function(mGL, "swapBuffers", GL_swapBuffers, 0);
  
  rb_define_const(mGL, "RED_SIZE", INT2NUM(SDL_GL_RED_SIZE));
  rb_define_const(mGL, "GREEN_SIZE", INT2NUM(SDL_GL_GREEN_SIZE));
  rb_define_const(mGL, "BLUE_SIZE", INT2NUM(SDL_GL_BLUE_SIZE));
  rb_define_const(mGL, "ALPHA_SIZE", INT2NUM(SDL_GL_ALPHA_SIZE));
  rb_define_const(mGL, "BUFFER_SIZE", INT2NUM(SDL_GL_BUFFER_SIZE));
  rb_define_const(mGL, "DOUBLEBUFFER", INT2NUM(SDL_GL_DOUBLEBUFFER));
  rb_define_const(mGL, "DEPTH_SIZE", INT2NUM(SDL_GL_DEPTH_SIZE));
  rb_define_const(mGL, "STENCIL_SIZE", INT2NUM(SDL_GL_STENCIL_SIZE));
  rb_define_const(mGL, "ACCUM_RED_SIZE", INT2NUM(SDL_GL_ACCUM_RED_SIZE));
  rb_define_const(mGL, "ACCUM_GREEN_SIZE", INT2NUM(SDL_GL_ACCUM_GREEN_SIZE));
  rb_define_const(mGL, "ACCUM_BLUE_SIZE", INT2NUM(SDL_GL_ACCUM_BLUE_SIZE));
  rb_define_const(mGL, "ACCUM_ALPHA_SIZE", INT2NUM(SDL_GL_ACCUM_ALPHA_SIZE));
}
#else /* ENABLE_OPENGL */
void rubysdl_init_GL(VALUE mSDL)
{
}
#endif /* ENABLE_OPENGL */
