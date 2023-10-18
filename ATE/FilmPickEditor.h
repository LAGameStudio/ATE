#pragma once

#include "EditGUI.h"

class FilmPickEditor : public MoveableEditorWindow {
public:
 Zp<Zp<Film>> bound;
 Zp<Film> was;
 Zstring va,fi;
 Zint scrolled;
 Cartesian window;
 FilmPickEditor( Zp<Film> *f ) : MoveableEditorWindow() {
  bound=f;
  Set(f->pointer);
  window.SetRect(64,64,512,384);
 }
 void OnSetup() {
  clip=false;
  v.SizeAndPosition(window.x,window.y,window.w,window.h);
  v.minimumSize.Set(384,256);
  v.maximumSize.Set(1024,768);
  v.noMove=false;
  v.noMinimize=false;
  v.noMaximize=true;
  v.noResize=false;
  v.noTitle=false;
  v.noClose=false;
  v.noClipTitle=true;
  v.background.Int(0,32,0,255);
  Name("Pick a Film");
 }
 void Set( Film *f ) {
  was=f;
  if ( !f ) { va=string(""); fi=string(""); return; }
  FilmLibrary *lib=f->GetLibrary();
  va=lib->title;
  fi=f->title;
 }
 void RenderWindow() {
  if ( !bound ) return;
  if ( was != bound->pointer ) Set(bound->pointer);
  Film *itwas=bound->pointer;
  bound->pointer=fast.FilmPicker(&va.value,&fi.value,&scrolled.value,x,y+32,w,h);
  if ( was != bound->pointer ) Set(bound->pointer);
 }
};

