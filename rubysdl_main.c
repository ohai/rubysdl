/* rubysdl.c -- SDL(Simple Directmedia Layer)
 */

#include "rubysdl.h"
#include <signal.h>
#include <stdio.h>
static VALUE sdl_init(VALUE obj,VALUE flags)
{
  Uint32 flag;

  flag= NUM2INT(flags);
  /* flag = flag & ~(SDL_INIT_TIMER | SDL_INIT_EVENTTHREAD); */
  return INT2FIX(SDL_Init(flag)); 
}

static VALUE sdl_quit(VALUE obj)
{
  SDL_Quit();
  return Qnil;
}

static void defineConst()
{
  rb_define_const(mSDL,"INIT_TIMER",INT2NUM(SDL_INIT_TIMER));
  rb_define_const(mSDL,"INIT_AUDIO",INT2NUM(SDL_INIT_AUDIO));
  rb_define_const(mSDL,"INIT_VIDEO",INT2NUM(SDL_INIT_VIDEO));
  rb_define_const(mSDL,"INIT_CDROM",INT2NUM(SDL_INIT_CDROM));
  rb_define_const(mSDL,"INIT_JOYSTICK",INT2NUM(SDL_INIT_JOYSTICK));
  rb_define_const(mSDL,"INIT_NOPARACHUTE",INT2NUM(SDL_INIT_NOPARACHUTE));
  rb_define_const(mSDL,"INIT_EVENTTHREAD",INT2NUM(SDL_INIT_EVENTTHREAD));
  rb_define_const(mSDL,"INIT_EVERYTHING",INT2NUM(SDL_INIT_EVERYTHING));
}



void Init_rubysdl()
{
  mSDL = rb_define_module("SDL");
  rb_define_module_function(mSDL,"init",sdl_init,1);
  rb_define_module_function(mSDL,"quit",sdl_quit,0);

  init_video();
  init_event();
  init_keyEvent();
#ifdef HAVE_SDL_MIXER
  init_mixer();
#endif
  defineConst();
}


