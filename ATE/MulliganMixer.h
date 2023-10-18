#pragma once

#include "EditGUI.h"
#include "MulliganMix.h"
#include "Workspace.h"
#include "Delay.h"

class MulliganWorkflow;

class MulliganMixer : public MoveableEditorWindow {
public:
 Zp<Workspace> workspace;
 Zp<MulliganMix> ed;
 Cartesian window, preview;
 Strings outSizes;
 Zbool playing,delayedPlayback;
 Delay delay;
 Zdouble buttonDelay;
 Zp<GLImage> playIcon, pauseIcon, stepIcon, removeIcon, addIcon;
 Zstring outPath,fnprefix,fnpostfix;
 Zbool no_leading_zeros;
 MulliganMixer(Workspace *ws, MulliganMix *m) : MoveableEditorWindow() {
  workspace = ws;
  ed = m;
  playIcon = library.Load("data/images/elements/next.png");
  pauseIcon = library.Load("data/images/icon3/stop.png");
  stepIcon = library.Load("data/images/icon3/step.png");
  removeIcon = library.Load("data/images/icon3/close.png");
  addIcon = library.Load("data/images/fonts/term/plus.png");
  outPath = workspace->path+string("exports\\mulligans\\");
  preview.SetRect(512, 512, 512, 512);
  Construct();
 }
 void Construct() {
  delay.Set(1.0);
  window.SetRect(512, 512, 1024, 768);
  outSizes.Add("16x16",0);
  outSizes.Add("32x32",1);
  outSizes.Add("48x48",2);
  outSizes.Add("64x64",3);
  outSizes.Add("128x128",4);
  outSizes.Add("256x256",5);
  outSizes.Add("512x512",6);
  outSizes.Add("1024x1024",7);
  outSizes.Add("2048x2048",8);
 }
 void OnSetup() {
  clip = false;
  v.alwaysOnTop = true;
  v.SizeAndPosition(window.x, window.y, window.w, window.h);
  v.minimumSize.Set(1024, 768);
  v.maximumSize.Set(1024, 768);
  v.noMove = false;
  v.noMinimize = false;
  v.noMaximize = true;
  v.noResize = true;
  v.noTitle = false;
  v.noClose = true;
  v.noClipTitle = true;
  v.background.x_Burlywood();
  v.background.Multiply(0.25f, true);
  Name((string("MIX: ")+ed->name).c_str());
  //  fd.description="Choose a sound file to add to this sound effect.";
  //  fd.path=workspace->path+string("sounds");
  //  fd.background.x_DarkSlateBlue();
  //  fd.bound=&filepicker.value;
  Repopulate();
 }
 void Repopulate() {
  UpdateOutSize();
 }
 void UpdateOutSize() {
  switch (ed->outSize) {
   default:
   case mmos_16x16:
   case mmos_32x32:
   case mmos_48x48:
   case mmos_64x64:
   case mmos_128x128:
   case mmos_256x256:
   case mmos_512x512: ed->scratch = &fbo512c; ed->out = &fbo512d; break;
   case mmos_1024x1024: ed->scratch = &fbo1024; ed->out = &fbo1024a; break;
   case mmos_2048x2048: ed->scratch = &fbo2048; ed->out = &fbo2048a; break;
  }
  ed->nodes.CaptureTo(ed->expired, ed->scratch);
 }
 Zint scrolled;
 Zp<MulliganMixNode> selectedNode;
 void RenderWindow() {
  fast.member = &v;
  if (v.closing) return;
  Cartesian pen;
  pen.Set(x+16, y+16);
  if (fast.button(playing ? playIcon : pauseIcon, pen.x, pen.y, 48, 48)) {
   playing = !playing;
   if (playing) ed->Rewind();
  }
  if (fast.button(stepIcon, pen.x+48+8, pen.y, 48, 48)) {
   ed->Advance(1, FRAMETIME);
  }
  pen.x += 64;
  MulliganMixOutSizes oldSize = ed->outSize;
  ed->outSize = (MulliganMixOutSizes) fast.enumerated(&outSizes, "", (int)ed->outSize, pen.x+48+16, pen.y, 48, 24, 10);
  if (oldSize != ed->outSize) UpdateOutSize();
  delayedPlayback = fast.toggle("Slow-Step Frames in Preview", delayedPlayback, pen.x + 48 + 16, pen.y + 32, 24, 24);
  pen.x = x + 16 + 512;
  std::string oldname = ed->name;
  ed->name = fast.textbox("NAME", ed->name, pen.x - 128, pen.y, 32, true);
  if (ed->name.length != oldname.length()) Name((string("MIX: ") + ed->name).c_str());
  fnprefix = fast.textbox("FILENAME PREFIX", fnprefix, pen.x + 128 + 64, pen.y, 32, true);
  fnpostfix = fast.textbox("FILENAME POSTFIX", fnpostfix, pen.x + 128 + 64, pen.y + 32, 32, true);
  pen.y += 64;
  pen.x = x + 16;
  buttonDelay += FRAMETIME;
  if (buttonDelay > 0.5 && fast.button("PICK FOLDER", pen.x, pen.y, 128, 24)) {
   string path = dialog_get_folder("ATLAS TILE EDITOR: Pick or create a new folder for exporting.", "Select folder to be filled with exported PNG images:", true);
   if ( path.length() > 0 ) outPath = path;
  }
  outPath = fast.textbox("EXPORT PATH", outPath, pen.x + 128 + 8, pen.y, 90, true);
  pen.y += 64;
  fast.rect(crayons.Pick(black), 0, pen.x, pen.y, preview.w, preview.h);
  if (delayedPlayback) {
   delay.Between();
   if ( !delay ) {
    if (playing) ed->Advance(1, FRAMETIME);
    delay.Reset();
   }
  } else if (playing) ed->Advance(1, FRAMETIME);
  if (!ed->limitFrames ) ed->scratch->RenderUpsideDown(pen.x, pen.y, pen.x + preview.w, pen.y + preview.h);
  else if (ed->limitFrames) {
   if (ed->endFrame > ed->startFrame) {
    if (ed->frame > ed->endFrame) {
     ed->Rewind();
    } else if (ed->frame > ed->startFrame) {
     ed->scratch->RenderUpsideDown(pen.x, pen.y, pen.x + preview.w, pen.y + preview.h);
    }
   }
   else {
    if (ed->frame >= ed->startFrame) ed->scratch->RenderUpsideDown(pen.x, pen.y, pen.x + preview.w, pen.y + preview.h);
   }
  }
  fast.text(FMT("%1.2f seconds  FRAME %4d  %3d particles %2d nodes",(double)ed->expired,(int)ed->frame, ed->TotalParticleCount(),(int)ed->nodes.count).c_str(), pen.x, pen.y - 24);
  pen.y += preview.h + 8;
  ed->limitFrames = fast.toggle("Limit to Frame Range", ed->limitFrames, pen.x, pen.y, 24, 24);
  pen.y += 48;
  ed->startFrame = fast.integer("Start Frame", ed->startFrame, 0, 100000, 1, pen.x+64, pen.y, 48, 24);
  ed->endFrame = fast.integer("End Frame", ed->endFrame, 0, 100000, 1, pen.x+256-8, pen.y, 48, 24);
  if ( buttonDelay > 0.5 && fast.button("EXPORT THIS FRAME", pen.x + 512 - 128, pen.y, 192+128, 32) ) {
   buttonDelay = 0.0;
   Zstring filename = dialog_saveas("Exporting Mulligan Single Frame as PNG");
   if (filename.length > 0) {
    Cartesiand outArea;
    switch (ed->outSize) {
     default:
    case mmos_16x16: outArea.SetRect(0.0, 0.0, 16.0, 16.0); break;
     case mmos_32x32: outArea.SetRect(0.0, 0.0, 32.0, 32.0); break;
     case mmos_48x48: outArea.SetRect(0.0, 0.0, 48.0, 48.0); break;
     case mmos_64x64: outArea.SetRect(0.0, 0.0, 64.0, 64.0); break;
     case mmos_128x128: outArea.SetRect(0.0, 0.0, 128.0, 128.0); break;
     case mmos_256x256: outArea.SetRect(0.0, 0.0, 256.0, 256.0); break;
     case mmos_512x512: outArea.SetRect(0.0, 0.0, 512.0, 512.0); break;
     case mmos_1024x1024: outArea.SetRect(0.0, 0.0, 1024.0, 1024.0); break;
     case mmos_2048x2048: outArea.SetRect(0.0, 0.0, 2048.0, 2048.0); break;
    }
    CPUImage *c = null, *d = null, *e = null;
    fboCDS2048.background.Pick(clear);
    fboCDS2048.Bind();
    art.PushViewport(&fboCDS2048);
    ed->scratch->Render(crayons.Pick(alabaster), transparency, outArea.x, outArea.y, outArea.w, outArea.h);
    fboCDS2048.Unbind();
    fboCDS2048.BindWithoutClearing();
    c = fboCDS2048.Snapshot();
    art.PopViewport();
    fboCDS2048.Unbind();
    d = c->FlipV();
    delete c; c = null;
    e = d->Crop((int)outArea.x, (int)outArea.y, (int)outArea.w, (int)outArea.h);
    delete d; d = null;
    d = e->Half();
    delete e; e = null;
    d->Save(filename.c_str());
    delete d; d = null;
   }
  }
  if ( buttonDelay > 0.5 && fast.button(digicaps, "EXPORT FRAMES", pen.x + 512 + 8 + 192, pen.y, 256, 32) ) {
   buttonDelay = 0.0;
   if (outPath.length == 0 || !folder_exists(outPath.c_str())) {
    string path = dialog_get_folder("ATLAS TILE EDITOR: Pick or create a new folder for exporting.", "Select folder to be filled with exported PNG images:", true);
    if (path.length() > 0) outPath = path;
   }
   if ( folder_exists(outPath) ) ExportAll();
  }
  pen.y += 32;
  pen.Set(x + 16 + preview.w + 8, y + 128);
  fast.text("INCLUDED MULLIGANS", pen.x + 256 - 16, pen.y, true);
  if (fast.button(addIcon, pen.x + 512 - 64, pen.y, 24, 24)) {
   AddMulligan();
  }
  pen.y += 32;
  int selected = fast.list(&ed->nodes, pen.x, pen.y, 512-96, 10, &scrolled.value);
  if (selected >= 0) {
   selectedNode = (MulliganMixNode *)ed->nodes.Element(selected);
  }
  pen.y += 384;
  if (selectedNode) {
   fast.text(selectedNode->mulligan.name, pen.x, pen.y);
   if (fast.button(removeIcon, x + w - 32, pen.y, 24, 24)) {
    ed->nodes.Remove(selectedNode);
    delete selectedNode.pointer;
    selectedNode = nullptr;
    return;
   }
   pen.y += 40;
   selectedNode->startTime = fast.decimal("Start time (s)", selectedNode->startTime, 0.0, 100.0, 0.1, pen.x, pen.y, 48, 24);
   selectedNode->endTime = fast.decimal("End time (s)", selectedNode->endTime, 0.0, 100.0, 0.1, pen.x+256-16, pen.y, 48, 24);
   if (fast.button("=", x + w - 32, pen.y, 24, 24)) {
    selectedNode->startTime = 0.0;
    selectedNode->endTime = 0.0;
   }
   pen.y += 32;
   Cartesiand xy(selectedNode->x, selectedNode->y);
   fast.arrows("Offset", &xy, pen.x + 32, pen.y + 32, 24);
   selectedNode->x = xy.x;
   selectedNode->y = xy.y;
   selectedNode->scaleX = fast.decimal("Scale X", selectedNode->scaleX, 0.0, 100.0, 0.1, pen.x + 128 + 32, pen.y, 48, 24);
   selectedNode->scaleY = fast.decimal("Scale Y", selectedNode->scaleY, 0.0, 100.0, 0.1, pen.x + 256 + 64, pen.y, 48, 24);
   if (fast.button("=", x + w - 32, pen.y, 24, 24)) {
    selectedNode->scaleX = 1.0;
    selectedNode->scaleY = 1.0;
   }
   pen.y += 32;
   selectedNode->vx = fast.decimal("Velocity X", selectedNode->vx, 0.0, 100.0, 0.1, pen.x + 128 + 32, pen.y, 48, 24);
   selectedNode->vy = fast.decimal("Velocity Y", selectedNode->vy, 0.0, 100.0, 0.1, pen.x + 256 + 64, pen.y, 48, 24);
  }
 }
 void AddMulligan();
 void ExportAll() {
  Cartesiand outArea;
  switch (ed->outSize) {
  default:
  case mmos_16x16: outArea.SetRect(0.0, 0.0, 16.0, 16.0); break;
  case mmos_32x32: outArea.SetRect(0.0, 0.0, 32.0, 32.0); break;
  case mmos_48x48: outArea.SetRect(0.0, 0.0, 48.0, 48.0); break;
  case mmos_64x64: outArea.SetRect(0.0, 0.0, 64.0, 64.0); break;
  case mmos_128x128: outArea.SetRect(0.0, 0.0, 128.0, 128.0); break;
  case mmos_256x256: outArea.SetRect(0.0, 0.0, 256.0, 256.0); break;
  case mmos_512x512: outArea.SetRect(0.0, 0.0, 512.0, 512.0); break;
  case mmos_1024x1024: outArea.SetRect(0.0, 0.0, 1024.0, 1024.0); break;
  case mmos_2048x2048: outArea.SetRect(0.0, 0.0, 2048.0, 2048.0); break;
  }
  std::string path=trailing_backslash(outPath.c_str());
  ed->Rewind();
  bool complete = false;
  if (ed->limitFrames) for (int i = 0; i < ed->startFrame; i++) ed->Advance(1);
  int saved = 0;
  while (!complete) {
   saved++;
   ed->Advance(1);
   std::string filename = FMT("%s%s%03d%s.png", path.c_str(), fnprefix.c_str(), saved, fnpostfix.c_str());
   CPUImage *c = null, *d = null, *e = null;
   fboCDS2048.background.Pick(clear);
   fboCDS2048.Bind();
   art.PushViewport(&fboCDS2048);
   ed->scratch->Render(crayons.Pick(alabaster), transparency, outArea.x, outArea.y, outArea.w, outArea.h);
   fboCDS2048.Unbind();
   fboCDS2048.BindWithoutClearing();
   c = fboCDS2048.Snapshot();
   art.PopViewport();
   fboCDS2048.Unbind();
   d = c->FlipV();
   delete c; c = null;
   e = d->Crop((int)outArea.x, (int)outArea.y, (int)outArea.w, (int)outArea.h); 
   delete d; d = null;
   d = e->Half();
   delete e; e = null;
   d->Save(filename.c_str());
   delete d; d = null;
   if (ed->limitFrames) { if (ed->frame >= ed->endFrame) break; } else if (ed->TotalParticleCount() == 0 && ed->ImplementationsLeft() == 0) break;
  }
 }
};


class MulliganAddSelector : public MoveableEditorWindow {
public:
 Cartesian window;
 Zp<Workspace> workspace;
 Zp<MulliganMixer> mixer;
 MulliganAddSelector() : MoveableEditorWindow() {
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
  v.noClose = false;
  v.noClipTitle = true;
  v.background.Int(0, 48, 16, 255);
  Name("ADD MULLIGAN TO MIX");
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
  selected = fast.list("", &mulligansByName, x + 16, y + 16, w - 32 - 48, 16, &scrolled.value);
  if (selected != -1) {
   Mulligan *me = (Mulligan *)workspace->mulligans.Element(selected);
   if (me) {
    MulliganMixNode *node = new MulliganMixNode;
    Mulligan *m = me->Duplicate();
    node->mulligan.Set(m);
    node->mulligan.name = me->name + string(" (copy)");
    delete m;
    mixer->ed->nodes.Append(node);
    this->dropMe = true;
   }
  }
 }
};