/*
  Ruby/SDL   Ruby extention library for SDL

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
/* rubysdl.c -- SDL(Simple Directmedia Layer)
 */

#include "rubysdl.h"
#include <signal.h>
#include <stdio.h>
static void sdl_quit();
static VALUE sdl_init(VALUE obj,VALUE flags)
{
  Uint32 flag;
  
  rb_secure(1);
  flag= NUM2UINT(flags);
  if( SDL_Init(flag) < 0 )
    rb_raise(eSDLError,"Couldn't initialize SDL: %s",SDL_GetError());
  atexit(sdl_quit);
  return Qnil;
}

static VALUE sdl_wasInit(VALUE mod,VALUE flags)
{
  return UINT2NUM( SDL_WasInit(NUM2UINT(flags)) );
}

static void sdl_quit()
{
#ifdef HAVE_SDL_MIXER
  quit_mixer();
#endif
#ifdef HAVE_SDL_TTF
  quit_ttf();
#endif
  SDL_Quit();
  return ;
}

static void defineConst()
{
  rb_define_const(mSDL,"INIT_TIMER",UINT2NUM(SDL_INIT_TIMER));
  rb_define_const(mSDL,"INIT_AUDIO",UINT2NUM(SDL_INIT_AUDIO));
  rb_define_const(mSDL,"INIT_VIDEO",UINT2NUM(SDL_INIT_VIDEO));
  rb_define_const(mSDL,"INIT_CDROM",UINT2NUM(SDL_INIT_CDROM));
  rb_define_const(mSDL,"INIT_JOYSTICK",UINT2NUM(SDL_INIT_JOYSTICK));
  rb_define_const(mSDL,"INIT_NOPARACHUTE",UINT2NUM(SDL_INIT_NOPARACHUTE));
  rb_define_const(mSDL,"INIT_EVENTTHREAD",UINT2NUM(SDL_INIT_EVENTTHREAD));
  rb_define_const(mSDL,"INIT_EVERYTHING",UINT2NUM(SDL_INIT_EVERYTHING));
}


void Init_rubysdl()
{
  mSDL = rb_define_module("SDL");
  eSDLError = rb_define_class_under(mSDL,"Error",rb_eStandardError);
  rb_define_module_function(mSDL,"init",sdl_init,1);
  rb_define_module_function(mSDL,"initedSystem",sdl_wasInit,1);
  
  defineConst();
  
  init_video();
#ifdef HAVE_SGE
  init_sge_video();
#endif
#ifdef HAVE_SDL_IMAGE
  init_sdl_image();
#endif
  init_event();
  init_keyEvent();
  init_mouse();
  init_joystick();
  init_cdrom();
  init_time();
  init_wm();
#ifdef HAVE_SDL_TTF
  init_ttf();
#endif
#ifdef HAVE_SDL_MIXER
  init_mixer();
#endif

}


