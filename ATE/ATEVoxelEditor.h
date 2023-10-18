#pragma once
#include "FastGUI.h"

#include "Workspace.h"
#include "ATEVoxel.h"

class ATEVoxelEditor : public GLWindow {
 public:
  Zp<GLWindow> back_to;
  Zp<Workspace> workspace;
  Zp<ATEVoxel> editing;
  Zint scrolled;
  Cartesian clickedSp0t;
  Zint col, buttonHeightSm, buttonHeightBig, tfontSize, smidge;
  Cartesian voxelCursor;
  Zp<FBOColorDepthStencil> fbo;
  void OnLoad() {
   Fullscreen();
   background.Int(32, 16, 24, 255);
   col = w < 1900 ? 256 : 384;
   buttonHeightSm = w < 1900 ? 20 : 32;
   buttonHeightBig = w < 1900 ? 24 : 48;
   tfontSize = w < 1900 ? 12 : 16;
   smidge = w < 1900 ? 4 : 8;
   fbo=&fboCDSScreen2;
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
   if (fast.button(digicaps, "GO BACK", w - (col / 2) - smidge, h - (buttonHeightBig + smidge), col / 2, buttonHeightBig) && back_to) {
    back_to->visible = true;
    this->destroyMe = true;
    fast.RecallState();
    return;
   }
   fast.RecallState();
  }
};
