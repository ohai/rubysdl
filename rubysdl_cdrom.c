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

static VALUE cCD;

typedef struct{
  SDL_CD* cd;
} CD;

DEFINE_GET_STRUCT(CD, GetCD, cCD, "SDL::CD");

static SDL_CD* Get_SDL_CD(VALUE obj)
{
  CD* cd = GetCD(obj);
  if(cd->cd == NULL)
    rb_raise(rb_eRuntimeError, "CD is closed");
  return cd->cd;
}

static void CD_free(CD* cd)
{
  if( !rubysdl_is_quit() && cd->cd )
    SDL_CDClose(cd->cd);
  free(cd);
}

static VALUE CD_s_alloc(VALUE klass)
{
  CD* cd = ALLOC(CD);
  cd->cd = NULL;
  return Data_Wrap_Struct(cCD, 0, CD_free, cd);
}

static VALUE CD_initialize(VALUE self, VALUE drive)
{
  CD* cd = GetCD(self);
  rb_secure(4);
  
  cd->cd = SDL_CDOpen(NUM2INT(drive));
  if(cd->cd == NULL)
    rb_raise(eSDLError, "Couldn't open drive %d: %s", 
	     NUM2INT(drive), SDL_GetError());
  return Qnil;
}

static VALUE CD_s_numDrive(VALUE klass)
{
  rb_secure(4);
  return INT2FIX(SDL_CDNumDrives());
}
static VALUE CD_s_name(VALUE klass, VALUE drive)
{
  rb_secure(4);
  return rb_str_new2(SDL_CDName(NUM2INT(drive)));
}
static VALUE CD_s_open(VALUE klass, VALUE drive)
{
  VALUE newobj = CD_s_alloc(klass);
  CD_initialize(newobj, drive);
  return newobj;
}
static VALUE CD_status(VALUE self)
{
  rb_secure(4);
  return INT2FIX(SDL_CDStatus(Get_SDL_CD(self)));
}
static VALUE CD_play(VALUE self, VALUE start, VALUE length)
{
  rb_secure(4);
  if( SDL_CDPlay(Get_SDL_CD(self), NUM2INT(start), NUM2INT(length))==-1 )
    rb_raise(eSDLError, "Couldn't play cd :%s", SDL_GetError() );
  return Qnil;
}
static VALUE CD_playTracks(VALUE self, VALUE start_track, VALUE start_frame, 
			       VALUE ntracks, VALUE nframes)
{
  rb_secure(4);
  if( SDL_CDPlayTracks(Get_SDL_CD(self), NUM2INT(start_track),
                       NUM2INT(start_frame), NUM2INT(ntracks),
		       NUM2INT(nframes))==-1 )
    rb_raise(eSDLError, "Couldn't play cd :%s", SDL_GetError() );
  return Qnil;
}
static VALUE CD_pause(VALUE self)
{
  rb_secure(4);
  if( SDL_CDPause(Get_SDL_CD(self))==-1 )
    rb_raise(eSDLError, "cd pause failed :%s", SDL_GetError());
  return Qnil;
}
static VALUE CD_resume(VALUE self)
{
  rb_secure(4);
  if( SDL_CDResume(Get_SDL_CD(self))==-1 )
    rb_raise(eSDLError, "cd resume failed :%s", SDL_GetError());
  return Qnil;
}
static VALUE CD_stop(VALUE self)
{
  rb_secure(4);
  if( SDL_CDStop(Get_SDL_CD(self))==-1 )
    rb_raise(eSDLError, "cd pause failed :%s", SDL_GetError());
  return Qnil;
}
static VALUE CD_eject(VALUE self)
{
  rb_secure(4);
  if( SDL_CDEject(Get_SDL_CD(self))==-1 )
    rb_raise(eSDLError, "cd eject failed :%s", SDL_GetError());
  return Qnil;
}

static VALUE CD_numTracks(VALUE self)
{
  return INT2NUM(Get_SDL_CD(self)->numtracks);
}
static VALUE CD_currentTrack(VALUE self)
{
  return INT2NUM(Get_SDL_CD(self)->cur_track);
}
static VALUE CD_currentFrame(VALUE self)
{
  return INT2NUM(Get_SDL_CD(self)->cur_frame);
}
static VALUE CD_trackType(VALUE self, VALUE track)
{
  return INT2FIX(Get_SDL_CD(self)->track[NUM2INT(track)].type);
}
static VALUE CD_trackLength(VALUE self, VALUE track)
{
  return INT2FIX(Get_SDL_CD(self)->track[NUM2INT(track)].length);
}

static VALUE CD_s_framesToMSF(VALUE klass, VALUE frames)
{
  int m, s, f;
  FRAMES_TO_MSF(NUM2INT(frames), &m, &s, &f);
  return rb_ary_new3(3, INT2FIX(m), INT2FIX(s), INT2FIX(f));
}
static VALUE CD_s_MSFToFrames(VALUE klass, VALUE m, VALUE s, VALUE f)
{
  return INT2FIX(MSF_TO_FRAMES(NUM2INT(m),NUM2INT(s),NUM2INT(f)));
}
static VALUE CD_close(VALUE self)
{
  CD* cd = GetCD(self);
  if( !rubysdl_is_quit() && cd->cd )
    SDL_CDClose(cd->cd);
  cd->cd = NULL;
  return Qnil;
}
static VALUE CD_closed(VALUE self)
{
  return INT2BOOL(GetCD(self)->cd == NULL);
}

void rubysdl_init_CD(VALUE mSDL)
{
  cCD = rb_define_class_under(mSDL, "CD", rb_cObject);
  
  rb_define_alloc_func(cCD, CD_s_alloc);
  rb_define_private_method(cCD, "initialize", CD_initialize, 1);
  
  rb_define_singleton_method(cCD, "numDrive", CD_s_numDrive, 0);
  rb_define_singleton_method(cCD, "indexName", CD_s_name, 1);
  rb_define_singleton_method(cCD, "open", CD_s_open, 1);
  rb_define_singleton_method(cCD, "framesToMSF", CD_s_framesToMSF,1);
  rb_define_singleton_method(cCD,"MSFToFrames",CD_s_MSFToFrames,3);
  rb_define_method(cCD, "status", CD_status, 0);
  rb_define_method(cCD, "play", CD_play, 2);
  rb_define_method(cCD, "playTracks", CD_playTracks, 4);
  rb_define_method(cCD, "pause", CD_pause, 0);
  rb_define_method(cCD, "resume", CD_resume, 0);
  rb_define_method(cCD, "stop", CD_stop, 0);
  rb_define_method(cCD, "eject", CD_eject, 0);

  rb_define_method(cCD, "numTracks", CD_numTracks, 0);
  rb_define_method(cCD, "currentTrack", CD_currentTrack, 0);
  rb_define_method(cCD, "currentFrame", CD_currentFrame, 0);
  rb_define_method(cCD, "trackType", CD_trackType, 1);
  rb_define_method(cCD, "trackLength", CD_trackLength, 1);
  rb_define_method(cCD, "close", CD_close, 0);
  rb_define_method(cCD, "closed?", CD_closed, 0);
  
  rb_define_const(cCD, "TRAYEMPTY", INT2NUM(CD_TRAYEMPTY));
  rb_define_const(cCD, "STOPPED", INT2NUM(CD_STOPPED));
  rb_define_const(cCD, "PLAYING", INT2NUM(CD_PLAYING));
  rb_define_const(cCD, "PAUSED", INT2NUM(CD_PAUSED));
  rb_define_const(cCD, "ERROR", INT2NUM(CD_ERROR));
  
  rb_define_const(cCD, "AUDIO_TRACK", UINT2NUM(SDL_AUDIO_TRACK));
  rb_define_const(cCD, "DATA_TRACK", UINT2NUM(SDL_DATA_TRACK));
  rb_define_const(cCD, "FPS", UINT2NUM(CD_FPS));
}
