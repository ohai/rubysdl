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

#include <SDL.h>
#include <ruby.h>
#include <stdio.h>
#ifndef UINT2NUM
#define UINT2NUM(v) INT2NUM(v)
#endif

#ifdef DEF_GLOBAL
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL VALUE mSDL;
GLOBAL VALUE eSDLError;
GLOBAL VALUE cSurface;
GLOBAL VALUE cScreen;
GLOBAL VALUE cPixelFormat;
GLOBAL VALUE cEvent;
GLOBAL VALUE mKey;
GLOBAL VALUE mMixer;
GLOBAL VALUE cWave;
GLOBAL VALUE cMusic;
GLOBAL VALUE mWM;
GLOBAL VALUE mMouse;
GLOBAL VALUE cTTF;
GLOBAL VALUE cJoystick;
GLOBAL VALUE cCD;

#define SetRect(Rect,X,Y,W,H) \
do{ \
  Rect.x=NUM2INT(X); \
  Rect.y=NUM2INT(Y); \
  Rect.w=NUM2INT(W); \
  Rect.h=NUM2INT(H); \
}while(0) \

#define BOOL(x) (x)?Qtrue:Qfalse
