#include "rubysdl.h"

static VALUE sdl_joyStick_num(VALUE class)
{
  return INT2FIX(SDL_NumJoysticks());
}
static VALUE sdl_joyStick_name(VALUE class,VALUE index)
{
  return rb_str_new2( SDL_JoystickName(NUM2INT(index)) );
}
static VALUE sdl_joyStick_open(VALUE class,VALUE index)
{
  SDL_Joystick *joyStick;
  joyStick=SDL_JoystickOpen(NUM2INT(index));
  if(joyStick==NULL)
    rb_raise(eSDLError,"Couldn't open JoyStick No.%d :%s",NUM2INT(index),
	     SDL_GetError());
  return Data_Wrap_Struct(class,0,SDL_JoystickClose,joyStick);
}
static VALUE sdl_joyStick_opened(VALUE class,VALUE index)
{
  return (SDL_JoystickOpened(NUM2INT(index)))?Qtrue:Qfalse;
}
static VALUE sdl_joyStick_update(VALUE class)
{
  SDL_JoystickUpdate();
  return Qnil;
}
static VALUE sdl_joyStick_index(VALUE obj)
{
  SDL_Joystick *joyStick;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  return INT2FIX(SDL_JoystickIndex(joyStick));
}
static VALUE sdl_joyStick_numAxes(VALUE obj)
{
  SDL_Joystick *joyStick;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  return INT2FIX(SDL_JoystickNumAxes(joyStick));
}
static VALUE sdl_joyStick_numBalls(VALUE obj)
{
  SDL_Joystick *joyStick;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  return INT2FIX(SDL_JoystickNumBalls(joyStick));
}
static VALUE sdl_joyStick_numHats(VALUE obj)
{
  SDL_Joystick *joyStick;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  return INT2FIX(SDL_JoystickNumHats(joyStick));
}
static VALUE sdl_joyStick_numButtons(VALUE obj)
{
  SDL_Joystick *joyStick;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  return INT2FIX(SDL_JoystickNumButtons(joyStick));
}

static VALUE sdl_joyStick_getAxis(VALUE obj,VALUE axis)
{
  SDL_Joystick *joyStick;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  return INT2NUM(SDL_JoystickGetAxis(joyStick,NUM2INT(axis)));
}
static VALUE sdl_joyStick_getHat(VALUE obj,VALUE hat)
{
  SDL_Joystick *joyStick;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  return UINT2NUM(SDL_JoystickGetHat(joyStick,NUM2INT(hat)));
}
static VALUE sdl_joyStick_getButton(VALUE obj,VALUE button)
{
  SDL_Joystick *joyStick;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  return (SDL_JoystickGetButton(joyStick,NUM2INT(button)))?Qtrue:Qfalse;
}
static VALUE sdl_joyStick_getBall(VALUE obj,VALUE ball)
{
  SDL_Joystick *joyStick;
  int dx,dy;
  Data_Get_Struct(obj,SDL_Joystick,joyStick);
  if( SDL_JoystickGetBall(joyStick,NUM2INT(ball),&dx,&dy)== -1 )
    rb_raise(eSDLError,"SDL_JoystickGetBall failed :%s",SDL_GetError());
  return rb_ary_new3(2,INT2FIX(dx),INT2FIX(dy));
}

static void defineConstForJoyStick()
{
  rb_define_const(cJoyStick,"HAT_CENTERED",UINT2NUM(SDL_HAT_CENTERED));
  rb_define_const(cJoyStick,"HAT_UP",UINT2NUM(SDL_HAT_UP));
  rb_define_const(cJoyStick,"HAT_RIGHT",UINT2NUM(SDL_HAT_RIGHT));
  rb_define_const(cJoyStick,"HAT_DOWN",UINT2NUM(SDL_HAT_DOWN));
  rb_define_const(cJoyStick,"HAT_LEFT",UINT2NUM(SDL_HAT_LEFT));
  rb_define_const(cJoyStick,"HAT_RIGHTUP",UINT2NUM(SDL_HAT_RIGHTUP));
  rb_define_const(cJoyStick,"HAT_RIGHTDOWN",UINT2NUM(SDL_HAT_RIGHTDOWN));
  rb_define_const(cJoyStick,"HAT_LEFTUP",UINT2NUM(SDL_HAT_LEFTUP));
  rb_define_const(cJoyStick,"HAT_LEFTDOWN",UINT2NUM(SDL_HAT_LEFTDOWN));
}
void init_joystick()
{
  cJoyStick = rb_define_class_under(mSDL,"JoyStick",rb_cObject);
  rb_define_singleton_method(cJoyStick,"num",sdl_joyStick_num,0);
  rb_define_singleton_method(cJoyStick,"name",sdl_joyStick_name,1);
  rb_define_singleton_method(cJoyStick,"open",sdl_joyStick_open,1);
  rb_define_singleton_method(cJoyStick,"open?",sdl_joyStick_opened,1);
  rb_define_singleton_method(cJoyStick,"updateAll",sdl_joyStick_update,0);
  
  rb_define_method(cJoyStick,"index",sdl_joyStick_index,0);
  rb_define_method(cJoyStick,"numAxes",sdl_joyStick_numAxes,0);
  rb_define_method(cJoyStick,"numBalls",sdl_joyStick_numBalls,0);
  rb_define_method(cJoyStick,"numHats",sdl_joyStick_numHats,0);
  rb_define_method(cJoyStick,"numButtons",sdl_joyStick_numButtons,0);
  
  rb_define_method(cJoyStick,"axis",sdl_joyStick_getAxis,1);
  rb_define_method(cJoyStick,"hat",sdl_joyStick_getHat,1);
  rb_define_method(cJoyStick,"button",sdl_joyStick_getButton,1);
  rb_define_method(cJoyStick,"ball",sdl_joyStick_getBall,1);

  defineConstForJoyStick();
}
