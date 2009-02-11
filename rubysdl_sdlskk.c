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
#ifdef HAVE_SDLSKK
#include "rubysdl.h"
#include <sdlskk.h>
#include <SDL_ttf.h>

static VALUE cEvent = Qnil;
static VALUE cKeyDownEvent = Qnil;

static VALUE cContext = Qnil;
static VALUE cDictionary = Qnil;
static VALUE cRomKanaRuleTable = Qnil;
static VALUE cKeybind = Qnil;

DEFINE_GET_STRUCT(SDLSKK_Context, Get_SDLSKK_Context, cContext, "SDL::SKK::Context");
DEFINE_GET_STRUCT(SDLSKK_Dictionary, Get_SDLSKK_Dictionary, cDictionary, "SDL::SKK::Dictionary");
DEFINE_GET_STRUCT(SDLSKK_RomKanaRuleTable, Get_SDLSKK_RomKanaRuleTable, cRomKanaRuleTable, "SDL::SKK::RomKanaRuleTable");
DEFINE_GET_STRUCT(SDLSKK_Keybind, Get_SDLSKK_Keybind, cKeybind, "SDL::SKK::Keybind");

typedef SDL_Surface* (*Renderer)(SDLSKK_Context*,TTF_Font*,SDL_Color);

static void skk_error_handler(SDLSKK_Error err)
{
  switch( err ){
  case SDLSKK_NOERROR:
    return;
  case SDLSKK_MEMERROR:
    rb_fatal("SDLSKK: memory allocation error");
  }
}

static VALUE SKK_set_encoding(VALUE mod, VALUE encoding)
{
  SDLSKK_set_encoding(NUM2INT(encoding));
  return Qnil;
}

static VALUE SKK_get_encoding(VALUE mod)
{
  return INT2FIX(SDLSKK_get_encoding());
}

static VALUE Context_s_new(VALUE klass, VALUE dict, VALUE rule_table,
                           VALUE keybind, VALUE use_minibuffer)
{
  SDLSKK_Context* c_context;
  VALUE context;
  
  c_context = SDLSKK_Context_new(Get_SDLSKK_Dictionary(dict),
                                 Get_SDLSKK_RomKanaRuleTable(rule_table),
                                 Get_SDLSKK_Keybind(keybind),
                                 RTEST(use_minibuffer));
  
  if(c_context == NULL)
    rb_raise(eSDLError,"Couldn't create Context");

  context = Data_Wrap_Struct(klass, 0, SDLSKK_Context_delete ,c_context);
  rb_iv_set(context, "dict", dict);
  rb_iv_set(context, "rule_table", rule_table);
  
  return context;
}

static VALUE Context_input(VALUE self, VALUE event)
{
  if(rb_obj_is_kind_of(event, cKeyDownEvent)){
    SDL_Event ev;
    ev.type = SDL_KEYDOWN;
    ev.key.keysym.sym = NUM2INT(rb_iv_get(event, "@sym"));
    ev.key.keysym.unicode = NUM2UINT(rb_iv_get(event, "@unicode"));
    ev.key.keysym.mod = NUM2INT(rb_iv_get(event, "@mod"));
    SDLSKK_Context_input_event(Get_SDLSKK_Context(self), &ev);
    return Qnil;
  }else if(rb_obj_is_kind_of(event, cEvent)){
    return Qnil;
  }else{
    rb_raise( rb_eArgError,"type mismatch(expect SDL::Event or SDL::Event2)");
  }
  /* NOT REACHED */
}

static VALUE Context_str(VALUE self)
{
  char cstr[10000];
#ifdef ENABLE_M17N
  rb_encoding* enc;
  switch (SDLSKK_get_encoding()) {
  case SDLSKK_UTF8:
    enc = utf8_enc;
  case SDLSKK_EUCJP:
    enc = eucjp_enc;
    break;
  case SDLSKK_SJIS:
    enc = sjis_enc;
    break;
  default:
    rb_raise(eSDLError, "SDLSKK encoding error");
  }
#endif
  SDLSKK_Context_get_str(Get_SDLSKK_Context(self), cstr, sizeof(cstr));
#ifdef ENABLE_M17N
  return ENC_STR_NEW2(cstr, enc);
#else
  return rb_str_new2(cstr);
#endif
}

static VALUE render_str(VALUE self, VALUE font, VALUE r, VALUE g, VALUE b,
                        Renderer func)
{
  SDL_Surface* surface;
  SDL_Color color;
  
  color.r = NUM2UINT(r);
  color.g = NUM2UINT(g);
  color.b = NUM2UINT(b);
  
  surface = func(Get_SDLSKK_Context(self), Get_TTF_Font(font), color);

  if(surface == NULL)
    return Qnil;

  return Surface_create(surface);
}

static VALUE Context_render_str(VALUE self, VALUE font,
                                VALUE r, VALUE g, VALUE b)
     
{
  return render_str(self, font, r, g, b, SDLSKK_Context_render_display_str);
}

static VALUE Context_render_minibuffer_str(VALUE self, VALUE font,
                                           VALUE r, VALUE g,VALUE b)
{
  return render_str(self, font, r, g, b, SDLSKK_Context_render_minibuffer_str);
}

static VALUE Context_get_basic_mode(VALUE self)
{
  return INT2BOOL(SDLSKK_Context_get_basic_mode(Get_SDLSKK_Context(self)));
}

static VALUE Context_clear(VALUE self)
{
  SDLSKK_Context_clear(Get_SDLSKK_Context(self));
  return Qnil;
}

static VALUE Context_clear_text(VALUE self)
{
  SDLSKK_Context_clear_text(Get_SDLSKK_Context(self));
  return Qnil;
}

static VALUE Dictionary_s_new(VALUE klass)
{
  SDLSKK_Dictionary* dict;

  dict = SDLSKK_Dict_new();
  if(dict == NULL)
    rb_raise(eSDLError, "Couldn't create SDL::SKK::Dictionary");
  
  return Data_Wrap_Struct(klass, 0, SDLSKK_Dict_delete, dict);
}

static VALUE Dictionary_load(VALUE self, VALUE filename, VALUE users)
{
  SDLSKK_Dictionary* dict = Get_SDLSKK_Dictionary(self);
  rb_secure(4);
  SafeStringValue(filename);
  
  if(!SDLSKK_Dict_load(dict, RSTRING_PTR(filename), RTEST(users)))
    rb_raise(eSDLError, "Couldn't load %s", RSTRING_PTR(filename));

  return Qnil;
}

static VALUE Dictionary_save(VALUE self, VALUE filename)
{
  SDLSKK_Dictionary* dict = Get_SDLSKK_Dictionary(self);
  rb_secure(4);
  SafeStringValue(self);
  
  if(!SDLSKK_Dict_save_user_dict(dict, RSTRING_PTR(filename)))
    rb_raise(eSDLError, "Couldn't save %s", RSTRING_PTR(filename));
  return Qnil;
}

static VALUE RomKanaRuleTable_s_new(VALUE klass, VALUE table_file)
{
  SDLSKK_RomKanaRuleTable* rule_table;
  rb_secure(4);
  SafeStringValue(table_file);
  
  rule_table = SDLSKK_RomKanaRuleTable_new(RSTRING_PTR(table_file));

  if(rule_table == NULL)
    rb_raise(eSDLError, "Couldn't load %s", RSTRING_PTR(table_file));
  
  return Data_Wrap_Struct(klass, 0, SDLSKK_RomKanaRuleTable_delete, rule_table);
}

static VALUE Keybind_s_new(VALUE klass)
{
  return Data_Wrap_Struct(klass, 0, SDLSKK_Keybind_delete, SDLSKK_Keybind_new());
}

static VALUE Keybind_set_key(VALUE self, VALUE key_str, VALUE cmd_str)
{
  SDLSKK_Keybind_set_key(Get_SDLSKK_Keybind(self),
                         StringValuePtr(key_str),
                         StringValuePtr(cmd_str));
  return Qnil;
}

static VALUE Keybind_set_default_key(VALUE self)
{
  SDLSKK_Keybind_set_default_key(Get_SDLSKK_Keybind(self));
  return Qnil;
}

static VALUE Keybind_unset_key(VALUE self, VALUE key_str)
{
  SDLSKK_Keybind_unset_key(Get_SDLSKK_Keybind(self), StringValuePtr(key_str));
  return Qnil;
}

void rubysdl_init_SKK(VALUE mSDL)
{
  VALUE mSDLSKK;
  
  cEvent = rb_const_get(mSDL,  rb_intern("Event"));
  cKeyDownEvent = rb_const_get(cEvent,  rb_intern("KeyDown"));
  
  mSDLSKK = rb_define_module_under(mSDL, "SKK");
  cContext = rb_define_class_under(mSDLSKK, "Context", rb_cObject);
  cDictionary = rb_define_class_under(mSDLSKK, "Dictionary", rb_cObject);
  cRomKanaRuleTable = rb_define_class_under(mSDLSKK, "RomKanaRuleTable", 
					    rb_cObject);
  cKeybind = rb_define_class_under(mSDLSKK, "Keybind", rb_cObject);

  rb_undef_alloc_func(cContext);
  rb_undef_alloc_func(cDictionary);
  rb_undef_alloc_func(cRomKanaRuleTable);
  rb_undef_alloc_func(cKeybind);
  
  rb_define_module_function(mSDLSKK, "encoding=", SKK_set_encoding, 1);
  rb_define_module_function(mSDLSKK, "encoding", SKK_get_encoding, 0);
  
  rb_define_singleton_method(cContext, "new", Context_s_new, 4);
  rb_define_method(cContext, "input", Context_input, 1);
  rb_define_method(cContext, "str", Context_str, 0);
  rb_define_method(cContext, "render_str", Context_render_str, 4);
  rb_define_method(cContext, "render_minibuffer_str",
                   Context_render_minibuffer_str, 4);
  rb_define_method(cContext, "get_basic_mode", Context_get_basic_mode, 0);
  rb_define_method(cContext, "clear", Context_clear, 0);
  rb_define_method(cContext, "clear_text", Context_clear_text, 0);
  
  rb_define_singleton_method(cDictionary, "new", Dictionary_s_new, 0);
  rb_define_method(cDictionary, "load", Dictionary_load, 2);
  rb_define_method(cDictionary, "save", Dictionary_save, 1);
  
  rb_define_singleton_method(cRomKanaRuleTable, "new", 
			     RomKanaRuleTable_s_new, 1);

  rb_define_singleton_method(cKeybind, "new", Keybind_s_new, 0);
  rb_define_method(cKeybind, "set_key", Keybind_set_key, 2);
  rb_define_method(cKeybind, "set_default_key", Keybind_set_default_key, 0);
  rb_define_method(cKeybind, "unset_key", Keybind_unset_key, 1);
  
  SDLSKK_set_error_func(skk_error_handler);

  
  rb_define_const(mSDLSKK, "EUCJP", INT2NUM(SDLSKK_EUCJP));
  rb_define_const(mSDLSKK, "UTF8", INT2NUM(SDLSKK_UTF8));
  rb_define_const(mSDLSKK, "SJIS", INT2NUM(SDLSKK_SJIS));
}
#else /* HAVE_SDLSKK */
#include "rubysdl.h"
void rubysdl_init_SKK(VALUE mSDL)
{
}
#endif /* HAVE_SDLSKK */
