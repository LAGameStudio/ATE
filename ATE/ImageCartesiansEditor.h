#pragma once
#include "EditGUI.h"
#include "AtlasTileset.h"

class ImageCartesiansEditor : public MoveableEditorWindow {
public:
 Zp<Cartesian> selected;
 Zp<Cartesians> cartesians;
 Zp<Zp<GLImage>> image;
 Cartesian window;
 Zp<AtlasTileset> ats;
 Zp<AtlasTile> at;
 Zp<Zbool> atlas2X,atlas4X;
 Zbool tileCenter;
 Strings options;
 Zdis<Cartesian> creation;
 Cartesiand mouseScreen;
 ImageCartesiansEditor( Zp<GLImage> *image, Cartesians *out ) : MoveableEditorWindow() {
  cartesians=out;
  this->image=image;
 }
 void OnSetup() {
  clip=false;
  v.alwaysOnTop=true;
  v.SizeAndPosition(window.x,window.y,1024+32,512+64);
  v.minimumSize.Set(1024+32,512+64);
  v.maximumSize.Set(1024+32,512+64);
  v.noMove=false;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noClose=false;
  v.noClipTitle=true;
  v.background.x_Black();
  v.Editing(cartesians);
  Name("Collider Settings");
//  fd.description="Choose a sound file to add to this sound effect.";
//  fd.path=workspace->path+string("sounds");
//  fd.background.x_DarkSlateBlue();
//  fd.bound=&filepicker.value;
  Repopulate();
 }
 void Repopulate() {
  options.Clear();
  if ( cartesians ) {
   int i=0;
   EACH(cartesians->first,Cartesian,c) { i++; options.Add(FORMAT("Box %d",i).c_str()); }
  }
 }
 void RenderWindow() {
  mouseScreen.Set(input.mxd*v.vdisplay.w,input.myd*v.vdisplay.h);
  Cartesiand previewExtents; previewExtents.SetRect(x+16,y+16,512,512);
  bool mouseWithinPreview=previewExtents.within(mouseScreen.x,mouseScreen.y);
  Cartesian pen(x+16,y+32);
  double px=x+256;
  double py=y+256;
  Area(crayons.Pick(black),&previewExtents);
  DrawGrid(&previewExtents,crayons.Pick(indigo),16,16);
  if ( at && ats && ats->atlas ) {
   Cartesiand aPos(&previewExtents);
   if ( atlas2X && atlas2X->value ) aPos.SetRect(aPos.x-128,aPos.y-256,256,256);
   else if ( atlas4X && atlas4X->value ) aPos.SetRect(aPos.x-256,aPos.y-512,512,512);
   else if ( tileCenter ) aPos.SetRect(aPos.x+aPos.w/2-64-16,aPos.y+aPos.h/2-64-16,128,128);
   else aPos.SetRect(aPos.x-64,aPos.y-128,128,128);
   at->Render(crayons.Pick(gray),ats->atlas,aPos.x,aPos.y,aPos.w,aPos.h);
  }
  if ( image && image.pointer->pointer ){
   art.Pivot(image.pointer->pointer,crayons.Pick(gray),transparency,px,py,0.0,0.0,
    (double)(image.pointer->pointer->height/2.0),
    (double)(image.pointer->pointer->width/2.0),
    0.0
   );
  }
  Rectangle(crayons.Pick(cyan),&previewExtents);
  int i=0;
  bool hovered=false;
  if ( cartesians ) EACHN(cartesians->first,Cartesian,c,{ i++;
   Cartesian drawRect;
   drawRect.SetRect(c->x,c->y,c->w,c->h);
   drawRect.MoveBy((int)px,(int)py);
   bool hovering=false;
   if ( WITHIN(mouseScreen.x,mouseScreen.y,drawRect.x,drawRect.y,drawRect.x2,drawRect.y2) ) {
    hovering=true;
    hovered=true;
   }
   Blending(additive);
   Rectangle(hovering?crayons.Pick(alabaster):crayons.jami(0,(i%8*15),127+((i+3)%8*15),127+((i+6)%8*15),255),&drawRect);
   if ( !creation ) {
    if ( input.left ) selected=c;
    else if ( selected && input.right ) {
     cartesians->Remove(selected);
     delete selected;
     selected=null;
    }
   }
  });
  Blending(none);
  if ( WITHIN(mouseScreen.x,mouseScreen.y,previewExtents.x,previewExtents.y,previewExtents.x2,previewExtents.y2) ) {
   DrawCross(crayons.Pick(orange),(int)mouseScreen.x,(int)mouseScreen.y,8);
   if ( creation ) {
    Cartesian endPoint((int)(mouseScreen.x-px),(int)(mouseScreen.y-py));
    Cartesian creating;
    creating.Corners(
     creation->x < endPoint.x ? creation->x : endPoint.x, 
     creation->y < endPoint.y ? creation->y : endPoint.y,
     creation->x > endPoint.x ? creation->x : endPoint.x, 
     creation->y > endPoint.y ? creation->y : endPoint.y
    );
    Cartesian creatingDisplayed;
    creatingDisplayed.SetRect((int)(px+creating.x),(int)(py+creating.y),creating.w,creating.h);
    Rectangle(crayons.Pick(green255),&creatingDisplayed);
    if ( input.leftReleased() ) {
     creation->Set(&creating);
     cartesians->Append(creation);
     creation.pointer=null;
    }
   } else {
    if ( input.left && !hovered && !creation ) {
     creation.Recycle();
     creation->Set((int)(mouseScreen.x-px),(int)(mouseScreen.y-py));
    }
   }
  }
  pen.x+=512+16;
  if ( fast.button(glowTermFont->glyph('+'),pen.x,pen.y,24,32) ) {
   Cartesian *c=new Cartesian;
   selected=c;
   c->SetRect(
    (int)(double)(-image.pointer->pointer->width/2.0),
    (int)(double)(-image.pointer->pointer->height/2.0),
    image.pointer->pointer->width,
    image.pointer->pointer->height
   );
   cartesians->Append(c);
   Repopulate();
  }
  if ( fast.button("+50%",pen.x+32,pen.y,48,32) ) {
   Cartesian *c=new Cartesian;
   selected=c;
   c->SetRect(
    (int)(double)(-image.pointer->pointer->width/4.0),
    (int)(double)(-image.pointer->pointer->height/4.0),
    image.pointer->pointer->width/2,
    image.pointer->pointer->height/2
   );
   cartesians->Append(c);
   Repopulate();
  }
  if ( fast.button("+75%",pen.x+32+48+8,pen.y,48,32) ) {
   Cartesian *c=new Cartesian;
   selected=c;
   c->SetRect(
    (int)(double)(-(image.pointer->pointer->width/2+image.pointer->pointer->width/4)/2.0),
    (int)(double)(-(image.pointer->pointer->height/2+image.pointer->pointer->height/4)/2.0),
    image.pointer->pointer->width/2+image.pointer->pointer->width/4,
    image.pointer->pointer->height/2+image.pointer->pointer->height/4
   );
   cartesians->Append(c);
   Repopulate();
  }
  pen.y+=48;
  
  if ( cartesians ) { int i=0; EACHN(cartesians->first,Cartesian,c,{ i++;
    if ( c == selected ) {
     glLineWidth(3.0);
     Blending(additive);
     DrawLine2Color(crayons.Pick(clear),crayons.Pick(gray),(int)(c->x+c->w/2+px),(int)(c->y+c->h/2+py),pen.x-6,pen.y+8);
     glLineWidth(1.0);
    }
    fast.xywh(c,pen.x,pen.y,-256,256,FORMAT("Box %d",i).c_str());
    if ( fast.button("<",x+w-32-48,pen.y+8,32,24) ) {
     selected=c;
    }
    if ( selected==c && fast.button("X",x+w-32,pen.y+8,24,24) ) {
     cartesians->Remove(c);
     if ( selected == c ) selected=null;
     delete c;
     return;
    }
    pen.y+=48;
   });
  }
 }
};

