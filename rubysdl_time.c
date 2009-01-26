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

static VALUE sdl_getTicks(VALUE mod)
{
  return UINT2NUM(SDL_GetTicks());
}

static VALUE delay(void* t)
{
  SDL_Delay(*((Uint32*)t));
  return Qnil;
}

static VALUE sdl_delay(VALUE mod,VALUE ms)
{
  Uint32 t = NUM2UINT(ms);
#ifdef HAVE_RB_THREAD_BLOCKING_REGION
  rb_thread_blocking_region(delay, &t, RUBY_UBF_IO, NULL);
#else
  SDL_Delay(t);
#endif
  return Qnil;
}

void rubysdl_init_time(VALUE mSDL)
{
  rb_define_module_function(mSDL,"getTicks",sdl_getTicks,0);
  rb_define_module_function(mSDL,"delay",sdl_delay,1);
}
