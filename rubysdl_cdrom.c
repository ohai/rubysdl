/*
  Ruby/SDL   Ruby extension library for SDL

  Copyright (C) 2001-2004 Ohbayashi Ippei
  
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

static VALUE CD_s_numDrive(VALUE class)
{
  rb_secure(4);
  return INT2FIX(SDL_CDNumDrives());
}
static VALUE CD_s_name(VALUE class, VALUE drive)
{
  rb_secure(4);
  return rb_str_new2(SDL_CDName(NUM2INT(drive)));
}
static VALUE CD_s_open(VALUE class, VALUE drive)
{
  SDL_CD *cd;
  rb_secure(4);
  cd=SDL_CDOpen(NUM2INT(drive));
  if(cd==NULL)
    rb_raise(eSDLError, "Couldn't open drive %d: %s", 
	     NUM2INT(drive), SDL_GetError());
  return Data_Wrap_Struct(class, 0, SDL_CDClose, cd);
}
static VALUE CD_status(VALUE obj)
{
  SDL_CD *cd;
  rb_secure(4);
  Data_Get_Struct(obj, SDL_CD, cd);
  return INT2FIX(SDL_CDStatus(cd));
}
static VALUE CD_play(VALUE obj, VALUE start, VALUE length)
{
  SDL_CD *cd;
  rb_secure(4);
  Data_Get_Struct(obj, SDL_CD, cd);
  if( SDL_CDPlay(cd, NUM2INT(start), NUM2INT(length))==-1 )
    rb_raise(eSDLError, "Couldn't play cd :%s", SDL_GetError() );
  return Qnil;
}
static VALUE CD_playTracks(VALUE obj, VALUE start_track, VALUE start_frame, 
			       VALUE ntracks, VALUE nframes)
{
  SDL_CD *cd;
  rb_secure(4);
  Data_Get_Struct(obj, SDL_CD, cd);
  if( SDL_CDPlayTracks(cd, NUM2INT(start_track), NUM2INT(start_frame), 
		       NUM2INT(ntracks), NUM2INT(nframes))==-1 )
    rb_raise(eSDLError, "Couldn't play cd :%s", SDL_GetError() );
  return Qnil;
}
static VALUE CD_pause(VALUE obj)
{
  SDL_CD *cd;
  rb_secure(4);
  Data_Get_Struct(obj, SDL_CD, cd);
  if( SDL_CDPause(cd)==-1 )
    rb_raise(eSDLError, "cd pause failed :%s", SDL_GetError());
  return Qnil;
}
static VALUE CD_resume(VALUE obj)
{
  SDL_CD *cd;
  rb_secure(4);
  Data_Get_Struct(obj, SDL_CD, cd);
  if( SDL_CDResume(cd)==-1 )
    rb_raise(eSDLError, "cd resume failed :%s", SDL_GetError());
  return Qnil;
}
static VALUE CD_stop(VALUE obj)
{
  SDL_CD *cd;
  rb_secure(4);
  Data_Get_Struct(obj, SDL_CD, cd);
  if( SDL_CDStop(cd)==-1 )
    rb_raise(eSDLError, "cd pause failed :%s", SDL_GetError());
  return Qnil;
}
static VALUE CD_eject(VALUE obj)
{
  SDL_CD *cd;
  rb_secure(4);
  Data_Get_Struct(obj, SDL_CD, cd);
  if( SDL_CDEject(cd)==-1 )
    rb_raise(eSDLError, "cd eject failed :%s", SDL_GetError());
  return Qnil;
}

static VALUE CD_numTracks(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj, SDL_CD, cd);
  return INT2NUM(cd->numtracks);
}
static VALUE CD_currentTrack(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj, SDL_CD, cd);
  return INT2NUM(cd->cur_track);
}
static VALUE CD_currentFrame(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj, SDL_CD, cd);
  return INT2NUM(cd->cur_frame);
}
static VALUE CD_trackType(VALUE obj, VALUE track)
{
  SDL_CD *cd;
  int index=NUM2INT(track);
  Data_Get_Struct(obj, SDL_CD, cd);
  return INT2FIX(cd->track[index].type);
}
static VALUE CD_trackLength(VALUE obj, VALUE track)
{
  SDL_CD *cd;
  int index=NUM2INT(track);
  Data_Get_Struct(obj, SDL_CD, cd);
  return INT2FIX(cd->track[index].length);
}

void rubysdl_init_CD()
{
  VALUE cCD = rb_define_class_under(mSDL, "CD", rb_cObject);
  rb_define_singleton_method(cCD, "numDrive", CD_s_numDrive, 0);
  rb_define_singleton_method(cCD, "indexName", CD_s_name, 1);
  rb_define_singleton_method(cCD, "open", CD_s_open, 1);
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

  
  rb_define_const(cCD, "TRAYEMPTY", INT2NUM(CD_TRAYEMPTY));
  rb_define_const(cCD, "STOPPED", INT2NUM(CD_STOPPED));
  rb_define_const(cCD, "PLAYING", INT2NUM(CD_PLAYING));
  rb_define_const(cCD, "PAUSED", INT2NUM(CD_PAUSED));
  rb_define_const(cCD, "ERROR", INT2NUM(CD_ERROR));
  
  rb_define_const(cCD, "AUDIO_TRACK", UINT2NUM(SDL_AUDIO_TRACK));
  rb_define_const(cCD, "DATA_TRACK", UINT2NUM(SDL_DATA_TRACK));

}
