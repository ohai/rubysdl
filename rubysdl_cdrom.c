#include "rubysdl.h"

static VALUE sdl_cd_numDrive(VALUE class)
{
  return INT2FIX(SDL_CDNumDrives());
}
static VALUE sdl_cd_name(VALUE class,VALUE drive)
{
  return rb_str_new2(SDL_CDName(NUM2INT(drive)));
}
static VALUE sdl_cd_open(VALUE class,VALUE drive)
{
  SDL_CD *cd;
  cd=SDL_CDOpen(NUM2INT(drive));
  if(cd==NULL)
    rb_raise(eSDLError,"Couldn't open drive %d: %s",
	     NUM2INT(drive),SDL_GetError());
  return Data_Wrap_Struct(class,0,SDL_CDClose,cd);
}
static VALUE sdl_cd_status(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  return INT2FIX(SDL_CDStatus(cd));
}
static VALUE sdl_cd_play(VALUE obj,VALUE start,VALUE length)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  if( SDL_CDPlay(cd,NUM2INT(start),NUM2INT(length))==-1 )
    rb_raise(eSDLError,"Couldn't play cd :%s",SDL_GetError() );
  return Qnil;
}
static VALUE sdl_cd_playTracks(VALUE obj,VALUE start_track,VALUE start_frame,
			       VALUE ntracks,VALUE nframes)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  if( SDL_CDPlayTracks(cd,NUM2INT(start_track),NUM2INT(start_frame),
		       NUM2INT(ntracks),NUM2INT(nframes))==-1 )
    rb_raise(eSDLError,"Couldn't play cd :%s",SDL_GetError() );
  return Qnil;
}
static VALUE sdl_cd_pause(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  if( SDL_CDPause(cd)==-1 )
    rb_raise(eSDLError,"cd pause failed :%s",SDL_GetError());
  return Qnil;
}
static VALUE sdl_cd_resume(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  if( SDL_CDResume(cd)==-1 )
    rb_raise(eSDLError,"cd resume failed :%s",SDL_GetError());
  return Qnil;
}
static VALUE sdl_cd_stop(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  if( SDL_CDStop(cd)==-1 )
    rb_raise(eSDLError,"cd pause failed :%s",SDL_GetError());
  return Qnil;
}
static VALUE sdl_cd_eject(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  if( SDL_CDEject(cd)==-1 )
    rb_raise(eSDLError,"cd eject failed :%s",SDL_GetError());
  return Qnil;
}

static VALUE sdl_cd_numTracks(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  return INT2NUM(cd->numtracks);
}
static VALUE sdl_cd_currentTrack(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  return INT2NUM(cd->cur_track);
}
static VALUE sdl_cd_currentFrame(VALUE obj)
{
  SDL_CD *cd;
  Data_Get_Struct(obj,SDL_CD,cd);
  return INT2NUM(cd->cur_frame);
}
static VALUE sdl_cd_trackType(VALUE obj,VALUE track)
{
  SDL_CD *cd;
  int index=NUM2INT(track);
  Data_Get_Struct(obj,SDL_CD,cd);
  return INT2FIX(cd->track[index].type);
}
static VALUE sdl_cd_trackLength(VALUE obj,VALUE track)
{
  SDL_CD *cd;
  int index=NUM2INT(track);
  Data_Get_Struct(obj,SDL_CD,cd);
  return INT2FIX(cd->track[index].length);
}

static void defineConstForCDROM()
{
  rb_define_const(cCD,"TRAYEMPTY",INT2NUM(CD_TRAYEMPTY));
  rb_define_const(cCD,"STOPPED",INT2NUM(CD_STOPPED));
  rb_define_const(cCD,"PLAYING",INT2NUM(CD_PLAYING));
  rb_define_const(cCD,"PAUSED",INT2NUM(CD_PAUSED));
  rb_define_const(cCD,"ERROR",INT2NUM(CD_ERROR));
  
  rb_define_const(cCD,"AUDIO_TRACK",UINT2NUM(SDL_AUDIO_TRACK));
  rb_define_const(cCD,"DATA_TRACK",UINT2NUM(SDL_DATA_TRACK));
}
void init_cdrom()
{
  cCD=rb_define_class_under(mSDL,"CD",rb_cObject);
  rb_define_singleton_method(cCD,"numDrive",sdl_cd_numDrive,0);
  rb_define_singleton_method(cCD,"name",sdl_cd_name,1);
  rb_define_singleton_method(cCD,"open",sdl_cd_open,1);
  rb_define_method(cCD,"status",sdl_cd_status,0);
  rb_define_method(cCD,"play",sdl_cd_play,2);
  rb_define_method(cCD,"playTracks",sdl_cd_playTracks,4);
  rb_define_method(cCD,"pause",sdl_cd_pause,0);
  rb_define_method(cCD,"resume",sdl_cd_resume,0);
  rb_define_method(cCD,"stop",sdl_cd_stop,0);
  rb_define_method(cCD,"eject",sdl_cd_eject,0);

  rb_define_method(cCD,"numTracks",sdl_cd_numTracks,0);
  rb_define_method(cCD,"currentTrack",sdl_cd_currentTrack,0);
  rb_define_method(cCD,"currentFrame",sdl_cd_currentFrame,0);
  rb_define_method(cCD,"trackType",sdl_cd_trackType,1);
  rb_define_method(cCD,"trackLength",sdl_cd_trackLength,1);
  
  defineConstForCDROM();
}
