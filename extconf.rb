require 'mkmf'

sdl_config = with_config('sdl-config', 'sdl-config')

$CFLAGS += ' ' + `#{sdl_config} --cflags`.chomp
$LDFLAGS += ' ' + `#{sdl_config} --libs`.chomp

if enable_config("event2",false) then
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
if enable_config("opengl",false) then
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

