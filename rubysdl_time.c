#include "rubysdl.h"

static VALUE sdl_getTicks(VALUE mod)
{
  return UINT2NUM(SDL_GetTicks());
}
static VALUE sdl_delay(VALUE mod,VALUE ms)
{
  SDL_delay(NUM2UINT(ms));
  return Qnil;
}

void init_time()
{
  rb_define_module_function(mSDL,"getTicks",sdl_getTicks,0);
  rb_define_module_function(mSDL,"delay",sdl_delay,1);
}
