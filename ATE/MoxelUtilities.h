#pragma once
#include "EditGUI.h"
#include "MulliganEditor.h"
#include "Workspace.h"
#include "Moxel.h"

 class MoxelUtilities : public MoveableWindowManager {
 public:
  Zp<Workspace> workspace;
  Delay saveStateInterval;
  Zstring statefile;
  Zdouble frameCounter;
  Strings picked;
  Zdouble ignoreNext;
  Zbool doTheFileImport,doTheFolderImport,openExplorerAfter,topDownHeights,saveNormalMaps,sliced3DSprites,save3DSpriteIndividually;
  void ScrubNonVox() {
   Strings other;
   EACHN(picked.first, String, s, {
    Zstring data = s->s;
    data.toLower();
    bool valid_name = data.ending(".vox");
    if (valid_name) other.Add(s->s);
    })
    picked.Clear();
    picked.Concat(&other);
  }
  void HeightToNormal( CPUImage *in, const char *fn_nm ) {
   GLImage *c = new GLImage(fn_nm, in);
   c->deleteAfterUpload = false;
   c->Upload(&gl, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT);
   c->image = nullptr;
   Cartesiand outArea;
   outArea.SetRect(0.0, 0.0, (double)in->getWidth()*2.0, (double)in->getHeight()*2.0);

   glFinish();
   fboCDS2048.background.Pick(clear);
   fboCDS2048.Bind();
   art.PushViewport(&fboCDS2048);
   art.Stretch(c, crayons.Pick(alabaster), outArea.x, outArea.y, outArea.w, outArea.h);
   art.PopViewport();
   fboCDS2048.Unbind();
   delete c;
   glFinish();
   fboCDS2048.BindWithoutClearing();
   CPUImage *o = fboCDS2048.Snapshot();
   fboCDS2048.Unbind();
   glFinish();
   CPUImage *d = o->FlipV();
   delete o; o = null;
   CPUImage *e = d->Crop(0,0,(int)outArea.w, (int)outArea.h);
   delete d; d = null;
   d = e->Half();
   delete e; e = null;
   e = d->HeightToNormalMap();
   delete d; d = null;
   d = e->FlipV();
   delete e; e = null;
   d->Save(fn_nm);
   delete d; d = null;
  }
  void ProcessVoxList() {
   EACH(picked.first, String, p) {
    MV_Model model;
    if (model.Load(p->s.c_str())) {
     Moxel moxel;
     moxel.Import(&model);
     if (topDownHeights) {
      CPUImage *out1 = moxel.TopDownColorMap();
      CPUImage *out2 = moxel.TopDownHeightMap();
      Zstring fn = p->s;
      fn += ".top.color.png";
      out1->Save(fn.c_str());
      fn = p->s;
      fn += ".top.height.png";
      out2->Save(fn.c_str());
      fn = p->s;
      fn += ".top.normal.png";
      if (saveNormalMaps) HeightToNormal(out2, fn.c_str());
      delete out1;
      delete out2;
      if (openExplorerAfter) open_explorer(fn.c_str());
     }
     if (sliced3DSprites) {
      Zstring fn;
      if (save3DSpriteIndividually) {
       for (int i = 0; i < (int)moxel.nodes.length; i++) {
        fn = p->s;
        fn += FMT(".layer.%d.png", i);
        CPUImage *out = moxel.LayerColorMap(i);
        out->Save(fn.c_str());
        delete out;
       }
      } else {
       fn = p->s;
       fn += ".3d.strip.png";
       CPUImage *out = moxel.PackedLayers();
       out->Save(fn.c_str());
       delete out;
      }
     }
    }
   }
  }
  void DoTheFileImport() {
   picked.Clear();
   Strings temp;
   dialog_multiselect(&temp, "ATLAS TILE EDITOR: Pick one or more PNG or JPG image files to add to the import list.");
   if (temp.count == 1 && ((String *)temp.first)->s.length() < 1) {/*none selected*/ }
   else {
    picked.Concat(&temp);
    ScrubNonVox();
    ProcessVoxList();
   }
  }
  void DoTheFolderImport(){
   string path = dialog_get_folder("ATLAS TILE EDITOR: Pick a folder full of image files to add to the import list.", "Select entire folder to be imported below:", false);
   picked.Clear();
   if (path.length() > 0) {
    Strings *temp = ls(path.c_str());
    picked.Concat(temp);
    delete temp;
    ScrubNonVox();
    ProcessVoxList();
   }
  }
  void Between() {
   if (doTheFileImport) {
    DoTheFileImport();
    doTheFileImport = false;
   }
   if (doTheFolderImport) {
    DoTheFolderImport();
    doTheFolderImport = false;
   }
  }
  void RenderBeforeWindow() {
   frameCounter += FRAMETIME;
   fast.rr_border.x_LimeGreen();
   fast.tblend = additive;
   fast.tfont.Set(16, 22);
   fast.highlight.x_LimeGreen();
   fast.activated.x_DarkGreen();
   fast.color.Pick(darkChocolate);
   input->cursorFilm = null;
   fast.ljust(digicaps, FORMAT("%s", workspace->name.c_str()).c_str(), 16, 32, 32, 32, 2, 3, crayons.Pick(aliceBlue), additive);
   fast.ljust(glowTermFont, workspace->path.c_str(), 16, 64, -4, 3, crayons.Pick(aliceBlue));

   fast.text("CHOOSE ONE OR MORE .VOX FILES OR AN ENTIRE FOLDER OF .VOX FILES TO PROCESS",32,128);

   topDownHeights = fast.toggle("EXPORT TOP-DOWN HEIGHT AND COLOR MAPS", topDownHeights, 32, 128 + 32, 32, 32);
   if ( topDownHeights ) saveNormalMaps = fast.toggle("PROCESS HEIGHT MAPS TO NORMAL MAPS", saveNormalMaps, 32, 128 + 64 + 8, 32, 32);
   sliced3DSprites = fast.toggle("SLICE LAYERS INTO 3D SPRITE STRIPS", sliced3DSprites, 32, 128 + 96 + 16, 32, 32);
   if ( sliced3DSprites ) save3DSpriteIndividually = fast.toggle("SAVE 3D SPRITE LAYERS INDIVIDUALLY", save3DSpriteIndividually, 32, 128 + 128 + 24, 32, 32);

   if (topDownHeights || sliced3DSprites) {
    if (!doTheFileImport && fast.button(digicaps, "PROCESS FILES", 32 + 8, 256 + 256, 256, 32, false)) {
     doTheFileImport = true;
     return;
    } else {
     ignoreNext -= FRAMETIME;
     if (ignoreNext < 0.0) ignoreNext = 0.0;
    }
    if (!doTheFolderImport && fast.button(digicaps, "PROCESS FOLDER", 32 + 8, 256 + 256 + 48, 256, 32, false)) {
     doTheFolderImport = true;
     return;
    }
   }
   else {
    fast.dbutton(digicaps, "PROCESS FILES", 32 + 8, 256 + 256, 256, 32);
    fast.dbutton(digicaps, "PROCESS FOLDER", 32 + 8, 256 + 256 + 48, 256, 32);
   }


   if (fast.button(digicaps, "GO BACK", 32 + 8, h - 48, 256, 32, false)) {
    back_to->visible = true;
    destroyMe = true;
    return;
   }
   if (frameCounter > 1.0 && vgui.windows.count > 2) if (fast.button(digicaps, "CLOSE WINDOWS", 32 + 8, h - 48 + 4 + 32, 256, 32, false)) {
    destroyMe = true;
    windows.Visible();
    return;
   }
   //if (fast.button("SAVE", 16, 128-32, 64, 24)) {
   // workspace->Save();
   //}
  }
  void RenderAfterWindow() {
  }
};
