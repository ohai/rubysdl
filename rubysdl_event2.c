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

#ifdef DEF_EVENT2

#include "rubysdl.h"

typedef VALUE (*createEventObjFunc)(SDL_Event *);
static createEventObjFunc createEventObj[SDL_NUMEVENTS];

static createNoEvent(SDL_Event *event)
{
  return Qnil;
}

static VALUE createActiveEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cActiveEvent);
  rb_iv_set(obj,"@gain",BOOL(event->active.gain));
  rb_iv_set(obj,"@state",INT2FIX(event->active.state));
  return obj;
}

static VALUE createKeyEvent(VALUE obj)
{
  rb_iv_set(obj,"@press",BOOL(event->key.state==SDL_PRESSED));
  rb_iv_set(obj,"@sym",INT2FIX(event->key.keysym.sym));
  rb_iv_set(obj,"@mod",UINT2NUM(event->key.keysym.mod));
  return obj;
}

static VALUE createKeyDownEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cKeyDownEvent);
  return createKeyEvent(obj);
}

static VALUE createKeyUpEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cKeyUpEvent);
  return createKeyEvent(obj);
}

static VALUE createMouseMotionEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cMouseMotionEvent);
  rb_iv_set(obj,"@state",INT2FIX(event->motion.state));
  rb_iv_set(obj,"@x",INT2FIX(event->motion.x));
  rb_iv_set(obj,"@y",INT2FIX(event->motion.y));
  rb_iv_set(obj,"@xrel",INT2FIX(event->motion.xrel));
  rb_iv_set(obj,"@yrel",INT2FIX(event->motion.yrel));
  return obj;
}

static VALUE createMouseButtonEvent(VALUE obj)
{
  rb_iv_set(obj,"@button",INT2FIX(event->button.button));
  rb_iv_set(obj,"@press",BOOL(event->button.state==SDL_PRESSED));
  rb_iv_set(obj,"@x",INT2FIX(event->button.x));
  rb_iv_set(obj,"@y",INT2FIX(event->button.y));
  return obj;
}

static VALUE createMouseButtonDownEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cMouseButtonDownEvent);
  return createMouseButtonEvent(obj);
}

static VALUE createMouseButtonUpEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cMouseButtonUpEvent);
  return createMouseButtonEvent(obj);
}

static VALUE createJoyAxisEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cJoyAxisEvent);
  rb_iv_set(obj,"@which",INT2FIX(event->jaxis.which));
  rb_iv_set(obj,"@axis",INT2FIX(event->jaxis.axis));
  rb_iv_set(obj,"@value",INT2FIX(event->jaxis.value));
  return obj;
}

static VALUE createJoyBallEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cJoyBallEvent);
  rb_iv_set(obj,"@which",INT2FIX(event->jball.which));
  rb_iv_set(obj,"@ball",INT2FIX(event->jball.ball));
  rb_iv_set(obj,"@xrel",INT2FIX(event->jball.xrel));
  rb_iv_set(obj,"@yrel",INT2FIX(event->jball.yrel));
  return obj;
}

static VALUE createJoyHatEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cJoyHatEvent);
  rb_iv_set(obj,"@which",INT2FIX(event->jhat.which));
  rb_iv_set(obj,"@hat",INT2FIX(event->jhat.hat));
  rb_iv_set(obj,"@value",INT2FIX(event->jhat.value));
  return obj;
}

static VALUE createJoyButtonEvent(VALUE obj)
{
  rb_iv_set(obj,"@which",INT2FIX(event->jbutton.which));
  rb_iv_set(obj,"@button",INT2FIX(event->jbutton.button));
  rb_iv_set(obj,"press",BOOL(event->jbutton.state==SDL_PRESSED));
  return obj;
}
static VALUE createJoyButtonUpEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cJoyButtonUpEvent);
  return createJoyButtonEvent(obj);
}

static VALUE createJoyButtonDownEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cJoyButtonUpEvent);
  return createJoyButtonEvent(obj);
}

static VALUE createQuitEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cQuitEvent);
  return obj;
}

static VALUE createSysWMEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cSysWMEvent);
  return obj;
}

static VALUE createVideoResizeEvent(SDL_Event *event)
{
  VALUE obj=rb_obj_alloc(cVideoResizeEvent);
  rb_iv_set(obj,"@w",INT2FIX(event->resize.w));
  rb_iv_set(obj,"@h",INT2FIX(event->resize.h));
  return obj;
}

static VALUE sdl_event2_pall(VALUE class)
{
  SDL_Event event;
  if( SDL_PollEvent(&event)==1)
    return createEventObj[event->type](&event);
  else
    return Qnil;
}
static VALUE sdl_event2_wait(VALUE class)
{
  SDL_Event event;
  if( SDL_WaitEvent(&event)==1)
    return createEventObj[event->type](&event);
  else
    rb_raise(eSDLError,"Event handling error");
}
void init_event2(void)
{
  int i;
  
  cEvent2=rb_define_class_under(mSDL,"Event2",rb_cObject);
  rb_define_singleton_method(cEvent2,"poll",sdl_event2_poll,0);
  rb_define_singleton_method(cEvent2,"wait",sdl_event2_wait,0);
  /*rb_define_attr(cEvent2,"type",1,0);*/
  
  cActiveEvent=rb_define_class_under(mSDL,"Active",cEvent2);
  rb_define_attr(cActiveEvent,"gain",1,1);
  rb_define_attr(cActiveEvent,"state",1,1);
  
  cKeyDownEvent=rb_define_class_under(mSDL,"KeyDown",cEvent2);
  rb_define_attr(cKeyDownEvent,"press",1,1);
  rb_define_attr(cKeyDownEvent,"sym",1,1);
  rb_define_attr(cKeyDownEvent,"mod",1,1);
  
  cKeyUpEvent=rb_define_class_under(mSDL,"KeyUp",cEvent2);
  rb_define_attr(cKeyUpEvent,"press",1,1);
  rb_define_attr(cKeyUpEvent,"sym",1,1);
  rb_define_attr(cKeyUpEvent,"mod",1,1);
  
  cMouseMotionEvent=rb_define_class_under(mSDL,"MouseMotion",cEvent2);
  rb_define_attr(cMouseMotionEvent,"state",1,1);
  rb_define_attr(cMouseMotionEvent,"x",1,1);
  rb_define_attr(cMouseMotionEvent,"y",1,1);
  rb_define_attr(cMouseMotionEvent,"xrel",1,1);
  rb_define_attr(cMouseMotionEvent,"yrel",1,1);
  
  cMouseButtonDownEvent=rb_define_class_under(mSDL,"MouseButtonDown",cEvent2);
  rb_define_attr(cMouseButtonDownEvent,"button",1,1);
  rb_define_attr(cMouseButtonDownEvent,"press",1,1);
  rb_define_attr(cMouseButtonDownEvent,"x",1,1);
  rb_define_attr(cMouseButtonDownEvent,"y",1,1);

  cMouseButtonUpEvent=rb_define_class_under(mSDL,"MouseButtonUp",cEvent2);
  rb_define_attr(cMouseButtonUpEvent,"button",1,1);
  rb_define_attr(cMouseButtonUpEvent,"press",1,1);
  rb_define_attr(cMouseButtonUpEvent,"x",1,1);
  rb_define_attr(cMouseButtonUpEvent,"y",1,1);
  
  cJoyAxisEvent=rb_define_class_under(mSDL,"JoyAxis",cEvent2);
  rb_define_attr(cJoyAxisEvent,"which",1,1);
  rb_define_attr(cJoyAxisEvent,"axis",1,1);
  rb_define_attr(cJoyAxisEvent,"value",1,1);
  
  cJoyBallEvent=rb_define_class_under(mSDL,"JoyBall",cEvent2);
  rb_define_attr(cJoyBallEvent,"which",1,1);
  rb_define_attr(cJoyBallEvent,"ball",1,1);
  rb_define_attr(cJoyBallEvent,"xrel",1,1);
  rb_define_attr(cJoyBallEvent,"yrel",1,1);
  
  cJoyHatEvent=rb_define_class_under(mSDL,"JoyHat",cEvent2);
  rb_define_attr(cJoyHatEvent,"which",1,1);
  rb_define_attr(cJoyHatEvent,"hat",1,1);
  rb_define_attr(cJoyHatEvent,"value",1,1);
  
  cJoyButtonUpEvent=rb_define_class_under(mSDL,"JoyButtonUp",cEvent2);
  rb_define_attr(cJoyButtonUpEvent,"which",1,1);
  rb_define_attr(cJoyButtonUpEvent,"button",1,1);
  rb_define_attr(cJoyButtonUpEvent,"press",1,1);

  cJoyButtonDownEvent=rb_define_class_under(mSDL,"JoyButtonDown",cEvent2);
  rb_define_attr(cJoyButtonDownEvent,"which",1,1);
  rb_define_attr(cJoyButtonDownEvent,"button",1,1);
  rb_define_attr(cJoyButtonDownEvent,"press",1,1);
  
  cQuitEvent=rb_define_class_under(mSDL,"Quit",cEvent2);
  
  cSysWMEvent=rb_define_class_under(mSDL,"SysWM",cEvent2);
  
  cVideoResizeEvent=rb_define_class_under(mSDL,"VideoResize",cEvent2);
  rb_define_attr(cVideoResizeEvent,"x",1,1);
  rb_define_attr(cVideoResizeEvent,"y",1,1);

  for(i=0;i<SDL_NUMEVENS;++i)
    createEventObj[i]=createNoEvent;
  createEventObj[SDL_ACTIVEEVENT]=createActiveEvent;
  createEventObj[SDL_KEYDOWN]=createKeyDownEvent
  createEventObj[SDL_KEYUP]=createKeyUpEvent;
  createEventObj[SDL_MOUSEMOTION]=createMouseMotionEvent;
  createEventObj[SDL_MOUSEBUTTONDOWN]=createMouseButtonDownEvent;
  createEventObj[SDL_MOUSEBUTTONUP]=createMouseButtonUpEvent;
  createEventObj[SDL_JOYAXISMOTION]=createJoyAxisEvent;
  createEventObj[SDL_JOYBALLMOTION]=createJoyBallEvent;
  createEventObj[SDL_JOYHATMOTION]=createJoyHatEvent;
  createEventObj[SDL_JOYBUTTONDOWN]=createJoyButtonDownEvent;
  createEventObj[SDL_JOYBUTTONUP]=createJoyButtonUpEvent;
  createEventObj[SDL_QUIT]=createQuitEvent;
  createEventObj[SDL_SYSWMEVENT]=createSysWMEvent;
  createEventObj[SDL_VIDEORESIZE]=createVideoResizeEvent;

		 
}
  /*
    Active;
    KeyDown;
    KeyUp;
    MouseMotion;
    MouseButtonDown;
    MouseButtonUp;
    JoyAxis;
    JoyBall;
    JoyHat;
    JoyButtonUp;
    JoyButtonDown;
    Quit;
    SysWM;
    VideoResize;
  */
    
#endif /* DEF_EVENT2 */
