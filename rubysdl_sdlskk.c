/*
  Ruby/SDL   Ruby extension library for SDL

  Copyright (C) 2001 Ohbayashi Ippei
  
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

static VALUE skk_Context_new(VALUE class,VALUE dict,VALUE rule_table)
{
  SDLSKK_Context* c_context;
  SDLSKK_RomKanaRuleTable* c_table;
  SDLSKK_Dictionary* c_dict;
  VALUE context;
  
  if( !rb_obj_is_kind_of(dict,cDictionary) )
    rb_raise(rb_eArgError,"type mismatch(expect SDL::SKK::Dictionary)");
  if( !rb_obj_is_kind_of(rule_table,cRomKanaRuleTable) )
    rb_raise(rb_eArgError,"type mismatch(expect SDL::SKK::RomKanaRuleTable)");

  Data_Get_Struct(dict,SDLSKK_Dictionary,c_dict);
  Data_Get_Struct(rule_table,SDLSKK_RomKanaRuleTable,c_table);

  c_context = SDLSKK_Context_new( c_dict, c_table );
  if( c_context == NULL )
    rb_raise(eSDLError,"Couldn't create Context");

  context = Data_Wrap_Struct(class,0,SDLSKK_Context_delete,c_context);
  rb_iv_set(context,"dict",dict);
  rb_iv_set(context,"rule_table",rule_table);
  
  return context;
}

static VALUE skk_Context_input_event(VALUE obj,VALUE event)
{
  SDLSKK_Context* context;
  SDL_Event *ev;
  
  Data_Get_Struct(obj,SDLSKK_Context,context);
  if( rb_obj_is_kind_of( event,cEvent ) ){
    Data_Get_Struct(event,SDL_Event,ev);
    SDLSKK_Context_input_event( context, ev );
    return Qnil;
  }
  
#ifdef DEF_EVENT2
  if( rb_obj_is_kind_of( event,cEvent2 ) ){
    if( rb_obj_is_kind_of( event,cKeyDownEvent ) ){
      SDL_Event ev2;
      ev2.type = SDL_KEYDOWN;
      ev2.key.keysym.sym = NUM2INT( rb_iv_get(event,"@sym") );
      ev2.key.keysym.unicode = NUM2UINT( rb_iv_get(event,"@unicode") );
      SDLSKK_Context_input_event( context, &ev2 );
      return Qnil;
    }else{
      return Qnil;
    }
  }
    
#endif
  rb_raise( rb_eArgError,"type mismatch(expect SDL::Event or SDL::Event2)");

  /* NOT REACHED */
}

static VALUE skk_Context_get_str(VALUE obj)
{
  SDLSKK_Context* context;
  char cstr[10000];
  
  Data_Get_Struct(obj,SDLSKK_Context,context);

  SDLSKK_Context_get_str(context,cstr,sizeof(cstr));

  return rb_str_new2(cstr);
}

static VALUE skk_Context_render_str(VALUE obj,VALUE font,VALUE r,VALUE g,
				    VALUE b)
{
  SDLSKK_Context* context;
  SDL_Surface* surface;
  TTF_Font* ttf_font;
  SDL_Color color;

  if( !rb_obj_is_kind_of(font,cTTF) )
    rb_raise( rb_eArgError,"type mismatch(expect SDL::TTF)");
  
  color.r = NUM2UINT(r);
  color.g = NUM2UINT(g);
  color.b = NUM2UINT(b);
  
  Data_Get_Struct(obj,SDLSKK_Context,context);
  Data_Get_Struct(font,TTF_Font,ttf_font);
  
  surface = SDLSKK_Context_render_display_str(context,ttf_font,color);

  if( surface == NULL )
    rb_raise(eSDLError,"Couldn't render string" );

  return Data_Wrap_Struct(cSurface,0,SDL_FreeSurface,surface);  
}

static VALUE skk_Dictionary_new(VALUE class,VALUE dict_file)
{
  SDLSKK_Dictionary* dict;

  dict = SDLSKK_Dict_new();
  if( dict == NULL )
    rb_raise(eSDLError,"Couldn't create SDL::SKK::Dictionary" );
  
  return Data_Wrap_Struct(class,0,SDLSKK_Dict_delete,dict);
}

static VALUE skk_Dict_load(VALUE obj,VALUE filename,VALUE users)
{
  SDLSKK_Dictionary* dict;

  Data_Get_Struct(obj,SDLSKK_Dictionary,dict);

  if( !SDLSKK_Dict_load(dict,STR2CSTR(filename),RTEST(users)) )
    rb_raise(eSDLError,"Couldn't load %s",STR2CSTR(filename));

  return Qnil;
}

static VALUE skk_RomKanaRuleTable_new(VALUE class,VALUE table_file)
{
  SDLSKK_RomKanaRuleTable* rule_table;

  rule_table = SDLSKK_RomKanaRuleTable_new( STR2CSTR(table_file) );

  if( rule_table == NULL )
    rb_raise(eSDLError,"Couldn't load %s",STR2CSTR(table_file));
  
  return Data_Wrap_Struct(class,0,SDLSKK_RomKanaRuleTable_delete,rule_table);
}

void init_sdlskk(void)
{
  mSDLSKK = rb_define_module_under(mSDL,"SKK");
  cContext = rb_define_class_under(mSDLSKK,"Context",rb_cObject);
  cDictionary = rb_define_class_under(mSDLSKK,"Dictionary",rb_cObject);
  cRomKanaRuleTable = rb_define_class_under(mSDLSKK,"RomKanaRuleTable",
					    rb_cObject);

  rb_define_singleton_method(cContext,"new",skk_Context_new,2);
  rb_define_method(cContext,"input",skk_Context_input_event,1);
  rb_define_method(cContext,"str",skk_Context_get_str,0);
  rb_define_method(cContext,"render_str",skk_Context_render_str,4);
  
  rb_define_singleton_method(cDictionary,"new",skk_Dictionary_new,0);
  rb_define_method(cDictionary,"load",skk_Dict_load,2);
  rb_define_singleton_method(cRomKanaRuleTable,"new",
			     skk_RomKanaRuleTable_new,1);  
}
#endif /* HAVE_SDLSKK */