#include "rubysdl.h"

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
static VALUE sdl_eventKeySym(VALUE obj)
{
  SDL_Event *event;
  
  Data_Get_Struct(obj,SDL_Event,event);
  if((event->type != SDL_KEYDOWN) && (event->type != SDL_KEYDOWN))
    rb_raise(rb_eRuntimeError,"this event is not key event");
  return INT2FIX(event->key.keysym.sym);
}

static void defineConstForEvent()
{
  rb_define_const(gEventClass,"NOEVENT",INT2NUM(SDL_NOEVENT));
  rb_define_const(gEventClass,"ACTIVEEVENT",INT2NUM(SDL_ACTIVEEVENT));	/* Application loses/gains visibility */
  rb_define_const(gEventClass,"KEYDOWN",INT2NUM(SDL_KEYDOWN));/* Keys pressed */
  rb_define_const(gEventClass,"KEYUP",INT2NUM(SDL_KEYUP));/* Keys released */
  rb_define_const(gEventClass,"MOUSEMOTION",INT2NUM(SDL_MOUSEMOTION));/* Mouse moved */
  rb_define_const(gEventClass,"MOUSEBUTTONDOWN",INT2NUM(SDL_MOUSEBUTTONDOWN));/* Mouse button pressed */
  rb_define_const(gEventClass,"MOUSEBUTTONUP",INT2NUM(SDL_MOUSEBUTTONUP));/* Mouse button released */
  rb_define_const(gEventClass,"JOYAXISMOTION",INT2NUM(SDL_JOYAXISMOTION));/* Joystick axis motion */
  rb_define_const(gEventClass,"JOYBALLMOTION",INT2NUM(SDL_JOYBALLMOTION));/* Joystick trackball motion */
  rb_define_const(gEventClass,"JOYHATMOTION",INT2NUM(SDL_JOYHATMOTION));/* Joystick hat position change */
  rb_define_const(gEventClass,"JOYBUTTONDOWN",INT2NUM(SDL_JOYBUTTONDOWN));/* Joystick button pressed */
  rb_define_const(gEventClass,"JOYBUTTONUP",INT2NUM(SDL_JOYBUTTONUP));	/* Joystick button released */
  rb_define_const(gEventClass,"QUIT",INT2NUM(SDL_QUIT));/* User-requested quit */
  rb_define_const(gEventClass,"SYSWMEVENT",INT2NUM(SDL_SYSWMEVENT));/* System specific event */
  rb_define_const(gEventClass,"EVENT_RESERVEDA",INT2NUM(SDL_EVENT_RESERVEDA));/* Reserved for future use.. */
  rb_define_const(gEventClass,"EVENT_RESERVEDB",INT2NUM(SDL_EVENT_RESERVEDB));/* Reserved for future use.. */
  rb_define_const(gEventClass,"VIDEORESIZE",INT2NUM(SDL_VIDEORESIZE));/* User resized video mode */
  rb_define_const(gEventClass,"EVENT_RESERVED1",INT2NUM(SDL_EVENT_RESERVED1));/* Reserved for future use.. */
  rb_define_const(gEventClass,"EVENT_RESERVED2",INT2NUM(SDL_EVENT_RESERVED2));/* Reserved for future use.. */
  rb_define_const(gEventClass,"EVENT_RESERVED3",INT2NUM(SDL_EVENT_RESERVED3));/* Reserved for future use.. */
  rb_define_const(gEventClass,"EVENT_RESERVED4",INT2NUM(SDL_EVENT_RESERVED4));/* Reserved for future use.. */
  rb_define_const(gEventClass,"EVENT_RESERVED5",INT2NUM(SDL_EVENT_RESERVED5));/* Reserved for future use.. */
  rb_define_const(gEventClass,"EVENT_RESERVED6",INT2NUM(SDL_EVENT_RESERVED6));/* Reserved for future use.. */
  rb_define_const(gEventClass,"EVENT_RESERVED7",INT2NUM(SDL_EVENT_RESERVED7));/* Reserved for future use.. */
  /* Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
  rb_define_const(gEventClass,"USEREVENT",INT2NUM(SDL_USEREVENT));
  /* This last event is only for bounding internal arrays
     It is the number of bits in the event mask datatype -- Uint32
     */
  rb_define_const(gEventClass,"NUMEVENTS",INT2NUM(SDL_NUMEVENTS));
}

void init_event()
{
  gEventClass = rb_define_class_under(gSDLModule,"Event",rb_cObject);
  rb_define_singleton_method(gEventClass,"new",createEventObject,0);
  rb_define_method(gEventClass,"pollEvent",sdl_pollEvent,0);
  rb_define_method(gEventClass,"type",sdl_eventType,0);
  rb_define_method(gEventClass,"keySym",sdl_eventKeySym,0);

  defineConstForEvent();
  
}
