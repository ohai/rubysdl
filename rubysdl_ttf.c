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
#ifdef HAVE_SDL_TTF
#include "rubysdl.h"
#include <SDL_ttf.h>

typedef SDL_Surface* (*RenderFunc)(TTF_Font *,const char *,SDL_Color,SDL_Color);

static int ttf_initialized=0;

static VALUE sdl_ttf_init(VALUE class)
{
  if( TTF_Init()== -1 )
    rb_raise(eSDLError,"Couldn't initialize TTF engine: %s",TTF_GetError());
  ttf_initialized=1;
  return Qnil;
}
static VALUE sdl_ttf_open(int argc, VALUE *argv, VALUE class)
{
  TTF_Font *font;
  VALUE filename, size, index;
  rb_scan_args( argc, argv, "21", &filename, &size, &index );
  if( NIL_P(index) )
    font=TTF_OpenFont( STR2CSTR(filename),NUM2INT(size) );
  else
#ifdef HAVE_TTF_OPENFONTINDEX
    font=TTF_OpenFontIndex( STR2CSTR(filename),NUM2INT(size),NUM2INT(index) );
#else
    if( index != 0)
      rb_raise(rb_eRuntimeError,"Not supported for selecting indivisual font face by SDL_ttf. The feature is in SDL_ttf 2.0.4 or later.");
#endif
  if( font==NULL )
    rb_raise(eSDLError,"Couldn't open font %s: %s",STR2CSTR(filename),
	     TTF_GetError());
  return Data_Wrap_Struct(class,0,TTF_CloseFont,font);
}
static VALUE sdl_ttf_getFontStyle(VALUE obj)
{
  TTF_Font *font;
  Data_Get_Struct(obj,TTF_Font,font);
  return INT2FIX( TTF_GetFontStyle(font) );
}
static VALUE sdl_ttf_setFontStyle(VALUE obj,VALUE style)
{
  TTF_Font *font;
  Data_Get_Struct(obj,TTF_Font,font);
  TTF_SetFontStyle(font,NUM2UINT(style));
  return Qnil;
}
static VALUE sdl_ttf_getFontFaces(VALUE obj)
{
#ifdef TTF_FONTFACES
  TTF_Font *font;
  Data_Get_Struct(obj,TTF_Font,font);
  return UINT2NUM( TTF_FontFaces(font) );
#else
  rb_raise(rb_eRuntimeError,"Not supported. The feature is in SDL_ttf 2.0.4 or later.");
#endif
}
static VALUE sdl_ttf_FontFaceIsFixedWidth(VALUE obj)
{
#ifdef TTF_FONTISFIXEDWIDTH
  TTF_Font *font;
  Data_Get_Struct(obj,TTF_Font,font);
  if( TTF_FontFaceIsFixedWidth(font) )
    return Qtrue;
  else
    return Qfalse;
#else
  rb_raise(rb_eRuntimeError,"Not supported. The feature is in SDL_ttf 2.0.4 or later.");
#endif
}
static VALUE sdl_ttf_FontFaceFamilyName(VALUE obj)
{
#ifdef TTF_FONTFACEFAMILYNAME
  TTF_Font *font;
  Data_Get_Struct(obj,TTF_Font,font);
  return rb_str_new2( (const char *)TTF_FontFaceFamilyName(font) );
#else
  rb_raise(rb_eRuntimeError,"Not supported. The feature is in SDL_ttf 2.0.4 or later.");
#endif
}
static VALUE sdl_ttf_FontFaceStyleName(VALUE obj)
{
#ifdef TTF_FONTFACESTYLENAME
  TTF_Font *font;
  Data_Get_Struct(obj,TTF_Font,font);
  return rb_str_new2( (const char *)TTF_FontFaceStyleName(font) );
#else
  rb_raise(rb_eRuntimeError,"Not supported. The feature is in SDL_ttf 2.0.4 or later.");
#endif
}
static VALUE sdl_ttf_sizeText(VALUE obj,VALUE text)
{
  TTF_Font *font;
  int w,h;
  Data_Get_Struct(obj,TTF_Font,font);
  TTF_SizeUTF8(font,STR2CSTR(text),&w,&h);
  return rb_ary_new3(2,INT2FIX(w),INT2FIX(h));
}
static VALUE ttf_draw(VALUE obj,VALUE dest,VALUE text,VALUE x,
		      VALUE y,VALUE fgr,VALUE fgg,VALUE fgb,
		      VALUE bgr,VALUE bgg,VALUE bgb,RenderFunc render)
{
  TTF_Font *font;
  SDL_Surface *destSurface, *tmpSurface;
  SDL_Color fg,bg;
  SDL_Rect destRect;
  int result;
  char *ctext=STR2CSTR(text);
  /* If text=="" , TTF_RenderUTF8_Solid() and etc fail to render */
  if( ctext[0]=='\0' )return INT2FIX(0);
  
  if( !rb_obj_is_kind_of( dest,cSurface ) )
    rb_raise( rb_eArgError,"type mismatch(expect Surface)");
  Data_Get_Struct(obj,TTF_Font,font);
  Data_Get_Struct(dest,SDL_Surface,destSurface);
  fg.r=NUM2UINT(fgr); fg.g=NUM2UINT(fgg); fg.b=NUM2UINT(fgb);
  bg.r=NUM2UINT(bgr); bg.g=NUM2UINT(bgg); bg.b=NUM2UINT(bgb);
  SetRect(destRect,x,y,1,1);
  
  tmpSurface=render(font,ctext,fg,bg);
  if( tmpSurface==NULL )
    rb_raise(eSDLError,"Text Render fail: %s",TTF_GetError());
  
  result=SDL_BlitSurface(tmpSurface,NULL,destSurface,&destRect);
  SDL_FreeSurface(tmpSurface);
  if( result == -1 ){
    rb_raise(eSDLError,"SDL_BlitSurface fail: %s",SDL_GetError());
  }
  return INT2NUM(result);
}

static SDL_Surface* ttf_wrap_RenderUTF8_Solid(TTF_Font *font,
					      const char *text,
					      SDL_Color fg,
					      SDL_Color bg)
{
  return TTF_RenderUTF8_Solid(font,text,fg);
}

static VALUE sdl_ttf_drawSolidUTF8(VALUE obj,VALUE dest,VALUE text,VALUE x,
				   VALUE y,VALUE r,VALUE g,VALUE b)
{
  return ttf_draw(obj,dest,text,x,y,r,g,b,1,1,1,ttf_wrap_RenderUTF8_Solid);
}

static SDL_Surface* ttf_wrap_RenderUTF8_Blended(TTF_Font *font,
						const char *text,
						SDL_Color fg,
						SDL_Color bg)
{
  return TTF_RenderUTF8_Blended(font,text,fg);
}

static VALUE sdl_ttf_drawBlendedUTF8(VALUE obj,VALUE dest,VALUE text,VALUE x,
				   VALUE y,VALUE r,VALUE g,VALUE b)
{
  return ttf_draw(obj,dest,text,x,y,r,g,b,1,1,1,ttf_wrap_RenderUTF8_Blended);
}

static VALUE sdl_ttf_drawShadedUTF8(VALUE obj,VALUE dest, VALUE text,VALUE x,
				    VALUE y,VALUE fgr,VALUE fgg,VALUE fgb,
				    VALUE bgr,VALUE bgg,VALUE bgb)
{
  return ttf_draw(obj,dest,text,x,y,fgr,fgg,fgb,bgr,bgg,bgb,
		  TTF_RenderUTF8_Shaded);
}
static void defineConstForTTF()
{
  rb_define_const(cTTF,"STYLE_NORMAL",UINT2NUM(TTF_STYLE_NORMAL));
  rb_define_const(cTTF,"STYLE_BOLD",UINT2NUM(TTF_STYLE_BOLD));
  rb_define_const(cTTF,"STYLE_ITALIC",UINT2NUM(TTF_STYLE_ITALIC));
  rb_define_const(cTTF,"STYLE_UNDERLINE",UINT2NUM(TTF_STYLE_UNDERLINE));
}
void init_ttf()
{
  cTTF=rb_define_class_under(mSDL,"TTF",rb_cObject);
  rb_define_singleton_method(cTTF,"init",sdl_ttf_init,0);
  rb_define_singleton_method(cTTF,"open",sdl_ttf_open,-1);
  
  rb_define_method(cTTF,"style",sdl_ttf_getFontStyle,0);
  rb_define_method(cTTF,"style=",sdl_ttf_setFontStyle,1);
  rb_define_method(cTTF,"textSize",sdl_ttf_sizeText,1);
  rb_define_method(cTTF,"faces",sdl_ttf_getFontFaces,0);
  rb_define_method(cTTF,"fixedWidth?",sdl_ttf_FontFaceIsFixedWidth,0);
  rb_define_method(cTTF,"familyName",sdl_ttf_FontFaceFamilyName,0);
  rb_define_method(cTTF,"styleName",sdl_ttf_FontFaceStyleName,0);

  rb_define_method(cTTF,"drawSolidUTF8",sdl_ttf_drawSolidUTF8,7);
  rb_define_method(cTTF,"drawBlendedUTF8",sdl_ttf_drawBlendedUTF8,7);
  rb_define_method(cTTF,"drawShadedUTF8",sdl_ttf_drawShadedUTF8,10);
  
  defineConstForTTF();
}
void quit_ttf()
{
  if(ttf_initialized)
    TTF_Quit();
}

#endif /* HAVE_SDL_TTF */
