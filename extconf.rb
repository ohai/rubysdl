require 'mkmf'

sdl_config = with_config('sdl-config', 'sdl-config')

$CFLAGS += ' ' + `#{sdl_config} --cflags`.chomp
$LOCAL_LIBS += ' ' + `#{sdl_config} --libs`.chomp

have_func("TTF_OpenFontIndex","SDL_ttf.h")
have_func("TTF_FontFaces","SDL_ttf.h")
have_func("TTF_FontFaceIsFixedWidth","SDL_ttf.h")
have_func("TTF_FontFaceFamilyName","SDL_ttf.h")
have_func("TTF_FontFaceStyleName","SDL_ttf.h")

if enable_config("event2",true) then
  $CFLAGS+= " -D DEF_EVENT2"
end

if have_library("SDL_mixer","Mix_OpenAudio") then
  $CFLAGS+= " -D HAVE_SDL_MIXER "
end
if have_library("SGE","sge_Line") then
  $CFLAGS+= " -D HAVE_SGE "
end
if have_library("SDL_image","IMG_Load") then
  $CFLAGS+= " -D HAVE_SDL_IMAGE "
end
if have_library("SDL_ttf","TTF_Init") then
  $CFLAGS+= " -D HAVE_SDL_TTF "
end
if have_library("smpeg","SMPEG_new") then
  $CFLAGS+= " -D HAVE_SMPEG "
end
if have_library("SDLSKK","SDLSKK_Context_new") then
  $CFLAGS+= " -D HAVE_SDLSKK "
end
if enable_config("opengl",true) then
  dir_config('x11','/usr/X11R6')
  
  $CFLAGS+= " -D DEF_OPENGL "
  if arg_config("--linkoglmodule",false) then
    $CFLAGS+= " -D INIT_OGLMODULE_FROM_SDL "
  end

  if /linux/ =~ CONFIG["arch"] then
    have_library("GL","glVertex3d")
  elsif /mingw32/ =~ CONFIG["arch"] then
    have_library("opengl32","glVertex3d")
    have_library("glu32","gluGetString")
  end
end
create_makefile("sdl")

