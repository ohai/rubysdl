#ifdef HAVE_SDL_MIXER

#include "rubysdl.h"
#include <SDL_mixer.h>

static VALUE mixer=Qnil;

static VALUE mix_freeAudio(void* audio)
{
  Mix_CloseAudio();
}

static VALUE mix_openAudio(VALUE class,VALUE frequency,VALUE format,
			   VALUE channels,VALUE chunksize)
{
  if( mixer != Qnil ){
    /* rb_raise(); */
  }
  if( Mix_OpenAudio( NUM2INT(frequency),NUM2INT(format),NUM2INT(channels),
		     NUM2INT(chunksize) ) != 1){
    /* rb_raise(); */
  }
  mixer = Data_Wrap_Struct(class,0,mix_freeAudio,NULL);
  rb_global_variable(&mixer);
  return mixer;
}
static VALUE mix_instance(VALUE class)
{
  if(mixer==Qnil){
    /* rb_raise(); */
  }else{
    return mixer;
  }
}

/* Returns which channel was used to play the sound. */
static VALUE mix_playChannel(VALUE obj,VALUE channel,VALUE wave,VALUE looping)
{
  Mix_Chunk *chunk;
  Data_Get_Struct(wave,Mix_Chunk,chunk);
  return INT2FIX( Mix_PlayChannel(NUM2INT(channel),chunk,looping) );
}

/*  need changeing */
static VALUE mix_playing(VALUE obj,VALUE channel)
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
  wave = Mix_LoadWAV(STR2CSTR(filename));
  if( wave == NULL ){
    /* rb_raise("not find file"); */
  }
  return Data_Wrap_Struct(class,0,Mix_FreeChunk,wave);
}
void init_mixer()
{
  cMixer = rb_define_class_under(mSDL,"Mixer",rb_cObject);
  rb_define_singleton_method(cMixer,"open",mix_openAudio,4);
  rb_define_singleton_method(cMixer,"instance",mix_instance,0);
  rb_define_method(cMixer,"close",mix_freeAudio,0);

  rb_define_method(cMixer,"playChannel",mix_playChannel,3);
  rb_define_method(cMixer,"play?",mix_playing,1);
  
  cWave = rb_define_class_under(cMixer,"Wave",rb_cObject);
  rb_define_singleton_method(cWave,"load",mix_loadWav,1);
  return ;
}

#endif  /* HAVE_SDL_MIXER */
