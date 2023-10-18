#pragma once
#include "EditGUI.h"
#include "MulliganEditor.h"
#include "Workspace.h"
#include "MulliganMixer.h"

class MulliganSelector : public MoveableEditorWindow {
public:
 Cartesian window;
 Zp<Workspace> workspace;
 MulliganSelector() : MoveableEditorWindow() {
  window.SetRect(32, 256, 512, 512);
 }
 void OnSetup() {
  clip = false;
  v.alwaysOnTop = true;
  v.SizeAndPosition(window.x, window.y, window.w, window.h);
  v.minimumSize.Set(512, 512);
  v.maximumSize.Set(512, 512);
  v.noMove = false;
  v.noMinimize = false;
  v.noMaximize = true;
  v.noResize = true;
  v.noTitle = false;
  v.noClose = true;
  v.noClipTitle = true;
  v.background.Int(0, 48, 16, 255);
  Name("MULLIGANS");
  //  fd.description="Choose a sound file to add to this sound effect.";
  //  fd.path=workspace->path+string("sounds");
  //  fd.background.x_DarkSlateBlue();
  //  fd.bound=&filepicker.value;
  Repopulate();
 }
 void Repopulate() {
  mulligansByName.Clear();
  int i = 0;
  EACH(workspace->mulligans.first, Mulligan, m) {
   i++;
   mulligansByName.Add(FMT("%d) %s", i, m->name.c_str()));
  }
 }
 void Between() {
  selected = -1;
 }
 Strings mulligansByName;
 Zint selected, scrolled;
 void RenderWindow() {
  if (fast.button("Create New", (int)v.position.x + 16, (int)v.position.y2 - 32, (int)v.position.w - 32, 32)) {
   Mulligan *m = new Mulligan;
   m->name = "New Mulligan";
   workspace->mulligans.Append(m);
   MulliganEditor *me = new MulliganEditor(workspace, m);
   me->Setup();
   me->Name(FMT("MULLIGAN: %s",m->name.c_str()));
   me->mwm = this->mwm;
   this->mwm->Add(me);
   v.holder->Arrange2Rows();
   v.holder->SlideTo(&me->v);
   Repopulate();
   return;
  }
  selected = fast.list("",&mulligansByName, x + 16, y + 16, w - 32 - 48, 16, &scrolled.value);
  if (selected != -1) {
   Mulligan *m = (Mulligan *)workspace->mulligans.Element(selected);
   if (m) {
    MulliganEditor *me = new MulliganEditor(workspace, m);
    me->Setup();
    me->Name(FMT("MULLIGAN: %s", m->name.c_str()));
    me->mwm = this->mwm;
    this->mwm->Add(me);
    v.holder->Arrange2Rows();
    v.holder->SlideTo(&me->v);
    Repopulate();
   }
   selected = -1;
  }
 }
};




class MulliganMixSelector : public MoveableEditorWindow {
public:
 Cartesian window;
 Zp<Workspace> workspace;
 MulliganMixSelector() : MoveableEditorWindow() {
  window.SetRect(32, 256, 512, 512);
 }
 void OnSetup() {
  clip = false;
  v.alwaysOnTop = true;
  v.SizeAndPosition(window.x, window.y, window.w, window.h);
  v.minimumSize.Set(512, 512);
  v.maximumSize.Set(512, 512);
  v.noMove = false;
  v.noMinimize = false;
  v.noMaximize = true;
  v.noResize = true;
  v.noTitle = false;
  v.noClose = true;
  v.noClipTitle = true;
  v.background.x_DarkGray();
  v.background.Multiply(0.25f, true);
  Name("MULLIGAN MIXES");
  //  fd.description="Choose a sound file to add to this sound effect.";
  //  fd.path=workspace->path+string("sounds");
  //  fd.background.x_DarkSlateBlue();
  //  fd.bound=&filepicker.value;
  Repopulate();
 }
 void Repopulate() {
  mixesByName.Clear();
  int i = 0;
  EACH(workspace->mulliganMixes.first, MulliganMix, m) {
   i++;
   mixesByName.Add(FMT("%d) %s", i, m->name.c_str()));
  }
 }
 void Between() {
  selected = -1;
 }
 Strings mixesByName;
 Zint selected, scrolled;
 void RenderWindow() {
  if (fast.button("Create New", (int)v.position.x + 16, (int)v.position.y2 - 32, (int)v.position.w - 32, 32)) {
   MulliganMix *m = new MulliganMix;
   m->name = "New Mix";
   m->name += FMT(" %d", workspace->mulliganMixes.count.value);
   workspace->mulliganMixes.Append(m);   
   MulliganMixer *me = new MulliganMixer(workspace, m);
   me->Setup();
   me->Name(FMT("MIX: %s", m->name.c_str()));
   me->mwm = this->mwm;
   this->mwm->Add(me);
   v.holder->Arrange2Rows();
   v.holder->SlideTo(&me->v);
   Repopulate();
   return;
  }
  selected = fast.list("", &mixesByName, x + 16, y + 16, w - 32 - 48, 16, &scrolled.value);
  if (selected != -1) {
   MulliganMix *m = (MulliganMix *)workspace->mulliganMixes.Element(selected);
   if (m) {
    MulliganMixer *me = new MulliganMixer(workspace, m);
    me->Setup();
    me->Name(FMT("MIX: %s", m->name.c_str()));
    me->mwm = this->mwm;
    this->mwm->Add(me);
    v.holder->Arrange2Rows();
    v.holder->SlideTo(&me->v);
    Repopulate();
   }
   selected = -1;
  }
 }
};



class MulliganWorkflow : public MoveableWindowManager {
public:
  Zp<Workspace> workspace;
  Delay saveStateInterval;
  Zstring statefile;
  Zp<MulliganSelector> mulligans;
  Zp<MulliganMixSelector> mulliganMixes;
  Zdouble frameCounter;
  void LoadEditors() {
   statefile = specialPaths.historic("mulligans.editor.windows.txt");
   vgui.Arrange2Rows();
   if (file_exists(statefile.c_str())) vgui.LoadState(statefile.c_str());
   else vgui.SaveState(statefile.c_str());
   mulliganMixes = new MulliganMixSelector;
   mulliganMixes->workspace = workspace;
   Add(mulliganMixes);
   mulliganMixes->mwm = this;
   mulligans = new MulliganSelector;
   mulligans->workspace = workspace;
   Add(mulligans);
   mulligans->mwm = this;
   vgui.Arrange2Rows();
   vgui.SlideTo(&mulligans->v);
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
   if (fast.button(digicaps, "GO BACK", 32 + 8, h - 48, 256, 32, false)) {
    back_to->visible = true;
    destroyMe = true;
    return;
   }
   if (frameCounter > 1.0 && vgui.windows.count > 2) if (fast.button(digicaps, "CLOSE WINDOWS", 32 + 8, h - 48+4+32, 256, 32, false)) {
    MulliganWorkflow *r = new MulliganWorkflow;
    r->back_to = this->back_to;
    r->workspace = this->workspace;
    windows.Add(r);
    destroyMe = true;
    return;
   }
   if (fast.button("SAVE", 16, 128, 64, 24)) {
    workspace->Save();
   }
  }
  void RenderAfterWindow() {
  }
};

