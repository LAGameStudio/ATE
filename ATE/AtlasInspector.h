#pragma once
#include "FastGUI.h"
#include "Atlas.h"
#include "Workspace.h"

class AtlasInspector : public GLWindow {
public:
 Zp<Workspace> workspace;
 Zp<Atlas> atlas;
 Zp<GLWindow> back_to;
 void OnLoad() {
  Fullscreen();
  background.Int(32,16,24,255);
  fast.ignoreNextRelease=1.0;
 }
 void Render() {
  if ( !visible ) return;
  Cartesiand area(128.0,128.0,800.0,800.0);
  fast.DefaultState();
  fast.ResponsiveRatio(w,h);
  fast.SaveState();
  fast.tfont.Set(16,20);
  bool isEditingNumbers = fast.hoverarea(128+800+32,128,384,128);
  atlas->name=fast.textbox(termFont,additive,calibri,additive,"\nATLAS NAME",atlas->name,16+384+32,64,80,false,1,2,4,!isEditingNumbers);
  if ( atlas->texture ) {
//   Cartesiand colRow; colRow.Set(atlas->columns,atlas->rows);
   art.Stretch(atlas->texture,crayons.Pick(alabaster),transparency,area.x,area.y,area.w,area.h);
   DrawGrid(area.x,area.y,area.w,area.h,crayons.Pick(red255),atlas->rows,atlas->columns);
   fast.ljust(calibri, FORMAT("Image is %d by %d pixels tall, atlas is %d wide by %d cells",
    (int)atlas->texture->width,(int)atlas->texture->height,(int)atlas->columns,(int)atlas->rows).c_str(),
    (int)area.x,(int)area.y2+32
   );
   atlas->mirrored=fast.toggle("MIRRORED (4 way)",atlas->mirrored,(int)area.x2+32,(int)(area.y+area.w/2.0),24,24);
   atlas->nearest=fast.toggle("NEAREST NEIGHBOR (Retain Sharp Pixel Edges When Scaled)",atlas->nearest,(int)area.x2+32,(int)(area.y+area.w/2.0)+32,24,24);
  }
  atlas->height=fast.imagePicker("HEIGHT",atlas->height,(int)area.x2+32,(int)(area.y+area.w/2.0)+128,256,256,24,true);
  atlas->normal=fast.imagePicker("NORMAL",atlas->normal,(int)area.x2+32+256+16,(int)(area.y+area.w/2.0)+128,256,256,24,true);
  if ( fast.button(digicaps,"Go Back",w-192-16,h-64,192,48) ) {
   ReturnTo();
   return;
  }
//  if ( targetPath.length > 0 ) fast.ljust(termFont,(string("Outputting files to:\n")+targetPath).c_str(),16+384+16,128,-4);
  fast.RecallState();
 }
 void ReturnTo() {
  back_to->visible=true;
  this->destroyMe=true;
 }
};

