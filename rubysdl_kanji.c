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

#include "SDL_kanji.h"
#include "rubysdl.h"

static VALUE cKanjiFont;
typedef int (*Drawer)(Kanji_Font*, int, int, SDL_Surface*, const char*, SDL_Color);

typedef struct {
  Kanji_Font* font;
} KFont;

DEFINE_GET_STRUCT(KFont, Get_KFont, cKanjiFont, "SDL::Kanji::Font");
static Kanji_Font* Get_Kanji_Font(VALUE obj)
{
  KFont* kfont = Get_KFont(obj);
  if (kfont->font == NULL)
    rb_raise(eSDLError, "Kanji Font data is already disposed");
  return kfont->font;
}
static void Font_free(KFont* kfont)
{
  if (kfont->font)
    Kanji_CloseFont(kfont->font);
  free(kfont);
}
static VALUE Font_s_alloc(VALUE klass)
{
  KFont* kfont = ALLOC(KFont);
  kfont->font = NULL;
  return Data_Wrap_Struct(klass, 0, Font_free, kfont);
}
static VALUE Font_create(Kanji_Font* font)
{
  VALUE newobj = Font_s_alloc(cKanjiFont);
  Get_KFont(newobj)->font = font;
  return newobj;
}

#ifdef ENABLE_M17N
static rb_encoding* get_enc(Kanji_Font* font)
{
  switch (font->sys) {
  case KANJI_JIS:
    return iso2022jp_enc;
    break;
  case KANJI_EUC:
    return eucjp_enc;
    break;
  case KANJI_SJIS:
    return sjis_enc;
    break;
  default:
    rb_raise(eSDLError, "Unsupported Kanji encoding");
    return NULL;
    break;
  }
}
#endif

static VALUE Font_s_open(VALUE klass, VALUE filename, VALUE size)
{
  Kanji_Font* font;

  rb_secure(4);
  ExportFilenameStringValue(filename);
  
  font = Kanji_OpenFont(RSTRING_PTR(filename), NUM2INT(size));
  if(font == NULL)
    rb_raise(eSDLError,"Couldn't open bdf font: %s", RSTRING_PTR(filename));
  return Font_create(font);
}
static VALUE Font_close(VALUE self)
{
  KFont* kfont;
  
  rb_secure(4);
  kfont = Get_KFont(self);
  if (kfont->font)
    Kanji_CloseFont(kfont->font);
  kfont->font = NULL;
  return Qnil;
}
static VALUE Font_closed(VALUE self)
{
  return INT2BOOL(Get_KFont(self)->font == NULL);
}

static VALUE Font_setCodingSystem(VALUE self, VALUE sys)
{
  Kanji_SetCodingSystem(Get_Kanji_Font(self), NUM2INT(sys));
  return Qnil;
}
static VALUE Font_getCodingSystem(VALUE self)
{
  return INT2NUM(Get_Kanji_Font(self)->sys);
}

static VALUE Font_add(VALUE self, VALUE filename)
{
  rb_secure(4);
  ExportFilenameStringValue(filename);
  if(Kanji_AddFont(Get_Kanji_Font(self), RSTRING_PTR(filename)) == -1)
    rb_raise(eSDLError, "Couldn't use font: %s", RSTRING_PTR(filename));
  return Qnil;
}

static VALUE Font_textwidth(VALUE self, VALUE text)
{
  Kanji_Font* font;
  rb_secure(4);
  font = Get_Kanji_Font(self);
  ExportStringValueToEnc(text, get_enc(font));
  return INT2FIX(Kanji_FontWidth(font, RSTRING_PTR(text)));
}

static VALUE Font_width(VALUE self)
{
  return INT2FIX(Kanji_FontWidth(Get_Kanji_Font(self), NULL));
}

static VALUE Font_height(VALUE self)
{
  return INT2FIX(Kanji_FontHeight(Get_Kanji_Font(self)));
}

static void Font_put(VALUE self, VALUE surface, VALUE text,
                     VALUE x, VALUE y,
                     VALUE r, VALUE g, VALUE b, Drawer draw)
{
  SDL_Color color;
  Kanji_Font* font;
  rb_secure(4);
  font = Get_Kanji_Font(self);
  ExportStringValueToEnc(text, get_enc(font));
  color.r = NUM2INT(r);color.g = NUM2INT(g); color.b = NUM2INT(b);
  
  draw(Get_Kanji_Font(self), NUM2INT(x), NUM2INT(y),
       Get_SDL_Surface(surface), RSTRING_PTR(text), color);
}
  
static VALUE Font_putText(VALUE self, VALUE surface, VALUE text,
                          VALUE x, VALUE y,
                          VALUE r, VALUE g, VALUE b)
{
  Font_put(self, surface, text, x, y, r, g, b, Kanji_PutText);
  return Qnil;
}

static VALUE Font_putTextTate(VALUE self, VALUE surface, VALUE text,
                              VALUE x, VALUE y,
                              VALUE r, VALUE g, VALUE b)
{
  Font_put(self, surface, text, x, y, r, g, b, Kanji_PutTextTate);
  return Qnil;
}

void rubysdl_init_Kanji(VALUE mSDL)
{
  cKanjiFont = rb_define_class_under(mSDL, "Kanji", rb_cObject); 
  rb_undef_alloc_func(cKanjiFont);
  
  rb_define_singleton_method(cKanjiFont, "open", Font_s_open, 2);
  rb_define_method(cKanjiFont, "close", Font_close, 0);
  rb_define_method(cKanjiFont, "closed?", Font_closed, 0);
  rb_define_method(cKanjiFont, "add", Font_add, 1);
  rb_define_method(cKanjiFont, "setCodingSystem", Font_setCodingSystem, 1);
  rb_define_method(cKanjiFont, "getCodingSystem", Font_getCodingSystem, 0);
  rb_define_method(cKanjiFont, "textwidth", Font_textwidth, 1);
  rb_define_method(cKanjiFont, "width", Font_width, 0);
  rb_define_method(cKanjiFont, "height", Font_height, 0);
  rb_define_method(cKanjiFont, "put", Font_putText, 7);
  rb_define_method(cKanjiFont, "putTate", Font_putTextTate, 7);
  
  rb_define_const(cKanjiFont, "SJIS", INT2NUM(KANJI_SJIS));
  rb_define_const(cKanjiFont, "EUC", INT2NUM(KANJI_EUC));
  rb_define_const(cKanjiFont, "JIS", INT2NUM(KANJI_JIS));
}
