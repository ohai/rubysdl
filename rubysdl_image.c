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
#ifdef HAVE_SDL_IMAGE
#include "rubysdl.h"
#include <SDL_image.h>

static VALUE sdl_load(VALUE class,VALUE filename)
{
  SDL_Surface *surface=IMG_Load(STR2CSTR(filename));
  return Data_Wrap_Struct(class,0,SDL_FreeSurface,surface);
}

void init_sdl_image()
{
  rb_define_singleton_method(cSurface,"load",sdl_load,1);
}
#endif /* HAVE_SDL_IMAGE */
