#include "rubysdl.h"

void eventCheck(int pred,char *msg)
{
  if(!pred) 
    rb_raise(eSDLError,"this event is not %s event",msg) ;
}
    
static VALUE createEventObject(VALUE class)
{
  SDL_Event *event;
  
  return Data_Make_Struct(class,SDL_Event,0,free,event);
}

static VALUE sdl_pollEvent(VALUE obj)
{
  SDL_Event *event;

  Data_Get_Struct(obj,SDL_Event,event);
  return INT2NUM(SDL_PollEvent(event));
}
static VALUE sdl_eventType(VALUE obj)
{
  SDL_Event *event;

  Data_Get_Struct(obj,SDL_Event,event);
  return INT2FIX(event->type);
}

/* --KeyboardEvent-- */
static VALUE sdl_eventKeyPressed(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck((event->type == SDL_KEYDOWN)||(event->type == SDL_KEYDOWN),"key");
  if( event->key.state==SDL_PRESSED )
    return Qtrue;
  else
    return Qfalse;
}
static VALUE sdl_eventKeyMod(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck((event->type == SDL_KEYDOWN)||(event->type == SDL_KEYDOWN),"key");
  return INT2NUM(event->key.keysym.mod);
}
static VALUE sdl_eventKeySym(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck((event->type == SDL_KEYDOWN)||(event->type == SDL_KEYDOWN),"key");
  return INT2FIX(event->key.keysym.sym);
}

/* --ActiveEvent-- */
static VALUE sdl_eventActiveGained(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck( event->type == ACTIVEEVENT , "active" );
  return (event->active.gain)?Qtrue:Qfalse;
}
static VALUE sdl_eventActiveState(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck( event->type == ACTIVEEVENT , "active" );
  return INT2NUM(event->active.state);
}

/* --MouseMotionEvent-- */
static VALUE sdl_eventMouseX(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  switch (event->type){
  case SDL_MOUSEMOTION:
    return INT2NUM(event->motion.x);
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    return INT2NUM(event->button.x);
  default:
    eventCheck(0,"mouse"); /* raise exception */
  }
  return Qnil; /* never reach */
}
static VALUE sdl_eventMouseY(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  switch (event->type){
  case SDL_MOUSEMOTION:
    return INT2NUM(event->motion.y);
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    return INT2NUM(event->button.y);
  default:
    eventCheck(0,"mouse"); /* raise exception */
  }
  return Qnil; /* never reach */
}
static VALUE sdl_eventMouseXrel(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck(event->type==SDL_MOUSEMOTION,"mouse motion");
  return INT2NUM(event->motion.xrel);
}
static VALUE sdl_eventMouseYrel(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck(event->type==SDL_MOUSEMOTION,"mouse motion");
  return INT2NUM(event->motion.yrel);
}

/* --SDL_MouseButtonEvent-- */
static VALUE sdl_eventMouseButton(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck( (event->type==SDL_MOUSEBUTTONUP)||
	     ( event->type==SDL_MOUSEBUTTONUP)   , "mouse button" );
  return INT2NUM( event->button.button );
}
static VALUE sdl_eventMousePressed(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  eventCheck( (event->type==SDL_MOUSEBUTTONUP)||
	      ( event->type==SDL_MOUSEBUTTONUP)   , "mouse button" );
  return (event->button.state==SDL_PRESSED)?Qtrue:Qfalse;
}

static void defineConstForEvent()
{
  rb_define_const(cEvent,"NOEVENT",INT2NUM(SDL_NOEVENT));
  rb_define_const(cEvent,"ACTIVEEVENT",INT2NUM(SDL_ACTIVEEVENT));
  rb_define_const(cEvent,"KEYDOWN",INT2NUM(SDL_KEYDOWN));
  rb_define_const(cEvent,"KEYUP",INT2NUM(SDL_KEYUP));
  rb_define_const(cEvent,"MOUSEMOTION",INT2NUM(SDL_MOUSEMOTION));
  rb_define_const(cEvent,"MOUSEBUTTONDOWN",INT2NUM(SDL_MOUSEBUTTONDOWN));
  rb_define_const(cEvent,"MOUSEBUTTONUP",INT2NUM(SDL_MOUSEBUTTONUP));
  rb_define_const(cEvent,"JOYAXISMOTION",INT2NUM(SDL_JOYAXISMOTION));
  rb_define_const(cEvent,"JOYBALLMOTION",INT2NUM(SDL_JOYBALLMOTION));
  rb_define_const(cEvent,"JOYHATMOTION",INT2NUM(SDL_JOYHATMOTION));
  rb_define_const(cEvent,"JOYBUTTONDOWN",INT2NUM(SDL_JOYBUTTONDOWN));
  rb_define_const(cEvent,"JOYBUTTONUP",INT2NUM(SDL_JOYBUTTONUP));
  rb_define_const(cEvent,"QUIT",INT2NUM(SDL_QUIT));
  rb_define_const(cEvent,"SYSWMEVENT",INT2NUM(SDL_SYSWMEVENT));
  rb_define_const(cEvent,"EVENT_RESERVEDA",INT2NUM(SDL_EVENT_RESERVEDA));
  rb_define_const(cEvent,"EVENT_RESERVEDB",INT2NUM(SDL_EVENT_RESERVEDB));
  rb_define_const(cEvent,"VIDEORESIZE",INT2NUM(SDL_VIDEORESIZE));
  rb_define_const(cEvent,"EVENT_RESERVED1",INT2NUM(SDL_EVENT_RESERVED1));
  rb_define_const(cEvent,"EVENT_RESERVED2",INT2NUM(SDL_EVENT_RESERVED2));
  rb_define_const(cEvent,"EVENT_RESERVED3",INT2NUM(SDL_EVENT_RESERVED3));
  rb_define_const(cEvent,"EVENT_RESERVED4",INT2NUM(SDL_EVENT_RESERVED4));
  rb_define_const(cEvent,"EVENT_RESERVED5",INT2NUM(SDL_EVENT_RESERVED5));
  rb_define_const(cEvent,"EVENT_RESERVED6",INT2NUM(SDL_EVENT_RESERVED6));
  rb_define_const(cEvent,"EVENT_RESERVED7",INT2NUM(SDL_EVENT_RESERVED7));
  /* Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
  rb_define_const(cEvent,"USEREVENT",INT2NUM(SDL_USEREVENT));
  /* This last event is only for bounding internal arrays
     It is the number of bits in the event mask datatype -- Uint32
     */
  rb_define_const(cEvent,"NUMEVENTS",INT2NUM(SDL_NUMEVENTS));
}

void init_event()
{
  cEvent = rb_define_class_under(mSDL,"Event",rb_cObject);
  rb_define_singleton_method(cEvent,"new",createEventObject,0);
  rb_define_method(cEvent,"poll",sdl_pollEvent,0);
  
  rb_define_method(cEvent,"type",sdl_eventType,0);
  
  rb_define_method(cEvent,"keyPress?",sdl_eventKeyPressed,0)
  rb_define_method(cEvent,"keySym",sdl_eventKeySym,0);
  rb_define_method(cEvent,"keyMod",sdl_eventKeyMod,0);

  rb_define_method(cEvent,"gain?",sdl_eventActiveGained,0);
  rb_define_method(cEvent,"appState",sdl_eventActiveState,0);

  rb_define_method(cEvent,"mouseX",sdl_eventMouseX,0);
  rb_define_method(cEvent,"mouseY",sdl_eventMouseY,0);
  rb_define_method(cEvent,"mouseXrel",sdl_eventMouseXrel,0);
  rb_define_method(cEvent,"mouseYrel",sdl_eventMouseYrel,0);

  rb_define_method(cEvent,"mouseButton",sdl_eventMouseButton,0);
  rb_define_method(cEvent,"mousePress?",sdl_eventMousePressed,0);
  
  defineConstForEvent();
  
}
