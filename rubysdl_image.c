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
#ifdef HAVE_SDL_IMAGE
#include <SDL_image.h>

static VALUE Surface_s_load(VALUE klass, VALUE filename)
{
  SDL_Surface *surface;
  
  rb_secure(4);
  ExportFilenameStringValue(filename);

  surface = IMG_Load(RSTRING_PTR(filename));
  if(surface == NULL)
    rb_raise(eSDLError,"Couldn't load %s: %s",
             RSTRING_PTR(filename), SDL_GetError());
  return Surface_create(surface);
}
static VALUE Surface_s_loadFromIO(VALUE class,VALUE io)
{
  volatile VALUE guard = io;
  SDL_Surface *surface;
  
  surface = IMG_Load_RW(rubysdl_RWops_from_ruby_obj(io), 1);
  
  if(surface==NULL){
    rb_raise(eSDLError,"Couldn't load image from IO: %s",
	     SDL_GetError());
  }
  return Surface_create(surface);
}
static VALUE Surface_s_loadFromString(VALUE class,VALUE str)
{
  SDL_Surface *surface;
  rb_secure(4);
  SafeStringValue(str);
  
  surface = IMG_Load_RW(SDL_RWFromConstMem(RSTRING_PTR(str),
                                           RSTRING_LEN(str)),
                        1);
  
  if(surface==NULL){
    rb_raise(eSDLError,"Couldn't load image from String: %s",
	     SDL_GetError());
  }
  return Surface_create(surface);
}

void rubysdl_init_image(VALUE mSDL, VALUE cSurface)
{
  rb_define_singleton_method(cSurface, "load", Surface_s_load, 1);
  rb_define_singleton_method(cSurface, "loadFromIO", Surface_s_loadFromIO, 1);
  rb_define_singleton_method(cSurface, "loadFromString", Surface_s_loadFromString, 1);
}
#else /* HAVE_SDL_IMAGE */
void rubysdl_init_image(VALUE mSDL, VALUE cSurface)
{
}
#endif /* HAVE_SDL_IMAGE */
