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

/* basic */
typedef struct{
  SDL_Joystick* joystick;
} Joystick;

static VALUE cJoystick=Qnil;

static Joystick* GetJoystick(VALUE obj)
{
  Joystick* joy;
  
  if(!rb_obj_is_kind_of(obj, cJoystick)){
    rb_raise(rb_eTypeError, "wrong argument type %s (expected SDL::Joystick)",
             rb_obj_classname(obj));
  }
  
  Data_Get_Struct(obj, Joystick, joy);
  return joy;
}

static SDL_Joystick* Get_SDL_Joystick(VALUE obj)
{
  Joystick* joy = GetJoystick(obj);
  if(joy->joystick == NULL){
    rb_raise(rb_eRuntimeError, "joystick is closed");
  }

  return joy->joystick;
}

static void Joystick_free(Joystick* joy)
{
  if(rubysdl_is_quit() || joy->joystick == NULL){
    free(joy);
  }else{
    SDL_JoystickClose(joy->joystick);
    free(joy);
  }
}

/* initialize methods */
static VALUE Joystick_s_alloc(VALUE klass)
{
  Joystick* joy = ALLOC(Joystick);
  joy->joystick = NULL;
  return Data_Wrap_Struct(cJoystick, 0, Joystick_free, joy);
}

static VALUE Joystick_initialize(VALUE self, VALUE index)
{
  Joystick* joy = GetJoystick(self);
  rb_secure(4);
  
  joy->joystick = SDL_JoystickOpen(NUM2INT(index));
  if( joy->joystick == NULL ){
    rb_raise(eSDLError, "Couldn't open joystick No.%d :%s", NUM2INT(index),
	     SDL_GetError());
  }
  return Qnil;
}

/* class methods */
static VALUE Joystick_s_poll(VALUE klass)
{
  rb_secure(4);
  return INT2BOOL(SDL_JoystickEventState(SDL_QUERY) == SDL_ENABLE);
}

static VALUE Joystick_s_set_poll(VALUE klass, VALUE poll)
{
  rb_secure(4);
  
  if(RTEST(poll))
    SDL_JoystickEventState(SDL_ENABLE);
  else
    SDL_JoystickEventState(SDL_IGNORE);
  return poll;
}

static VALUE Joystick_s_num(VALUE klass)
{
  rb_secure(4);
  return INT2FIX(SDL_NumJoysticks());
}

static VALUE Joystick_s_indexName(VALUE klass, VALUE index)
{
  rb_secure(4);
  return rb_str_new2(SDL_JoystickName(NUM2INT(index)));
}

static VALUE Joystick_s_open_p(VALUE klass, VALUE index)
{
  rb_secure(4);
  return INT2BOOL(SDL_JoystickOpened(NUM2INT(index)));
}

static VALUE Joystick_s_open(VALUE klass, VALUE index)
{
  VALUE newobj;
  rb_secure(4);
  
  newobj = Joystick_s_alloc(klass);
  Joystick_initialize(newobj, index);
  return newobj;
}

static VALUE Joystick_s_update(VALUE klass)
{
  rb_secure(4);
  SDL_JoystickUpdate();
  return Qnil;
}

/* methods */
static VALUE Joystick_close(VALUE self)
{
  Joystick* joy;
  rb_secure(4);
  
  joy = GetJoystick(self);
  SDL_JoystickClose(joy->joystick);
  joy->joystick = NULL;
  return Qnil;
}
static VALUE Joystick_index(VALUE self)
{
  SDL_Joystick* joystick;
  rb_secure(4);
  
  joystick = Get_SDL_Joystick(self);
  return INT2FIX(SDL_JoystickIndex(joystick));
}

static VALUE Joystick_numAxes(VALUE self)
{
  SDL_Joystick* joystick;
  rb_secure(4);
  
  joystick = Get_SDL_Joystick(self);
  return INT2FIX(SDL_JoystickNumAxes(joystick));
}

static VALUE Joystick_numBalls(VALUE self)
{
  SDL_Joystick *joystick;
  rb_secure(4);
  
  joystick = Get_SDL_Joystick(self);
  return INT2FIX(SDL_JoystickNumBalls(joystick));
}

static VALUE Joystick_numHats(VALUE self)
{
  SDL_Joystick *joystick;
  rb_secure(4);
  joystick = Get_SDL_Joystick(self);
  return INT2FIX(SDL_JoystickNumHats(joystick));
}

static VALUE Joystick_numButtons(VALUE self)
{
  SDL_Joystick *joystick;
  rb_secure(4);
  
  joystick = Get_SDL_Joystick(self);
  return INT2FIX(SDL_JoystickNumButtons(joystick));
}

static VALUE Joystick_getAxis(VALUE self, VALUE axis)
{
  SDL_Joystick *joystick;
  rb_secure(4);
  
  joystick = Get_SDL_Joystick(self);
  return INT2NUM(SDL_JoystickGetAxis(joystick, NUM2INT(axis)));
}

static VALUE Joystick_getHat(VALUE self, VALUE hat)
{
  SDL_Joystick *joystick;
  rb_secure(4);
  
  joystick = Get_SDL_Joystick(self);
  return UINT2NUM(SDL_JoystickGetHat(joystick, NUM2INT(hat)));
}

static VALUE Joystick_getButton(VALUE self, VALUE button)
{
  SDL_Joystick *joystick;
  rb_secure(4);
  
  joystick = Get_SDL_Joystick(self);
  return INT2BOOL(SDL_JoystickGetButton(joystick, NUM2INT(button)));
}

static VALUE Joystick_getBall(VALUE self, VALUE ball)
{
  SDL_Joystick *joystick;
  int dx,dy;

  rb_secure(4);
  
  joystick = Get_SDL_Joystick(self);
  if( SDL_JoystickGetBall(joystick, NUM2INT(ball), &dx, &dy) == -1 )
    rb_raise(eSDLError,"SDL_JoystickGetBall failed :%s",SDL_GetError());
  return rb_ary_new3(2, INT2FIX(dx), INT2FIX(dy));
}

void rubysdl_init_Joystick(VALUE mSDL)
{
  cJoystick = rb_define_class_under(mSDL, "Joystick", rb_cObject);
  
  rb_define_singleton_method(cJoystick, "poll", Joystick_s_poll, 0);
  rb_define_singleton_method(cJoystick, "poll=", Joystick_s_set_poll, 1);
  rb_define_singleton_method(cJoystick, "num", Joystick_s_num, 0);
  rb_define_singleton_method(cJoystick, "indexName", Joystick_s_indexName, 1);
  rb_define_singleton_method(cJoystick, "open?", Joystick_s_open_p, 1);
  rb_define_singleton_method(cJoystick, "update", Joystick_s_update, 0);
  rb_define_singleton_method(cJoystick, "updateAll", Joystick_s_update, 0);
  rb_define_singleton_method(cJoystick, "open", Joystick_s_open, 1);

  rb_define_alloc_func(cJoystick, Joystick_s_alloc);
  rb_define_private_method(cJoystick, "initialize", Joystick_initialize, 1);

  rb_define_method(cJoystick, "close", Joystick_close, 0);
  rb_define_method(cJoystick, "index", Joystick_index, 0);
  rb_define_method(cJoystick, "numAxes", Joystick_numAxes, 0);
  rb_define_method(cJoystick, "numBalls", Joystick_numBalls, 0);
  rb_define_method(cJoystick, "numHats", Joystick_numHats, 0);
  rb_define_method(cJoystick, "numButtons", Joystick_numButtons, 0);  
  
  rb_define_method(cJoystick, "axis", Joystick_getAxis, 1);
  rb_define_method(cJoystick, "hat", Joystick_getHat, 1);
  rb_define_method(cJoystick, "button", Joystick_getButton, 1);
  rb_define_method(cJoystick, "ball", Joystick_getBall, 1);
  
  rb_define_const(cJoystick,"HAT_CENTERED",UINT2NUM(SDL_HAT_CENTERED));
  rb_define_const(cJoystick,"HAT_UP",UINT2NUM(SDL_HAT_UP));
  rb_define_const(cJoystick,"HAT_RIGHT",UINT2NUM(SDL_HAT_RIGHT));
  rb_define_const(cJoystick,"HAT_DOWN",UINT2NUM(SDL_HAT_DOWN));
  rb_define_const(cJoystick,"HAT_LEFT",UINT2NUM(SDL_HAT_LEFT));
  rb_define_const(cJoystick,"HAT_RIGHTUP",UINT2NUM(SDL_HAT_RIGHTUP));
  rb_define_const(cJoystick,"HAT_RIGHTDOWN",UINT2NUM(SDL_HAT_RIGHTDOWN));
  rb_define_const(cJoystick,"HAT_LEFTUP",UINT2NUM(SDL_HAT_LEFTUP));
  rb_define_const(cJoystick,"HAT_LEFTDOWN",UINT2NUM(SDL_HAT_LEFTDOWN));
}
