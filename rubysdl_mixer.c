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
#ifdef HAVE_SDL_MIXER

#include "rubysdl.h"
#include <SDL_mixer.h>

static int mix_opened=0;

static VALUE mMixer;
static VALUE cWave;
static VALUE cMusic;

static VALUE playing_wave = Qnil;
static VALUE playing_music = Qnil;

static Mix_Chunk* Get_Mix_Chunk(VALUE obj)
{
  Mix_Chunk* chunk;
  
  if( !rb_obj_is_kind_of(obj, cWave) )
    rb_raise(rb_eTypeError, "wrong argument type %s (expected SDL::Wave)",
             rb_obj_classname(obj));
  
  Data_Get_Struct(obj, Mix_Chunk, chunk);
  return chunk;
}

static Mix_Music* Get_Mix_Music(VALUE obj)
{
  Mix_Music* mus;

  if( !rb_obj_is_kind_of(obj, cMusic) )
    rb_raise(rb_eTypeError, "wrong argument type %s (expected SDL::Music)",
             rb_obj_classname(obj));
  Data_Get_Struct(obj, Mix_Music, mus);
  return mus;
}

static void mix_FreeChunk(Mix_Chunk *chunk)
{
  if( ! rubysdl_is_quit() ){
    Mix_FreeChunk( chunk );
  }
}

static void mix_FreeMusic(Mix_Music *music)
{
  if( !rubysdl_is_quit() ){
    Mix_FreeMusic( music );
  }
}

static VALUE Mixer_s_open(VALUE mod, VALUE frequency, VALUE format,
			   VALUE channels, VALUE chunksize)
{
  rb_secure(4);
  
  if( mix_opened ){
    rb_raise(eSDLError, "already initialize SDL::Mixer");
  }
  if( Mix_OpenAudio( NUM2INT(frequency), NUM2UINT(format), NUM2INT(channels), 
		     NUM2INT(chunksize) ) < 0 ){
    rb_raise(eSDLError, "Couldn't open audio: %s", SDL_GetError());
  }

  mix_opened = 1;
  return Qnil;
}

static VALUE Mixer_s_spec(VALUE mod)
{
  int rate;
  Uint16 format;
  int channels;

  if( !Mix_QuerySpec(&rate, &format, &channels) )
    rb_raise(eSDLError, "audio have not been opened yet");
  return rb_ary_new3(3, INT2NUM(rate), UINT2NUM(format), INT2NUM(channels));
}

static VALUE Mixer_s_allocateChannels(VALUE mod, VALUE numchannels)
{
  rb_secure(4);
  return INT2FIX( Mix_AllocateChannels(NUM2INT(numchannels)) );  
}

/* Returns which channel was used to play the sound. */
static VALUE Mixer_s_playChannel(VALUE mod, VALUE channel, VALUE wave, VALUE loops)
{
  int play_channel;
  rb_secure(4);
  
  play_channel = Mix_PlayChannel(NUM2INT(channel), Get_Mix_Chunk(wave),
                                 NUM2INT(loops));
                            
  if( play_channel == -1 ){
    rb_raise(eSDLError,  "couldn't play wave");
  }

  rb_ary_store(playing_wave, play_channel, wave);/* to avoid gc problem */
  return INT2FIX(play_channel);
}

static VALUE Mixer_s_play_p(VALUE mod, VALUE channel)
{
  return INT2BOOL(Mix_Playing(NUM2INT(channel)));
}
  
static VALUE Wave_s_load(VALUE class, VALUE filename)
{
  Mix_Chunk *wave;
  rb_secure(4);
  SafeStringValue(filename);
  
  wave = Mix_LoadWAV(RSTRING(filename)->ptr);
  if( wave == NULL ){
    rb_raise(eSDLError, "Couldn't load wave file %s: %s",
             RSTRING(filename)->ptr, SDL_GetError());
  }
  return Data_Wrap_Struct(class, 0, mix_FreeChunk, wave);
}

/* Volume setting functions and methods : volume in 0..128 */
static VALUE Mixer_s_setVolume(VALUE mod, VALUE channel, VALUE volume)
{
  rb_secure(4);
  return INT2FIX(Mix_Volume(NUM2INT(channel), NUM2INT(volume)));
}

static VALUE Wave_s_setVolume(VALUE self, VALUE volume)
{
  return INT2FIX(Mix_VolumeChunk(Get_Mix_Chunk(self), NUM2INT(volume)));
}

/* Halt,Pause function */
static VALUE Mixer_s_halt(VALUE mod, VALUE channel)
{
  rb_secure(4);
  Mix_HaltChannel(NUM2INT(channel));
  return Qnil;
}

static VALUE Mixer_s_pause(VALUE mod, VALUE channel)
{
  rb_secure(4);
  Mix_Pause(NUM2INT(channel));
  return Qnil;
}

static VALUE Mixer_s_resume(VALUE mod, VALUE channel)
{
  rb_secure(4);
  Mix_Resume(NUM2INT(channel));
  return Qnil;
}

static VALUE Mixer_s_pause_p(VALUE mod, VALUE channel)
{
  rb_secure(4);
  return Mix_Paused(NUM2INT(channel));
}

/* music functions */
#define MakeSimpleRubyFunc(rubyFunc, sdlFunc) \
static VALUE rubyFunc(VALUE mod) \
{ \
  rb_secure(4); \
  sdlFunc(); \
  return Qnil; \
} \

static VALUE Mixer_s_playMusic(VALUE mod, VALUE music, VALUE loops)
{
  rb_secure(4);

  Mix_PlayMusic(Get_Mix_Music(music), NUM2INT(loops));
  playing_music = music; /* to avoid gc problem */
  return Qnil;
}

static VALUE Mixer_s_fadeInMusic(VALUE mod, VALUE music, VALUE loops, VALUE ms)
{
  rb_secure(4);
  Mix_FadeInMusic(Get_Mix_Music(music), NUM2INT(loops), NUM2INT(ms));
  playing_music = music; /* to avoid gc problem */
  return Qnil;
}

static VALUE Mixer_s_setVolumeMusic(VALUE mod, VALUE volume)
{
  rb_secure(4);
  Mix_VolumeMusic( NUM2INT(volume) );
  return Qnil;
}

static VALUE Mixer_s_fadeOutMusic(VALUE mod, VALUE ms)
{
  rb_secure(4);
  Mix_FadeOutMusic(NUM2INT(ms));
  return Qnil;
}

MakeSimpleRubyFunc(Mixer_s_haltMusic, Mix_HaltMusic)
MakeSimpleRubyFunc(Mixer_s_pauseMusic, Mix_PauseMusic)
MakeSimpleRubyFunc(Mixer_s_resumeMusic, Mix_ResumeMusic)
MakeSimpleRubyFunc(Mixer_s_rewindMusic, Mix_RewindMusic)

static VALUE Mixer_s_pauseMusic_p(VALUE mod)
{
  return INT2BOOL(Mix_PausedMusic());
}

static VALUE Mixer_s_playMusic_p(VALUE mod)
{
  return INT2BOOL(Mix_PlayingMusic());
}

static VALUE Music_s_load(VALUE class, VALUE filename)
{
  Mix_Music* music;
  
  rb_secure(4);
  SafeStringValue(filename);
  
  music = Mix_LoadMUS(RSTRING(filename)->ptr);
  if( music == NULL )
    rb_raise(eSDLError, 
	     "Couldn't load %s: %s", GETCSTR(filename), SDL_GetError());
  return Data_Wrap_Struct(class, 0, mix_FreeMusic, music);
}


void rubysdl_init_Mixer()
{
  mMixer = rb_define_module_under(mSDL, "Mixer");
  rb_define_module_function(mMixer, "open", Mixer_s_open, 4);
  rb_define_module_function(mMixer, "spec", Mixer_s_spec, 0);
  rb_define_module_function(mMixer, "playChannel", Mixer_s_playChannel, 3);
  rb_define_module_function(mMixer, "play?", Mixer_s_play_p, 1);
  rb_define_module_function(mMixer, "setVolume", Mixer_s_setVolume, 2);
  rb_define_module_function(mMixer, "allocateChannels", Mixer_s_allocateChannels, 1);
  rb_define_module_function(mMixer, "halt", Mixer_s_halt, 1);
  rb_define_module_function(mMixer, "pause", Mixer_s_pause, 1);
  rb_define_module_function(mMixer, "resume", Mixer_s_resume, 1);
  rb_define_module_function(mMixer, "pause?", Mixer_s_pause_p, 1);

  rb_define_module_function(mMixer, "playMusic", Mixer_s_playMusic, 2);
  rb_define_module_function(mMixer, "fadeInMusic", Mixer_s_fadeInMusic, 3);
  rb_define_module_function(mMixer, "setVolumeMusic", Mixer_s_setVolumeMusic, 1);
  rb_define_module_function(mMixer, "haltMusic", Mixer_s_haltMusic, 0);
  rb_define_module_function(mMixer, "fadeOutMusic", Mixer_s_fadeOutMusic, 1);
  rb_define_module_function(mMixer, "pauseMusic", Mixer_s_pauseMusic, 0);
  rb_define_module_function(mMixer, "resumeMusic", Mixer_s_resumeMusic, 0);
  rb_define_module_function(mMixer, "rewindMusic", Mixer_s_rewindMusic, 0);
  rb_define_module_function(mMixer, "pauseMusic?", Mixer_s_pauseMusic_p, 0);
  rb_define_module_function(mMixer, "playMusic?", Mixer_s_playMusic_p, 0);
  
  cWave = rb_define_class_under(mMixer, "Wave", rb_cObject);
  rb_define_singleton_method(cWave, "load", Wave_s_load, 1);
  rb_define_method(cWave, "setVolume", Wave_s_setVolume, 1);

  cMusic = rb_define_class_under(mMixer, "Music", rb_cObject);
  rb_define_singleton_method(cMusic, "load", Music_s_load, 1);

  /* to avoid to do garbage collect when playing */
  rb_global_variable( &playing_wave );
  rb_global_variable( &playing_music );
  playing_wave = rb_ary_new();

  rb_define_const(mMixer, "FORMAT_U8", UINT2NUM(AUDIO_U8));
  rb_define_const(mMixer, "FORMAT_S8", UINT2NUM(AUDIO_S8));
  rb_define_const(mMixer, "FORMAT_U16LSB", UINT2NUM(AUDIO_U16LSB));
  rb_define_const(mMixer, "FORMAT_S16LSB", UINT2NUM(AUDIO_S16LSB));
  rb_define_const(mMixer, "FORMAT_U16MSB", UINT2NUM(AUDIO_U16MSB));
  rb_define_const(mMixer, "FORMAT_S16MSB", UINT2NUM(AUDIO_S16MSB));
  rb_define_const(mMixer, "FORMAT_U16", UINT2NUM(AUDIO_U16));
  rb_define_const(mMixer, "FORMAT_S16", UINT2NUM(AUDIO_S16));

  rb_define_const(mMixer, "FORMAT_U16SYS", UINT2NUM(AUDIO_U16SYS));
  rb_define_const(mMixer, "FORMAT_S16SYS", UINT2NUM(AUDIO_S16SYS));

  rb_define_const(mMixer, "CHANNELS", INT2NUM(MIX_CHANNELS));
  rb_define_const(mMixer, "DEFAULT_FREQUENCY", INT2NUM(MIX_DEFAULT_FREQUENCY));
  rb_define_const(mMixer, "DEFAULT_FORMAT", UINT2NUM(MIX_DEFAULT_FORMAT));
  rb_define_const(mMixer, "DEFAULT_CHANNELS", UINT2NUM(MIX_DEFAULT_CHANNELS));
  rb_define_const(mMixer, "MAX_VOLUME", INT2NUM(MIX_MAX_VOLUME));

  return;
}

void rubysdl_quit_Mixer()
{
  if( mix_opened ){
    Mix_CloseAudio();
  }
}
#endif  /* HAVE_SDL_MIXER */
