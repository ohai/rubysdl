require 'mkmf'

$CFLAGS += `sdl-config --cflags`.chop
$LDFLAGS += `sdl-config --libs`.chop
#$CFLAGS += " -I /usr/local/include/SDL "
#$LDFLAGS += " -lSDL -lpthread "
#$CFLAGS += `sdl-config --cflags`
#$LDFLAGS += `sdl-config --libs`
#have_library("pthread","pthread_create")
#have_library("SDL","SDL_Init")
#find_library("SDL","SDL_Init","/tmp/usr/local/lib")
#
if have_library("SDL_mixer","Mix_OpenAudio") then
  $CFLAGS+= " -D HAVE_SDL_MIXER "
end

create_makefile("rubysdl")

