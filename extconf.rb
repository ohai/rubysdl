require 'mkmf'

sdl_config = with_config('sdl-config', 'sdl-config')

$CFLAGS += ' ' + `#{sdl_config} --cflags`.chomp
$LDFLAGS += ' ' + `#{sdl_config} --libs`.chomp

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
create_makefile("sdl")

