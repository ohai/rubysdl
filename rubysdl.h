
#include <SDL.h>
#include <ruby.h>
#include <stdio.h>
#ifndef UINT2NUM
#define UINT2NUM(v) INT2NUM(v)
#endif
VALUE mSDL;
VALUE eSDLError;
VALUE cSurface;
VALUE cScreen;
VALUE cPixelFormat;
VALUE cEvent;
VALUE mKey;
VALUE mMixer;
VALUE cWave;
VALUE mWM;
VALUE mMouse;
VALUE cTTF;
VALUE cJoyStick;
VALUE cCD;

#define SetRect(Rect,X,Y,W,H) \
do{ \
  Rect.x=NUM2INT(X); \
  Rect.y=NUM2INT(Y); \
  Rect.w=NUM2INT(W); \
  Rect.h=NUM2INT(H); \
}while(0) \

#define TORF(x) (x)?Qtrue:Qfalse
