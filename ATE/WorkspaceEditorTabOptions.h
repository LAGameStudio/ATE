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

#include "EditGUI.h"
#include "Workspace.h"

class PresentationEditor;
class ThingsEditor;
class PeopleEditor;
class WorldEditor;

enum WorkspaceEditorTabSelections {
 cets_None=0,
 cets_Presentations=1,
 cets_Places=2,
 cets_People=3,
 cets_Things=4
};

class WorkspaceEditorTabOptions : public MoveableEditorWindow {
public:
 Zp<Workspace> workspace;
 Zp<GLWindow> back_to;
 Zp<GLWindow> guiWrapper;
 Zbool opened;
 WorkspaceEditorTabSelections selected;
 Zstring statefile;
 WorkspaceEditorTabOptions( Workspace *c ) : MoveableEditorWindow() {
  workspace=c;
  opened=false;
  selected=cets_None;
  statefile=specialPaths.historic("workspace.editor.windows.txt");
 }
 void OnSetup() {
  clip=false;
  v.background.Pick(black);
  v.noMove=true;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=true;
  v.noTitleBar=true;
  v.noBacking=true;
  v.alwaysOnTop=true;
  v.noClose=true;
  v.noCenter=true;
  v.noClipTitle=true;
  v.minimumSize.Set(60,56);
  v.maximumSize.Set(650,64);
  v.SizeAndPosition(1920-650-96,1080-64-16,650,56);
  Name("");
 }
 void Between() {
  if (opened) v.SizeAndPosition(1920-650-96,1080-64-16,650,56);
  else v.SizeAndPosition(1920-650-96+586,1080-64-16,48,56);
 }
 void RenderWindow();
};
