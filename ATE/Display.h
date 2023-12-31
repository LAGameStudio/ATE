/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once
#include <Windows.h>
#include <stdio.h>
#include "macros.h"
#include "moremath.h"
#include "Cartesian.h"

class GLSetup;

struct Display {
public:
 int w,h;
 double wd,hd,w2d,h2d;
 float wf,hf,w2f,h2f;
 int midx,midy,w2,h2;
 float aspect,inverseaspect;
 int bits;
 HWND hwnd;
 HINSTANCE hinstance;
 HDC hdc;
 HGLRC hglrc;
 GLSetup *gl;
 Cartesian virtualResolution;
 Cartesiand virtualRatio,inverseVirtualRatio;
 Cartesian screen;
 Cartesiand screend;
 bool probablyMatroxTripleHead2Go,probablyMatroxDualMode,probablyMatroxTripleHead;
 void Constructor() {
  w=h=midx=midy=w2=h2=0;
  bits=8;
  wd=hd=0.0;
  w2d=h2d=0.0;
  wf=hf=0.0f;
  w2f=h2f=0.0f;
  aspect=4.0f/3.0f;
  inverseaspect=3.0f/4.0f;
  hwnd=NULL;
  hinstance=NULL;
  hdc=NULL;
  hglrc=NULL;
  gl=null;
  probablyMatroxDualMode=probablyMatroxTripleHead=probablyMatroxTripleHead2Go=false;
 }
 Display(void) { Constructor(); }
 Display(int ww, int hh, int bpp ) {
  Constructor();
  bits=bpp;
  Resized(ww,hh);
 }
 void Virtual( int ww, int hh ) {
  virtualResolution.SetRect(0,0,ww,hh);
  virtualRatio.Set(iratiod(ww,w),iratiod(hh,h));
  inverseVirtualRatio.Set(iratiod(w,ww),iratiod(h,hh));
 }
 void VirtualScale2d();
 void UnVirtualScale2d();
 void Resized( int ww, int hh, bool updateWindowManager=true );
 bool MakeCurrent() {
 	if(!wglMakeCurrent(hdc, hglrc))	{
 		OUTPUT("Display:MakeCurrent() is unable to make Display the current context");
 		return false;
 	} 
 	return true;
 }
 void CommandLine();
 
 //Save a screenshot
 void SaveScreenshot();
~Display(void) {}
};

extern Display display;
extern Display *displayPointer;