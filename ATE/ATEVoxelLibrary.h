#pragma once
#include "GLWindow.h"
#include "Atlas.h"
#include "FastGUI.h"
#include "ATEVoxel.h"
#include "Workspace.h"

#include "ATEVoxelEditor.h"

class ATEVoxelSetup : public GLWindow {
public:
 Zp<ATEVoxel> editing;
 Zp<GLWindow> back_to;
 Zp<Workspace> workspace;
 Zint scrolled;
 Cartesian clickedSp0t;
 Zint col, buttonHeightSm, buttonHeightBig, tfontSize, smidge;
 void OnLoad() {
  Fullscreen();
  background.Int(32, 16, 24, 255);
  col = w < 1900 ? 256 : 384;
  buttonHeightSm = w < 1900 ? 20 : 32;
  buttonHeightBig = w < 1900 ? 24 : 48;
  tfontSize = w < 1900 ? 12 : 16;
  smidge = w < 1900 ? 4 : 8;
 }
 void Render() {
  if (!visible) return;
  fast.DefaultState();
  fast.ResponsiveRatio(w, h);
  if (input->wheelUp > 0) scrolled -= 2;
  if (input->wheelDown > 0) scrolled += 2;
  if (scrolled < 0) scrolled = 0;
  if (scrolled > atlasLibrary.count - 3) scrolled = atlasLibrary.count - 3;
  fast.SaveState();
  fast.tfont.Set(tfontSize, tfontSize);
  Zstring voxelName=editing->name;
  voxelName=fast.textbox(termFont,additive,calibri,additive,"\nNAME",voxelName,16-6,h-72-6,45,false,1,2,4,true,-4,nullptr,nullptr,false,false,true);
  if (!voxelName.Equals(editing->name.c_str())) {
   editing->name = voxelName;
  }
  if (fast.button(digicaps, "EDIT", smidge, h - (buttonHeightBig + smidge), col, buttonHeightBig)) {
   ATEVoxelEditor *ave=new ATEVoxelEditor;
   ave->editing=editing;
   ave->back_to=this;
   ave->workspace=workspace;
   this->visible=false;
   windows.Add(ave);
   return;
  }
  if (fast.button(digicaps, "GO BACK", w - (col / 2) - smidge, h - (buttonHeightBig + smidge), col / 2, buttonHeightBig) && back_to) {
   back_to->visible = true;
   this->destroyMe = true;
   fast.RecallState();
   return;
  }
  fast.RecallState();
 }
};

class ATEVoxelLibrary : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<Workspace> workspace;
 Zint scrolled;
 Cartesian clickedSp0t;
 Zint col, buttonHeightSm, buttonHeightBig, tfontSize, smidge;
 void OnLoad() {
  Fullscreen();
  background.Int(32, 16, 24, 255);
  col = w < 1900 ? 256 : 384;
  buttonHeightSm = w < 1900 ? 20 : 32;
  buttonHeightBig = w < 1900 ? 24 : 48;
  tfontSize = w < 1900 ? 12 : 16;
  smidge = w < 1900 ? 4 : 8;
 }
 void Render() {
  if (!visible) return;
  fast.DefaultState();
  fast.ResponsiveRatio(w, h);
  if (input->wheelUp > 0) scrolled -= 2;
  if (input->wheelDown > 0) scrolled += 2;
  if (scrolled < 0) scrolled = 0;
  if (scrolled > atlasLibrary.count - 3) scrolled = atlasLibrary.count - 3;
  fast.SaveState();
  fast.tfont.Set(tfontSize, tfontSize);
  if (fast.button(digicaps, "CREATE NEW VOXEL", smidge, h - (buttonHeightBig + smidge), col, buttonHeightBig)) {
   ATEVoxel *editing=new ATEVoxel;
   workspace->voxels.Append(editing);
   ATEVoxelSetup *avs=new ATEVoxelSetup;
   avs->editing=editing;
   avs->back_to=this;
   avs->workspace=workspace;
   this->visible=false;
   windows.Add(avs);
   return;
  }
  if (fast.button(digicaps, "GO BACK", w - (col / 2) - smidge, h - (buttonHeightBig + smidge), col / 2, buttonHeightBig) && back_to) {
   back_to->visible = true;
   this->destroyMe = true;
   fast.RecallState();
   return;
  }
  fast.RecallState();
 }
};

