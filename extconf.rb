require 'mkmf'

$CFLAGS += " -I /usr/local/include/SDL "
$LDFLAGS += " -lSDL -lpthread "
create_makefile("rubysdl")

