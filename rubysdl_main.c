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

#define DEF_GLOBAL
#include "rubysdl.h"

static VALUE sdl_s_init(VALUE mod, VALUE flags)
{
  rb_secure(4);
  if( SDL_Init(NUM2UINT(flags)) < 0 )
    rb_raise(eSDLError, "Couldn't initialize SDL: %s", SDL_GetError());
  return Qnil;
}

static VALUE sdl_s_initSubSystem(VALUE mod, VALUE flags)
{
  if( SDL_InitSubSystem(NUM2UINT(flags)) < 0 )
    rb_raise(eSDLError, "Couldn't initialize SDL subsystem: %s", SDL_GetError());
  return Qnil;
}

static VALUE sdl_s_inited_system(VALUE mod, VALUE flags)
{
  return UINT2NUM(SDL_WasInit(NUM2UINT(flags)));
}

static VALUE sdl_s_putenv(VALUE mod, VALUE var)
{
  rb_secure(4);
  SafeStringValue(var);
  
  if( putenv(RSTRING_PTR(var)) < 0 ){
    rb_raise(eSDLError, "Can't put environ variable: %s",  RSTRING_PTR(var));
  }
  return Qnil;
}

static VALUE sdl_s_getenv(VALUE mod, VALUE name)
{
  char* result;
  SafeStringValue(name);
  result = getenv(RSTRING_PTR(name));
  if(result == NULL){
    rb_raise(eSDLError, "Can't get environ variable: %s",  RSTRING_PTR(name));
  }
  return rb_str_new2(result);
}

static int is_quit=0;
int rubysdl_is_quit(void)
{
  return is_quit;
}
static void sdl_quit(VALUE v)
{
  if(rubysdl_is_quit())
    return;
  rubysdl_quit_Mixer();
  rubysdl_quit_TTF();
  SDL_Quit();
  is_quit = 1;
}

static VALUE sdl_s_quit(VALUE obj)
{
  sdl_quit(0);
  return Qnil;
}

void Init_sdl_ext()
{
  VALUE mSDL = rb_define_module("SDL");
  VALUE cSurface;

#ifdef ENABLE_M17N
  utf8_enc = rb_utf8_encoding();
  eucjp_enc = rb_enc_find("EUC-JP");
  iso2022jp_enc = rb_enc_find("ISO-2022-JP");
  sjis_enc = rb_enc_find("SJIS");
#endif

  eSDLError = rb_define_class_under(mSDL, "Error", rb_eStandardError);
  rb_define_module_function(mSDL, "init", sdl_s_init, 1);
  rb_define_module_function(mSDL, "initedSystem", sdl_s_inited_system, 1);
  rb_define_module_function(mSDL, "initSubSystem", sdl_s_initSubSystem, 1);
  rb_define_module_function(mSDL, "quit", sdl_s_quit, 0);
  rb_define_module_function(mSDL, "putenv", sdl_s_putenv, 1);
  rb_define_module_function(mSDL, "getenv", sdl_s_getenv, 1);
  
  rb_define_const(mSDL, "INIT_TIMER", UINT2NUM(SDL_INIT_TIMER));
  rb_define_const(mSDL, "INIT_AUDIO", UINT2NUM(SDL_INIT_AUDIO));
  rb_define_const(mSDL, "INIT_VIDEO", UINT2NUM(SDL_INIT_VIDEO));
  rb_define_const(mSDL, "INIT_CDROM", UINT2NUM(SDL_INIT_CDROM));
  rb_define_const(mSDL, "INIT_JOYSTICK", UINT2NUM(SDL_INIT_JOYSTICK));
  rb_define_const(mSDL, "INIT_NOPARACHUTE", UINT2NUM(SDL_INIT_NOPARACHUTE));
  rb_define_const(mSDL, "INIT_EVENTTHREAD", UINT2NUM(SDL_INIT_EVENTTHREAD));
  rb_define_const(mSDL, "INIT_EVERYTHING", UINT2NUM(SDL_INIT_EVERYTHING));


  cSurface = rubysdl_init_video(mSDL);
  rubysdl_init_sge(mSDL, cSurface);
  rubysdl_init_GL(mSDL);
  rubysdl_init_image(mSDL, cSurface);
  rubysdl_init_Event(mSDL);
  rubysdl_init_Key(mSDL);
  rubysdl_init_Mouse(mSDL);
  rubysdl_init_Joystick(mSDL);
  rubysdl_init_CD(mSDL);
  rubysdl_init_time(mSDL);
  rubysdl_init_WM(mSDL);
  rubysdl_init_Kanji(mSDL);
  rubysdl_init_TTF(mSDL);
  rubysdl_init_Mixer(mSDL);
  rubysdl_init_MPEG(mSDL);
  rubysdl_init_SKK(mSDL);
  rb_set_end_proc(sdl_quit, 0); 
}


