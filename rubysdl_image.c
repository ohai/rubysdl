/*
  Ruby/SDL   Ruby extension library for SDL

  Copyright (C) 2001-2004 Ohbayashi Ippei
  
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
#ifdef HAVE_SDL_IMAGE
#include "rubysdl.h"
#include <SDL_image.h>

static VALUE Surface_s_load(VALUE klass, VALUE filename)
{
  SDL_Surface *surface;
  
  rb_secure(4);
  SafeStringValue(filename);

  surface = IMG_Load(RSTRING(filename)->ptr);
  if(surface == NULL)
    rb_raise(eSDLError,"Couldn't load %s: %s",
             RSTRING(filename)->ptr, SDL_GetError());
  return Surface_create(surface);
}

void init_sdl_image()
{
  rb_define_singleton_method(cSurface, "load", Surface_s_load, 1);
}
#endif /* HAVE_SDL_IMAGE */
