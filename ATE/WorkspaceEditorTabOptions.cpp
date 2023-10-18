#include "WorkspaceEditorTabOptions.h"

#include "WorkspaceEditor.h"
#include "AtlasTileEditor.h"

 void WorkspaceEditorTabOptions::RenderWindow() {
  fast.member=&v;
  if ( !opened ) {
   if ( fast.button("<<",x+w-32,y+h-32,32,32) ) opened=true;
   return;
  } else if ( fast.button(">>",x+w-32,y+h-32,32,32) ) opened=false;
  if ( fast.tbutton("PRESENTATIONS",x+4,y+4,(128+16),24,selected==cets_Presentations) ) {
   v.holder->SaveState(statefile.c_str());
   PresentationEditor *pe=new PresentationEditor((workspace->path+string("presentations\\")).c_str());
   pe->back_to=this->back_to;
   this->v.holder->glWindow->destroyMe=true;
   windows.Add(pe);
   return;
  }/*
  if ( fast.tbutton("THINGS",x+4+(128+16)+4,y+4,96,24,selected==cets_Things) ) {
   v.holder->SaveState(statefile.c_str());
   ThingsEditor *we=new ThingsEditor(workspace);
   we->back_to=this->back_to;
   this->v.holder->glWindow->destroyMe=true;
   windows.Add(we);
   return;
  }
  if ( fast.tbutton("PEOPLE",x+4+(128+16)+4+96+4,y+4,96,24,selected==cets_People) ) {
   v.holder->SaveState(statefile.c_str());
   PeopleEditor *pe=new PeopleEditor(workspace);
   pe->back_to=this->back_to;
   this->v.holder->glWindow->destroyMe=true;
   windows.Add(pe);
   return;
  }
  if ( fast.tbutton("PLACES",x+4+(128+16)+4+96+4+96+4,y+4,96,24,selected==cets_Places) ) {
   v.holder->SaveState(statefile.c_str());
   WorldEditor *we=new WorldEditor(workspace);
   we->back_to=this->back_to;
   this->v.holder->glWindow->destroyMe=true;
   windows.Add(we);
   return;
  }*/
  if ( fast.tbutton("ATLAS TILES",x+4+(128+16)+4+96+4+96+4+96+4,y+4,128,24,false) ) {
   v.holder->SaveState(statefile.c_str());
   AtlasTileEditor *ate=new AtlasTileEditor();
   ate->back_to=this->back_to;
   ate->workspace=workspace;
   this->hidden=true;
   windows.Add(ate);
   return;
  }
 }