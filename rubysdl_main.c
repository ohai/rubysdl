/* rubysdl.c -- SDL(Simple Directmedia Layer)
 */

#include "rubysdl.h"
#include <signal.h>
#include <stdio.h>
static VALUE sdl_init(VALUE obj,VALUE flags)
{
  Uint32 flag;
  
  rb_secure(1);
  flag= NUM2UINT(flags);
  if( SDL_Init(flag) < 0 )
    rb_raise(eSDLError,"Couldn't initialize SDL: %s",SDL_GetError());
  return Qnil;
}

static VALUE sdl_wasInit(VALUE mod,VALUE flags)
{
  return UINT2NUM( SDL_WasInit(NUM2UINT(flags)) );
}

static VALUE sdl_quit(VALUE obj)
{
#ifdef HAVE_SDL_MIXER
  quit_mixer();
#endif
  SDL_Quit();
  return Qnil;
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
  rb_define_module_function(mSDL,"quit",sdl_quit,0);
  rb_define_module_function(mSDL,"initedSystem",sdl_wasInit,1);
  
  defineConst();
  
  init_video();
  init_event();
  init_keyEvent();
  init_mouse();
  init_time();
  init_wm();
#ifdef HAVE_SDL_MIXER
  init_mixer();
#endif

}


