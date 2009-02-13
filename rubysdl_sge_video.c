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
#ifdef HAVE_SGE

#include "rubysdl.h"
#include <sge.h>

static VALUE cCollisionMap = Qnil;
static VALUE cBMFont = Qnil;

typedef struct {
  sge_bmpFont* font;
} BmpFont;

DEFINE_GET_STRUCT(BmpFont, Get_BmpFont, cBMFont, "SDL::BMFont");
DEFINE_GET_STRUCT(sge_cdata, Get_sge_cdata, cCollisionMap, "SDL::CollisionMap");

static sge_bmpFont* Get_sge_bmpFont(VALUE obj)
{
  BmpFont* bfont = Get_BmpFont(obj);
  if (bfont->font == NULL)
    rb_raise(eSDLError, "Bitmap font is already closed");
  return bfont->font;
}
static void BMFont_free(BmpFont* bfont)
{
  if (!rubysdl_is_quit() && bfont->font)
    sge_BF_CloseFont(bfont->font);
  free(bfont);
}

static VALUE BMFont_create(sge_bmpFont* font)
{
  BmpFont* bfont = ALLOC(BmpFont);
  bfont->font = font;
  return Data_Wrap_Struct(cBMFont, 0, BMFont_free, bfont);
}

static VALUE Surface_s_autoLock_p(VALUE klass)
{
  rb_secure(4);
  return INT2BOOL(sge_getLock());
}

static VALUE Surface_s_autoLockON(VALUE klass)
{
  rb_secure(4);
  sge_Lock_ON();
  return Qnil;
}

static VALUE Surface_s_autoLockOFF(VALUE klass)
{
  rb_secure(4);
  sge_Lock_OFF();
  return Qnil;
}

static VALUE Surface_drawLine(int argc, VALUE* argv, VALUE self)
{
  VALUE x1_, y1_, x2_, y2_, color_, aa_, alpha_;
  Sint16 x1, y1, x2, y2;
  Uint32 color;
  SDL_Surface* surface;
  
  rb_secure(4);
  rb_scan_args(argc, argv, "52", &x1_, &y1_, &x2_, &y2_, &color_, &aa_, &alpha_);
  surface = Get_SDL_Surface(self);
  
  x1 = NUM2INT(x1_); y1 = NUM2INT(y1_);
  x2 = NUM2INT(x2_); y2 = NUM2INT(y2_);
  color = VALUE2COLOR(color_, surface->format);
  
  if(RTEST(aa_) && RTEST(alpha_))
    sge_AALineAlpha(surface, x1, y1, x2, y2, color, NUM2UINT(alpha_));
  
  if(RTEST(aa_) && !RTEST(alpha_))
    sge_AALine(surface, x1, y1, x2, y2, color);

  if(!RTEST(aa_) && RTEST(alpha_))
    sge_LineAlpha(surface, x1, y1, x2, y2, color, NUM2UINT(alpha_));

  if(!RTEST(aa_) && !RTEST(alpha_))
    sge_Line(surface, x1, y1, x2, y2, color);

  return Qnil;
}

static VALUE Surface_drawRect(int argc, VALUE* argv, VALUE self)
{
  VALUE x_, y_, w_, h_, color_, fill_, alpha_;
  Sint16 x1, y1, x2, y2;
  Uint32 color;
  SDL_Surface* surface;

  rb_scan_args(argc, argv, "52", &x_, &y_, &w_, &h_, &color_, &fill_, &alpha_);
  
  surface = Get_SDL_Surface(self);
  x1 = NUM2INT(x_); y1 = NUM2INT(y_);
  x2 = x1 + NUM2INT(w_); y2 = y1 + NUM2INT(h_);
  color = VALUE2COLOR(color_, surface->format);
  
  if(RTEST(fill_) && RTEST(alpha_))
    sge_FilledRectAlpha(surface, x1, y1, x2, y2, color, NUM2UINT(alpha_));

  if(RTEST(fill_) && !RTEST(alpha_))
    sge_FilledRect(surface, x1, y1, x2, y2, color);

  if(!RTEST(fill_) && RTEST(alpha_))
    sge_RectAlpha(surface, x1, y1, x2, y2, color, NUM2UINT(alpha_));

  if(!RTEST(fill_) && !RTEST(alpha_))
    sge_Rect(surface, x1, y1, x2, y2, color);
  
  return Qnil;
}

static VALUE Surface_drawCircle(int argc, VALUE* argv, VALUE self)
{
  SDL_Surface* surface;
  VALUE x_, y_, r_,  color_, fill_, aa_, alpha_;
  Sint16 x, y, r;
  Uint32 color;

  rb_secure(4);
  rb_scan_args(argc, argv, "43", &x_, &y_, &r_, &color_, &fill_, &aa_, &alpha_);

  surface = Get_SDL_Surface(self);
  x = NUM2INT(x_);
  y = NUM2INT(y_);
  r = NUM2INT(r_);
  color = VALUE2COLOR(color_, surface->format);

  if(RTEST(fill_) && RTEST(aa_) && RTEST(alpha_))
    rb_raise(eSDLError, "can't draw filled antialiased alpha circle");

  if(RTEST(fill_) && RTEST(aa_) && !RTEST(alpha_))
    sge_AAFilledCircle(surface, x, y, r, color);

  if(RTEST(fill_) && !RTEST(aa_) && RTEST(alpha_))
    sge_FilledCircleAlpha(surface, x, y, r, color, NUM2UINT(alpha_));

  if(RTEST(fill_) && !RTEST(aa_) && !RTEST(alpha_))
    sge_FilledCircle(surface, x, y, r, color);

  
  if(!RTEST(fill_) && RTEST(aa_) && RTEST(alpha_))
    sge_AACircleAlpha(surface, x, y, r, color, NUM2UINT(alpha_));

  if(!RTEST(fill_) && RTEST(aa_) && !RTEST(alpha_))
    sge_AACircle(surface, x, y, r, color);

  if(!RTEST(fill_) && !RTEST(aa_) && RTEST(alpha_))
    sge_CircleAlpha(surface, x, y, r, color, NUM2UINT(alpha_));

  if(!RTEST(fill_) && !RTEST(aa_) && !RTEST(alpha_))
    sge_Circle(surface, x, y, r, color);

  return Qnil;
}

static VALUE Surface_drawEllipse(int argc, VALUE* argv, VALUE self)
{
  SDL_Surface* surface;
  VALUE x_, y_, rx_, ry_, color_, fill_, aa_, alpha_;
  Sint16 x, y, rx, ry;
  Uint32 color;

  rb_secure(4);
  rb_scan_args(argc, argv, "53", &x_, &y_, &rx_, &ry_, &color_,
               &fill_, &aa_, &alpha_);

  surface = Get_SDL_Surface(self);
  x = NUM2INT(x_);
  y = NUM2INT(y_);
  rx = NUM2INT(rx_);
  ry = NUM2INT(ry_);
  color = VALUE2COLOR(color_, surface->format);

  if(RTEST(fill_) && RTEST(aa_) && RTEST(alpha_))
    rb_raise(eSDLError, "can't draw filled antialiased alpha ellipse");

  if(RTEST(fill_) && RTEST(aa_) && !RTEST(alpha_))
    sge_AAFilledEllipse(surface, x, y, rx, ry, color);

  if(RTEST(fill_) && !RTEST(aa_) && RTEST(alpha_))
    sge_FilledEllipseAlpha(surface, x, y, rx, ry, color, NUM2UINT(alpha_));

  if(RTEST(fill_) && !RTEST(aa_) && !RTEST(alpha_))
    sge_FilledEllipse(surface, x, y, rx, ry, color);

  if(!RTEST(fill_) && RTEST(aa_) && RTEST(alpha_))
    sge_AAEllipseAlpha(surface, x, y, rx, ry, color, NUM2UINT(alpha_));

  if(!RTEST(fill_) && RTEST(aa_) && !RTEST(alpha_))
    sge_AAEllipse(surface, x, y, rx, ry, color);

  if(!RTEST(fill_) && !RTEST(aa_) && RTEST(alpha_))
    sge_EllipseAlpha(surface, x, y, rx, ry, color, NUM2UINT(alpha_));

  if(!RTEST(fill_) && !RTEST(aa_) && !RTEST(alpha_))
    sge_Ellipse(surface, x, y, rx, ry, color);

  return Qnil;
}

static VALUE Surface_drawBezier(int argc, VALUE* argv, VALUE self)
{
  SDL_Surface* surface;
  VALUE coords_[8], level_, color_;
  VALUE aa_, alpha_;
  int coords[8];
  Uint32 color;
  int level;
  int i;
  
  rb_secure(4);
  /* WARNING:  ':' == '9' + 1
  */
  rb_scan_args(argc, argv, ":2",
               &coords_[0], &coords_[1],
               &coords_[2], &coords_[3],
               &coords_[4], &coords_[5],
               &coords_[6], &coords_[7],
               &level_, &color_,
               &aa_, &alpha_);
  surface = Get_SDL_Surface(self);
  for (i=0; i<8; i++)
    coords[i] = NUM2INT(coords_[i]);
  color = VALUE2COLOR(color_, surface->format);
  level = NUM2INT(level_);
  
  if(RTEST(aa_) && RTEST(alpha_))
    sge_AABezierAlpha(surface,
                      coords[0], coords[1],
                      coords[2], coords[3],
                      coords[4], coords[5],
                      coords[6], coords[7],
                      level, color, NUM2UINT(alpha_));
  if(!RTEST(aa_) && RTEST(alpha_))
    sge_BezierAlpha(surface,
                    coords[0], coords[1],
                    coords[2], coords[3],
                    coords[4], coords[5],
                    coords[6], coords[7],
                    level, color, NUM2UINT(alpha_));
  if(RTEST(aa_) && !RTEST(alpha_))
    sge_AABezier(surface,
                 coords[0], coords[1],
                 coords[2], coords[3],
                 coords[4], coords[5],
                 coords[6], coords[7],
                 level, color);
  if(!RTEST(aa_) && !RTEST(alpha_))
    sge_Bezier(surface,
               coords[0], coords[1],
               coords[2], coords[3],
               coords[4], coords[5],
               coords[6], coords[7],
               level, color);
  return Qnil;
}
  
static VALUE Surface_s_transformDraw(VALUE klass, VALUE src, VALUE dst,
                                     VALUE angle, 
                                     VALUE xscale, VALUE yscale,
                                     VALUE px, VALUE py, 
                                     VALUE qx, VALUE qy,
                                     VALUE flags)
{
  rb_secure(4);
  sge_transform(Get_SDL_Surface(src), Get_SDL_Surface(dst),
                NUM2DBL(angle), NUM2DBL(xscale), NUM2DBL(yscale),
		NUM2INT(px), NUM2INT(py), NUM2INT(qx), NUM2INT(qy),
                NUM2UINT(flags));
  return Qnil;
}

static VALUE Surface_transform(VALUE self, VALUE bgcolor, VALUE angle, 
                               VALUE xscale, VALUE yscale, VALUE flags)
{
  SDL_Surface *surface, *result;
  
  rb_secure(4);
  surface = Get_SDL_Surface(self);
  result = sge_transform_surface(surface, VALUE2COLOR(bgcolor, surface->format), 
				 NUM2DBL(angle), NUM2DBL(xscale), 
				 NUM2DBL(yscale), NUM2UINT(flags));
  if(result == NULL)
    rb_raise(eSDLError, "Couldn't Create Surface: %s", SDL_GetError());
  return Surface_create(result);
}

static VALUE Surface_makeCollisionMap(VALUE self)
{
  sge_cdata * cdata;
  rb_secure(4);
  cdata = sge_make_cmap(Get_SDL_Surface(self));
  if(cdata == NULL)
    rb_raise(eSDLError, "Couldn't Create CollisionMap: %s", SDL_GetError());
  return Data_Wrap_Struct(cCollisionMap, 0, sge_destroy_cmap, cdata);
}


static VALUE CollisionMap_s_boundingBoxCheck(VALUE klass, 
                                             VALUE x1, VALUE y1,
                                             VALUE w1, VALUE h1, 
                                             VALUE x2, VALUE y2,
                                             VALUE w2, VALUE h2)
{
  return INT2BOOL(_sge_bbcheck
                  ((Sint16) NUM2INT(x1),  (Sint16) NUM2INT(y1), 
                   (Sint16) NUM2INT(w1),  (Sint16) NUM2INT(h1), 
                   (Sint16) NUM2INT(x2),  (Sint16) NUM2INT(y2), 
                   (Sint16) NUM2INT(w2),  (Sint16) NUM2INT(h2)));
}

static VALUE CollisionMap_collisionCheck(VALUE collisionMap1,
                                           VALUE x1,  VALUE y1, 
                                           VALUE collisionMap2,
                                           VALUE x2,  VALUE y2)
{
  sge_cdata * cdata1 = Get_sge_cdata(collisionMap1);
  sge_cdata * cdata2 = Get_sge_cdata(collisionMap2);
  int collided;
  collided = sge_cmcheck
    (cdata1,  (Sint16) NUM2INT(x1),  (Sint16) NUM2INT(y1), 
     cdata2,  (Sint16) NUM2INT(x2),  (Sint16) NUM2INT(y2));
  if(!collided)
    return Qnil;
  return rb_ary_new3(2,  INT2NUM(sge_get_cx()),  INT2NUM(sge_get_cy()));
}

static VALUE CollisionMap_boundingBoxCheck(VALUE collisionMap1,
                                           VALUE x1,  VALUE y1, 
                                           VALUE collisionMap2,
                                           VALUE x2,  VALUE y2)
{
  sge_cdata * cdata1 = Get_sge_cdata(collisionMap1);
  sge_cdata * cdata2 = Get_sge_cdata(collisionMap2);
  return INT2BOOL(sge_bbcheck
                  (cdata1,  (Sint16) NUM2INT(x1),  (Sint16) NUM2INT(y1), 
                   cdata2,  (Sint16) NUM2INT(x2),  (Sint16) NUM2INT(y2)));
}

static VALUE CollisionMap_set(VALUE self, VALUE vx, VALUE vy, VALUE vw, VALUE vh)
{
  sge_cdata * cdata = Get_sge_cdata(self);
  Sint16 x,  y,  w,  h;

  rb_secure(4);
  x = NUM2INT(vx);
  y = NUM2INT(vy);
  w = NUM2INT(vw);
  h = NUM2INT(vh);
  if( x < 0 || y < 0 || x+w > cdata->w || y+h > cdata->h ){
    rb_raise(eSDLError, "Couldn't set that area");
  }
  sge_set_cdata(cdata,  x,  y,  w,  h);
  return Qnil;
}

static VALUE CollisionMap_clear(VALUE self, VALUE vx, VALUE vy, VALUE vw, VALUE vh)
{
  sge_cdata * cdata = Get_sge_cdata(self);
  Sint16 x,  y,  w,  h;

  rb_secure(4);
  x = NUM2INT(vx);
  y = NUM2INT(vy);
  w = NUM2INT(vw);
  h = NUM2INT(vh);
  if( x < 0 || y < 0 || x+w > cdata->w || y+h > cdata->h ){
    rb_raise(eSDLError, "Couldn't clear that area");
  }
  sge_unset_cdata(cdata,  x,  y,  w,  h);
  return Qnil;
}

static VALUE CollisionMap_w(VALUE self)
{
  return INT2FIX(Get_sge_cdata(self)->w);
}

static VALUE CollisionMap_h(VALUE self)
{
  return INT2FIX(Get_sge_cdata(self)->h);
}

/* bitmap font */

static VALUE BMFont_open(VALUE klass,  VALUE file,  VALUE flags)

{
  sge_bmpFont* font;
  rb_secure(4);
  ExportFilenameStringValue(file);
  
  font = sge_BF_OpenFont(RSTRING_PTR(file), NUM2UINT(flags));
  if(font == NULL)
    rb_raise(eSDLError, "Couldn't open font: %s", RSTRING_PTR(file));
  
  return BMFont_create(font);
}
static VALUE BMFont_close(VALUE self)
{
  BmpFont* bfont = Get_BmpFont(self);
  if (!rubysdl_is_quit() && bfont->font)
    sge_BF_CloseFont(bfont->font);
  bfont->font = NULL;
  return Qnil;
}
static VALUE BMFont_closed(VALUE self)
{
  return INT2BOOL(Get_BmpFont(self)->font == NULL);
}

static VALUE BMFont_setColor(VALUE self, VALUE r, VALUE g, VALUE b)
{
  sge_BF_SetColor(Get_sge_bmpFont(self),
                  NUM2UINT(r), NUM2UINT(g), NUM2UINT(b));
  return Qnil;
}

static VALUE BMFont_getHeight(VALUE self)
{
  return INT2FIX(sge_BF_GetHeight(Get_sge_bmpFont(self)));
}

static VALUE BMFont_getWidth(VALUE self)
{
  return INT2FIX(sge_BF_GetWidth(Get_sge_bmpFont(self)));
}

static VALUE BMFont_textSize(VALUE self, VALUE text)
{
  SDL_Rect rect;
  SafeStringValue(text);
  rect = sge_BF_TextSize(Get_sge_bmpFont(self),
                         RSTRING_PTR(text));
  return rb_ary_new3(2, INT2FIX(rect.w), INT2FIX(rect.h));
}

static VALUE BMFont_textout(VALUE self,
                            VALUE surface, VALUE string, 
                            VALUE x,  VALUE y)

{
  rb_secure(4);
  SafeStringValue(string);
  
  sge_BF_textout(Get_SDL_Surface(surface), Get_sge_bmpFont(self),
                 RSTRING_PTR(string), NUM2INT(x), NUM2INT(y));
  return Qnil;
}

void rubysdl_init_sge(VALUE mSDL, VALUE cSurface)
{
  sge_Update_OFF();
  sge_Lock_ON();
  
  rb_define_module_function(cSurface, "autoLock?", Surface_s_autoLock_p, 0);
  rb_define_module_function(cSurface, "autoLockON", Surface_s_autoLockON, 0);
  rb_define_module_function(cSurface, "autoLockOFF", Surface_s_autoLockOFF, 0);

  /* primitive drawing */
  rb_define_method(cSurface, "drawLine", Surface_drawLine, -1);
  rb_define_method(cSurface, "drawRect", Surface_drawRect, -1);
  rb_define_method(cSurface, "drawCircle", Surface_drawCircle, -1);
  rb_define_method(cSurface, "drawEllipse", Surface_drawEllipse, -1);
  rb_define_method(cSurface, "drawBezier", Surface_drawBezier, -1);

  /* rotation and scaling */
  rb_define_module_function(cSurface, "transformDraw",
                            Surface_s_transformDraw, 10);
  rb_define_method(cSurface, "transformSurface", Surface_transform, 5);

  /* collision detection */
  cCollisionMap = rb_define_class_under(mSDL, "CollisionMap", rb_cObject);
  rb_undef_alloc_func(cCollisionMap);
  
  rb_define_method(cSurface, "makeCollisionMap",  Surface_makeCollisionMap,  0);
  rb_define_singleton_method(cCollisionMap, "boundingBoxCheck", 
                             CollisionMap_s_boundingBoxCheck,  8);
  rb_define_method(cCollisionMap, "collisionCheck",
                   CollisionMap_collisionCheck,  5);
  rb_define_method(cCollisionMap, "boundingBoxCheck",
                   CollisionMap_boundingBoxCheck,  5);
  rb_define_method(cCollisionMap, "clear",  CollisionMap_clear,  4);
  rb_define_method(cCollisionMap, "set",  CollisionMap_set,  4);
  rb_define_method(cCollisionMap, "w", CollisionMap_w, 0);
  rb_define_method(cCollisionMap, "h", CollisionMap_h, 0);
  

  /* bitmap font */
  cBMFont = rb_define_class_under(mSDL, "BMFont", rb_cObject);
  rb_undef_alloc_func(cBMFont);
    
  rb_define_singleton_method(cBMFont, "open", BMFont_open, 2);
  rb_define_method(cBMFont, "close", BMFont_close, 0);
  rb_define_method(cBMFont, "closed?", BMFont_closed, 0);
  rb_define_method(cBMFont, "setColor", BMFont_setColor, 3);
  rb_define_method(cBMFont, "height", BMFont_getHeight, 0);
  rb_define_method(cBMFont, "width", BMFont_getWidth, 0);
  rb_define_method(cBMFont, "textSize", BMFont_textSize,1);
  rb_define_method(cBMFont, "textout", BMFont_textout, 4);


  rb_define_const(cSurface, "TRANSFORM_AA", UINT2NUM(SGE_TAA));
  rb_define_const(cSurface, "TRANSFORM_SAFE", UINT2NUM(SGE_TSAFE));
  rb_define_const(cSurface, "TRANSFORM_TMAP", UINT2NUM(SGE_TTMAP));

  rb_define_const(cBMFont, "TRANSPARENT", UINT2NUM(SGE_BFTRANSP));
  rb_define_const(cBMFont, "NOCONVERT", UINT2NUM(SGE_BFNOCONVERT));
  rb_define_const(cBMFont, "SFONT", UINT2NUM(SGE_BFSFONT));
  rb_define_const(cBMFont, "PALETTE", UINT2NUM(SGE_BFPALETTE));
}
#else /* HAVE_SGE */
#include "rubysdl.h"
void rubysdl_init_sge(VALUE mSDL, VALUE cSurface)
{
}
#endif /* HAVE_SGE */
