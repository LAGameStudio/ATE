#pragma once
#include "ZeroTypes.h"
#include "GLWindow.h"
#include "TextureLibrary.h"

#include "Graphics.h"
#include "Workspace.h"

class AssetLoader : public GLWindow {
public:
 Zp<GLImage> backing;
 Strings workspace_files;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  backing=library.Load("data/images/splash.png");
//  workspaces.Load((specialPaths.working_directory+string("data/workspaces.txt")).c_str());
 }
 void Between() {
 }
 void Render() {
  vg.Start();
  vg.Stretchi(backing,crayons.Pick(alabaster),1.0f,none,0,0,vg.w,vg.h);
  vg.End();
 }
};

