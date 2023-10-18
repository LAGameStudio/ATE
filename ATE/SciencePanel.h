#pragma once

#include "Cartesian.h"
#include "GLWindow.h"
#include "Chemicals.h"
#include "Player.h"

#include "fx_FireAndForget.h"
#include "fx_List.h"
#include "TexFont.h"

extern TexFont bytes;

 // Displays your collected chemical cargo, as well as exploration metrics such as stars/planets visited, and collected life forms.

class CollectedCompoundList : public fx_List<CompoundList,CompoundHandle> {
public:
 Crayon color, drawing, colorHi, drawingHi,hover,select;
 Zpointer<GLImage> m;
 CollectedCompoundList( int x, int y, int w, int h, int ItemHeight, int Margin ) : fx_List( x, y, w, h, ItemHeight, Margin ) {
  drawing.Pick(green);
  color.Pick(cyan);
  drawingHi.Pick(orange);
  select.Pick(red255);
  colorHi.Pick(blue);
  hover.Pick(blue);
  itemBorder.Pick(clear);
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  backingBlend=transparency;
  backingTint.Float(0.0f,0.0f,0.2f,1.0f);
  duration=60.0f;
  this->displaying=9;
 }
 void RenderItem( CompoundHandle *ch, Cartesian *extents, bool isSelected, bool isHovering ) {
  FORMAT(buf,128,"%1.2f",(float)ch->quantity);
  int len=strlen(buf);
  Text(&bytes,color,additive,extents->x+extents->w-24-len*(16+2)-32-4,extents->y+2,18,28,0,8,buf);
  glColor3d(0.0,1.0,1.0);
  Text("hectoTons",extents->x+extents->w-64,extents->y+16+18,5,7,false);
  glColor3d(0.0,1.0,0.0);
  Text(ch->compound->name,extents->x+16,extents->y+31,8,10,false);
 }
};

class CollectedElementList : public fx_List<UniversalElementList,UniversalElementHandle> {
public:
 Crayon color, drawing, colorHi, drawingHi,hover,select;
 Zpointer<GLImage> m;
 CollectedElementList( int x, int y, int w, int h, int ItemHeight, int Margin ) : fx_List( x, y, w, h, ItemHeight, Margin ) {
  drawing.Pick(green);
  color.Pick(cyan);
  drawingHi.Pick(orange);
  select.Pick(red255);
  colorHi.Pick(blue);
  hover.Pick(blue);
  itemBorder.Pick(clear);
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  backingBlend=transparency;
  backingTint.Float(0.0f,0.0f,0.2f,1.0f);
  duration=60.0f;
  this->displaying=9;
 }
 void RenderItem( UniversalElementHandle *ch, Cartesian *extents, bool isSelected, bool isHovering ) {
  FORMAT(buf,128,"%1.2f",(float)ch->quantity);
  int len=strlen(buf);
  Text(&bytes,color,additive,extents->x+extents->w-24-len*(16+2)-32-4,extents->y+2,18,28,0,8,buf);
  glColor3d(0.0,1.0,1.0);
  Text("hectoTons",extents->x+extents->w-64,extents->y+16+18,5,7,false);
  glColor3d(0.0,1.0,0.0);
  Text(ch->element->name,extents->x+16,extents->y+31,8,10,false);
 }
};

class SciencePanel : public Proce55Viewer2d {
public:
 Cartesian rect;
 Zpointer<GLImage> backing;
 Crayon backingTint;
 CollectedElementList *cel;
 CollectedCompoundList *ccl;
 Zdouble total;
 Zstring total_buf;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  visible=false;
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  backingTint.Pick(black);
  ccl=new CollectedCompoundList(32+32,64+32,256,256+64,34,0);
  Add(ccl);
  cel=new CollectedElementList(32+32+256+16,64+32,256,256+64,34,0);
  Add(cel);
  UpdateLists();
 }
 void UpdateLists() {
  ccl->SetList(&players.active->profile.active->chemicals.chemicals);
  cel->SetList(&players.active->profile.active->chemicals.elements);
  total=0.0;
  EACH(players.active->profile.active->chemicals.chemicals.first,CompoundHandle,h) total+=h->quantity;
  EACH(players.active->profile.active->chemicals.elements.first,UniversalElementHandle,h) total+=h->quantity;
  FORMAT(buf,1024,"Total Load: %1.1f hectoTons",(float)total);
  total_buf=string(buf);
 }
 void PreProcessing() {
  QuadiNoAlpha(backing,backingTint,transparency,32,64, 512+64, 256+128 );
  glColor3d(1.0,1.0,1.0);
  Text("Compounds",32+32+16,64+32-6,8,10,false);
  Text("Elements",32+32+256+16+16,64+32-6,8,10,false);
  Text(total_buf,32+256+64,64+256+128-16,8,10,false);
 }
};

