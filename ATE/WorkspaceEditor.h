/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "PresentationEditor.h"
#include "AtlasTileEditor.h"
#include "AtlasLibraryBrowser.h"
#include "MulliganWorkflow.h"
#include "MoxelUtilities.h"

//////////////////
// Workspace Editor

class WorkspaceEditor : public MoveableWindowManager {
public:
/// Zp<GLWindow> back_to;
 Zp<Workspace> workspace;
 Delay saveStateInterval;
 Zstring statefile;
 void LoadEditors() {
  statefile=specialPaths.historic("workspace.editor.windows.txt");

  vgui.Arrange2Rows();
  if ( file_exists(statefile.c_str()) ) vgui.LoadState(statefile.c_str());
  else vgui.SaveState(statefile.c_str());
 }
 void RenderBeforeWindow() {
  fast.rr_border.x_LimeGreen();
  fast.tblend = additive;
  fast.tfont.Set(16, 22);
  fast.highlight.x_LimeGreen();
  fast.activated.x_DarkGreen();
  fast.color.Pick(darkChocolate);
  input->cursorFilm = null;
  fast.ljust(digicaps, FORMAT("%s",workspace->name.c_str()).c_str(),16,32,32,32,2,3,crayons.Pick(aliceBlue),additive);
  fast.ljust(glowTermFont,workspace->path.c_str(),16,64,-4,3,crayons.Pick(aliceBlue));
 }
 void RenderAfterWindow() {  
  fast.ResponsiveRatio(fbo->w, fbo->h);
  if ( fast.button(digicaps,"ATLAS LIBRARY",512,512,384,64) ) {
   AtlasLibraryBrowser *alb=new AtlasLibraryBrowser;
   alb->back_to=this;
   alb->workspace=workspace;
   this->visible=false;
   windows.Add(alb);
   return;
  }
  if ( fast.button(digicaps,"ATLAS TILES",512+384+16,512,384,64) ) {
   AtlasTileEditor *ate=new AtlasTileEditor();
   ate->back_to=this;
   ate->workspace=workspace;
   this->visible=false;
   windows.Add(ate);
   return;
  }
  if ( fast.button(digicaps, "MULLIGANS", 512, 512 + 64 + 16, 384, 64) ) {
   MulliganWorkflow *mw = new MulliganWorkflow;
   mw->back_to = this;
   mw->workspace = workspace;
   this->visible = false;
   windows.Add(mw);
   return;
  }
  fast.dbutton(digicaps,"PRESENTATIONS",512+384+16,512+64+16,384,64);
  fast.dbutton(digicaps,"ISOMETRIC TILES",512,512+(64+16)*2,384,64);
  fast.dbutton(digicaps,"3D ATLAS BLOCKS",512+384+16,512+(64+16)*2,384,64);

  if (fast.button(digicaps, "MAGICAVOXELIZER", 512, 512 + (64 + 16) * 3, 384, 64)) {
   MoxelUtilities *mu = new MoxelUtilities;
   mu->back_to = this;
   mu->workspace = workspace;
   this->visible = false;
   windows.Add(mu);
   return;
  }

  if ( fast.button(digicaps,"SAVE WORKSPACE",32+8,96,256,32,false) ) {
   if ( workspace ) workspace->Save();
   vgui.SaveState(statefile.c_str());
  }
  if ( fast.clicking ) fast.text("SAVING...",16+32+8+32+8,96+16+32);
  if ( fast.button(digicaps,"GO BACK",32+8,h-48,256,32,false) ) {
   back_to->visible=true;
   destroyMe=true;
   return;
  }
  
  if ( fast.button(digicaps,"OPEN FOLDER",32+8,h-48-48,256,32,false) ) {
   open_explorer(workspace->path.c_str());
   vgui.SaveState(statefile.c_str());
  }
 }
};

/////////////////////////////////////////////////////////
// Workspace Selector, for getting into a Workspace Editor.

// Workspaces are created by first creating a folder on the disc,
// in the workspace folder, and placing an empty text file called
// workspaces.txt within it.  Once this is done, workspaces can be
// selected, their details edited, and the other files and
// folders will be maintained automatically.  Modders can put
// their files into the managed folders and then associate
// them inside the workspace manager and sub-editors.

class WorkspaceSelector : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<FBOColorDepth> fbo;
 Zp<Workspace> ed;
 Zp<WorkspaceEditor> editor;
 Strings workspacesList;
 Zint scrolled;
 Zbool editWorkspaceDetailsMode;
 Zstring newfoldername;
 Dim2i tfont;
 Crayon oldColor;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  fbo=&fbo1080p;
  Repopulate();
  fast.rr_border.x_LimeGreen();
  fast.tblend=additive;
  oldColor=fast.color;
  tfont.w=fast.tfont.w;
  tfont.h=fast.tfont.h;
  fast.tfont.Set(16,22);
  fast.highlight.x_LimeGreen();
  fast.activated.x_DarkGreen();
  fast.color.Pick(darkChocolate);
  input->cursorFilm=null;
 }
 void Repopulate() {
  workspacesList.Clear();
  EACH(workspaces.first,Workspace,c) {
   c->path("//","/");
   c->path("\\\\","\\");
   workspacesList.Add(FORMAT("%s (%s)",c->name.c_str(),c->path.c_str()));
  }
 }
 void Between() {
 }
 void Render() {
  if ( visible ) {
   fast.vwindows=null;
   fast.fontSize.x=8.0;
   fast.fontSize.y=11.0;
   fast.ResponsiveRatio(fbo1080p.w,fbo1080p.h);
   art.PushViewport(fbo);
   fbo->background.Pick(indigo);
   fbo->Bind();
   if ( !editWorkspaceDetailsMode ) {
    int selection=fast.list(digicaps,"Load Existing Workspace",&workspacesList,16,32,1920-32,40,&scrolled.value);
    if ( selection >= 0 ) {
     ed=(Workspace *) workspaces.Element(selection);
     if ( ed ) editWorkspaceDetailsMode=true;
    }
    if ( fast.button(digicaps,"GO BACK",32,1000,192,32,false) ) {
     fbo->Unbind();
     art.PopViewport();
     destroyMe=true;
     if ( back_to ) back_to->visible=true;
     fast.tfont.w=tfont.w;
     fast.tfont.h=tfont.h;
     fast.color=oldColor;
     return;
    }
    newfoldername=fast.textbox(glowTermFont,transparency,calibri,additive,"New Workspace Folder Name",newfoldername,1920-1024-64,1000-64,80,true,1,2,4,false,-4,null,null,true,true);
    newfoldername("/","");
    newfoldername("\\","");
    newfoldername(",","");
    newfoldername("+","");
    newfoldername("=","");
    newfoldername(")","");
    newfoldername("(","");
    newfoldername("[","");
    newfoldername("]","");
    newfoldername("{","");
    newfoldername("}","");
    newfoldername("`","");
    newfoldername("~","");
    newfoldername("'","");
    newfoldername("\"","");
    newfoldername("\n","");
    newfoldername("\r","");
    newfoldername("\t","");
    newfoldername(":","");
    newfoldername(" ","");
    Zstring newWorkspaceFolder=FORMAT("%s\\ATE\\workspaces\\%s\\",specialPaths.saved_documents.c_str(),newfoldername.c_str());
    if ( !file_exists(newWorkspaceFolder.c_str()) 
     && fast.button(calibri,"Create New Workspace",1920-1024-64,1000,512,32,false) ) {
     if ( CreateDirectory(newWorkspaceFolder.c_str(),NULL) != 0 ) {
      ed=new Workspace(newfoldername.c_str());
      workspaces.Append(ed);
      ed->path=newWorkspaceFolder;
      ed->filename=newWorkspaceFolder+string("workspace.txt");
      ed->ValidateFolderStructure();
      ed->Save();
      editor=new WorkspaceEditor;
      activeWorkspace = ed;
      editor->workspace=ed;
      editor->back_to=this;
      windows.Add(editor);
      visible=false;
      fbo->Unbind();
      art.PopViewport();
      Repopulate();
      return;
     }
    }
   } else {
    ed->name = fast.textbox(glowTermFont,transparency,digicaps,additive,"Title",ed->name,192,128,80,true,1,2,4,false,-4);
    ed->description = fast.textbox(glowTermFont,transparency,digicaps,additive,"Summary",ed->description,192,256,80,true,3,2,4,false,-4);
    fast.ljust(glowTermFont, FORMAT("Workspace Folder:\n%s",ed->path.c_str()).c_str(),16,16,16,20,-4,3,crayons.Pick(aliceBlue),additive);
    if ( fast.button(digicaps,"EDIT",1920-512-32,1000,512,32,false) ) {
     ed->ValidateFolderStructure();
//     ed->Save();///! Is this needed here?
     // Create child..
     editor=new WorkspaceEditor;
     activeWorkspace = ed;
     editor->workspace=ed;
     editor->back_to=this;
     windows.Add(editor);
     visible=false;
     fbo->Unbind();
     art.PopViewport();
     Repopulate();
     return;
    }
    if ( fast.button(digicaps,"GO BACK",32,1000,192,32,false) ) {
     editWorkspaceDetailsMode=false;
     Repopulate();
    }
   }
   fbo->Unbind();
   art.PopViewport();
   fbo->blend=none;  
   fbo->RenderUpsideDown(0,0,display->w,display->h);
  }
 }
};
