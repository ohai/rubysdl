/*
  Ruby/SDL   Ruby extension library for SDL

  Copyright (C) 2001-2007 Ohbayashi Ippei
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  */
#include "rubysdl.h"

static VALUE cScreen = Qnil;
static VALUE cPixelFormat = Qnil;
static VALUE cVideoInfo = Qnil;
static VALUE cSurface = Qnil;
static VALUE eSurfaceLostMem = Qnil;

typedef struct {
  SDL_Surface* surface;
} Surface;

DEFINE_GET_STRUCT(Surface, GetSurface, cSurface, "SDL::Surface");
DEFINE_GET_STRUCT(SDL_PixelFormat, Get_PixelFormat, cPixelFormat, "SDL::PixelFormat");

SDL_Surface* Get_SDL_Surface(VALUE obj)
{
  Surface* sur = GetSurface(obj);
  if(sur->surface == NULL)
    rb_raise(eSDLError, "Surface is already destroyed");
  return sur->surface;
}

SDL_PixelFormat* Get_SDL_PixelFormat(VALUE obj)
{
  if(rb_obj_is_kind_of(obj, cSurface)){
    return Get_SDL_Surface(obj)->format;
  }else{
    return Get_PixelFormat(obj);
  }
}
  
static void Surface_free(Surface* sur)
{
  if(!rubysdl_is_quit() && sur->surface)
    SDL_FreeSurface(sur->surface);
  free(sur);
}

static void PixelFormat_free(SDL_PixelFormat* format)
{
  if(format->palette){
    free(format->palette->colors);
    free(format->palette);
  }
  free(format);
}

static VALUE Surface_s_alloc(VALUE klass)
{
  Surface* sur = ALLOC(Surface);
  sur->surface = NULL;
  return Data_Wrap_Struct(klass, 0, Surface_free, sur);
}

VALUE Surface_create(SDL_Surface* surface)
{
  VALUE newobj = Surface_s_alloc(cSurface);
  GetSurface(newobj)->surface = surface;
  return newobj;
}

static VALUE Screen_create(SDL_Surface* surface)
{
  VALUE newobj = Surface_s_alloc(cScreen);
  GetSurface(newobj)->surface = surface;
  return newobj;
}

Uint32 VALUE2COLOR(VALUE color,SDL_PixelFormat *format)
{
  if( rb_obj_is_kind_of( color, rb_cArray ) ){
    switch( RARRAY_LEN(color) ){
    case 3:
      return SDL_MapRGB(format,
			NUM2UINT(rb_ary_entry(color,0)),
			NUM2UINT(rb_ary_entry(color,1)),
			NUM2UINT(rb_ary_entry(color,2)) );
    case 4:
      return SDL_MapRGBA(format,
                         NUM2UINT(rb_ary_entry(color,0)),
                         NUM2UINT(rb_ary_entry(color,1)),
                         NUM2UINT(rb_ary_entry(color,2)),
                         NUM2UINT(rb_ary_entry(color,3)) );
    default:
      rb_raise(rb_eArgError,"type mismatch:color array needs 3 or 4 elements");
    }
  }else{
    return NUM2UINT(color);
  }
}

static VALUE Screen_s_get(VALUE klass)
{
  SDL_Surface *surface;
  rb_secure(4);
  surface = SDL_GetVideoSurface();
  if(surface==NULL)
    rb_raise(eSDLError,"Couldn't get video surface: %s",SDL_GetError());
  return Screen_create(surface);
}

static VALUE Screen_s_driverName(VALUE klass)
{
  char namebuf[256];
  rb_secure(4);
  if(SDL_VideoDriverName(namebuf, sizeof(namebuf)) == NULL) {
    rb_raise(eSDLError, "SDL is not initialized yet: %s", SDL_GetError());
  }
  return rb_str_new2( namebuf );
}

static VALUE Screen_s_listModes(VALUE klass,VALUE flags)
{
  SDL_Rect **modes;
  int i;
  VALUE array;

  rb_secure(4);
  modes = SDL_ListModes(NULL,NUM2UINT(flags));

  if( modes == NULL )
    return Qnil;/* no modes available */
  if( modes == (SDL_Rect **)-1)
    return Qtrue;/* all resolutions available */

  /* available modes into modesArray */
  array = rb_ary_new();
  
  for(i=0;modes[i]!=NULL;++i){
    rb_ary_push(array,
                rb_ary_new3(2, INT2NUM(modes[i]->w), INT2NUM(modes[i]->h)));
  }
  return array;
}

static VALUE Screen_s_checkMode(VALUE klass,VALUE w,VALUE h,VALUE bpp,
                                VALUE flags)
{
  rb_secure(4);
  return INT2FIX(SDL_VideoModeOK(NUM2INT(w),NUM2INT(h),NUM2INT(bpp),
                                 NUM2UINT(flags)));
}

static VALUE Screen_s_info(VALUE klass)
{
  const SDL_VideoInfo *info;
  VALUE obj;
  rb_secure(4);
  
  info = SDL_GetVideoInfo();
  if(info==NULL)
    rb_raise(eSDLError,"Couldn't get video information");
  obj=rb_obj_alloc(cVideoInfo);
  rb_iv_set(obj,"@hw_available",INT2BOOL(info->hw_available));
  rb_iv_set(obj,"@wm_available",INT2BOOL(info->wm_available));
  rb_iv_set(obj,"@blit_hw",INT2BOOL(info->blit_hw));
  rb_iv_set(obj,"@blit_hw_CC",INT2BOOL(info->blit_hw_CC));
  rb_iv_set(obj,"@blit_hw_A",INT2BOOL(info->blit_hw_A));
  rb_iv_set(obj,"@blit_sw",INT2BOOL(info->blit_sw));
  rb_iv_set(obj,"@blit_sw_CC",INT2BOOL(info->blit_sw_CC));
  rb_iv_set(obj,"@blit_sw_A",INT2BOOL(info->blit_sw_A));
  rb_iv_set(obj,"@blit_fill",INT2BOOL(info->blit_fill));
  rb_iv_set(obj,"@video_mem",UINT2NUM(info->video_mem));
  rb_iv_set(obj,"@bpp",UINT2NUM(info->vfmt->BitsPerPixel));
  return obj;
}


static VALUE Screen_updateRect(VALUE self,VALUE x,VALUE y,VALUE w,VALUE h)
{
  rb_secure(4);
  SDL_UpdateRect(Get_SDL_Surface(self),
                 NUM2INT(x),NUM2INT(y),NUM2INT(w),NUM2INT(h));
  return Qnil;
}

static VALUE Screen_updateRects(int argc, VALUE *argv, VALUE self)
{
  SDL_Rect* rects;
  int i;

  rects = ALLOCA_N(SDL_Rect, argc);
  for (i=0; i<argc; i++) {
    rects[i].x = NUM2INT(rb_ary_entry(argv[i], 0));
    rects[i].y = NUM2INT(rb_ary_entry(argv[i], 1));
    rects[i].w = NUM2INT(rb_ary_entry(argv[i], 2));
    rects[i].h = NUM2INT(rb_ary_entry(argv[i], 3));
  }

  SDL_UpdateRects(Get_SDL_Surface(self), argc, rects);
  return Qnil;
}

static VALUE Screen_flip(VALUE self)
{
  rb_secure(4);
  if( SDL_Flip(Get_SDL_Surface(self)) < 0 ){
    rb_raise(eSDLError,"flip fail : %s",SDL_GetError());
  }
  return Qnil;
}

static VALUE Screen_toggleFullScreen(VALUE self)
{
  rb_secure(4);
  if( SDL_WM_ToggleFullScreen(Get_SDL_Surface(self)) == 0 ){
    rb_raise( eSDLError,"toggle full screen fail : %s", SDL_GetError() );
  }
  return Qnil;
}

static VALUE Screen_s_open(VALUE klass,VALUE w,VALUE h,VALUE bpp,
                           VALUE flags)
{
  SDL_Surface *screen;
  rb_secure(4);
  screen=SDL_SetVideoMode(NUM2INT(w),NUM2INT(h),NUM2INT(bpp),
			  NUM2UINT(flags));
  if( screen==NULL ){
    rb_raise(eSDLError,"Couldn't set %dx%d %d bpp video mode: %s",
	     NUM2INT(w),NUM2INT(h),NUM2INT(bpp),SDL_GetError());
  }  
  return Screen_create(screen);
}

static VALUE Screen_s_setGamma(VALUE klass,VALUE rgamma,VALUE ggamma,VALUE bgamma)
{
  rb_secure(4);
  if(SDL_SetGamma(NUM2DBL(rgamma),NUM2DBL(ggamma),NUM2DBL(bgamma))==-1)
    rb_raise(eSDLError,"set gamma failed: %s",SDL_GetError());
  return Qnil;
}

static VALUE Screen_s_getGammaRamp(VALUE klass)
{
  Uint16 table[3][256];
  VALUE ary_table, ary_subtable;
  int i,j;
  rb_secure(4);
  
  if( SDL_GetGammaRamp( table[0], table[1], table[2] ) == -1 ){
    rb_raise(eSDLError,"cannot get gamma lookup table: %s",SDL_GetError());
  }
  
  ary_table = rb_ary_new();
  for( i=0; i<3; ++i ){
    ary_subtable = rb_ary_new();
    for( j=0; j<256; ++j ){
      rb_ary_push( ary_subtable, INT2FIX(table[i][j]) );
    }
    rb_ary_push( ary_table, ary_subtable );
  }
  return ary_table;
}

static VALUE Screen_s_setGammaRamp(VALUE klass, VALUE ary_table)
{
  Uint16 table[3][256];
  VALUE subtable;
  int i,j;
  
  rb_secure(4);
  Check_Type(ary_table, T_ARRAY);
  
  for( i=0; i<3; ++i ){
    subtable = rb_ary_entry(ary_table, i);
    Check_Type(subtable, T_ARRAY);
    for( j=0; j<256; ++j ){
      table[i][j] = NUM2INT(rb_ary_entry(subtable, j));
    }
  }
  if(SDL_SetGammaRamp(table[0], table[1], table[2]) == -1){
    rb_raise(eSDLError,"cannot set gamma lookup table: %s",SDL_GetError());
  }
  return Qnil;
}

static VALUE Surface_s_create(VALUE klass,VALUE flags,VALUE w,VALUE h,
                              VALUE pixel_format)
{
  SDL_Surface* surface;
  SDL_PixelFormat* format;

  rb_secure(4);
  format = Get_SDL_PixelFormat(pixel_format);
  surface = SDL_CreateRGBSurface(NUM2UINT(flags),NUM2INT(w),NUM2INT(h),
                                 format->BitsPerPixel,
                                 format->Rmask,format->Gmask,
                                 format->Bmask,format->Amask);
  if(surface == NULL)
    rb_raise(eSDLError,"Couldn't Create Surface: %s",SDL_GetError());

  return Surface_create(surface);
}

static VALUE Surface_s_createWithFormat(VALUE klass,VALUE flags,VALUE w,
                                      VALUE h,VALUE depth,VALUE Rmask,
                                      VALUE Gmask,VALUE Bmask,
                                      VALUE Amask)
{
  SDL_Surface *surface;

  rb_secure(4);
  surface = SDL_CreateRGBSurface(NUM2UINT(flags),NUM2INT(w),NUM2INT(h),
                                 NUM2UINT(depth),NUM2UINT(Rmask),
                                 NUM2UINT(Gmask),NUM2UINT(Bmask),
                                 NUM2UINT(Amask));
  if( surface == NULL ){
    rb_raise(eSDLError,"Couldn't Create Surface: %s",SDL_GetError());
  }
  return Surface_create(surface);
}


static VALUE Surface_s_createFrom(VALUE klass,VALUE pixels,VALUE w,
                                  VALUE h,VALUE depth,VALUE pitch,
                                  VALUE Rmask,VALUE Gmask,VALUE Bmask,
                                  VALUE Amask)
{
  SDL_Surface *surface;
  void* pixel_data;
  
  SafeStringValue(pixels);
  pixel_data = ALLOC_N(char, RSTRING_LEN(pixels));
  memcpy(pixel_data,RSTRING_PTR(pixels),RSTRING_LEN(pixels));
  
  surface = SDL_CreateRGBSurfaceFrom(pixel_data,NUM2INT(w),NUM2INT(h),
                                     NUM2UINT(depth),NUM2INT(pitch),
                                     NUM2UINT(Rmask),NUM2UINT(Gmask),
                                     NUM2UINT(Bmask),NUM2UINT(Amask));
  if( surface == NULL ){
    rb_raise(eSDLError,"Couldn't Create Surface: %s",SDL_GetError());
  }
  surface->flags &= ~SDL_PREALLOC;
  return Surface_create(surface);
}

static VALUE Surface_s_loadBMP(VALUE klass,VALUE filename)
{
  SDL_Surface *image;
  rb_secure(4);
  ExportFilenameStringValue(filename);
  
  image = SDL_LoadBMP(RSTRING_PTR(filename));
  if(image == NULL){
    rb_raise(eSDLError,"Couldn't Load BMP file %s : %s",
	     RSTRING_PTR(filename),SDL_GetError());
  }
  return Surface_create(image);
}

static VALUE Surface_s_loadBMPFromIO(VALUE class, VALUE io)
{
  volatile VALUE guard = io;
  SDL_Surface* image;
  image = SDL_LoadBMP_RW(rubysdl_RWops_from_ruby_obj(io), 1);
  if(image == NULL)
    rb_raise(eSDLError, "Couldn't Load BMP file from IO : %s",
             SDL_GetError());
  return Surface_create(image);
}

static VALUE Surface_s_loadBMPFromString(VALUE class, VALUE str)
{
  SDL_Surface* image;
  rb_secure(4);
  SafeStringValue(str);
  
  image = SDL_LoadBMP_RW(SDL_RWFromConstMem(RSTRING_PTR(str),
                                            RSTRING_LEN(str)),
                         1);
  if(image == NULL)
    rb_raise(eSDLError, "Couldn't Load BMP file from String : %s",
             SDL_GetError());
  return Surface_create(image);
}

static VALUE Surface_saveBMP(VALUE self,VALUE filename)
{
  rb_secure(4);
  ExportFilenameStringValue(filename);
  if( SDL_SaveBMP(Get_SDL_Surface(self), RSTRING_PTR(filename))==-1 ){
    rb_raise(eSDLError,"cannot save %s: %s",RSTRING_PTR(filename),SDL_GetError());
  }
  return Qnil;
}

static VALUE Surface_destroy(VALUE self)
{
  Surface* sur = GetSurface(self);
  if(!rubysdl_is_quit() && sur->surface)
    SDL_FreeSurface(sur->surface);
  sur->surface = NULL;
  return Qnil;
}
static VALUE Surface_destroyed(VALUE self)
{
  Surface* sur = GetSurface(self);
  return INT2BOOL(sur->surface == NULL);
}

static VALUE Surface_format(VALUE self)
{
  SDL_Surface* surface = Get_SDL_Surface(self);
  SDL_PixelFormat* format = ALLOC(SDL_PixelFormat);
  SDL_Palette* palette;
  
  if(surface->format->palette){
    palette = ALLOC(SDL_Palette);
    palette->ncolors = surface->format->palette->ncolors;
    palette->colors = ALLOC_N(SDL_Color, surface->format->palette->ncolors);
    memcpy(palette->colors, surface->format->palette->colors,
           surface->format->palette->ncolors * sizeof(SDL_Color));
  }else{
    palette = NULL;
  }
  
  *format = *(surface->format);
  format->palette = palette;

  return Data_Wrap_Struct(cPixelFormat,0,PixelFormat_free,format);
}

static VALUE Surface_displayFormat(VALUE self)
{
  SDL_Surface *result;

  rb_secure(4);
  result = SDL_DisplayFormat(Get_SDL_Surface(self));
  if(result==NULL){
    rb_raise(eSDLError,"Couldn't convert surface format: %s",SDL_GetError());
  }
  return Surface_create(result);
}

static VALUE Surface_displayFormatAlpha(VALUE self)
{
  SDL_Surface *result;

  rb_secure(4);
  result = SDL_DisplayFormatAlpha(Get_SDL_Surface(self));
  if(result==NULL){
    rb_raise(eSDLError,"Couldn't convert surface format: %s",SDL_GetError());
  }
  return Surface_create(result);
}

static VALUE Surface_setColorKey(VALUE self,VALUE flag,VALUE key)
{
  SDL_Surface *surface = Get_SDL_Surface(self);
  rb_secure(4);
  
  if( SDL_SetColorKey(surface,NUM2UINT(flag),VALUE2COLOR(key,surface->format))
      < 0 ){
    rb_raise(eSDLError,"setColorKey failed: %s",SDL_GetError());
  }
  return Qnil;
}

static VALUE zero_rect_p(SDL_Rect rect)
{
  return rect.x == 0 && rect.y == 0 && rect.w == 0 && rect.h == 0;
}

/* return 0 if succeed, return -2 video memory lost (on Direct X)*/
static VALUE Surface_s_blit(VALUE klass,VALUE src,VALUE srcX,VALUE srcY,
                            VALUE srcW,VALUE srcH,VALUE dst,VALUE destX,
                            VALUE destY)
{
  SDL_Surface *src_surface,*dst_surface;
  SDL_Rect src_rect,dst_rect;
  SDL_Rect *sr, *dr;
  int result;

  rb_secure(4);
  SetRect(dst_rect,destX,destY,srcW,srcH);
  SetRect(src_rect,srcX,srcY,srcW,srcH);

  src_surface = Get_SDL_Surface(src);
  dst_surface = Get_SDL_Surface(dst);

  sr = (zero_rect_p(src_rect))?NULL:&src_rect;
  dr = (zero_rect_p(dst_rect))?NULL:&dst_rect;
  
  result = SDL_BlitSurface(src_surface, sr, dst_surface, dr);

  switch(result) {
  case -1:
    rb_raise(eSDLError,"SDL::Surface.blit fail: %s",SDL_GetError());
    break;
  case -2:
    rb_raise(eSurfaceLostMem, "SDL::Surface lost video memory");
    break;
  }
  return INT2NUM(result);
}

static VALUE Surface_setAlpha(VALUE self,VALUE flag,VALUE alpha)
{
  rb_secure(4);
  SDL_SetAlpha(Get_SDL_Surface(self),NUM2UINT(flag),NUM2INT(alpha));
  return Qnil;
}

static VALUE Surface_setClipRect(VALUE self,VALUE x,VALUE y,VALUE w,VALUE h)
{
  SDL_Rect rect;

  rb_secure(4);
  SetRect(rect,x,y,w,h);
  SDL_SetClipRect(Get_SDL_Surface(self),&rect);
  return Qnil;
}

static VALUE Surface_getClipRect(VALUE self)
{
  SDL_Rect rect;

  rb_secure(4);
  SDL_GetClipRect( Get_SDL_Surface(self), &rect );
  return rb_ary_new3(INT2FIX(rect.x),INT2FIX(rect.y),
                     INT2FIX(rect.w),INT2FIX(rect.h));
}

static VALUE Surface_fillRect(VALUE self,VALUE x,VALUE y,VALUE w,VALUE h,
                              VALUE color)
{
  SDL_Surface *surface = Get_SDL_Surface(self);
  SDL_Rect rect;

  rb_secure(4);
  SetRect(rect,x,y,w,h);
  if( SDL_FillRect(surface,&rect,VALUE2COLOR(color,surface->format)) < 0 ){
    rb_raise(eSDLError,"fillRect fail: %s",SDL_GetError());
  }
  return Qnil;
}

static VALUE Surface_h(VALUE self)
{
  return INT2NUM(Get_SDL_Surface(self)->h);
}

static VALUE Surface_w(VALUE self)
{
  return INT2NUM(Get_SDL_Surface(self)->w);
}

static VALUE Surface_getPixel(VALUE self, VALUE x, VALUE y)
{
  return UINT2NUM(rubysdl_getPixel(Get_SDL_Surface(self),
                                   NUM2INT(x), NUM2INT(y)));
}
static VALUE Surface_putPixel(VALUE self, VALUE x, VALUE y, VALUE color)
{
  SDL_Surface* surface = Get_SDL_Surface(self);
  rubysdl_putPixel(surface,
                   NUM2INT(x), NUM2INT(y), VALUE2COLOR(color,surface->format));
  return Qnil;
}

/* palette and colormap methods */
static void check_colors(VALUE colors,VALUE firstcolor)
{
  if( NUM2INT(firstcolor)<0 || NUM2INT(firstcolor)>255 )
    rb_raise(eSDLError,"firstcolor must be more than 0,less than 255");
  
  Check_Type(colors,T_ARRAY);
  
  if( RARRAY_LEN(colors)+NUM2INT(firstcolor) > 256 )
    rb_raise(eSDLError,"colors is too large");
}
static void set_colors_to_array(VALUE colors,SDL_Color palette[])
{
  VALUE color;
  int i;
  
  for( i=0; i < RARRAY_LEN(colors); ++i){
    color = rb_ary_entry(colors,i);
    Check_Type(color,T_ARRAY);
    if( RARRAY_LEN(color) != 3)
      rb_raise(rb_eArgError,"a color must be array that has 3 length");
    palette[i].r = NUM2INT(rb_ary_entry(color,0));
    palette[i].g = NUM2INT(rb_ary_entry(color,1));
    palette[i].b = NUM2INT(rb_ary_entry(color,2));
  }
  
}

static VALUE Surface_setPalette(VALUE self,VALUE flags,
                                VALUE colors,VALUE firstcolor)
{
  SDL_Color palette[256];
  
  check_colors(colors,firstcolor);
  set_colors_to_array(colors,palette);

  rb_secure(4);
  return INT2BOOL(SDL_SetPalette(Get_SDL_Surface(self), NUM2UINT(flags), palette,
                                 NUM2INT(firstcolor), RARRAY_LEN(colors)));
}

static VALUE Surface_setColors(VALUE self,VALUE colors,VALUE firstcolor)
{
  SDL_Color palette[256];

  rb_secure(4);
  check_colors(colors,firstcolor);
  set_colors_to_array(colors,palette);
  return INT2BOOL(SDL_SetColors(Get_SDL_Surface(self), palette,
                                NUM2INT(firstcolor), RARRAY_LEN(colors)));
}

static VALUE PixelFormat_pallete(VALUE self)
{
  SDL_PixelFormat* format = Get_SDL_PixelFormat(self);
  int i;
  VALUE palette;
  SDL_Color *colors;
  VALUE color;

  if(format->palette == NULL)
    return Qnil;

  palette = rb_ary_new();
  colors = format->palette->colors;

  for( i=0; i < format->palette->ncolors; ++i ){
    color = rb_ary_new3(3, INT2NUM(colors[i].r), INT2NUM(colors[i].g), INT2NUM(colors[i].b));
    rb_ary_push(palette, color);
  }
  return palette;
}

/* surface lock methods */
static VALUE Surface_mustlock_p(VALUE self)
{
  rb_secure(4);
  return INT2BOOL(SDL_MUSTLOCK(Get_SDL_Surface(self)));
}
static VALUE Surface_lock(VALUE self)
{
  rb_secure(4);
  return INT2FIX(SDL_LockSurface(Get_SDL_Surface(self)));
}
static VALUE Surface_unlock(VALUE self)
{
  rb_secure(4);
  SDL_UnlockSurface(Get_SDL_Surface(self));
  return Qnil;
}

/* methods to get infomation from SDL_PixelFormat */
static VALUE PixelFormat_mapRGB(VALUE self,VALUE r,VALUE g,VALUE b)
{
  return UINT2NUM(SDL_MapRGB(Get_SDL_PixelFormat(self),NUM2INT(r),NUM2INT(g),
                             NUM2INT(b)));
}
static VALUE PixelFormat_mapRGBA(VALUE self,VALUE r,VALUE g,VALUE b,VALUE a)
{
  return UINT2NUM(SDL_MapRGBA(Get_SDL_PixelFormat(self),NUM2INT(r),NUM2INT(g),
                              NUM2INT(b),NUM2INT(a)));
}
static VALUE PixelFormat_getRGB(VALUE self,VALUE pixel)
{
  Uint8 r,g,b;
  SDL_GetRGB(NUM2UINT(pixel),Get_SDL_PixelFormat(self),&r,&g,&b);
  return rb_ary_new3(3,UINT2NUM(r),UINT2NUM(g),UINT2NUM(b));
}
static VALUE PixelFormat_getRGBA(VALUE self,VALUE pixel)
{
  Uint8 r,g,b,a;
  SDL_GetRGBA(NUM2UINT(pixel),Get_SDL_PixelFormat(self),&r,&g,&b,&a);
  return rb_ary_new3(4,UINT2NUM(r),UINT2NUM(g),UINT2NUM(b),UINT2NUM(a));
}
static VALUE PixelFormat_bpp(VALUE self)
{
  return INT2FIX(Get_SDL_PixelFormat(self)->BitsPerPixel);
}

static VALUE PixelFormat_colorkey(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->colorkey);
}

static VALUE PixelFormat_alpha(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->alpha);
}

static VALUE Surface_flags(VALUE self)
{
  return UINT2NUM(Get_SDL_Surface(self)->flags);
}

static VALUE PixelFormat_rmask(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Rmask);
}

static VALUE PixelFormat_gmask(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Gmask);
}

static VALUE PixelFormat_bmask(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Bmask);
}

static VALUE PixelFormat_amask(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Amask);
}

static VALUE PixelFormat_Rloss(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Rloss);
}
static VALUE PixelFormat_Gloss(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Gloss);
}
static VALUE PixelFormat_Bloss(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Rloss);
}
static VALUE PixelFormat_Aloss(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Aloss);
}

static VALUE PixelFormat_Rshift(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Rshift);
}
static VALUE PixelFormat_Gshift(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Gshift);
}
static VALUE PixelFormat_Bshift(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Bshift);
}
static VALUE PixelFormat_Ashift(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->Ashift);
}

static VALUE Surface_pitch(VALUE self)
{
  return UINT2NUM(Get_SDL_Surface(self)->pitch);
}
static VALUE PixelFormat_bytesPerPixel(VALUE self)
{
  return UINT2NUM(Get_SDL_PixelFormat(self)->BytesPerPixel);
}

static VALUE Surface_pixels(VALUE self)
{
  SDL_Surface *surface = Get_SDL_Surface(self);
  return rb_str_new(surface->pixels,
                    surface->h * surface->pitch);
}

VALUE rubysdl_init_video(VALUE mSDL)
{
  cSurface = rb_define_class_under(mSDL,"Surface",rb_cObject);
  cScreen = rb_define_class_under(mSDL,"Screen",cSurface);
  cPixelFormat = rb_define_class_under(mSDL, "PixelFormat", rb_cObject);
  eSurfaceLostMem = rb_define_class_under(cSurface, "VideoMemoryLost",
                                          rb_eStandardError);
  rb_define_alloc_func(cSurface, Surface_s_alloc);
  rb_undef_alloc_func(cPixelFormat);
  
  rb_define_singleton_method(cScreen, "get", Screen_s_get, 0);
  rb_define_singleton_method(cScreen, "driverName", Screen_s_driverName, 0);
  rb_define_singleton_method(cSurface, "blit", Surface_s_blit, 8);

  rb_define_singleton_method(cScreen, "open", Screen_s_open, 4);
  rb_define_singleton_method(cScreen, "checkMode", Screen_s_checkMode, 4);
  rb_define_singleton_method(cScreen, "listModes", Screen_s_listModes, 1);
  rb_define_singleton_method(cScreen, "setGamma", Screen_s_setGamma, 3);
  rb_define_singleton_method(cScreen, "getGammaRamp", Screen_s_getGammaRamp, 0);
  rb_define_singleton_method(cScreen, "setGammaRamp", Screen_s_setGammaRamp, 1);
  
  cVideoInfo=rb_define_class_under(mSDL,"VideoInfo",rb_cObject);
  rb_define_attr(cVideoInfo,"hw_available",1,0);
  rb_define_attr(cVideoInfo,"wm_available",1,0);
  rb_define_attr(cVideoInfo,"blit_hw",1,0);
  rb_define_attr(cVideoInfo,"blit_hw_CC",1,0);
  rb_define_attr(cVideoInfo,"blit_hw_A",1,0);
  rb_define_attr(cVideoInfo,"blit_sw",1,0);
  rb_define_attr(cVideoInfo,"blit_sw_CC",1,0);
  rb_define_attr(cVideoInfo,"blit_sw_A",1,0);
  rb_define_attr(cVideoInfo,"blit_fill",1,0);
  rb_define_attr(cVideoInfo,"video_mem",1,0);
  rb_define_attr(cVideoInfo,"bpp",1,0);

  rb_define_singleton_method(cScreen, "info", Screen_s_info, 0);

  rb_define_singleton_method(cSurface,"create",Surface_s_create,4);
  rb_define_singleton_method(cSurface,"createWithFormat",Surface_s_createWithFormat,8);
  rb_define_singleton_method(cSurface,"new_from",Surface_s_createFrom,9);
  rb_define_singleton_method(cSurface,"loadBMP",Surface_s_loadBMP,1);
  rb_define_singleton_method(cSurface,"loadBMPFromIO",Surface_s_loadBMPFromIO,1);
  rb_define_singleton_method(cSurface,"loadBMPFromString",Surface_s_loadBMPFromString,1);
  
  rb_define_method(cSurface,"saveBMP",Surface_saveBMP,1);
  rb_define_method(cSurface,"destroy",Surface_destroy,0);
  rb_define_method(cSurface,"destroyed?",Surface_destroyed,0);
  rb_define_method(cSurface,"displayFormat",Surface_displayFormat,0);
  rb_define_method(cSurface,"displayFormatAlpha",Surface_displayFormatAlpha,0);
  rb_define_method(cSurface,"setColorKey",Surface_setColorKey,2);
  rb_define_method(cSurface,"fillRect",Surface_fillRect,5);
  rb_define_method(cSurface,"setClipRect",Surface_setClipRect,4);
  rb_define_method(cSurface,"getClipRect",Surface_getClipRect,0);
  rb_define_method(cSurface,"setAlpha",Surface_setAlpha,2);
  rb_define_method(cSurface,"h",Surface_h,0);
  rb_define_method(cSurface,"w",Surface_w,0);
  rb_define_method(cSurface,"flags",Surface_flags,0);

  rb_define_method(cSurface,"getPixel",Surface_getPixel,2);
  rb_define_method(cSurface,"putPixel",Surface_putPixel,3);
  rb_define_method(cSurface,"[]",Surface_getPixel,2);
  rb_define_method(cSurface,"[]=",Surface_putPixel,3);

  rb_define_method(cSurface,"setPalette",Surface_setPalette,3);
  rb_define_method(cSurface,"setColors",Surface_setColors,2);
  rb_define_method(cPixelFormat,"palette",PixelFormat_pallete,0);
  
  rb_define_method(cSurface,"mustLock?",Surface_mustlock_p,0);
  rb_define_method(cSurface,"lock",Surface_lock,0);
  rb_define_method(cSurface,"unlock",Surface_unlock,0);

  rb_define_method(cSurface,"format",Surface_format,0);
  rb_define_method(cPixelFormat,"mapRGB",PixelFormat_mapRGB,3);
  rb_define_method(cPixelFormat,"mapRGBA",PixelFormat_mapRGBA,4);
  rb_define_method(cPixelFormat,"getRGB",PixelFormat_getRGB,1);
  rb_define_method(cPixelFormat,"getRGBA",PixelFormat_getRGBA,1);
  rb_define_method(cPixelFormat,"bpp",PixelFormat_bpp,0);
  rb_define_method(cPixelFormat,"bytesPerPixel",PixelFormat_bytesPerPixel,0);
  rb_define_method(cPixelFormat,"colorkey",PixelFormat_colorkey,0);
  rb_define_method(cPixelFormat,"alpha",PixelFormat_alpha,0);
  rb_define_method(cPixelFormat,"Rmask",PixelFormat_rmask,0);
  rb_define_method(cPixelFormat,"Gmask",PixelFormat_gmask,0);
  rb_define_method(cPixelFormat,"Bmask",PixelFormat_bmask,0);
  rb_define_method(cPixelFormat,"Amask",PixelFormat_amask,0);
  rb_define_method(cPixelFormat,"Rloss",PixelFormat_Rloss,0);
  rb_define_method(cPixelFormat,"Gloss",PixelFormat_Gloss,0);
  rb_define_method(cPixelFormat,"Bloss",PixelFormat_Bloss,0);
  rb_define_method(cPixelFormat,"Aloss",PixelFormat_Aloss,0);
  rb_define_method(cPixelFormat,"Rshift",PixelFormat_Rshift,0);
  rb_define_method(cPixelFormat,"Gshift",PixelFormat_Gshift,0);
  rb_define_method(cPixelFormat,"Bshift",PixelFormat_Bshift,0);
  rb_define_method(cPixelFormat,"Ashift",PixelFormat_Ashift,0);
  rb_define_method(cSurface,"pixels",Surface_pixels,0);
  rb_define_method(cSurface,"pitch",Surface_pitch,0);
  
  rb_define_method(cScreen,"updateRect",Screen_updateRect,4);
  rb_define_method(cScreen,"updateRects",Screen_updateRects,-1);
  rb_define_method(cScreen,"flip",Screen_flip,0);
  rb_define_method(cScreen,"toggleFullScreen",Screen_toggleFullScreen,0);


  /* Available for Screen.open */
  rb_define_const(mSDL,"SWSURFACE",UINT2NUM(SDL_SWSURFACE));
  rb_define_const(mSDL,"HWSURFACE",UINT2NUM(SDL_HWSURFACE));
  rb_define_const(mSDL,"ASYNCBLIT",UINT2NUM(SDL_ASYNCBLIT));
  rb_define_const(mSDL,"ANYFORMAT",UINT2NUM(SDL_ANYFORMAT));
  rb_define_const(mSDL,"HWPALETTE",UINT2NUM(SDL_HWPALETTE));
  rb_define_const(mSDL,"DOUBLEBUF",UINT2NUM(SDL_DOUBLEBUF));
  rb_define_const(mSDL,"FULLSCREEN",UINT2NUM(SDL_FULLSCREEN));
  rb_define_const(mSDL,"OPENGL",UINT2NUM(SDL_OPENGL));
  rb_define_const(mSDL,"OPENGLBLIT",UINT2NUM(SDL_OPENGLBLIT));
  rb_define_const(mSDL,"RESIZABLE",UINT2NUM(SDL_RESIZABLE));
  rb_define_const(mSDL,"NOFRAME", UINT2NUM(SDL_NOFRAME));
  rb_define_const(mSDL,"HWACCEL",UINT2NUM(SDL_HWACCEL));
  rb_define_const(mSDL,"SRCCOLORKEY",UINT2NUM(SDL_SRCCOLORKEY));
  rb_define_const(mSDL,"RLEACCELOK",UINT2NUM(SDL_RLEACCELOK));
  rb_define_const(mSDL,"RLEACCEL",UINT2NUM(SDL_RLEACCEL));
  rb_define_const(mSDL,"SRCALPHA",UINT2NUM(SDL_SRCALPHA));
  rb_define_const(mSDL,"PREALLOC",UINT2NUM(SDL_PREALLOC));

  /*Transparency definitions: These define alpha as the opacity of a surface*/
  rb_define_const(mSDL,"ALPHA_OPAQUE",INT2NUM(SDL_ALPHA_OPAQUE));
  rb_define_const(mSDL,"ALPHA_TRANSPARENT",INT2NUM(SDL_ALPHA_TRANSPARENT));

  /* flags for SDL::Surface.setPalette */
  rb_define_const(mSDL,"LOGPAL",INT2NUM(SDL_LOGPAL));
  rb_define_const(mSDL,"PHYSPAL",INT2NUM(SDL_PHYSPAL));

  return cSurface;
}
