require 'mkmf'

$CFLAGS += " -I /usr/local/include/SDL "
#$LDFLAGS += " -lSDL -lpthread "
have_library("pthread","pthread_create")
#have_library("SDL","SDL_Init")
find_library("SDL","SDL_Init","/tmp/usr/local/lib")
#
create_makefile("rubysdl")

