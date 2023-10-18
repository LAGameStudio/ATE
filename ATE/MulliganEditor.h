/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once
#include "EditGUI.h"
#include "FastGUI.h"
#include "FileDialog.h"

#include "Workspace.h"
#include "SoundEffectEditor.h"
#include "Mulligan.h"
#include "Art2d.h"
#include "MulliganMixer.h"

// Explosions
C_ONE(MulliganEditor,MoveableEditorWindow,{
  window.SetRect(512,512,1280,768);
 })
 Zp<GLImage> grid;
 Crayon gridColor;
 Zp<Mulligan> ed;
 Zp<PFX> pfx,pfxParent;
 Zp<Workspace> workspace;
 Cartesian window;
 Strings pfxTypes;
 Zstring mulliganImagePath;
 Zstring fImage,fImageA,fImageB;
 FileDialog fdImage,fdImageA,fdImageB;
 Zbool mute,editingTrails;
 Cartesiand velocity; // for testing
 MulliganEditor( Workspace *c, Mulligan *b ) : MoveableEditorWindow() {
  ed=b;
  workspace=c;
  window.SetRect(512,512,1280,768);
  pfxTypes.Add("Spin-Grow-Fade",(int)pfx_SpinningExpandingFading);
  pfxTypes.Add("Grow-Fade",(int)pfx_ExpandFader);
  pfxTypes.Add("Wander-Grow-Fade",(int)pfx_WanderingExpandFader);
  pfxTypes.Add("Grow-Shrink-Fade",(int)pfx_ExpandContractFader);
  pfxTypes.Add("Grow-Shrink",(int)pfx_LegacyExpandContract);
  pfxTypes.Add("Grow-Shrink-Burn",(int)pfx_ExpandContractBurn);
  pfxTypes.Add("Grow-Shrink-2",(int)pfx_ExpandContract2Images);
  pfxTypes.Add("Spin-Grow-Shrink-Fade",(int)pfx_SpinningExpandContractFader);
  pfxTypes.Add("Flicker",(int)pfx_Flicker);
  pfxTypes.Add("Flicker-2",(int)pfx_Flicker2Images);
  mulliganImagePath=c->path+string("particles");
  fdImage.description= string("Choose an image from ") + mulliganImagePath;
  fdImage.bound=&fImage.value;
  fdImage.path=workspace?(workspace->path+string("particles")):mulliganImagePath;
  fdImage.includedLocations.Add("data/images/flares");;
  fdImageA.description= string("Choose an image from ") + mulliganImagePath;
  fdImageA.bound=&fImageA.value;
  fdImageA.path=mulliganImagePath;
  fdImageA.includedLocations.Add("data/images/flares");
  fdImageB.description=string("Choose an image from ")+mulliganImagePath;
  fdImageB.bound=&fImageB.value;
  fdImageB.path=mulliganImagePath;
  fdImageB.includedLocations.Add("data/images/flares");
  grid = library.Load("data/images/elements/rounder1.png");
  gridColor.Int(32, 32, 32, 255);
 }
 MulliganEditor(Mulligan *b) : MoveableEditorWindow() {
  ed = b;
  workspace = activeWorkspace;
  window.SetRect(512, 512, 1280, 768);
  pfxTypes.Add("Spin-Grow-Fade", (int)pfx_SpinningExpandingFading);
  pfxTypes.Add("Grow-Fade", (int)pfx_ExpandFader);
  pfxTypes.Add("Wander-Grow-Fade", (int)pfx_WanderingExpandFader);
  pfxTypes.Add("Grow-Shrink-Fade", (int)pfx_ExpandContractFader);
  pfxTypes.Add("Grow-Shrink", (int)pfx_LegacyExpandContract);
  pfxTypes.Add("Grow-Shrink-Burn", (int)pfx_ExpandContractBurn);
  pfxTypes.Add("Grow-Shrink-2", (int)pfx_ExpandContract2Images);
  pfxTypes.Add("Spin-Grow-Shrink-Fade", (int)pfx_SpinningExpandContractFader);
  pfxTypes.Add("Flicker", (int)pfx_Flicker);
  pfxTypes.Add("Flicker-2", (int)pfx_Flicker2Images);
  mulliganImagePath = activeWorkspace->path + string("particles");
  fdImage.description = string("Choose an image from ") + mulliganImagePath;
  fdImage.bound = &fImage.value;
  fdImage.path = activeWorkspace ? (activeWorkspace->path + string("particles")) : mulliganImagePath;
  fdImage.includedLocations.Add("data/images/flares");
  fdImageA.description = string("Choose an image from ") + mulliganImagePath;
  fdImageA.bound = &fImageA.value;
  fdImageA.path = mulliganImagePath;
  fdImageA.includedLocations.Add("data/images/flares");
  fdImageB.description = string("Choose an image from ") + mulliganImagePath;
  fdImageB.bound = &fImageB.value;
  fdImageB.path = mulliganImagePath;
  fdImageB.includedLocations.Add("data/images/flares");
 }
 void OnSetup() {
  clip=false;
  v.alwaysOnTop=true;
  v.SizeAndPosition(window.x,window.y,window.w,window.h);
  v.minimumSize.Set(1280,768);
  v.maximumSize.Set(1280,768);
  v.noMove=false;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noClose=false;
  v.noClipTitle=true;
  v.background.Int(0,48,16,255);
  v.Editing(ed);
  Name(FMT("MULLIGAN: %s", ed ? ed->name.c_str() : "New Mulligan"));
//  fd.description="Choose a sound file to add to this sound effect.";
//  fd.path=workspace->path+string("sounds");
//  fd.background.x_DarkSlateBlue();
//  fd.bound=&filepicker.value;
  Repopulate();
 }
 void Between() {
 }
 Strings pfxFrames;
 Zint pfxSelected;
 Zint pfxScrolled;
 void Repopulate() {
  pfxFrames.Clear();
  if ( ed ) {
   int counter=0;
   EACH(ed->particleProgram.first,PFX,p) {
    counter++;
    pfxFrames.Add(FORMAT("%d",counter),counter-1);
   }
  }
  previewProce55ors.Clear();
  ed->Implement(&previewProce55ors,fbo512.w/2,fbo512.h/2);
 }
 Proce55ors previewProce55ors;
 void RenderWindow() {
  fast.member=&v;
  if ( v.closing ) return;
  Cartesian previewArea(x+256-6,y+4,512,512);
  fbo512.background.Pick(black);
  fbo512.Bind();
  art.PushViewport(fbo512.w,fbo512.h);
  Cartesian delta;
  if ( input.KeyDown(DX_LEFT) ) delta.x=-1;
  if ( input.KeyDown(DX_RIGHT) ) delta.x=1;
  if ( input.KeyDown(DX_UP) ) delta.y=-1;
  if ( input.KeyDown(DX_DOWN) ) delta.y=1;
  previewProce55ors.MoveBy(delta.x,delta.y);
  previewProce55ors.Between();
  previewProce55ors.DropDone();
  previewProce55ors.Render();
  art.PopViewport();
  fbo512.Unbind();
  fbo512.blend=none;
  fbo512.RenderUpsideDown(previewArea.x,previewArea.y,previewArea.x2,previewArea.y2);
  Rectangle(crayons.Pick(red255),&previewArea);
  Cartesian pen(x+8,y+8);
  int result=fast.list(termFont,"",&pfxFrames,pen.x,pen.y,32,25,&pfxSelected.value,&pfxScrolled.value,4,-6,false,4);
  if ( result >= 0 ) {
   PFX *m=(PFX *) ed->particleProgram.Element(result);
   if (m) {
    pfx=m;
    pfxParent=null;
   }
   Repopulate();
  }
  pen.y=y+768-64-24;
  std::string oldname = ed->name;
  ed->name = fast.textbox("NAME", ed->name, pen.x, pen.y - 32, 30);
  if ( ed->name.length != oldname.length() )  Name(FMT("MULLIGAN: %s", ed ? ed->name.c_str() : "New Mulligan"));
  if ( pfx && fast.button("Copy",pen.x,pen.y,32,16) ) {
   PFX *m=pfx->Duplicate();
   ed->particleProgram.Append(m);
   pfx=m;
   pfxParent=null;
   editingTrails=false;
   pfxSelected=ed->particleProgram.count-1;
   Repopulate();
  }
  pen.y+=24;
  if ( ed->particleProgram.count < 30 && fast.button("+ FX",pen.x,pen.y,32,16) ) {
   PFX *m=new PFX;
   ed->particleProgram.Append(m);
   pfx=m;
   pfxParent=null;
   editingTrails=false;
   pfxSelected=ed->particleProgram.count-1;
   Repopulate();
  }
  pen.y+=24;
  if ( pfx && fast.button(FORMAT("X Delete #%d",(pfxSelected.value+1)).c_str(),pen.x,pen.y,96,16) ) {
   ed->particleProgram.Remove(pfx);
   pfx.Delete();
   pfx.pointer=null;
   pfxParent=null;
   editingTrails=false;
   Repopulate();
  }
  if ( pfx ) {
   pen.Set(x+196,y+h-32);
   PFXTypes oldType = pfx->type;
   pfx->type=(PFXTypes)fast.enumerated(&pfxTypes,"",(int)pfx->type,pen.x-48,pen.y,48,24,10);
   if (oldType != pfx->type) {
    previewProce55ors.Clear();
    ed->Implement(&previewProce55ors, fbo512.w / 2, fbo512.h / 2);
   }
   pen.Set(x + 8 + 64, y + 8);
   pfx->tint.Redf((float)fast.hslider("",(double)pfx->tint.rf,0.0,1.0, pen.x,pen.y,128,16,false,true));
   pen.y+=24;
   pfx->tint.Greenf((float)fast.hslider("",(double)pfx->tint.gf,0.0,1.0, pen.x,pen.y,128,16, false, true));
   pen.y+=24;
   pfx->tint.Bluef((float)fast.hslider("",(double)pfx->tint.bf,0.0,1.0, pen.x,pen.y,128,16, false, true));
   pen.y+=24;
   pfx->tint.Alpha(1.0f);
   pfx->image=fast.imagePicker(&fdImage,pfx->image,pfx->tint,none,pen.x,pen.y,64,64);
   pen.y += 64+16+8;
   fast.image(grid, pen.x, pen.y, 64, 64, gridColor);
   fast.xyAngle(&velocity.x,&velocity.y,pen.x,pen.y,32,32);
   pen.y+=64+16+8;
   pfx->imageA=fast.imagePicker(&fdImageA,pfx->imageA,pfx->tint,none,pen.x,pen.y,64,64);
   pen.x+=64+16;
   pfx->imageB=fast.imagePicker(&fdImageB,pfx->imageB,pfx->tint,none,pen.x,pen.y,64,64);
   pen.x-=64+16;
   pen.y+=64+16+8;
   pfx->circularSpread=fast.toggle("Circular Spread",pfx->circularSpread,pen.x-32,pen.y,16,16);
   pen.y+=48;   
   editingTrails=fast.toggle("Trails","Head","",editingTrails,pen.x,pen.y,32,16);
   if ( editingTrails && !pfxParent ) {
    pfxParent=pfx;
    if ( !pfx->trailsEffect ) {
     pfx->trailsEffect.Instantiate();
    }
    pfx=pfx->trailsEffect;
   } else if ( !editingTrails && pfxParent ) {
    pfx=pfxParent;
    pfxParent=null;
   }
   pen.y+=64;
//   pen.y=y+8+32+24*3+(64+16+8)*3+64*3;
   int OldY=y+8+32+24*3+(64+16+8)*3+64*3;
//   pen.x-=32;
   if ( !editingTrails ) {
    pfx->fizzle=fast.toggle("Fizzle",pfx->fizzle,pen.x,pen.y,16,16);
    pen.y+=24;
    pfx->shoot=fast.toggle("Shoot",pfx->shoot,pen.x,pen.y,16,16);
    pen.y+=24;
    pfx->trails=fast.toggle("Trails",pfx->trails,pen.x,pen.y,16,16);
    pen.y+=24;
    pfx->splits=fast.toggle("Splits",pfx->splits,pen.x,pen.y,16,16);
    pen.y+=24;
   }
   pfx->nearby=fast.toggle("Nearby",pfx->nearby,pen.x,pen.y,16,16);
   pen.y+=64;
   pfx->nearbyRange=(float)fast.hslider("Nearby (Pixels)",pfx->nearbyRange,0.0,256.0, pen.x,pen.y,196,16,true);
   pen.Set(x + 256+64, y+512+64);
   pfx->ejectionSpeed=fast.hslider("Eject/coelesce speed",pfx->ejectionSpeed,0.0,16.0,pen.x+16,pen.y,96,16,true);
   pen.y+=32;
   pfx->coalesce=fast.toggle("Coalesce",pfx->coalesce,pen.x,pen.y,16,16);
   pen.y+=32;
   pfx->ejects=fast.toggle("Eject",pfx->ejects,pen.x,pen.y,16,16);
   pen.y+=32;
   pfx->calculateDirectionFromShotAngle=fast.toggle("Dir = Shot",pfx->calculateDirectionFromShotAngle,pen.x,pen.y,16,16);
   pen.Set(x + 512, y + 512 + 64);
   pfx->color=fast.colorPickerTiny(pfx->color,pen.x,pen.y,96,64);
   pen.y+=32;
   pfx->burn=fast.colorPickerTiny(pfx->burn,pen.x,pen.y,96,64);
   pen.y+=64;
   pfx->quantity=fast.hslider("quantity",pfx->quantity,1,16,pen.x,pen.y,148,16,true);
   pen.y+=48;
   pfx->burnSize=fast.hslider("burn size (pixels)",pfx->burnSize,0,32,pen.x,pen.y,148,16,true);
   pen.Set(x + w - 256, y + 32);
   pfx->emitDuration=fast.hslider("emit duration (s)",pfx->emitDuration,0.0,10.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->effectDuration=fast.hslider("fx duration (s)",pfx->effectDuration,0.0,10.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->width=fast.hslider("width",pfx->width,0.0,512.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->height=fast.hslider("height",pfx->height,0.0,512.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->splitAngle=fast.hslider("split angle",pfx->splitAngle,0.0,180.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->spin=fast.hslider("spin",pfx->spin,0.0,10.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->effectAngle=fast.hslider("effect angle",pfx->effectAngle,0.0,360.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->waiting=fast.hslider("waiting",pfx->waiting,0.0,10.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->interval=fast.hslider("interval",pfx->interval,0.1,10.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->widthTo=fast.hslider("to width",pfx->widthTo,0.0,512.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->heightTo=fast.hslider("to height",pfx->heightTo,0.0,512.0,pen.x,pen.y,148,16);
   pen.y+=48;
   pfx->trailDuration=fast.hslider("trail duration (s)",pfx->trailDuration,0.0,10.0,pen.x,pen.y,148,16);
   pen.y+=64;
   fast.hrange("in-out",&pfx->in.value,&pfx->out.value,0.0,1.0,pen.x+8,pen.y+4,150,16);
   pen.y+=64;
   fast.hrange("e-in s-out",&pfx->endIn.value,&pfx->startOut.value,0.0,pfx->effectDuration,pen.x+8,pen.y,150,16);
   pen.Set(x + 256 + 512, y + 512 + 64);
   pfx->deltaWidth = fast.hslider("delta width", pfx->deltaWidth, 0.0, 10.0, pen.x, pen.y, 150, 16);
   pen.y += 48;
   pfx->mina=fast.hslider("minimum alpha",pfx->mina,(1.0/255.0),1.0,pen.x,pen.y,100,16);
   pen.y+=48;
   pfx->emitSpreadAngleRange=fast.hslider("spread angle range",pfx->emitSpreadAngleRange,0.0,180.0,pen.x,pen.y,100,16);
   pen.y+=48;
   pfx->burnDelay=fast.hslider("burn delay (s)",pfx->burnDelay,0.0,10.0,pen.x,pen.y,100,16);
   pen.Set(x+256+512,y+8);
   pen.y+=24;
   pen.y+=24;
   pfx->blend=fast.blend("",pfx->blend,pen.x,pen.y,64,32);
   pen.y += 48;
   if (fast.button("CLEAR PREVIEW", pen.x, pen.y, 192, 32)) previewProce55ors.Clear();
   pen.y += 48;
   if (fast.button("CENTER, ONCE", pen.x, pen.y, 192, 32)) {
    previewProce55ors.Clear();
    ed->Implement(&previewProce55ors, fbo512.w / 2, fbo512.h / 2);
   }
   pen.y += 48;
   if (fast.button("MIX + EXPORT", pen.x, pen.y, 192, 32)) {
    previewProce55ors.Clear();
    MulliganMix *mixer = new MulliganMix;
    mixer->name = "New Mix ";
    mixer->name += FMT("%d", workspace->mulliganMixes.count.value);
    MulliganMixNode *node = new MulliganMixNode;
    Mulligan *m = ed->Duplicate();
    node->mulligan.Set(m);
    node->mulligan.name = ed->name + string(" (copy)");
    delete m;
    mixer->nodes.Append(node);
    workspace->mulliganMixes.Append(mixer);
    MulliganMixer *mm = new MulliganMixer(workspace, mixer);
    mm->mwm = this->mwm;
    this->mwm->Add(mm);
    v.holder->Arrange2Rows();
    v.holder->SlideTo(&mm->v);
   }
   pen.y += 48;
   pfx->xin = fast.tween(pfx->xin, pen.x, pen.y, 196, 32);
   pen.y += 48;
   pfx->xout = fast.tween(pfx->xout, pen.x, pen.y, 196, 32);
  }
  Cartesiand mouseScreen(input.mxd*fast.vdisplay.w,input.myd*fast.vdisplay.h);
  if ( previewArea.within((int)mouseScreen.x,(int)mouseScreen.y) ) {
   if ( input.leftReleased() ) {
    if ( ed ) {
     mouseScreen.x -= (double)previewArea.x;
     mouseScreen.y -= (double)previewArea.y;
     mouseScreen.x /= (double)previewArea.w;
     mouseScreen.y /= (double)previewArea.h;
     ed->Implement(&previewProce55ors,
      (int)(mouseScreen.x*(double)fbo512.w),
      (int)(mouseScreen.y*(double)fbo512.h),
      velocity.x,velocity.y,mute?0.0f:0.5f
     );
    }
   }
  }
  pen.Set(previewArea.x+8,previewArea.y2-64-32);
  // Associated Sound
  Text(digicaps,crayons.Pick(alabaster),additive,
   pen.x,pen.y-(int)fast.fontSize.y,(int)fast.fontSize.x,(int)fast.fontSize.y,0,0,
   "SOUND");
  if ( fast.squarebutton(fast.soundIcon,fast.rounded,pen.x,pen.y,32,32,fast.color,fast.excited,fast.deactivated) ) {
   VirtualWindowHandle *existing=v.holder->Editing(&ed->sound);
   if ( existing ) {
    v.holder->SlideTo(existing->p);
   } else {
    SoundEffectEditor *sfe=new SoundEffectEditor(workspace,&ed->sound);
    v.holder->Add(&sfe->v);
    sfe->window.SetRect( x+512+16,y,512,512 );
    sfe->Setup();
    sfe->Name((this->v.title+string(" Sound Effect")).c_str());
    sfe->hideAll=true;
    sfe->v.Attach(&v,0,0);
    v.holder->Arrange2Rows();
    v.holder->SlideTo(&sfe->v);
    v.holder->MoveStayInFrontToTop();
    return;
   }
  }
  Blending(none);
  crayons.Pick(green255).gl();
  Text(FORMAT("%d particles",previewProce55ors.count.value).c_str(),
   (double)(previewArea.x2-256),(double)(previewArea.y2-32),8.0,11.0,false
  );
  mute=fast.toggle("Mute",mute,previewArea.x+8,previewArea.y2-32,24,24);
 }
MANY(MulliganEditor,MulliganEditorHandle,MulliganEditorHandles,"MulliganEditor",MulliganEditors,{})
DONE(MulliganEditor);