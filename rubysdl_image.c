#ifdef HAVE_SDL_IMAGE
#include "rubysdl.h"
#include <SDL_image.h>

static VALUE sdl_load(VALUE class,VALUE filename)
{
  SDL_Surface *surface=IMG_Load(STR2CSTR(filename));
  return Data_Wrap_Struct(class,0,SDL_FreeSurface,surface);
}

void init_sdl_image()
{
  rb_define_singleton_method(cSurface,"load",sdl_load,1);
}
#endif /* HAVE_SDL_IMAGE */
