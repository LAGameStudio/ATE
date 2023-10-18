
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "Art.h"
#include "Vertex.h"
#include "UglyFont.h"
#include "Trigger.h"

#include "Art2d.h"

#if defined(DEBUG)
Blends currentBlend=none;
#endif

inline void Blending( Blends blend ) {
#if defined(DEBUG)
 currentBlend=blend;
#endif
 Blending(blend.blend);
}

////
// SUPER USEFUL http://www.andersriggelsen.dk/glblendfunc.php
// glBlendEquation(GL_FUNC_ADD, GL_FUNC_SUBTRACT, or GL_FUNC_REVERSE_SUBTRACT);
////
inline void Blending( Blendings blend ) {
 glEnable(GL_BLEND);
 // Assumption:  glBlendEquation(GL_FUNC_ADD);
   switch ( blend ) {
         case _solarize: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO); break;
             case _mask: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_DST_COLOR,GL_ZERO); break;
         case _screened: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_ONE,GL_ONE); break;
         case _additive: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
      case _subtractive: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE); break;
         case _multiply: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_ZERO, GL_SRC_COLOR); break; // GL_ONE, GL_SRC_COLOR
          case _overlay: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
     case _transparency: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
         case _saturate: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE); break;
            case _mask1: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_DST_COLOR,GL_ONE); break;
             case _none: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_ONE,GL_ZERO); break;
     case _translucency: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR); break;
    case _transluminant: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA); break;
       case _silhouette: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
    case _subtractAlpha: glBlendEquation(GL_FUNC_REVERSE_SUBTRACT); glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_ALPHA); break;
 case _transparentAlpha: glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA); break;
     default: break;
   }
}

void DrawLine(GLWindow *surface, Crayon color, int x1, int y1, int x2, int y2)
{
 //if(x1 == x2) { x2++; x1--; } else if (y1 == y2) { x2++; x1--; } // Why is this here? I "fixed it?" below?
 glColor4fv(color.floats);
 glBegin(GL_LINES);
 glVertex2i(x1+surface->x, y1+surface->y);
 glVertex2i(x2+surface->x, y2+surface->y);
 glEnd();
}

void DrawLine(Crayon color, double x1, double y1, double x2, double y2)
{
 glColor4fv(color.floats);
 glBegin(GL_LINES);
 glVertex2d(x1, y1);
 glVertex2d(x2, y2);
 glEnd();
}

void DrawLine(Crayon color, int x1, int y1, int x2, int y2)
{
 glColor4fv(color.floats);
 glBegin(GL_LINES);
 glVertex2i(x1, y1);
 glVertex2i(x2, y2);
 glEnd();
}

void DrawCross(Crayon color, int x, int y, int cw)
{
 DrawLine(color,x-cw,y,x+cw,y);
 DrawLine(color,x,y-cw,x,y+cw);
}

void DrawLine2Color(GLWindow *surface, Crayon color1, Crayon color2, int x1, int y1, int x2, int y2)
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex2i(x1+surface->x, y1+surface->y);
 glColor4fv(color2.floats);
 glVertex2i(x2+surface->x, y2+surface->y);
 glEnd();
}

void DrawLine2Color(Crayon color1, Crayon color2, int x1, int y1, Zint x2, Zint y2) { DrawLine2Color(color1,color2,x1,y1,(int)x2,(int)y2); }
void DrawLine2Color(Crayon color1, Crayon color2, int x1, Zint y1, int x2, Zint y2) { DrawLine2Color(color1,color2,x1,(int)y1,x2,(int)y2); }
void DrawLine2Color(Crayon color1, Crayon color2, Zint x1, int y1, Zint x2, int y2) { DrawLine2Color(color1,color2,(int)x1,y1,(int)x2,y2); }
void DrawLine2Color(Crayon color1, Crayon color2, Zint x1, Zint y1, Zint x2, Zint y2) { DrawLine2Color(color1,color2,(int)x1,(int)y1,(int)x2,(int)y2); }
void DrawLine2Color(Crayon color1, Crayon color2, int x1, int y1, int x2, int y2)
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex2i(x1, y1);
 glColor4fv(color2.floats);
 glVertex2i(x2, y2);
 glEnd();
}

void DrawLine2Color(Crayon color1, Crayon color2, Zfloat x1, Zfloat y1, Zfloat x2, Zfloat y2) { DrawLine2Color(color1,color2,(float)x1,(float)y1,(float)x2,(float)y2); }
void DrawLine2Color(Crayon color1, Crayon color2, float x1, float y1, float x2, float y2)
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex2f(x1 + 0.5f, y1 + 0.5f);
 glColor4fv(color2.floats);
 glVertex2f(x2 + 0.5f, y2 + 0.5f);
 glEnd();
}

void DrawLine2Color(Crayon color1, Crayon color2, Zdouble x1, Zdouble y1, Zdouble x2, Zdouble y2) { DrawLine2Color(color1,color2,(double)x1,(double)y1,(double)x2,(double)y2); }
void DrawLine2Color(Crayon color1, Crayon color2, double x1, double y1, double x2, double y2)
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex2d(x1 + 0.5, y1 + 0.5);  // why add 0.5? ...
 glColor4fv(color2.floats);
 glVertex2d(x2 + 0.5, y2 + 0.5);
 glEnd();
}

void DrawLine2Color3d(Crayon color1, Crayon color2, double x1, double y1, double z1, double x2, double y2, double z2 ) {
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex3d(x1,y1,z1);
 glColor4fv(color2.floats);
 glVertex3d(x2,y2,z2);
 glEnd();
}

void DrawLine3d2Color(Crayon color1, Crayon color2, Vertexd *source, Vertexd *terminus )
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex3d(source->x,source->y,source->z);
 glColor4fv(color2.floats);
 glVertex3d(terminus->x,terminus->y,terminus->z);
 glEnd();
}

void Rectangle(Crayon color, int x, Zint y, Zint x2, Zint y2 ) { Rectangle(color,x,(int)y,(int)x2,(int)y2); }
void Rectangle(Crayon color, int x, int y, Zint x2, Zint y2 ) { Rectangle(color,x,y,(int)x2,(int)y2); }
void Rectangle(Crayon color, int x, int y, int x2, Zint y2 ) { Rectangle(color,x,y,x2,(int)y2); }
void Rectangle(Crayon color, Zint x, Zint y, Zint x2, Zint y2 ) { Rectangle(color,(int)x,(int)y,(int)x2,(int)y2); }
void Rectangle(Crayon color,int x, int y, int x2, int y2) {
 art.Rectangle(color,x,y,x2-x,1);
 art.Rectangle(color,x2,y,1,y2-y);
 art.Rectangle(color,x,y,1,y2-y);
 art.Rectangle(color,x,y2,x2-x,1);
}

void Rectangle(Crayon color,double x, Zdouble y, Zdouble x2, Zdouble y2 ) { Rectangle(color,x,(double)y,(double)x2,(double)y2); }
void Rectangle(Crayon color,double x, double y, Zdouble x2, Zdouble y2 ) { Rectangle(color,x,y,(double)x2,(double)y2); }
void Rectangle(Crayon color,double x, double y, double x2, Zdouble y2 ) { Rectangle(color,x,y,x2,(double)y2); }
void Rectangle(Crayon color,Zdouble x, Zdouble y, Zdouble x2, Zdouble y2 ) { Rectangle(color,(double)x,(double)y,(double)x2,(double)y2); }
void Rectangle(Crayon color,double x, double y, double x2, double y2) {
 art.Rectangle(color,x,y,x2-x,1.0);
 art.Rectangle(color,x2,y,1.0,y2-y);
 art.Rectangle(color,x,y,1.0,y2-y);
 art.Rectangle(color,x,y2,x2-x,1.0);
}

void Rectangle(Crayon color, Cartesian *rect) {
 Rectangle(color,rect->x,rect->y,rect->x2,rect->y2);
}
void Rectangle(Crayon color, Cartesiand *rect) {
 Rectangle(color,rect->x,rect->y,rect->x2,rect->y2);
}

void Rectangle(GLWindow *surface, Crayon color, int x1, Zint y1, Zint x2, Zint y2 ) { Rectangle(surface,color,x1,(int)y1,(int)x2,(int)y2); }
void Rectangle(GLWindow *surface, Crayon color, int x1, int y1, Zint x2, Zint y2 ) { Rectangle(surface,color,x1,y1,(int)x2,(int)y2); }
void Rectangle(GLWindow *surface, Crayon color, int x1, int y1, int x2, Zint y2 ) { Rectangle(surface,color,x1,y1,x2,(int)y2); }
void Rectangle(GLWindow *surface, Crayon color, Zint x1, Zint y1, Zint x2, Zint y2 ) { Rectangle(surface,color,(int)x1,(int)y1,(int)x2,(int)y2); }
void Rectangle(GLWindow *surface, Crayon color, int x1,int y1,int x2,int y2) 
{
 Rectangle(color,x1,y1,x2,y2);
}

void Rectangle(GLWindow *surface, Crayon color, Cartesian *c) 
{
 Rectangle(color,c->x+surface->x,c->y+surface->y,c->x2+surface->x,c->y2+surface->y);
}

void Square(Crayon color, Cartesian *rect) {
 Rectangle(
  color,
  rect->x-rect->w/2,rect->y-rect->h/2,
  rect->x+rect->w/2,rect->y+rect->h/2
 );
}

void Square(Crayon color, Cartesiand *rect) {
 Rectangle(
  color,
  rect->x-rect->w/2,rect->y-rect->h/2,
  rect->x+rect->w/2,rect->y+rect->h/2
 );
}

void Square(Crayon color,int x, int y, int w) {
 Rectangle( color, x-w/2,y-w/2, x+w/2,y+w/2 );
}

void Square(Crayon color,double x, double y, double w) {
 Rectangle( color, x-w/2,y-w/2, x+w/2,y+w/2 );
}

void Area(GLWindow *surface, Crayon color, int x1, Zint y1, Zint x2, Zint y2 ) { Area(surface,color,x1,(int)y1,(int)x2,(int)y2); }
void Area(GLWindow *surface, Crayon color, int x1, int y1, Zint x2, Zint y2 ) { Area(surface,color,x1,y1,(int)x2,(int)y2); }
void Area(GLWindow *surface, Crayon color, int x1, int y1, int x2, Zint y2 ) { Area(surface,color,x1,y1,x2,(int)y2); }
void Area(GLWindow *surface, Crayon color, Zint x1, Zint y1, Zint x2, Zint y2 ) { Area(surface,color,(int)x1,(int)y1,(int)x2,(int)y2); }
void Area(GLWindow *surface, Crayon color, int x1,int y1,int x2,int y2)
{
 glColor4fv((color.floats));
 glRecti(x1+surface->x, y1+surface->y, x2+surface->x/* + 1*/, y2+surface->y/* + 1*/);
 /* glRectf(x1+GLWindow *.offset.x, y1+GLWindow *.offset.y, x2+GLWindow *.offset.x + 1, y2+GLWindow *.offset.y + 1); */
}

void Area(Crayon color, int x1, Zint y1, Zint x2, Zint y2 ) { Area(color,x1,(int)y1,(int)x2,(int)y2); }
void Area(Crayon color, int x1, int y1, Zint x2, Zint y2 ) { Area(color,x1,y1,(int)x2,(int)y2); }
void Area(Crayon color, int x1, int y1, int x2, Zint y2 ) { Area(color,x1,y1,x2,(int)y2); }
void Area(Crayon color, Zint x1, Zint y1, Zint x2, Zint y2 ) { Area(color,(int)x1,(int)y1,(int)x2,(int)y2); }
void Area(Crayon color, int x1,int y1,int x2,int y2)
{
 glColor4fv((color.floats));
 glRecti(x1,y1,x2/*+1*/,y2/*+1*/);
 /* glRectf(x1+GLWindow *.offset.x, y1+GLWindow *.offset.y, x2+GLWindow *.offset.x + 1, y2+GLWindow *.offset.y + 1); */
}
void Area(Crayon color, double x1,double y1,double x2,double y2)
{
 glColor4fv((color.floats));
 glRectd(x1,y1,x2/*+1*/,y2/*+1*/);
 /* glRectf(x1+GLWindow *.offset.x, y1+GLWindow *.offset.y, x2+GLWindow *.offset.x + 1, y2+GLWindow *.offset.y + 1); */
}

void Area(Crayon color, Cartesiand *rect ) {
 glColor4fv((color.floats));
 glRectd(rect->x,rect->y,rect->x2,rect->y2);
}

void Area(Crayon color, Cartesian *rect ) {
 glColor4fv((color.floats));
 glRecti(rect->x,rect->y,rect->x2,rect->y2);
}

void Area(GLImage *texture, Crayon color, Blends blend, ScreenPosition *screen) {
 Stretchi(texture,color,1.0f,blend, screen->x,screen->y,screen->w,screen->h);
}

void BlendArea(Crayon color, Blends blend, int x1,int y1,int x2,int y2)
{
 Blending(blend);
 glColor4fv((color.floats));
 glRecti(x1,y1,x2,y2);
}

void BlendArea(GLWindow *surface, Crayon color, Blends blend, int x1,int y1,int x2,int y2)
{
 Blending(blend);
 glColor4fv((color.floats));
 glRecti(x1+surface->x, y1+surface->y, x2+surface->x/* + 1*/, y2+surface->y/* + 1*/);
 /* glRectf(x1+GLWindow *.offset.x, y1+GLWindow *.offset.y, x2+GLWindow *.offset.x + 1, y2+GLWindow *.offset.y + 1); */
}

void Stretch(GLWindow * surface, GLImage * b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh)
{
 int OX=(int) surface->x+dx;
 int OY=(int) surface->y+dy;
 art.Stretch(b,crayons.Pick(alabaster),OX,OY,w,h,false,h<0);
}

void Stretchi( GLWindow * dest, GLImage * source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h ) {
 int OX=(int) dest->x+x;
 int OY=(int) dest->y+y;
 Blending(blend);
 tint.ScaleByAlphaf(alpha);
 art.Stretch(source,tint,blend,OX,OY,w,h,w<0,h<0);
}

void Stretchi( GLImage * source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h ) {
 Blending(blend);
 tint.ScaleByAlphaf(alpha);
 art.Stretch(source,tint,blend,x,y,w,h,w<0,h<0);
}


void Stretchd( GLImage * source, Crayon tint, float alpha, Blends blend, Cartesiand *rect ) {
 Blending(blend);
 tint.ScaleByAlphaf(alpha);
 art.Stretch(source,tint,blend,rect->x,rect->y,rect->w,rect->h);
}

void StretchdNoAlpha( GLWindow * dest, GLImage * source, Crayon tint, Blends blend, double x, double y, int w, int h ) {
 double OX=(double) dest->x;
 double OY=(double) dest->y;
 double W=(double)w;
 double H=(double)h;
 Blending(blend);
 art.Stretch(source,tint,blend,OX,OY,w,h);
}

void StretchdNoAlpha( GLImage * source, Crayon tint, Blends blend, double x, double y, int w, int h ) {
 Blending(blend);
 art.Stretch(source,tint,blend,x,y,w,h);
}

void Stretch( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 Blending(blend);
 art.Stretch(source,tint,blend,x,y,w,h);
}

void StretchiNoAlpha( GLWindow * dest, GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 int OX=(int) dest->x+x;
 int OY=(int) dest->y+y;
 Blending(blend);
 art.Stretch(source,tint,blend,OX,OY,w,h);
}


void StretchiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 Blending(blend);
 art.Stretch(source,tint,blend,x,y,w,h);
}

void StretchiNoAlphaXPartial( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTC, double endTC ) {
#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(startTC, 0.0);  glVertex2i(x,   y);
 glTexCoord2d(endTC,   0.0);  glVertex2i(x+w, y);
 glTexCoord2d(endTC,   1.0);  glVertex2i(x+w, y+h);
 glTexCoord2d(startTC, 1.0);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}
void StretchiNoAlphaYPartial( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTC, double endTC ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(0.0, startTC);  glVertex2i(x,   y);
 glTexCoord2d(1.0, startTC);  glVertex2i(x+w, y);
 glTexCoord2d(1.0, endTC);    glVertex2i(x+w, y+h);
 glTexCoord2d(0.0, endTC);    glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}

void StretchiNoAlphaXYPartial( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTCx, double endTCx, double startTCy, double endTCy ) {
#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(startTCx, startTCy); glVertex2i(x,   y);
 glTexCoord2d(endTCx,   startTCy); glVertex2i(x+w, y);
 glTexCoord2d(endTCx,   endTCy);   glVertex2i(x+w, y+h);
 glTexCoord2d(startTCx, endTCy);   glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}


void StretchXtc( GLImage *source, Crayon tint, Blends blend, double x, double y, double w, double h, double startTC, double endTC ) {
#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(startTC, 0.0f);  glVertex2d(x,   y);
 glTexCoord2d(endTC,   0.0f);  glVertex2d(x+w, y);
 glTexCoord2d(endTC,   1.0f);  glVertex2d(x+w, y+h);
 glTexCoord2d(startTC, 1.0f);  glVertex2d(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}
void StretchYtc( GLImage *source, Crayon tint, Blends blend, double x, double y, double w, double h, double startTC, double endTC ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(0.0, startTC);  glVertex2d(x,   y);
 glTexCoord2d(1.0, startTC);  glVertex2d(x+w, y);
 glTexCoord2d(1.0, endTC);    glVertex2d(x+w, y+h);
 glTexCoord2d(0.0, endTC);    glVertex2d(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}

void StretchXYtc( GLImage *source, Crayon tint, Blends blend, double x, double y, double w, double h, double startTCx, double endTCx, double startTCy, double endTCy ) {
#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(startTCx, startTCy); glVertex2d(x,   y);
 glTexCoord2d(endTCx, startTCy);   glVertex2d(x+w, y);
 glTexCoord2d(endTCx, startTCy);   glVertex2d(x+w, y+h);
 glTexCoord2d(startTCx, endTCy);   glVertex2d(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}

void StretchXtc( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTC, double endTC ) {
#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 /*
 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(startTC, 0.0f);  glVertex2i(x,   y);
 glTexCoord2d(endTC,   0.0f);  glVertex2i(x+w, y);
 glTexCoord2d(endTC,   1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2d(startTC, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();
*/
#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}
void StretchYtc( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTC, double endTC ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(0.0, startTC);  glVertex2i(x,   y);
 glTexCoord2d(1.0, startTC);  glVertex2i(x+w, y);
 glTexCoord2d(1.0, endTC);    glVertex2i(x+w, y+h);
 glTexCoord2d(0.0, endTC);    glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}

void StretchXYtc( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTCx, double endTCx, double startTCy, double endTCy ) {
#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(startTCx, startTCy); glVertex2i(x,   y);
 glTexCoord2d(endTCx, startTCy);   glVertex2i(x+w, y);
 glTexCoord2d(endTCx, startTCy);   glVertex2i(x+w, y+h);
 glTexCoord2d(startTCx, endTCy);   glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}


void StretchiNoAlphaXReversed( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 Blending(blend);
 art.Stretch(source,tint,blend,x,y,w,h,true);
}

// Tops: left to right top pixels (x,y to x2,y2)                      * ---> *
// Bottoms: left to right bottom pixels (when wound: x2,y2 to x,y)    * <--- *
void TexturedQuad( GLImage *source, Crayon tint, Blends blend, Cartesian *tops, Cartesian *bottoms ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(tops->x,   tops->y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(tops->x2, tops->y2);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(bottoms->x2, bottoms->y2);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(bottoms->x,bottoms->y);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}

// Tops: left to right top pixels (x,y to x2,y2)                      * ---> *
// Bottoms: left to right bottom pixels (when wound: x2,y2 to x,y)    * <--- *
void TexturedQuad( GLImage *source, Crayon tint, Blends blend, Cartesian *tops, Cartesian *bottoms, bool xreversed ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 if ( !xreversed ) {
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(tops->x,   tops->y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(tops->x2, tops->y2);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(bottoms->x2, bottoms->y2);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(bottoms->x,bottoms->y);
 } else {
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(tops->x,   tops->y);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(tops->x2, tops->y2);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(bottoms->x2, bottoms->y2);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(bottoms->x,bottoms->y);
 }
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}

void FBOTexturedQuad( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}

void FBOQuad( int x, int y, int w, int h ) {
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 glEnd();
}

void FBOQuadUpsideDown( int x, int y, int w, int h ) {
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y+h);
 glEnd();
}

void FBOQuadReversed( int x, int y, int w, int h ) {
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x,   y+h);
 glEnd();
}

void FBOQuadReversedUpsideDown( int x, int y, int w, int h ) {
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x,   y);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+w, y);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x,   y+h);
 glEnd();
}

/*
void TexturedQuad3d( GLImage *source, Crayon tint, Blends blend, Vertexd *a, Vertexd *b, Vertexd *c, Vertexd *d ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex3d(a->x, a->y, a->z);
 glTexCoord2f(1.0f, 0.0f);  glVertex3d(b->x, a->y, a->z);
 glTexCoord2f(1.0f, 1.0f);  glVertex3d(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex3d(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}
*/

void FBOTexturedQuadUpsideDown( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}

void Quadi( int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering Quadi()]" );
 glReportError( glGetError() );
 GL_Assert( "Quadi(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting Quadi()]" );
 glReportError( glGetError() );
#endif
}


void Quadi( GLImage * source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering Quadi()]" );
 glReportError( glGetError() );
 GL_Assert( "Quadi()" );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 tint.ScaleByAlphaf(alpha);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 if(h < 0)
 {
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y-h);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y-h);
 }
 else
 {
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 }
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting Quadi()]" );
 glReportError( glGetError() );
#endif

}


void QuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, Cartesian *rect ) {
 art.Stretch(source,tint,blend,rect->x,rect->y,rect->w,rect->h);
}

void QuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 art.Stretch(source,tint,blend,x,y,w,h);
}


void SkewedQuadiNoAlpha(
 GLImage * source, Crayon tint, Blends blend,
 int x, int y, int w, int h, Cartesian *skew ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+skew->x2,   y+skew->y2);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w+skew->x, y+skew->y2);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w+skew->x2, y+h+skew->y);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x-skew->x,   y+h+skew->y);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}


void SkewedQuadiNoAlpha(
 GLImage * source, Crayon tint, Blends blend,
 int x, int y, int w, int h, int xSkew ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w+xSkew, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x-xSkew, y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}


void SkewedQuaddNoAlpha(
 GLImage * source, Crayon tint, Blends blend,
 double x, double y, int w, int h, int xSkew ) {
 double XSKEW=(double)xSkew;
 double W=(double)w;
 double H=(double)h;
#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2d(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2d(x+W+XSKEW, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2d(x+W, y+H);
 glTexCoord2f(0.0f, 1.0f);  glVertex2d(x-XSKEW, y+H);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}
void QuadiNoAlphaReversed( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}
void QuaddNoAlpha( GLImage * source, Crayon tint, Blends blend, double x, double y, double w, double h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuaddNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuaddNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2d(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2d(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2d(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2d(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuaddNoAlpha()]" );
 glReportError( glGetError() );
#endif

}

void XReversibleStretchiNoAlpha( GLWindow * dest, GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h, bool xreversed ) {
 int OX=(int) dest->x+x;
 int OY=(int) dest->y+y;
 Blending(blend);
 art.Stretch(source,tint,blend,x,y,w,h,xreversed);
}


void XReversibleQuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h, bool xreversed ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering XReversibleQuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "XReversibleQuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 if ( !xreversed ) {
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 } else {
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x,   y);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+w, y);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+w, y+h);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x,   y+h);
 }
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting XReversibleStretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif

}



void StretchiNoAlpha2Sided( GLWindow * dest, GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 int OX=(int) dest->x+x;
 int OY=(int) dest->y+y;
 art.Stretch(source,tint,OX,OY,w,h,w<0,h<0);
}

void Additivei( GLWindow * dest, GLImage * source, Crayon tint, int x, int y, int w, int h ) {
 int OX=(int) dest->x+x;
 int OY=(int) dest->y+y;
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE);
 art.Stretch(source,tint,OX,OY,w,h,false,h<0);
}

void BlendTint( GLWindow * dest, GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 int OX=(int) dest->x+x;
 int OY=(int) dest->y+y;
 art.Stretch(source,tint,blend,OX,OY,w,h,false,h<0);
}

void Stretchf( GLWindow * dest, GLImage * source, Crayon tint, float alpha, float x, float y, float w, float h )
{
 float OX=(int) dest->x+x;
 float OY=(int) dest->y+y;
 art.Stretch(source,tint,OX,OY,w,h,false,h<0);
}

void DrawPixelf( GLWindow * dest, Crayon tint, float x, float y ) {
 art.Stretch(art.solidwhite,tint,x,y,1.0,1.0);
}

void ImageColorBlend( GLImage *i, Crayon t, Blends b ) {
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D,i->texture);
 Blending(b);
 t.gl();
}

void TexColorBlend( GLuint *i, Crayon t, Blends b ) {
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D,(*i));
 Blending(b);
 t.gl();
}

// Used for isometric tile effects
void Swabi( GLWindow * dest, GLImage * source, Crayon tint, float alpha, Blends blend, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) {
 float *t = tint.asFloatAlphai(alpha);
 int OX=(int) dest->x;
 int OY=(int) dest->y;
 glReportError( glGetError() );
 glPushAttrib(GL_ALL_ATTRIB_BITS);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glDisable(GL_CULL_FACE);
 glColor4fv(t); 
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x1+OX, y1+OY);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x2+OX, y2+OY);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x3+OX, y3+OY);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x4+OX, y4+OY);
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glPopAttrib();
 delete[] t;
}

// Used for isometric tile effects
void Swabf( GLWindow * dest, GLImage * source, Crayon tint, float alpha, Blends blend, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 ) {
 float *t = tint.asFloatAlphaf(alpha);
 float OX=(float) dest->x;
 float OY=(float) dest->y;
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 glColor4fv(t); 
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2f(x1+OX, y1+OY);
 glTexCoord2f(1.0f, 1.0f);  glVertex2f(x2+OX, y2+OY);
 glTexCoord2f(1.0f, 0.0f);  glVertex2f(x3+OX, y3+OY);
 glTexCoord2f(0.0f, 0.0f);  glVertex2f(x4+OX, y4+OY);
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glPopAttrib();
 delete[] t;
}

// Used for isometric tile effects
void Swabd( GLWindow * dest, GLImage * source, Crayon tint, Blends blend, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 ) {
 double OX=(double) dest->x;
 double OY=(double) dest->y;
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2i(0, 1);  glVertex2d(x1+OX, y1+OY);
 glTexCoord2i(1, 1);  glVertex2d(x2+OX, y2+OY);
 glTexCoord2i(1, 0);  glVertex2d(x3+OX, y3+OY);
 glTexCoord2i(0, 0);  glVertex2d(x4+OX, y4+OY);
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glPopAttrib();
}

// Used for isometric tile effects
void Swabd( GLImage * source, Crayon tint, Blends blend, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2i(0, 1);  glVertex2d(x1, y1);
 glTexCoord2i(1, 1);  glVertex2d(x2, y2);
 glTexCoord2i(1, 0);  glVertex2d(x3, y3);
 glTexCoord2i(0, 0);  glVertex2d(x4, y4);
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glPopAttrib();
}

// Used for isometric tile effects
void SwabdYInvert( GLImage * source, Crayon tint, Blends blend, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2i(0, 0);  glVertex2d(x1, y1);
 glTexCoord2i(1, 0);  glVertex2d(x2, y2);
 glTexCoord2i(1, 1);  glVertex2d(x3, y3);
 glTexCoord2i(0, 1);  glVertex2d(x4, y4);
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glPopAttrib();
}

void RotatedRecti( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, int x, int y, int l, int w, float a ) {
 art.Pivot(source,tint,blend,x,y,-l/2,l/2,w/2,a);
}

void RotatedRecti( GLImage *source, Crayon tint, Blends blend, int x, int y, int l, int w, float a ) {
 art.Pivot(source,tint,blend,x,y,-l/2,l/2,w/2,a);
}

// Rotates at angle a, blended textured rectangle around x,y that is h long and w2*2 wide
void RotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a ) {
 art.Pivot(source,tint,blend,x,y,-h/2,h/2,w2,a);
}

// Rotates at angle a, blended textured rectangle around x,y that is h long and w2*2 wide
void RotatedRectf( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a ) {
 art.Pivot(source,tint,blend,x,y,-h/2,h/2,w2,a);
}

// Rotates at angle a, blended textured rectangle around x,y that is h long and w2*2 wide
void RotatedRectd( GLImage *source, Crayon tint, Blends blend, double x, double y, double h, double w2, double a ) {
 art.Pivot(source,tint,blend,x,y,-h/2,h/2,w2,a);
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void PivotedRotatedRectf( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, 
                          float x, float y, float px, float py, float h2, float w2, float a ) {
 art.Pivot(source,tint,blend,x,y,px,py,h2,w2,a);
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void PivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, 
                          float x, float y, float px, float py, float h2, float w2, float a ) {
 art.Pivot(source,tint,blend,x,y,px,py,h2,w2,a);
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void PivotedRotatedRectd( GLImage *source, Crayon tint, Blends blend, 
                          double x, double y, double px, double py, double h2, double w2, double a ) {
 art.Pivot(source,tint,blend,x,y,px,py,h2,w2,a);
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void PivotedRotatedRectd( GLImage *source, Crayon tint, Blends blend, 
                          double x, double y, double px, double py, double h2, double w2, double a, having keepTextureMode ) {
 OUTPUT("PivotedRotatedRectd: FUNCTION OBSOLETE!\n");
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void PivotedRotatedFlippableRectd( GLImage *source, Crayon tint, Blends blend, 
                          double x, double y, double px, double py, double h2, double w2, double a, bool flipX, bool flipY ) {
 art.Pivot(source,tint,blend,x,y,px,py,h2,w2,a,flipX,flipY);
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void XInvertedPivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, 
                          float x, float y, float px, float py, float h2, float w2, float a ) {
 art.Pivot(source,tint,blend,x,y,px,py,h2,w2,a,true);
}


// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void FBOPivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, 
                          float x, float y, float px, float py, float h2, float w2, float a ) {
 art.Pivot(source,tint,blend,x,y,px,py,h2,w2,a);
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void FBOPivotedRotatedRectd( GLImage *source, Crayon tint, Blends blend, 
                          double x, double y, double px, double py, double h2, double w2, double a ) {
 art.Pivot(source,tint,blend,x,y,px,py,h2,w2,a);
}


// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void FBORotatedRectf( GLImage *source, Crayon tint, Blends blend, 
                          float x, float y, float h2, float w2, float a ) {
 RotatedRectf(source,tint,blend,x,y,h2,w2,a);
}

// Creates a textured line from c( x,y -> x2,y2 ), 
// store line distance in c->fh, and LineAngle in a,
// 
void TextureLine( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, Cartesian *c, int w2 ) {
 RotatedRectf( dest, source, tint, blend,
  (float) c->x, (float) c->y, c->fh, (float) w2, c->a );
}

void TextureLine( GLImage *source, Crayon tint, Blends blend, Cartesian *c, int w2 ) {
 RotatedRectd( source, tint, blend, (double) c->x, (double) c->y, (double) c->h, (double) w2, rad2deg(c->LineAngle())+90.0 );
}

void TextureLine( GLImage *source, Crayon tint, Blends blend, Cartesiand *c, double w2 ) {
 RotatedRectd( source, tint, blend, c->x, c->y, c->h, w2, rad2deg(c->LineAngle())+90.0 );
}

void LineRectf( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x1, float y1, float x2, float y2, float w2 ) {
 double d=ddistance(x1,y1,x2,y2);
 double X=x2-x1;
 double Y=y2-y1;
 if ( X==Y && X == 0 ) return;
 double a=atan2(Y,X)*180.0/PI+270.0;
 art.Pivot(source,tint,blend,x1,y1,-d/2,0.0,w2,d/2,a);
}

// Can't use quad due to perspective correction, so we use a series of 8 interlocking triangles of points,
//     D ____h___ C
//      \        /
//      f+  e.  +g
//        \____/
//        A  i  B
//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);? no
//This is the best I can do, it looks good if you use the swab.png and wanted to do a lamp/lightsource.
void RotatedIsotrapf( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w1, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 

 /* A square and two tris
 // Center Area
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f,    1.0f);  glVertex2f( w1, 0.0f); 
 glTexCoord2f(1.0f,    1.0f);  glVertex2f(-w1, 0.0f);
 glTexCoord2f(1.0f-tc, 0.0f);  glVertex2f(-w1, h);   
 glTexCoord2f(tc,      0.0f);  glVertex2f( w1, h);
 glEnd();

 // Left Side Triangle
 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f-tc, 0.0f);  glVertex2f(-w1, h);   
 glTexCoord2f(1.0f,    0.0f);  glVertex2f(-w2, h);
 glTexCoord2f(1.0f,    1.0f);  glVertex2f(-w1, 0.0f);
 glEnd();

 // Right Side Triangle
 glBegin(GL_TRIANGLES);
 glTexCoord2f(tc,    0.0f);  glVertex2f( w1, h);
 glTexCoord2f(0.0f,  0.0f);  glVertex2f( w2, h);
 glTexCoord2f(0.0f,  1.0f);  glVertex2f( w1, 0.0f); 
 glEnd();
 */

 /* Example of Tri Strips
 glBegin(GL_TRIANGLE_STRIP);
 glTexCoord2f(1.0f, 1.0f); glVertex2f(-w1, 0.0f);
 glTexCoord2f(1.0f, 0.0f); glVertex2f(-w2, h);   
 glTexCoord2f(0.5f, 1.0f); glVertex2f(0.0f,0.0f);
 glTexCoord2f(0.5f, 0.0f); glVertex2f(0.0f,h);
 glTexCoord2f(0.0f, 1.0f); glVertex2f(w1, 0.0f); 
 glTexCoord2f(0.0f, 0.0f); glVertex2f(w2, h);    
 glEnd();
 */

 // Interlocking triangle method
 float 
  Ax=-w1,              Ay=0.0f,
  Bx=w1,               By=0.0f,
  Cx=w2,               Cy=h,
  Dx=-w2,              Dy=h,
  ex=0.0f,             ey=h/2.0f,
  gx(w1+(w2-w1)/2),    gy=ey,
  fx=-gx,              fy=ey,
  hx=0.0f,             hy=h,
  ix=0.0f,             iy=0.0f;

 //1
 //glBegin(GL_TRIANGLES);
 //glTexCoord2f(1.0f,1.0f); glVertex2f(Ax,Ay);
 //glTexCoord2f(0.5f,0.0f); glVertex2f(ix,iy);
 //glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 //glEnd();
 //
 ////2
 //glBegin(GL_TRIANGLES);
 //glTexCoord2f(0.5f,0.0f); glVertex2f(ix,iy);
 //glTexCoord2f(0.0f,1.0f); glVertex2f(Bx,By);
 //glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 //glEnd();

 // combined 1+2
 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,1.0f); glVertex2f(Ax,Ay);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glTexCoord2f(0.0f,1.0f); glVertex2f(Bx,By);
 glEnd();

 //3
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,1.0f); glVertex2f(Bx,By);
 glTexCoord2f(0.0f,0.5f); glVertex2f(gx,gy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //4
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,0.5f); glVertex2f(gx,gy);
 glTexCoord2f(0.0f,0.0f); glVertex2f(Cx,Cy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //5
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,0.0f); glVertex2f(Cx,Cy);
 glTexCoord2f(0.5f,0.0f); glVertex2f(hx,hy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //6
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.5f,0.0f); glVertex2f(hx,hy);
 glTexCoord2f(1.0f,0.0f); glVertex2f(Dx,Dy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //7
 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,0.5f); glVertex2f(fx,fy);
 glTexCoord2f(1.0f,1.0f); glVertex2f(Ax,Ay);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //8
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glTexCoord2f(1.0f,0.5f); glVertex2f(fx,fy);
 glTexCoord2f(1.0f,0.0f); glVertex2f(Dx,Dy);
 glEnd();

 /*
 // Example of polygon
 float h2=h/2.0f;
 glBegin(GL_POLYGON);
 glTexCoord2f(0.5f,1.0f); glVertex2f(     0.0f,0.0f);
 glTexCoord2f(0.5f,1.0f); glVertex2f(       w1,0.0f);
// glTexCoord2f(0.0f,5.0f); glVertex2f((w2-w1)/2,  h2);
 glTexCoord2f(0.0f,0.0f); glVertex2f(       w2,   h);
 glTexCoord2f(0.5f,0.5f); glVertex2f(     0.0f,  h2);
 glTexCoord2f(0.0f,0.0f); glVertex2f(      -w2,   h);
// glTexCoord2f(0.5f,0.5f); glVertex2f(-(w2-w1)/2, h2);
 glTexCoord2f(1.0f,1.0f); glVertex2f(     -w1, 0.0f);
 glEnd();
 */


 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}


// Can't use quad due to perspective correction, so we use a series of 8 interlocking triangles of points,
//     D ____h___ C
//      \        /
//      f+  e.  +g
//        \____/
//        A  i  B
//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);? no
//This is the best I can do, it looks good if you use the swab.png and wanted to do a lamp/lightsource.
void RotatedIsotrapf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w1, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 

 // Interlocking triangle method
 float 
  Ax=-w1,              Ay=0.0f,
  Bx=w1,               By=0.0f,
  Cx=w2,               Cy=h,
  Dx=-w2,              Dy=h,
  ex=0.0f,             ey=h/2.0f,
  gx(w1+(w2-w1)/2),    gy=ey,
  fx=-gx,              fy=ey,
  hx=0.0f,             hy=h,
  ix=0.0f,             iy=0.0f;

 // combined 1+2
 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,1.0f); glVertex2f(Ax,Ay);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glTexCoord2f(0.0f,1.0f); glVertex2f(Bx,By);
 glEnd();

 //3
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,1.0f); glVertex2f(Bx,By);
 glTexCoord2f(0.0f,0.5f); glVertex2f(gx,gy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //4
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,0.5f); glVertex2f(gx,gy);
 glTexCoord2f(0.0f,0.0f); glVertex2f(Cx,Cy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //5
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,0.0f); glVertex2f(Cx,Cy);
 glTexCoord2f(0.5f,0.0f); glVertex2f(hx,hy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //6
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.5f,0.0f); glVertex2f(hx,hy);
 glTexCoord2f(1.0f,0.0f); glVertex2f(Dx,Dy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //7
 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,0.5f); glVertex2f(fx,fy);
 glTexCoord2f(1.0f,1.0f); glVertex2f(Ax,Ay);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //8
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glTexCoord2f(1.0f,0.5f); glVertex2f(fx,fy);
 glTexCoord2f(1.0f,0.0f); glVertex2f(Dx,Dy);
 glEnd();
 
 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}


// Can't use quad due to perspective correction, so we use a series of 8 interlocking triangles of points,
//     D ____h___ C
//      \        /
//      f+  e.  +g
//        \____/
//        A  i  B
//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);? no
//This is the best I can do, it looks good if you use the swab.png and wanted to do a lamp/lightsource.
void RotatedIsotrapd( GLImage *source, Crayon tint, Blends blend, double x, double y, double h, double w1, double w2, double a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslated(x,y,0.0);
 glRotated(a,0.0,0.0,1.0);
 glColor4fv(tint.floats); 

 // Interlocking triangle method
 double 
  Ax=-w1,              Ay=0.0,
  Bx=w1,               By=0.0,
  Cx=w2,               Cy=h,
  Dx=-w2,              Dy=h,
  ex=0.0,              ey=h/2.0,
  gx(w1+(w2-w1)/2.0),  gy=ey,
  fx=-gx,              fy=ey,
  hx=0.0,              hy=h,
  ix=0.0,              iy=0.0;

 // combined 1+2
 glBegin(GL_TRIANGLES);
 glTexCoord2d(1.0,1.0); glVertex2d(Ax,Ay);
 glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
 glTexCoord2d(0.0,1.0); glVertex2d(Bx,By);
// glEnd();

 //3
// glBegin(GL_TRIANGLES);
 glTexCoord2d(0.0,1.0); glVertex2d(Bx,By);
 glTexCoord2d(0.0,0.5); glVertex2d(gx,gy);
 glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
// glEnd();

 //4
// glBegin(GL_TRIANGLES);
 glTexCoord2d(0.0,0.5); glVertex2d(gx,gy);
 glTexCoord2d(0.0,0.0); glVertex2d(Cx,Cy);
 glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
// glEnd();

 //5
// glBegin(GL_TRIANGLES);
 glTexCoord2d(0.0,0.0); glVertex2d(Cx,Cy);
 glTexCoord2d(0.5,0.0); glVertex2d(hx,hy);
 glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
// glEnd();

 //6
// glBegin(GL_TRIANGLES);
 glTexCoord2d(0.5,0.0); glVertex2d(hx,hy);
 glTexCoord2d(1.0,0.0); glVertex2d(Dx,Dy);
 glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
// glEnd();

 //7
// glBegin(GL_TRIANGLES);
 glTexCoord2d(1.0,0.5); glVertex2d(fx,fy);
 glTexCoord2d(1.0,1.0); glVertex2d(Ax,Ay);
 glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
// glEnd();

 //8
// glBegin(GL_TRIANGLES);
 glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
 glTexCoord2d(1.0,0.5); glVertex2d(fx,fy);
 glTexCoord2d(1.0,0.0); glVertex2d(Dx,Dy);
 glEnd();
 
 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}


// Single triangle
void RotatedTrif( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 

 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,1.0f); glVertex2f(-w2,h);
 glTexCoord2f(0.0f,0.0f); glVertex2f(0,0);
 glTexCoord2f(0.0f,1.0f); glVertex2f(w2,h);
 glEnd();

 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}


// Single triangle
void RotatedTrif( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 

 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,1.0f); glVertex2f(-w2,h);
 glTexCoord2f(0.0f,0.0f); glVertex2f(0,0);
 glTexCoord2f(0.0f,1.0f); glVertex2f(w2,h);
 glEnd();

 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}


/*
#!/usr/bin/perl
#
# perspective_coords [-fx]   x1,y1 .. x4,y4    u1,v1 .. u4,v4
#
# Given two sets of four coordinate pairs, work out 8 constants needed
# to map the first set of coordinates to the second set of coordinates,
# using a perspective transform.
#
# The sixteen numbers given may be comma or space seperated, as one single
# argument or over a number of arguments.
#
# To use the constants generated use the following formulas..
#
#           c1*x + c2*y + c3
#    u  =  ------------------
#           c7*x + c8*y + 1
#
#           c4*x + c5*y + c6
#    v  =  ------------------
#           c7*x + c8*y + 1
#
# By default only the 8 constants needed for the above equation are returned
# (in the order c1 to c8).
#
# However you can also specify an '-fx' option which will output the above
# equations in the form of an ImageMagick "-fx" expression, to lookup colors
# the first 'u' image.
#
####
#
# The above equations can be re-written (by division) into the following form.
#
#   c7*x*u + c8*y*u + u  =  c1*x + c2*y + c3
#   c7*x*v + c8*y*v + v  =  c4*x + c5*y + c6
#
# And simplified (by subtraction) to this form.
#
#    u  =  c1*x + c2*y + c3 - c7*x*u - c8*y*u
#    v  =  c4*x + c5*y + c6 - c7*x*v - c8*y*v
#
# which allows us to generate the matrix of simultaneous equations,
# and thus work out the 8 constants needed.
#
####
#
# Special thanks goes to   Hugemann <Auto@hugemann.de>  for the discussions
# in the IM users mailing list which allowed the formulas to be worked out.
#
# Script by Anthony Thyssen  (December 2006)
#
# Requires the "Math::MatrixReal" perl module to be installed. No compilation
# is needed to build this module.  for the latest version of this module see
# http://leto.net/code/Math-MatrixReal/
#
use strict;
use Math::MatrixReal;
use FindBin;
my $prog = $FindBin::Script;

# Output the program comments as the programs manual
sub Usage {
  print STDERR "$prog: ", @_, "\n";
  @ARGV = ( "$FindBin::Bin/$prog" );
  while( <> ) {
    next if 1 .. 2;
    last if /^###/;
    s/^#$//; s/^# //;
    print STDERR "Usage: " if 3 .. 3;
    print STDERR;
  }
  exit 10;
}

@ARGV = map( /([^\s,]+)/g, @ARGV);   # Spilt arguments by spaces and commas

my $FX = 0;     # output a ImageMagick -fx perspective transformation formula
my $TEST = 0;   # Apply the constants to the input x,y points to test

if ( $ARGV[0] eq '-fx' ) {
  $FX = 1;  shift;
}

if ( @ARGV != 16 ) {
  Usage "Incorrect number of arguments, ", scalar(@ARGV),
        " given\n\t\t\t16 numbers (2 sets of 4 coordinate pairs) are needed.";
}

# Assign the coordinates of the two triangles (remove commas)
my ( $x1, $y1, $x2, $y2, $x3, $y3, $x4, $y4,
     $u1, $v1, $u2, $v2, $u3, $v3, $u4, $v4 ) = @ARGV;

# Convert coordinates into a matrix of simultanious equation
# Such that ...    A * c = r

my $A = Math::MatrixReal->new_from_rows(
  [  [ $x1, $y1, 1.0, 0.0, 0.0, 0.0, -$u1*$x1, -$u1*$y1 ],
     [ 0.0, 0.0, 0.0, $x1, $y1, 1.0, -$v1*$x1, -$v1*$y1 ],
     [ $x2, $y2, 1.0, 0.0, 0.0, 0.0, -$u2*$x2, -$u2*$y2 ],
     [ 0.0, 0.0, 0.0, $x2, $y2, 1.0, -$v2*$x2, -$v2*$y2 ],
     [ $x3, $y3, 1.0, 0.0, 0.0, 0.0, -$u3*$x3, -$u3*$y3 ],
     [ 0.0, 0.0, 0.0, $x3, $y3, 1.0, -$v3*$x3, -$v3*$y3 ],
     [ $x4, $y4, 1.0, 0.0, 0.0, 0.0, -$u4*$x4, -$u4*$y4 ],
     [ 0.0, 0.0, 0.0, $x4, $y4, 1.0, -$v4*$x4, -$v4*$y4 ]
  ] );

my $r = Math::MatrixReal->new_from_cols(
  [ [ $u1, $v1, $u2, $v2, $u3, $v3, $u4, $v4 ] ] );

# Debuging:  output an inverse matrix
#print $A->inverse();

# Solve the matrix for the constants
my ($dim, $c, $base) = $A->decompose_LR()->solve_LR($r);
if ( $dim ) {
  print STDERR
    "$prog: Coordinates given do not form solvable quadrangles.";
  exit 1;
}

# Extract resulting column vector as an array.
my @c = map { $c->element($_,1) }  ( 1 .. 8 );

# Test results with original coordinates
if ( 0 ) {
  print "Test Results against Original coordinates\n";
  printf "%5.1f,%-5.1f -> %5.1f,%-5.1f (should be %5.1f,%-5.1f )\n",
    $x1, $y1,
    ($c[0]*$x1 + $c[1]*$y1 + $c[2]) / ($c[6]*$x1 + $c[7]*$y1 + 1),
    ($c[3]*$x1 + $c[4]*$y1 + $c[5]) / ($c[6]*$x1 + $c[7]*$y1 + 1),
    $u1, $v1;
  printf "%5.1f,%-5.1f -> %5.1f,%-5.1f (should be %5.1f,%-5.1f )\n",
    $x2, $y2,
    ($c[0]*$x2 + $c[1]*$y2 + $c[2]) / ($c[6]*$x2 + $c[7]*$y2 + 1),
    ($c[3]*$x2 + $c[4]*$y2 + $c[5]) / ($c[6]*$x2 + $c[7]*$y2 + 1),
    $u2, $v2;
  printf "%5.1f,%-5.1f -> %5.1f,%-5.1f (should be %5.1f,%-5.1f )\n",
    $x3, $y3,
    ($c[0]*$x3 + $c[1]*$y3 + $c[2]) / ($c[6]*$x3 + $c[7]*$y3 + 1),
    ($c[3]*$x3 + $c[4]*$y3 + $c[5]) / ($c[6]*$x3 + $c[7]*$y3 + 1),
    $u3, $v3;
  printf "%5.1f,%-5.1f -> %5.1f,%-5.1f (should be %5.1f,%-5.1f )\n",
    $x4, $y4,
    ($c[0]*$x4 + $c[1]*$y4 + $c[2]) / ($c[6]*$x4 + $c[7]*$y4 + 1),
    ($c[3]*$x4 + $c[4]*$y4 + $c[5]) / ($c[6]*$x4 + $c[7]*$y4 + 1),
    $u4, $v4;
  print "\n";
}

if ( ! $FX ) {
  # Output the constants
  map { printf "%9.6f\n", $_ }  @c;
}
else {
  # Output a IM -fx expression
  printf "det=%+.6f*i %+.6f*j +1;\n".
         "xx=(%+.6f*i %+.6f*j %+.6f)/det;\n".
         "yy=(%+.6f*i %+.6f*j %+.6f)/det;\n",
         @c[6,7,0..5];
}

*/

// Options for OpenGL drawing in 2d

void Additive(GLWindow * dest, GLImage * b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh) {
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(additive); 
 Stretch(dest,b,dx,dy,sx,sy,w,h,sw,sh);
 glPopAttrib();
}

void Multiply(GLWindow * dest, GLImage * b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh) {
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(multiply);
 Stretch(dest,b,dx,dy,sx,sy,w,h,sw,sh);
 glPopAttrib();
}

void Mask(GLWindow * dest, GLImage * b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh)
{
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(mask);
 Stretch(dest,b,dx,dy,sx,sy,w,h,sw,sh);
 glPopAttrib();
}

void Solarize(GLWindow * dest, GLImage * b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh)
{
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(solarize);
 Stretch(dest,b,dx,dy,sx,sy,w,h,sw,sh);
 glPopAttrib();
}

void AdditiveTransformf(GLWindow * dest, GLImage * b, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4) {
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
 glPopAttrib();
}

void MaskTransformf(GLWindow * dest, GLImage * b, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4)
{
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 glBlendFunc(GL_DST_COLOR,GL_ZERO);
 glPopAttrib();
}

void SolarizeTransformf(GLWindow * dest, GLImage * b, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4)
{
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
 glPopAttrib();
}

void Blend(GLWindow * source, GLImage * b, Blends blend, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh ) {
 switch ( blend ) {
  case _additive: Additive( source, b, dx, dy, sx, sy, w, h, sw, sh ); break;
  case _solarize: Solarize( source, b, dx, dy, sx, sy, w, h, sw, sh ); break;
      case _mask:     Mask( source, b, dx, dy, sx, sy, w, h, sw, sh ); break;
  case _multiply: Multiply( source, b, dx, dy, sx, sy, w, h, sw, sh ); break;
 }
}

void Blendf(GLWindow * source, GLImage * b, Blends blend, float x, float y, float w, float h) {
}

void DrawRampGL( GLWindow * source, int x1, int y1, int w, int h, float *rgba, ColorChannel channel ) {
 int xx1,xx2,yy1,yy2;
 if ( rgba == null ) channel=RED;
 xx1=source->x+x1;
 xx2=source->x+x1+w;
 yy1=source->y+y1;
 yy2=source->y+y1+h;
 glReportError( glGetError() );
 glBegin(GL_QUADS);
 glColor4f(0,0,0,0);
 glVertex2i(xx1,yy1);
 glVertex2i(xx1,yy2);
 switch ( channel ) {
    case RED: glColor4f(255,   0,   0, 255); break;
  case GREEN: glColor4f(  0, 255,   0, 255); break;
   case BLUE: glColor4f(  0,   0, 255, 255); break;
     default: glColor4f(rgba[0]*255.0f, rgba[1]*255.0f, rgba[2]*255.0f, 255.0f); break;
 }
 glVertex2i(xx2,yy2); 
 glVertex2i(xx2,yy1);
 glEnd();
}

void DrawGrid( GLWindow *surface, Cartesian *extents, Crayon gridColor, int row, int col ) {
 float X=(float) extents->x;
 float Y=(float) extents->y;
 float deltaX = iratiof(extents->w,col);
 float deltaY = iratiof(extents->h,row);
 int i;
 Rectangle(surface,gridColor,extents->x,extents->y+1,extents->x2,extents->y2);
 //col--;
 //row--;
 for ( i=0; i < col; i++ ) {
  X+=deltaX;
  DrawLine(surface,gridColor,(int)X,extents->y,(int)X,extents->y2+1);
 }
 for ( i=0; i < row; i++ ) {
  Y+=deltaY;
  DrawLine(surface,gridColor,extents->x,(int)Y,extents->x2+1,(int)Y);
 }
}

void DrawGrid( Cartesiand *extents, Crayon gridColor, int row, int col ) {
 double X=extents->x;
 double Y=extents->y;
 double deltaX = extents->w/(double)col;
 double deltaY = extents->h/(double)row;
 int i;
 Rectangle(gridColor,extents->x,extents->y+1,extents->x2,extents->y2);
 //col--;
 //row--;
 for ( i=0; i < col; i++ ) {
  X+=deltaX;
  DrawLine(gridColor,X,extents->y,X,extents->y2+1.0);
 }
 for ( i=0; i < row; i++ ) {
  Y+=deltaY;
  DrawLine(gridColor,extents->x,Y,extents->x2+1.0,Y);
 }
}
 

void DrawGrid( Cartesian *extents, Crayon gridColor, int row, int col ) {
 float X=(float) extents->x;
 float Y=(float) extents->y;
 float deltaX = iratiof(extents->w,col);
 float deltaY = iratiof(extents->h,row);
 int i;
 Rectangle(gridColor,extents->x,extents->y+1,extents->x2,extents->y2);
 //col--;
 //row--;
 for ( i=0; i < col; i++ ) {
  X+=deltaX;
  DrawLine(gridColor,(int)X,extents->y,(int)X,extents->y2+1);
 }
 for ( i=0; i < row; i++ ) {
  Y+=deltaY;
  DrawLine(gridColor,extents->x,(int)Y,extents->x2+1,(int)Y);
 }
}
 
void DrawGrid( int x, int y, int w, int h, Crayon gridColor, int row, int col ) {
 float X=(float) x;
 float Y=(float) y;
 float deltaX = iratiof(w,col);
 float deltaY = iratiof(h,row);
 int x2=x+w;
 int y2=y+h;
 int i;
 Rectangle(gridColor,x,y+1,x2,y2);
 //col--;
 //row--;
 for ( i=0; i < col; i++ ) {
  X+=deltaX;
  DrawLine(gridColor,(int)X,y,(int)X,y2+1);
 }
 for ( i=0; i < row; i++ ) {
  Y+=deltaY;
  DrawLine(gridColor,x,(int)Y,x2+1,(int)Y);
 }
}

void DrawGrid( double x, double y, double w, double h, Crayon gridColor, int row, int col ) {
 row=UMAX(1,row);
 col=UMAX(1,col);
 double X=x;
 double Y=y;
 double deltaX = w/(double)col;
 double deltaY = w/(double)row;
 double x2=x+w;
 double y2=y+h;
 int i;
 Rectangle(gridColor,x,y+1.0,x2,y2);
 //col--;
 //row--;
 for ( i=0; i < col; i++ ) {
  X+=deltaX;
  DrawLine(gridColor,X,y,X,y2+1.0);
 }
 for ( i=0; i < row; i++ ) {
  Y+=deltaY;
  DrawLine(gridColor,x,Y,x2+1.0,Y);
 }
}

#include "GLSetup.h"

extern GLSetup gl;

void PointSprite( GLImage *source, Crayon tint, Vertexd *coord, float size ) {
 float renderSize=mapRangef(size,0.0f,1.0f,(float)gl.aliasedPointLow, (float)gl.aliasedPointHigh);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture );
 glEnable(GL_POINT_SPRITE);
 glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
 glPointSize(renderSize);
 glBegin(GL_POINTS);
 glVertex3d( coord->x,coord->y,coord->z );
 glEnd();
}

void Coordinator() { 
 glDisable(GL_TEXTURE_2D);
 glBegin(GL_LINES);
 glColor3d(1.0,0.0,0.0); glVertex3d(0.0,0.0,0.0); glVertex3d(1.0,0.0,0.0);
 glColor3d(0.0,1.0,0.0); glVertex3d(0.0,0.0,0.0); glVertex3d(0.0,1.0,0.0);
 glColor3d(0.0,0.0,1.0); glVertex3d(0.0,0.0,0.0); glVertex3d(0.0,0.0,1.0);
 glEnd(); 
}

void DrawBar( Crayon color, float one, int x, int y, int w, int h, int divisions ) {
 int x2=x+w;
 int y2=y+h;
 Blending(multiply);
 int filledy=(int)((float)h*(1.0f-one));
 Area(color,x,y+filledy,x2,y2);
 float dy=iratiof(h,divisions);
 int dyi=(int) dy;
 Cartesian rect;
 Crayon drawing;
 for ( int i=0; i<divisions; i++ ) {
  int deltay=(int)(dy*(float)i);
  Blending(additive);
  if ( deltay >= filledy )  drawing.fromCrayon(color);
  else drawing.Pick(gray);
  rect.SetRect(x,y+deltay,w,dyi);
  Rectangle(drawing,&rect);
 }
 Blending(additive);
}

void DrawBar( Blends blendFill, Blends blendLine, Crayon color, float one, int x, int y, int w, int h, int divisions ) {
 int x2=x+w;
 int y2=y+h;
 Blending(blendFill);
 int filledy=(int)((float)h*(1.0f-one));
 Area(color,x,y+filledy,x2,y2);
 float dy=iratiof(h,divisions);
 int dyi=(int) dy;
 Cartesian rect;
 Crayon drawing;
 for ( int i=0; i<divisions; i++ ) {
  int deltay=(int)(dy*(float)i);
  Blending(blendLine);
  if ( deltay >= filledy )  drawing.fromCrayon(color);
  else drawing.Pick(gray);
  rect.SetRect(x,y+deltay,w,dyi);
  Rectangle(drawing,&rect);
 }
 Blending(additive);
}

void DrawBarHoriz( Crayon color, float one, int x, int y, int w, int h, int divisions ) {
 int x2=x+w;
 int y2=y+h;
 Blending(multiply);
 int filledx=(int)((float)w*(1.0f-one));
 Area(color,x,y,x+filledx,y2);
 float dx=iratiof(w,divisions);
 int dxi=(int) dx;
 Cartesian rect;
 Crayon drawing;
 for ( int i=0; i<divisions; i++ ) {
  int deltax=(int)(dx*(float)i);
  Blending(additive);
  if ( deltax >= filledx )  drawing.fromCrayon(color);
  else drawing.Pick(gray);
  rect.SetRect(x+deltax,y,dxi,h);
  Rectangle(drawing,&rect);
 }
 Blending(additive);
}

void DrawBarHoriz( Blends blendFill, Blends blendLine, Crayon color, float one, int x, int y, int w, int h, int divisions ) {
 int x2=x+w;
 int y2=y+h;
 Blending(blendFill);
 int filledx=(int)((float)w*(1.0f-one));
 Area(color,x,y,x+filledx,y2);
 float dx=iratiof(w,divisions);
 int dxi=(int) dx;
 Cartesian rect;
 Crayon drawing;
 for ( int i=0; i<divisions; i++ ) {
  int deltax=(int)(dx*(float)i);
  Blending(blendLine);
  if ( deltax >= filledx )  drawing.fromCrayon(color);
  else drawing.Pick(gray);
  rect.SetRect(x+deltax,y,dxi,h);
  Rectangle(drawing,&rect);
 }
 Blending(additive);
}

void DrawCircle( double cx, double cy, double R, int segments ) {
 double delta=TWO_PI/(double) segments;
 glBegin(GL_LINE_LOOP);
 for ( double a=-PI; a<PI; a+=delta ) glVertex2d(cx+R*cos(a), cy+R*sin(a));
 glVertex2d(cx+R*cos(-PI), cy+R*sin(-PI));
 glEnd();
}

void DrawNamedPoint( Crayon color, const char *name, double x, double y ) {
 color.gl();
 Text(
  FORMAT(buf,512,"(%d,%d) %s",(int)x,(int)y,name),
  x+3,y-3,6,8,false
 );
 Area(color,(int)x-1,(int)y-1,(int)x+1,(int)y+1);
}

// Less efficient
#include "RoundedRectangle.h"

RoundedRectangle *RoundRect( Crayon border, Crayon fill, Crayon sep, int x, int y, int width, int height, int radius, int precision, float lineWidth ) {
 return roundrects.Render(border,fill,sep,x,y,width,height,radius,precision,lineWidth);
 ///if ( roundrects.count > rounderects.limit ) roundrects.Clear(); // Controversial
}

void RoundRect9( GLImage* corner, GLImage* border, GLImage* interior, int x, int y, int w, int h, int cornerBorder ) {
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, corner->texture);
 glPushMatrix();
  glBegin(GL_QUADS);
 //Top Left Corner
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)y); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+cornerBorder));
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+cornerBorder));   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)y);    
 //Top Right Corner
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)y); 
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+cornerBorder));
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+cornerBorder));   
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y));
 //Bottom Left Corner
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h-cornerBorder)); 
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h));
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+h));   
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+h-cornerBorder));

  //Bottom Right Corner
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+h));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h));
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h-cornerBorder)); 
 glEnd();
 glBindTexture(GL_TEXTURE_2D, border->texture);
 glBegin(GL_QUADS);

 //Left Side
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+cornerBorder));
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+cornerBorder)); 
 //Top Side
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)y); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+cornerBorder));
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+cornerBorder));   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)y);    
 //Right Side
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+cornerBorder));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+cornerBorder)); 
 //Bottom Side
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h));
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h-cornerBorder)); 
 glEnd();
 glBindTexture(GL_TEXTURE_2D, interior->texture);
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+cornerBorder)); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h-cornerBorder));   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+cornerBorder));    
 glEnd();
 glPopMatrix();

}

void DrawAngle( Crayon tint, int x, int y, double R, double lowDeg, double highDeg, double Precision ) {
 Circle c((double)x,(double)y,R);
 double deltaDeg = (highDeg-lowDeg) / Precision;
 double deg=lowDeg+deltaDeg;
 Vertexd A,B;
 c.Point(lowDeg/360.0,&A);
 DrawLine(tint,(int)A.x,(int)A.y,x,y);
 B.Set(&A);
 while ( deg < highDeg ) {
  c.Point(deg/360.0,&A);
  DrawLine(tint,(int)B.x,(int)B.y,(int)A.x,(int)A.y);
  DrawLine2Color(crayons.Pick(black),tint,(int)x,(int)y,(int)A.x,(int)A.y);
  B.Set(&A);
  deg+=deltaDeg;
 }
 c.Point(highDeg/360.0,&A);
 DrawLine(tint,(int)B.x,(int)B.y,(int)A.x,(int)A.y);
 DrawLine(tint,x,y,(int)A.x,(int)A.y);
}

void BestFit( GLImage *tex, Crayon tint, Blends t, double x, double y, double w, double h, double angle, double pivotOfsX, double pivotOfsY, bool clamp1, bool hflip, bool vflip ) {
 if ( !tex || tex->width == 0 || tex->height == 0 || w==0 || h== 0 ) return;
 double scale = UMIN( w/(double)tex->width, h/(double)tex->height );
 if ( clamp1 && scale > 1.0 ) scale=1.0;
 double width=scale*w;
 double height=scale*h;
 PivotedRotatedFlippableRectd(tex,tint,t,x+w/2.0,y+h/2.0,pivotOfsX,pivotOfsY,height/2.0,width/2.0,angle,hflip,vflip);
}

void BestFit( GLImage *tex, Crayon tint, Blends t, Cartesian *extents, double angle, double pivotOfsX, double pivotOfsY, bool clamp1, bool hflip, bool vflip ) {
 BestFit(tex,tint,t,(double)extents->x,(double)extents->y,(double)extents->w,(double)extents->h,angle,pivotOfsX,pivotOfsY,clamp1,hflip,vflip);
}

void BestFit( GLImage *tex, Crayon tint, Blends t, Cartesiand *extents, double angle, double pivotOfsX, double pivotOfsY, bool clamp1, bool hflip, bool vflip ) {
 BestFit(tex,tint,t,(double)extents->x,(double)extents->y,(double)extents->w,(double)extents->h,angle,pivotOfsX,pivotOfsY,clamp1,hflip,vflip);
}

void BestFit( GLImage *tex, Crayon tint, Blends t, ScreenPosition *extents, double angle, double pivotOfsX, double pivotOfsY, bool clamp1, bool hflip, bool vflip ) {
 BestFit(tex,tint,t,(double)extents->x,(double)extents->y,(double)extents->w,(double)extents->h,angle,pivotOfsX,pivotOfsY,clamp1,hflip,vflip);
}

void BestFit( GLImage *tex, Crayon tint, Blends t, double x, double y, int w, int h, double angle, double pivotOfsX, double pivotOfsY, bool clamp1, bool hflip, bool vflip ) {
 BestFit(tex,tint,t,x,y,(double)w,(double)h,angle,pivotOfsX,pivotOfsY,clamp1,hflip,vflip);
}
