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

#include "GLWindow.h"
#include "PresentationEditor.h"
#include "CommandLine.h"
#include "WorkspaceEditor.h"
#include "GameOptions.h"

class GameMenu : public GLWindow {
public:
 Zbool playingTitle;
 Strings options;
 Zp<GLImage> bgimage;
 Zp<FBOColor> fbo;
 void OnLoad() {
  fast.DefaultState();
  Fullscreen();
  input->cursorFilm=vault.find("cursors","cursor");
  background.Pick(black);
  fbo=&fbo1080p2;
  fast.ResponsiveRatio(fbo1080p2.w,fbo1080p2.h);
  bgimage = library.Load("data/images/ATE.png");
  options.Add("Edit");
  options.Add("Options");
  options.Add("Exit");
 }
 void Between() {
  if ( !visible ) return;
 }
 void RenderMenu() {
  fast.member=null;
  fast.ResponsiveRatio(fbo1080p2.w,fbo1080p2.h);
  fast.SaveState();
  fast.color.Alpha(0.75);
  fast.activated.Int(127,64,96,255);
  int selection=fast.menu(digicaps,&options,fbo1080p2.w/2-128,fbo1080p2.h-(48+8)*options.count-32,256,48);
  fast.RecallState();
  if ( selection >= 0 ) {
   switch ( selection ) {
    case 0: // Edit
      {
       dJ.FadeToSilence();
       WorkspaceSelector *ce=new WorkspaceSelector;
       ce->back_to=this;
       this->visible=false;
       windows.Add(ce);
      }
     break;
    case 1: // Options
      {
       GameOptionsPanel *gop=new GameOptionsPanel;
       gop->back_to=this;
       this->visible=false;
       windows.Add(gop);
      }
     break;
    case 2: // Exit
      exit(0);
     break;
   }
  }
 }
 void Render() {
  if ( !visible ) return;
  fbo=&fbo1080p2;
  fbo->background.Pick(black);
  art.ResetViewport();
  fbo->Bind();
  StretchiNoAlpha(bgimage,crayons.Pick(alabaster),none,0,0,display->w,display->h);
  RenderMenu();
  fbo->Unbind();
  fbo->RenderUpsideDown(0,0,display->w,display->h);
 }
};

