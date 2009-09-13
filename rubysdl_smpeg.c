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
#ifdef HAVE_SMPEG
#include "rubysdl.h"
#include <smpeg.h>

#ifdef HAVE_SDL_MIXER
#include "SDL_mixer.h"
#endif

static VALUE cMPEG;
static VALUE cMPEGInfo;

static SMPEG_Filter* filters[3];
#define NULL_FILTER 0
#define BILINEAR_FILTER 1
#define DEBLOCKING_FILTER 2
#define NUM_FILTERS 3

typedef struct{
  SMPEG* smpeg;
  int audio_enable;
}MPEG;

DEFINE_GET_STRUCT(MPEG, Get_MPEG, cMPEG, "SDL::MPEG");

static SMPEG* Get_SMPEG(VALUE self)
{
  MPEG* mpeg = Get_MPEG(self);
  if(mpeg->smpeg)
    return mpeg->smpeg;
  else
    rb_raise(eSDLError, "MPEG is already deleted");
  /* NOT REACHED */
}

static void MPEG_free(MPEG* mpg)
{
  if(!rubysdl_is_quit() && mpg->smpeg)
    SMPEG_delete(mpg->smpeg);
  free(mpg);
}

static VALUE MPEG_s_alloc(VALUE klass)
{
  MPEG* mpg = ALLOC(MPEG);
  mpg->smpeg = NULL;
  mpg->audio_enable = 1;
  return Data_Wrap_Struct(klass, 0, MPEG_free, mpg);
}

static VALUE MPEG_create(SMPEG* smpeg)
{
  VALUE newobj = MPEG_s_alloc(cMPEG);
  Get_MPEG(newobj)->smpeg = smpeg;
  return newobj;
}

static VALUE MPEG_delete(VALUE self)
{
  MPEG* mpg = Get_MPEG(self);
  SMPEG_delete(mpg->smpeg);
  mpg->smpeg = NULL;
  return Qnil;
}

static VALUE MPEG_deleted(VALUE self)
{
  return INT2BOOL(Get_MPEG(self)->smpeg == NULL);
}

static VALUE MPEG_s_load(VALUE klass, VALUE filename)
{
  SMPEG *smpeg;
  char error_msg[2048];
  
  rb_secure(4);
  SafeStringValue(filename);
  
  smpeg = SMPEG_new(RSTRING_PTR(filename), NULL, 0);
  if( SMPEG_error(smpeg) ){
    snprintf(error_msg, sizeof(error_msg), "Couldn't load %s: %s", 
	     RSTRING_PTR(filename), SMPEG_error(smpeg));
    SMPEG_delete(smpeg);
    rb_raise(eSDLError, "%s", error_msg);
  }
  return MPEG_create(smpeg);
}

static VALUE MPEGInfo_create(SMPEG_Info info)
{
  VALUE obj = rb_obj_alloc(cMPEGInfo);
  
  rb_iv_set(obj, "@has_audio", INT2BOOL(info.has_audio));
  rb_iv_set(obj, "@has_video", INT2BOOL(info.has_video));
  rb_iv_set(obj, "@width", INT2NUM(info.width));
  rb_iv_set(obj, "@height", INT2NUM(info.height));
  rb_iv_set(obj, "@current_frame", INT2NUM(info.current_frame));
  rb_iv_set(obj, "@current_fps", INT2NUM(info.current_fps));
  rb_iv_set(obj, "@audio_string", rb_str_new2(info.audio_string));
  rb_iv_set(obj, "@audio_current_frame", INT2NUM(info.audio_current_frame));
  rb_iv_set(obj, "@current_offset", UINT2NUM(info.current_offset));
  rb_iv_set(obj, "@total_size", UINT2NUM(info.total_size));
  rb_iv_set(obj, "@current_time", UINT2NUM(info.current_time));
  rb_iv_set(obj, "@total_time", UINT2NUM(info.total_time));
  return obj;
}


static VALUE MPEG_info(VALUE self)
{
  SMPEG_Info info;
  
  SMPEG_getinfo(Get_SMPEG(self), &info);
  return MPEGInfo_create(info);
}

static VALUE MPEG_enableAudio(VALUE self, VALUE enable)
{
  Get_MPEG(self)->audio_enable = RTEST(enable);
  return Qnil;
}

static VALUE MPEG_enableVideo(VALUE self, VALUE enable)
{
  SMPEG_enablevideo(Get_SMPEG(self), RTEST(enable));
  return Qnil;
}

static VALUE MPEG_status(VALUE self)
{
  return INT2FIX(SMPEG_status(Get_SMPEG(self)));
}

static VALUE MPEG_setVolume(VALUE self, VALUE volume)
{
  SMPEG_setvolume(Get_SMPEG(self), NUM2INT(volume));
  return Qnil;
}

static VALUE MPEG_setDisplay(VALUE self, VALUE dst)
{
  SMPEG_setdisplay(Get_SMPEG(self),
                   Get_SDL_Surface(dst), NULL, NULL);
  rb_iv_set(self, "display", dst);
  return Qnil;
}

static VALUE MPEG_setLoop(VALUE self, VALUE repeat)
{
  SMPEG_loop(Get_SMPEG(self), RTEST(repeat));
  return Qnil;
}

static VALUE MPEG_scaleXY(VALUE self, VALUE w, VALUE h)
{
  SMPEG_scaleXY(Get_SMPEG(self), NUM2INT(w), NUM2INT(h));
  return Qnil;
}

static VALUE MPEG_scale(VALUE self, VALUE scale)
{
  SMPEG_scale(Get_SMPEG(self), NUM2INT(scale));
  return Qnil;
}

static VALUE MPEG_move(VALUE self, VALUE x, VALUE y)
{
  SMPEG_move(Get_SMPEG(self), NUM2INT(x), NUM2INT(y));
  return Qnil;
}

static VALUE MPEG_setDisplayRegion(VALUE self, VALUE x, VALUE y, VALUE w, 
				    VALUE h)
{
  SMPEG_setdisplayregion(Get_SMPEG(self), NUM2INT(x), NUM2INT(y),
                         NUM2INT(w), NUM2INT(h));
  return Qnil;
}

static VALUE MPEG_play(VALUE self)
{
  SMPEG *mpeg = Get_SMPEG(self);
  int use_audio;
  
#ifdef HAVE_SDL_MIXER
  use_audio = Get_MPEG(self)->audio_enable && Mix_QuerySpec(NULL, NULL, NULL);
  if( use_audio ){
    SDL_AudioSpec audiofmt;
    Uint16 format;
    int freq,  channels;
    
    SMPEG_enableaudio(mpeg,  0);
    /* Tell SMPEG what the audio format is */
    Mix_QuerySpec(&freq,  &format,  &channels);
    audiofmt.format = format;
    audiofmt.freq = freq;
    audiofmt.channels = channels;
    SMPEG_actualSpec(mpeg,  &audiofmt);

    /* Hook in the MPEG music mixer */
    Mix_HookMusic(NULL, NULL);
    Mix_HookMusic(SMPEG_playAudioSDL,  mpeg);
    SMPEG_enableaudio(mpeg,  1);
  }
#else
  SMPEG_enableaudio(mpeg, Get_MPEG(self)->audio_enable);
#endif
  
  SMPEG_play(mpeg);
  return Qnil;
}

static VALUE MPEG_pause(VALUE self)
{
  SMPEG_pause(Get_SMPEG(self));
  return Qnil;
}

static VALUE MPEG_stop(VALUE self)
{
  SMPEG_stop(Get_SMPEG(self));
#ifdef HAVE_SDL_MIXER
  Mix_HookMusic(NULL,NULL);
#endif
  return Qnil;
}

static VALUE MPEG_rewind(VALUE self)
{
  SMPEG_rewind(Get_SMPEG(self));
  return Qnil;
}

static VALUE MPEG_seek(VALUE self, VALUE bytes)
{
  SMPEG_seek(Get_SMPEG(self), NUM2INT(bytes));
  return Qnil;
}

static VALUE MPEG_skip(VALUE self, VALUE seconds)
{
  SMPEG_skip(Get_SMPEG(self), NUM2DBL(seconds));
  return Qnil;
}

static VALUE MPEG_renderFrame(VALUE self, VALUE framenum)
{
  SMPEG_renderFrame(Get_SMPEG(self), NUM2INT(framenum));
  return Qnil;
}

static VALUE MPEG_renderFinal(VALUE self, VALUE dst,
                              VALUE x, VALUE y)
{
  SMPEG_renderFinal(Get_SMPEG(self), Get_SDL_Surface(dst),
                    NUM2INT(x), NUM2INT(y));
  return Qnil;
}

static VALUE MPEG_setFilter(VALUE self, VALUE filter)
{
  if( (NUM2INT(filter)<0) || (NUM2INT(filter)>=NUM_FILTERS) )
    rb_raise(eSDLError, "There isn't that filter");
  SMPEG_filter(Get_SMPEG(self), filters[NUM2INT(filter)]);
  return Qnil;
}

void rubysdl_init_MPEG(VALUE mSDL)
{
  cMPEG = rb_define_class_under(mSDL, "MPEG", rb_cObject);
  cMPEGInfo = rb_define_class_under(cMPEG, "Info", rb_cObject);

  rb_define_alloc_func(cMPEG, MPEG_s_alloc);
  
  filters[NULL_FILTER] = SMPEGfilter_null();
  filters[BILINEAR_FILTER] = SMPEGfilter_bilinear();
  filters[DEBLOCKING_FILTER] = SMPEGfilter_deblocking();
  
  rb_define_attr(cMPEGInfo, "has_audio", 1, 0);
  rb_define_attr(cMPEGInfo, "has_video", 1, 0);
  rb_define_attr(cMPEGInfo, "width", 1, 0);
  rb_define_attr(cMPEGInfo, "height", 1, 0);
  rb_define_attr(cMPEGInfo, "current_frame", 1, 0);
  rb_define_attr(cMPEGInfo, "current_fps", 1, 0);
  rb_define_attr(cMPEGInfo, "audio_string", 1, 0);
  rb_define_attr(cMPEGInfo, "audio_current_frame", 1, 0);
  rb_define_attr(cMPEGInfo, "current_offset", 1, 0);
  rb_define_attr(cMPEGInfo, "total_size", 1, 0);
  rb_define_attr(cMPEGInfo, "current_time", 1, 0);
  rb_define_attr(cMPEGInfo, "total_time", 1, 0);

  rb_define_singleton_method(cMPEG, "load", MPEG_s_load, 1);

  rb_define_method(cMPEG, "info", MPEG_info, 0);
  rb_define_method(cMPEG, "delete", MPEG_delete, 0);
  rb_define_method(cMPEG, "deleted?", MPEG_deleted, 0);
  rb_define_method(cMPEG, "enableAudio", MPEG_enableAudio, 1);
  rb_define_method(cMPEG, "enableVideo", MPEG_enableVideo, 1);
  rb_define_method(cMPEG, "status", MPEG_status, 0);
  rb_define_method(cMPEG, "setVolume", MPEG_setVolume, 1);
  rb_define_method(cMPEG, "setDisplay", MPEG_setDisplay, 1);
  rb_define_method(cMPEG, "setLoop", MPEG_setLoop, 1);
  rb_define_method(cMPEG, "scaleXY", MPEG_scaleXY, 2);
  rb_define_method(cMPEG, "scale", MPEG_scale, 1);
  rb_define_method(cMPEG, "move", MPEG_move, 2);
  rb_define_method(cMPEG, "setDisplayRegion", MPEG_setDisplayRegion, 4);
  rb_define_method(cMPEG, "play", MPEG_play, 0);
  rb_define_method(cMPEG, "pause", MPEG_pause, 0);
  rb_define_method(cMPEG, "stop", MPEG_stop, 0);
  rb_define_method(cMPEG, "rewind", MPEG_rewind, 0);
  rb_define_method(cMPEG, "seek", MPEG_seek, 1);
  rb_define_method(cMPEG, "skip", MPEG_skip, 1);
  rb_define_method(cMPEG, "renderFrame", MPEG_renderFrame, 1);
  rb_define_method(cMPEG, "renderFinal", MPEG_renderFinal, 3);
  rb_define_method(cMPEG, "setFilter", MPEG_setFilter, 1);

  rb_define_const(cMPEG, "ERROR", INT2FIX(SMPEG_ERROR));
  rb_define_const(cMPEG, "STOPPED", INT2FIX(SMPEG_STOPPED));
  rb_define_const(cMPEG, "PLAYING", INT2FIX(SMPEG_PLAYING));
  rb_define_const(cMPEG, "NULL_FILTER", INT2FIX(NULL_FILTER));
  rb_define_const(cMPEG, "BILINEAR_FILTER", INT2FIX(BILINEAR_FILTER));
  rb_define_const(cMPEG, "DEBLOCKING_FILTER", INT2FIX(DEBLOCKING_FILTER));
}
#else
#include "rubysdl.h"
void rubysdl_init_MPEG(VALUE mSDL)
{
}
#endif /* HAVE_SMPEG */
