#ifdef HAVE_SDL_MIXER

#include "rubysdl.h"
#include <SDL_mixer.h>

static int mix_opened=0;

static VALUE mix_openAudio(VALUE mod,VALUE frequency,VALUE format,
			   VALUE channels,VALUE chunksize)
{
  if( mix_opened ){
    rb_raise(eSDLError,"already initialize SDL::Mixer");
  }
  if( Mix_OpenAudio( NUM2INT(frequency),NUM2INT(format),NUM2INT(channels),
		     NUM2INT(chunksize) ) != 1){
    rb_raise(eSDLError,"Couldn't open audio: %s",SDL_GetError());
  }
  return Qnil;
}

/* Returns which channel was used to play the sound. */
static VALUE mix_playChannel(VALUE mod,VALUE channel,VALUE wave,VALUE looping)
{
  Mix_Chunk *chunk;
  if( ! rb_obj_is_kind_of(cWave,wave) )
    rb_raise(rb_eArgError,"type mismatch");
  Data_Get_Struct(wave,Mix_Chunk,chunk);
  return INT2FIX( Mix_PlayChannel(NUM2INT(channel),chunk,(looping)? -1 : 1 ) );
}

/*  need changeing */
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
    rb_raise( eSDLError,"Couldn't load wave file %s: %s"
	      STR2CSTR(filename),SDL_GetError() );
  }
  return Data_Wrap_Struct(class,0,Mix_FreeChunk,wave);
}
void init_mixer()
{
  mMixer = rb_define_module_under(mSDL,"Mixer");
  rb_define_module_function(mMixer,"open",mix_openAudio,4);
  rb_define_module_function(mMixer,"instance",mix_instance,0);
  rb_define_module_function(mMixer,"close",mix_freeAudio,0);

  rb_define_module_function(mMixer,"playChannel",mix_playChannel,3);
  rb_define_module_function(mMixer,"play?",mix_playing,1);
  
  cWave = rb_define_class_under(mMixer,"Wave",rb_cObject);
  rb_define_singleton_method(cWave,"load",mix_loadWav,1);
  return ;
}

void quit_mixer()
{
  if( mix_opened ){
    Mix_CloseAudio();
  }
}
#endif  /* HAVE_SDL_MIXER */
