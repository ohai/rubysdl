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
#ifdef HAVE_SDL_MIXER

#include "rubysdl.h"
#include <SDL_mixer.h>

static int mix_opened=0;

static VALUE playing_wave[MIX_CHANNELS] =
{ Qnil,Qnil,Qnil,Qnil,Qnil,Qnil,Qnil,Qnil };
static VALUE playing_music=Qnil;

static VALUE mix_openAudio(VALUE mod,VALUE frequency,VALUE format,
			   VALUE channels,VALUE chunksize)
{
  int i;
  if( mix_opened ){
    rb_raise(eSDLError,"already initialize SDL::Mixer");
  }
  if( Mix_OpenAudio( NUM2INT(frequency),NUM2UINT(format),NUM2INT(channels),
		     NUM2INT(chunksize) ) < 0 ){
    rb_raise(eSDLError,"Couldn't open audio: %s",SDL_GetError());
  }

  /* to avoid to do garbage collect when playing */
  for( i=0; i<MIX_CHANNELS; ++i)
    rb_global_variable( &(playing_wave[i]) );
  rb_global_variable( &playing_music );
  
  return Qnil;
}

static VALUE mix_querySpec(VALUE mod)
{
  int rate;
  Uint16 format;
  int channels;

  if( !Mix_QuerySpec(&rate,&format,&channels) )
    rb_raise(eSDLError,"audio have not been opened yet");
  return rb_ary_new3( 3,INT2NUM(rate),UINT2NUM(format),INT2NUM(channels) );
}
  
/* Returns which channel was used to play the sound. */
static VALUE mix_playChannel(VALUE mod,VALUE channel,VALUE wave,VALUE loops)
{
  Mix_Chunk *chunk;
  if( ! rb_obj_is_kind_of(wave,cWave) )
    rb_raise(rb_eArgError,"type mismatch");
  Data_Get_Struct(wave,Mix_Chunk,chunk);
  playing_wave[NUM2INT(channel)]=wave; /* to avoid gc problem */
  return INT2FIX( Mix_PlayChannel(NUM2INT(channel),chunk,NUM2INT(loops) ) );
}

static VALUE mix_playing(VALUE mod,VALUE channel)
{
  if( Mix_Playing( NUM2INT(channel) ) ){
    return Qtrue;
  }else{
    return Qfalse;
  }
}
  
static VALUE mix_loadWav(VALUE class,VALUE filename)
{
  Mix_Chunk *wave;
  Check_Type(filename,T_STRING);
  wave = Mix_LoadWAV( STR2CSTR(filename) );
  if( wave == NULL ){
    rb_raise( eSDLError,"Couldn't load wave file %s: %s",
	      STR2CSTR(filename),SDL_GetError() );
  }
  return Data_Wrap_Struct(class,0,Mix_FreeChunk,wave);
}

/* Volume setting functions and methods : volume in 0..128 */
static VALUE mix_volume(VALUE mod,VALUE channel,VALUE volume)
{
  return INT2FIX( Mix_Volume( NUM2INT(channel),NUM2INT(volume) ) );
}
static VALUE mix_wave_volume(VALUE obj,VALUE volume)
{
  Mix_Chunk *chunk;
  Data_Get_Struct(obj,Mix_Chunk,chunk);
  return INT2FIX( Mix_VolumeChunk( chunk,NUM2INT(volume) ) );
}

/* Halt,Pause function */
static VALUE mix_halt(VALUE mod,VALUE channel)
{
  Mix_HaltChannel(NUM2INT(channel));
  return Qnil;
}
static VALUE mix_pause(VALUE mod,VALUE channel)
{
  Mix_Pause(NUM2INT(channel));
  return Qnil;
}
static VALUE mix_resume(VALUE mod,VALUE channel)
{
  Mix_Resume(NUM2INT(channel));
  return Qnil;
}
static VALUE mix_paused(VALUE mod,VALUE channel)
{
  return Mix_Paused(NUM2INT(channel));
}

/* music functions */
#define MakeSimpleRubyFunc(rubyFunc,sdlFunc) \
static VALUE rubyFunc(VALUE mod) \
{ \
  sdlFunc(); \
  return Qnil; \
} \

static VALUE mix_playMusic(VALUE mod,VALUE music,VALUE loops)
{
  Mix_Music *mus;
  if( ! rb_obj_is_kind_of(music,cMusic) )
    rb_raise(rb_eArgError,"type mismatch");
  Data_Get_Struct(music,Mix_Music,mus);
  playing_music=music; /* to avoid gc problem */
  Mix_PlayMusic(mus,NUM2INT(loops));
  return Qnil;
}

static VALUE mix_fadeInMusic(VALUE mod,VALUE music,VALUE loops,VALUE ms)
{
  Mix_Music *mus;
  if( ! rb_obj_is_kind_of(music,cMusic) )
    rb_raise(rb_eArgError,"type mismatch");
  Data_Get_Struct(music,Mix_Music,mus);
  Mix_FadeInMusic(mus,NUM2INT(loops),NUM2INT(ms));
  return Qnil;
}

static VALUE mix_setVolumeMusic(VALUE mod,VALUE volume)
{
  Mix_VolumeMusic( NUM2INT(volume) );
}

static VALUE mix_fadeOutMusic(VALUE mod,VALUE ms)
{
  Mix_FadeOutMusic(NUM2INT(ms));
  return Qnil;
}

MakeSimpleRubyFunc(mix_haltMusic,Mix_HaltMusic)
MakeSimpleRubyFunc(mix_pauseMusic,Mix_PauseMusic)
MakeSimpleRubyFunc(mix_resumeMusic,Mix_ResumeMusic)
MakeSimpleRubyFunc(mix_rewindMusic,Mix_RewindMusic)

static VALUE mix_pausedMusic(VALUE mod)
{
  return BOOL(Mix_PausedMusic());
}

static VALUE mix_playingMusic(VALUE mod)
{
  return BOOL(Mix_PlayingMusic());
}

static VALUE mix_loadMus(VALUE class,VALUE filename)
{
  Mix_Music* music;
  music = Mix_LoadMUS(STR2CSTR(filename));
  if( music == NULL )
    rb_raise(eSDLError,
	     "Couldn't load %s: %s",STR2CSTR(filename),SDL_GetError());
  return Data_Wrap_Struct(class,0,Mix_FreeMusic,music);
}
static void defineConstForAudio()
{
  rb_define_const(mMixer,"FORMAT_U8",UINT2NUM(AUDIO_U8));
  rb_define_const(mMixer,"FORMAT_S8",UINT2NUM(AUDIO_S8));
  rb_define_const(mMixer,"FORMAT_U16LSB",UINT2NUM(AUDIO_U16LSB));
  rb_define_const(mMixer,"FORMAT_S16LSB",UINT2NUM(AUDIO_S16LSB));
  rb_define_const(mMixer,"FORMAT_U16MSB",UINT2NUM(AUDIO_U16MSB));
  rb_define_const(mMixer,"FORMAT_S16MSB",UINT2NUM(AUDIO_S16MSB));
  rb_define_const(mMixer,"FORMAT_U16",UINT2NUM(AUDIO_U16));
  rb_define_const(mMixer,"FORMAT_S16",UINT2NUM(AUDIO_S16));

  rb_define_const(mMixer,"FORMAT_U16SYS",UINT2NUM(AUDIO_U16SYS));
  rb_define_const(mMixer,"FORMAT_S16SYS",UINT2NUM(AUDIO_S16SYS));

  rb_define_const(mMixer,"CHANNELS",INT2NUM(MIX_CHANNELS));
  rb_define_const(mMixer,"DEFAULT_FREQUENCY",INT2NUM(MIX_DEFAULT_FREQUENCY));
  rb_define_const(mMixer,"DEFAULT_FORMAT",UINT2NUM(MIX_DEFAULT_FORMAT));
  rb_define_const(mMixer,"DEFAULT_CHANNELS",UINT2NUM(MIX_DEFAULT_CHANNELS));
  rb_define_const(mMixer,"MAX_VOLUME",INT2NUM(MIX_MAX_VOLUME));

}

void init_mixer()
{
  mMixer = rb_define_module_under(mSDL,"Mixer");
  rb_define_module_function(mMixer,"open",mix_openAudio,4);
  rb_define_module_function(mMixer,"spec",mix_querySpec,0);
  rb_define_module_function(mMixer,"playChannel",mix_playChannel,3);
  rb_define_module_function(mMixer,"play?",mix_playing,1);
  rb_define_module_function(mMixer,"setVolume",mix_volume,2);
  rb_define_module_function(mMixer,"halt",mix_halt,1);
  rb_define_module_function(mMixer,"pause",mix_pause,1);
  rb_define_module_function(mMixer,"resume",mix_resume,1);
  rb_define_module_function(mMixer,"pause?",mix_paused,1);

  rb_define_module_function(mMixer,"playMusic",mix_playMusic,2);
  rb_define_module_function(mMixer,"fadeInMusic",mix_fadeInMusic,3);
  rb_define_module_function(mMixer,"setVolumeMusic",mix_setVolumeMusic,1);
  rb_define_module_function(mMixer,"haltMusic",mix_haltMusic,0);
  rb_define_module_function(mMixer,"fadeOutMusic",mix_fadeOutMusic,1);
  rb_define_module_function(mMixer,"pauseMusic",mix_pauseMusic,0);
  rb_define_module_function(mMixer,"resumeMusic",mix_resumeMusic,0);
  rb_define_module_function(mMixer,"rewindMusic",mix_rewindMusic,0);
  rb_define_module_function(mMixer,"pauseMusic?",mix_pausedMusic,0);
  rb_define_module_function(mMixer,"playMusic?",mix_playingMusic,0);
  
  cWave = rb_define_class_under(mMixer,"Wave",rb_cObject);
  rb_define_singleton_method(cWave,"load",mix_loadWav,1);
  rb_define_method(cWave,"setVolume",mix_wave_volume,1);

  cMusic = rb_define_class_under(mMixer,"Music",rb_cObject);
  rb_define_singleton_method(cMusic,"load",mix_loadMus,1);
  
  defineConstForAudio();
  return ;
}

void quit_mixer()
{
  if( mix_opened ){
    Mix_CloseAudio();
  }
}
#endif  /* HAVE_SDL_MIXER */
