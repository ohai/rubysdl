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
#ifdef HAVE_SDL_TTF
#include "rubysdl.h"
#include <SDL_ttf.h>

typedef struct {
  TTF_Font* font;
} TTFont;
  
typedef SDL_Surface* (*RenderFunc)(TTF_Font *,const char *,SDL_Color,SDL_Color);

static int ttf_init = 0;
static VALUE cTTFFont = Qnil;

static void Font_free(TTFont *f)
{
  if(!rubysdl_is_quit() && f->font)
    TTF_CloseFont(f->font);
  free(f);
}

DEFINE_GET_STRUCT(TTFont, Get_TTFont, cTTFFont, "SDL::TT::Font");

TTF_Font* Get_TTF_Font(VALUE obj)
{
  TTFont* f = Get_TTFont(obj);
  if (f->font == NULL)
    rb_raise(eSDLError, "TTF is alreadly closed");
  return f->font;
}

static VALUE TTF_s_alloc(VALUE klass)
{
  TTFont* f = ALLOC(TTFont);
  f->font = NULL;
  return Data_Wrap_Struct(klass, 0, Font_free, f);
}
static VALUE TTF_create(TTF_Font* font)
{
  VALUE newobj = TTF_s_alloc(cTTFFont);
  Get_TTFont(newobj)->font = font;
  return newobj;
}

static VALUE TTF_s_init(VALUE klass)
{
  rb_secure(4);
  if(TTF_Init() == -1)
    rb_raise(eSDLError,"Couldn't initialize TTF engine: %s",TTF_GetError());
  ttf_init = 1;
  return Qnil;
}

static VALUE TTF_s_init_p(VALUE class)
{
  return INT2BOOL(TTF_WasInit());
}

static VALUE Font_s_open(int argc, VALUE *argv, VALUE class)
{
  TTF_Font *font;
  VALUE filename, size, index;
  
  rb_secure(4);
  rb_scan_args( argc, argv, "21", &filename, &size, &index );
  
  SafeStringValue(filename);
  
  if(NIL_P(index))
    font = TTF_OpenFont(RSTRING_PTR(filename), NUM2INT(size));
  else
    font = TTF_OpenFontIndex(RSTRING_PTR(filename),
                             NUM2INT(size), NUM2INT(index));
  
  if(font == NULL)
    rb_raise(eSDLError,"Couldn't open font %s: %s",
             RSTRING_PTR(filename), TTF_GetError());
  return TTF_create(font);
}

static VALUE Font_style(VALUE self)
{
  return INT2FIX(TTF_GetFontStyle(Get_TTF_Font(self)));
}

static VALUE Font_set_style(VALUE self, VALUE style)
{
  TTF_SetFontStyle(Get_TTF_Font(self), NUM2UINT(style));
  return Qnil;
}

static VALUE Font_faces(VALUE self)
{
  return UINT2NUM(TTF_FontFaces(Get_TTF_Font(self)));
}

static VALUE Font_fixedWidth_p(VALUE self)
{
  return INT2BOOL(TTF_FontFaceIsFixedWidth(Get_TTF_Font(self)));
}

static VALUE string_or_nil(char* str)
{
  if (str)
    return rb_str_new2(str);
  else
    return Qnil;
}

static VALUE Font_familyName(VALUE self)
{
  return string_or_nil(TTF_FontFaceFamilyName(Get_TTF_Font(self)));
}

static VALUE Font_styleName(VALUE self)
{
  return string_or_nil(TTF_FontFaceStyleName(Get_TTF_Font(self)));
}

static VALUE Font_textSize(VALUE self, VALUE text)
{
  int w,h;

  StringValue(text);
#ifdef ENABLE_M17N
  text = rb_str_export_to_enc(text, utf8_enc);
#endif
  TTF_SizeUTF8(Get_TTF_Font(self), RSTRING_PTR(text), &w, &h);
  return rb_ary_new3(2, INT2FIX(w), INT2FIX(h));
}

static VALUE Font_height(VALUE self)
{
  return INT2FIX(TTF_FontHeight(Get_TTF_Font(self)));
}

static VALUE Font_ascent(VALUE self)
{
  return INT2FIX(TTF_FontAscent(Get_TTF_Font(self)));
}

static VALUE Font_descent(VALUE self)
{
  return INT2FIX(TTF_FontDescent(Get_TTF_Font(self)));
}

static VALUE Font_lineSkip(VALUE self)
{
  return INT2FIX(TTF_FontLineSkip(Get_TTF_Font(self)));
}

static SDL_Color rgb_to_SDL_Color(VALUE r, VALUE g, VALUE b)
{
  SDL_Color color;
  color.r = NUM2UINT(r);
  color.g = NUM2UINT(g);
  color.b = NUM2UINT(b);
  color.unused = 0;
  return color;
}

static VALUE render(VALUE self, VALUE text,
                    VALUE fgr,VALUE fgg,VALUE fgb,
                    VALUE bgr,VALUE bgg,VALUE bgb,
                    int convert_enc,
                    RenderFunc renderer)
{
  SDL_Surface *surface;
  
  rb_secure(4);
  StringValue(text);
#ifdef ENABLE_M17N
  if (convert_enc)
    text = rb_str_export_to_enc(text, utf8_enc);
#endif
  surface = renderer(Get_TTF_Font(self),
                     RSTRING_PTR(text),
                     rgb_to_SDL_Color(fgr, fgg, fgb),
                     rgb_to_SDL_Color(bgr, bgg, bgb));
  
  if(surface == NULL)
    return Qnil;

  return Surface_create(surface);
}

static SDL_Surface* wrap_RenderUTF8_Solid(TTF_Font *font,
                                          const char *text,
                                          SDL_Color fg,
                                          SDL_Color bg)
{
  return TTF_RenderUTF8_Solid(font,text,fg);
}

static SDL_Surface* wrap_RenderUTF8_Blended(TTF_Font *font,
                                            const char *text,
                                            SDL_Color fg,
                                            SDL_Color bg)
{
  return TTF_RenderUTF8_Blended(font,text,fg);
}


/* 1 is ruby's zero */
static VALUE Font_renderSolidUTF8(VALUE self, VALUE text,
                                  VALUE r, VALUE g, VALUE b)
				     
{
  return render(self, text, r, g, b, 1, 1, 1, 0,
                wrap_RenderUTF8_Solid);
}

static VALUE Font_renderBlendedUTF8(VALUE self, VALUE text,
                                    VALUE r, VALUE g, VALUE b)
				     
{
  return render(self, text, r, g, b, 1, 1, 1, 0,
                wrap_RenderUTF8_Blended);
}

static VALUE Font_renderShadedUTF8(VALUE self, VALUE text,
                                   VALUE fgr,VALUE fgg,VALUE fgb,
                                   VALUE bgr,VALUE bgg,VALUE bgb)
{
  return render(self, text, fgr, fgg, fgb, bgr, bgg, bgb, 0,
                TTF_RenderUTF8_Shaded);
}

static VALUE Font_renderSolid(VALUE self, VALUE text,
                              VALUE r, VALUE g, VALUE b)
				     
{
  return render(self, text, r, g, b, 1, 1, 1, 1,
                wrap_RenderUTF8_Solid);
}

static VALUE Font_renderBlended(VALUE self, VALUE text,
                                VALUE r, VALUE g, VALUE b)
				     
{
  return render(self, text, r, g, b, 1, 1, 1, 1,
                wrap_RenderUTF8_Blended);
}

static VALUE Font_renderShaded(VALUE self, VALUE text,
                               VALUE fgr,VALUE fgg,VALUE fgb,
                               VALUE bgr,VALUE bgg,VALUE bgb)
{
  return render(self, text, fgr, fgg, fgb, bgr, bgg, bgb, 1,
                TTF_RenderUTF8_Shaded);
}

static VALUE Font_close(VALUE self)
{
  TTFont* f = Get_TTFont(self);
  if(!rubysdl_is_quit() && f->font)
    TTF_CloseFont(f->font);
  f->font = NULL;
  
  return Qnil;
}
static VALUE Font_closed(VALUE self)
{
  return INT2BOOL(Get_TTFont(self)->font == NULL);
}
                       
void rubysdl_init_TTF(VALUE mSDL)
{
  cTTFFont = rb_define_class_under(mSDL, "TTF", rb_cObject);
                                         
  rb_undef_alloc_func(cTTFFont);
  
  rb_define_singleton_method(cTTFFont,"init",TTF_s_init,0);
  rb_define_singleton_method(cTTFFont,"init?", TTF_s_init_p,0);
  rb_define_singleton_method(cTTFFont,"open",Font_s_open,-1);
  
  rb_define_method(cTTFFont,"style",Font_style,0);
  rb_define_method(cTTFFont,"style=",Font_set_style,1);
  rb_define_method(cTTFFont,"textSize",Font_textSize,1);
  rb_define_method(cTTFFont,"faces",Font_faces,0);
  rb_define_method(cTTFFont,"fixedWidth?",Font_fixedWidth_p,0);
  rb_define_method(cTTFFont,"familyName",Font_familyName,0);
  rb_define_method(cTTFFont,"styleName",Font_styleName,0);

  rb_define_method(cTTFFont,"height",Font_height,0);
  rb_define_method(cTTFFont,"ascent",Font_ascent,0);
  rb_define_method(cTTFFont,"descent",Font_descent,0);
  rb_define_method(cTTFFont,"lineSkip",Font_lineSkip,0);
  
  rb_define_method(cTTFFont,"renderSolidUTF8",Font_renderSolidUTF8,4);
  rb_define_method(cTTFFont,"renderBlendedUTF8",Font_renderBlendedUTF8,4);
  rb_define_method(cTTFFont,"renderShadedUTF8",Font_renderShadedUTF8,7);
  rb_define_method(cTTFFont,"renderSolid",Font_renderSolid,4);
  rb_define_method(cTTFFont,"renderBlended",Font_renderBlended,4);
  rb_define_method(cTTFFont,"renderShaded",Font_renderShaded,7);
  rb_define_method(cTTFFont,"close", Font_close, 0);
  rb_define_method(cTTFFont,"closed?", Font_closed, 0);
  
  rb_define_const(cTTFFont,"STYLE_NORMAL",UINT2NUM(TTF_STYLE_NORMAL));
  rb_define_const(cTTFFont,"STYLE_BOLD",UINT2NUM(TTF_STYLE_BOLD));
  rb_define_const(cTTFFont,"STYLE_ITALIC",UINT2NUM(TTF_STYLE_ITALIC));
  rb_define_const(cTTFFont,"STYLE_UNDERLINE",UINT2NUM(TTF_STYLE_UNDERLINE));
}

void rubysdl_quit_TTF(void)
{
  if(ttf_init)
    TTF_Quit();
}
#else /* HAVE_SDL_TTF */
void rubysdl_init_TTF(void)
{
}
void rubysdl_quit_TTF(void)
{
}
#endif /* HAVE_SDL_TTF */
