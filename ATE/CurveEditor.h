#pragma once
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

#include "Cartesian.h"
#include "EditGUI.h"
#include "FastGUI.h"
#include "FileDialog.h"

#include "Workspace.h"
#include "FormUI.h"
#include "Curve2d.h"

extern CurveDefinitions copiedCurveDefinitions;

// Curves and Lines
C_ONE(CurveEditor,MoveableEditorWindow,{
})
 Zp<CurveDefinitions> ed;
 Zp<CurveDefinition> curve;
 Zp<Vertex> nurbVert;
 Zint curveScrolled;
 Zint curveSelected;
 Cartesian window,previewArea,curveList,controlsArea;
 Zint pointActive;
 Strings curveTypes,pointNames;
 Zdouble time;
 CurveEditor( CurveDefinitions *curves ) : MoveableEditorWindow() {
  ed=curves;
  window.SetRect(512,512,768,512+32);
  CurveDefinitions::DeployTypes(&curveTypes);
  curveTypes.Add("1",1);
  curveTypes.Add("2",2);
  curveTypes.Add("3",3);
  curveTypes.Add("4",4);
 }
 void OnSetup() {
  clip=false;
  v.alwaysOnTop=true;
  v.SizeAndPosition(window.x,window.y,window.w,window.h);
  v.minimumSize.Set(768,512+32);
  v.maximumSize.Set(768,512+32);
  v.noMove=false;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noClose=false;
  v.noClipTitle=true;
  v.background.x_Black();
  v.Editing(ed);
  Name("PATH EDITOR");
  Repopulate();
 }
 void Between() {
  time += FRAMETIME/5.0;
  if ( time > 1.0 ) time=0.0;
  previewArea.SetRect((int)x+4,(int)y+4,512,512);
  curveList.SetRect((int)x2-32-16,(int)y,32,512-32-24);
  controlsArea.SetRect((int)previewArea.x2+4,y,192,512-32-24);
 }
 void RenderWindow() {
  fast.member=&v;
  if ( v.closing || v.moving || v.wasMoving || v.holder->sliding ) return;
  Cartesian rect; rect.Set(&previewArea);
  FastExtents e(&previewArea);
  if ( fast.button("",e) ) {
   // editor
   if ( curve ) {
    if ( curve->UsesABCD() ) {
     switch ( pointActive.value ) {
      case 1:
        curve->Ax=fast.point.x;
        curve->Ay=fast.point.y;
        if ( curve->prev ) {
        }
        pointActive=2;
       break;
      case 2:
        curve->lines=1;
        curve->Bx=fast.point.x;
        curve->By=fast.point.y;
        pointActive=3;
       break;
      case 3:
        curve->lines=2;
        curve->Cx=fast.point.x;
        curve->Cy=fast.point.y;
        pointActive=4;
       break;
      case 4:
        curve->lines=3;
        curve->Dx=fast.point.x;
        curve->Dy=fast.point.y;
        if ( curve->next ) {
        }
        pointActive=(curve->prev?2:1);
       break;
     }
    } else {
     if ( curve->type == c2d_clamped2Bspline ) {
     } else if ( curve->type == c2d_Nurbs ) {
     }
    }
   }
  }
  {
   Area(crayons.Pick(black),&rect);
   Rectangle(crayons.jami(0,24,24,32,255),&rect);
   DrawGrid(&rect,crayons.jami(0,16,24,16,255),10,10);
  }
  if ( fast.hovering ) {
   Cartesiand cursor;
   cursor.Set(fast.point.x*(double)e.w,fast.point.y*(double)e.h);
   DrawLine(crayons.Pick(blue255),e.x,e.y+(int)cursor.y,e.x+e.w,e.y+(int)cursor.y);
   DrawLine(crayons.Pick(blue255),e.x+(int)cursor.x,e.y,e.x+(int)cursor.x,e.y+e.h);
   fast.text(FORMAT("%d,%d",(int)(fast.point.x*3180.0),(int)(fast.point.y*2160.0)).c_str(),x+(int)cursor.x+8,y+(int)cursor.y-32);
  }
  e.Set(&curveList);
//  Rectangle(crayons.Pick(blue255),&rect);
  Cartesiand oldFontSize(fast.fontSize.x,fast.fontSize.y);
  fast.fontSize.x=18.0;
  fast.fontSize.y=24.0;
  int selected=fast.list(ed,curveList.x,curveList.y,curveList.w,curveList.h/32,&curveSelected.value,&curveScrolled.value);
  if ( selected >= 0 ) {
   curveSelected=selected;
   curve=(CurveDefinition *) ed->Element(selected);
  }
  fast.fontSize.x=oldFontSize.x;
  fast.fontSize.y=oldFontSize.y;
  e.Set(&controlsArea);
  Crayon color_chestnut(chestnut);
  Rectangle(color_chestnut,&rect);
  color_chestnut.Multiply(0.5f);
  color_chestnut.Alpha(1.0f);
  Rectangle(color_chestnut,rect.x+rect.w/4,rect.y+rect.h/4,rect.x+rect.w/4+rect.w/2,rect.y+rect.h/4+rect.h/2);
  if ( fast.button("+",x2-32,y2-32,16,16) ) {
   CurveDefinition *cd=new CurveDefinition;
   curve=cd;
   curveSelected=ed->count;
   ed->Append(cd);
   if ( ed->count == 1 ) pointActive=1;
   else {
    CurveDefinition *previous=(CurveDefinition *) cd->prev;
    if ( previous ) {
     cd->Ax=previous->Dx;
     cd->Ay=previous->Dy;
     pointActive=2;
    } else pointActive=1;
   }
  }
  if ( fast.button("X",x2-16,y2-32,16,16) ) {
   if ( curve ) {
    ed->Remove(curve);
    curve.Delete();
    curveSelected=-1;
   }
  }
  RenderPreview();
 }
 void RenderPreview() {
  Crayon color_grey; color_grey.Int(72,72,72,255);
  Crayon color_cyan; color_cyan.Int(72,200,200,255);
  EACH(ed->first,CurveDefinition,d) { // draw preview
   if ( d == curve ) { // draw selected
    if ( curve->UsesABCD() ) {
     if ( pointActive ==1 ) {
      Cartesiand point((double)previewArea.x+(double)d->Ax*(double)previewArea.w,(double)previewArea.y+(double)d->Ay*(double)previewArea.h);
      Area(color_cyan,point.x-8,point.y-8,point.x+8,point.y+8);
      fast.text("1",(int)point.x,(int)point.y,true,crayons.Pick(black));
     } else {
      Cartesiand point((double)previewArea.x+(double)d->Ax*(double)previewArea.w,(double)previewArea.y+(double)d->Ay*(double)previewArea.h);
      color_cyan.gl();
      DrawCircle(point.x,point.y,8.0,4);
     }
     if ( pointActive ==2 ) {
      Cartesiand point((double)previewArea.x+(double)d->Bx*(double)previewArea.w,(double)previewArea.y+(double)d->By*(double)previewArea.h);
      Area(color_cyan,point.x-8,point.y-8,point.x+8,point.y+8);
      fast.text("2",(int)point.x,(int)point.y,true,crayons.Pick(black));
     } else {
      Cartesiand point((double)previewArea.x+(double)d->Bx*(double)previewArea.w,(double)previewArea.y+(double)d->By*(double)previewArea.h);
      color_cyan.gl();
      DrawCircle(point.x,point.y,8.0,4);
     }
     if ( pointActive ==3 ) {
      Cartesiand point((double)previewArea.x+(double)d->Cx*(double)previewArea.w,(double)previewArea.y+(double)d->Cy*(double)previewArea.h);
      Area(color_cyan,point.x-8,point.y-8,point.x+8,point.y+8);
      fast.text("3",(int)point.x,(int)point.y,true,crayons.Pick(black));
     } else {
      Cartesiand point((double)previewArea.x+(double)d->Cx*(double)previewArea.w,(double)previewArea.y+(double)d->Cy*(double)previewArea.h);
      color_cyan.gl();
      DrawCircle(point.x,point.y,8.0,4);
     }
     if ( pointActive ==4 ) {
      Cartesiand point((double)previewArea.x+(double)d->Dx*(double)previewArea.w,(double)previewArea.y+(double)d->Dy*(double)previewArea.h);
      Area(color_cyan,point.x-8,point.y-8,point.x+8,point.y+8);
      fast.text("4",(int)point.x,(int)point.y,true,crayons.Pick(black));
     } else {
      Cartesiand point((double)previewArea.x+(double)d->Dx*(double)previewArea.w,(double)previewArea.y+(double)d->Dy*(double)previewArea.h);
      color_cyan.gl();
      DrawCircle(point.x,point.y,8.0,4);
     }
    }
    //...
    Vertices *in=d->Compute();
    if ( in ) {
     in->Draw(color_cyan,
      (float)previewArea.x,
      (float)previewArea.y,
      (float)previewArea.w,
      (float)previewArea.h
     );
     delete in;
    }
   } else { // draw other
    Vertices *in=d->Compute();
    if ( in ) {
     color_grey.gl();
     in->Draw(color_grey,     
      (float)previewArea.x,
      (float)previewArea.y,
      (float)previewArea.w,
      (float)previewArea.h
     );
     delete in;
    }
   }
  }
  if ( curve ) {
   Vertices path;
   ed->Compute(&path);
   if ( path.count > 0 ) {
    Vertex *cstart=(Vertex *) path.first;
    crayons.Pick(green255).gl();
    DrawCircle(
     (double)previewArea.x+((double)cstart->x*(double)previewArea.w),
     (double)previewArea.y+((double)cstart->y*(double)previewArea.h),
     10.0, 3
    );
    Vertex *cend=(Vertex *) path.last;
    crayons.Pick(red255).gl();
    DrawCircle(
     (double)previewArea.x+((double)cend->x*(double)previewArea.w),
     (double)previewArea.y+((double)cend->y*(double)previewArea.h),
     10.0, 5
    );
    Lines lines;
    lines.Add(&path,false);
    lines.TallyLength();
    Vertexd vert;
    lines.GetPoint(&vert,time);
    Cartesiand point(
     (double)previewArea.x+((double)vert.x*(double)previewArea.w),
     (double)previewArea.y+((double)vert.y*(double)previewArea.h)
    );
    point.SetRect(point.x-1.0,point.y-1.0,3.0,3.0);
    Rectangle(crayons.Pick(red255),&point);
   }
   Cartesian pen;
   pen.Set(controlsArea.x,controlsArea.y);
   pen.MoveBy(16,16);
   curve->type=(Curve2dTypes)fast.enumerated(&curveTypes,"Curve Type",(int)curve->type,pen.x,pen.y,32,16);
   pen.MoveBy(-16,32+8);
   if ( curve->type == c2d_Lines ) {
    fast.text(FORMAT("Lines: %d",curve->lines.value).c_str(),pen.x,pen.y);
    pen.MoveBy(0,32);
   }
   pointActive=fast.integer("Active Point",pointActive.value,1,4,1,pen.x,pen.y,32,16);
   pen.MoveBy(0,32);
   if ( curve->UsesVertices() ) {
    fast.text(FORMAT("CV points: %d",curve->cv.count.value).c_str(),pen.x,pen.y);
    pen.MoveBy(0,32);
    fast.text(FORMAT("Knots: %d",(int)curve->knots.length).c_str(),pen.x,pen.y);
    pen.MoveBy(0,32);
   } else {
    curve->Ax=fast.decimal("AX",curve->Ax*1024.0,0.0,1024.0,1.0,pen.x,pen.y,32,16)/1024.0;
    curve->Ay=fast.decimal("AY",curve->Ay*1024.0,0.0,1024.0,1.0,pen.x+96,pen.y,32,16)/1024.0;
    pen.MoveBy(0,32);
    curve->Bx=fast.decimal("BX",curve->Bx*1024.0,0.0,1024.0,1.0,pen.x,pen.y,32,16)/1024.0;
    curve->By=fast.decimal("BY",curve->By*1024.0,0.0,1024.0,1.0,pen.x+96,pen.y,32,16)/1024.0;
    pen.MoveBy(0,32);
    curve->Cx=fast.decimal("CX",curve->Cx*1024.0,0.0,1024.0,1.0,pen.x,pen.y,32,16)/1024.0;
    curve->Cy=fast.decimal("CY",curve->Cy*1024.0,0.0,1024.0,1.0,pen.x+96,pen.y,32,16)/1024.0;
    pen.MoveBy(0,32);
    curve->Dx=fast.decimal("DX",curve->Dx*1024.0,0.0,1024.0,1.0,pen.x,pen.y,32,16)/1024.0;
    curve->Dy=fast.decimal("DY",curve->Dy*1024.0,0.0,1024.0,1.0,pen.x+96,pen.y,32,16)/1024.0;
    pen.MoveBy(0,32);
   }
  }
 }
MANY(CurveEditor,CurveEditorHandle,CurveEditorHandles,"CurveEditor",CurveEditors,{})
DONE(CurveEditor);


// Quads (isotrap)
C_ONE(QuadEditor,MoveableEditorWindow,{})
 Zp<Quad> ed;
 Cartesian window,previewArea,controlsArea;
 Zint pointActive;
 Zdouble time;
 Zp<GLImage> quadOrientationImage;
 QuadEditor( Quad *q ) : MoveableEditorWindow() {
  ed=q;
  quadOrientationImage=library.Load("data/images/icons/quad.png");
  window.SetRect(512,512,768,512+32);
 }
 void OnSetup() {
  clip=false;
  v.alwaysOnTop=true;
  v.SizeAndPosition(window.x,window.y,window.w,window.h);
  v.minimumSize.Set(768,512+32);
  v.maximumSize.Set(768,512+32);
  v.noMove=false;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noClose=false;
  v.noClipTitle=true;
  v.background.x_Black();
  v.Editing(ed);
  Name("QUAD EDITOR");
  Repopulate();
 }
 void Between() {
  time += FRAMETIME/5.0;
  if ( time > 1.0 ) time=0.0;
  previewArea.SetRect((int)x+4,(int)y+4,512,512);
  controlsArea.SetRect((int)previewArea.x2+4,y,256-32,512-32-24);
 }
 void RenderWindow() {
  if ( v.closing || v.moving || v.wasMoving || v.holder->sliding ) return;
  Cartesian rect; rect.Set(&previewArea);
  fast.member=&v;
  FastExtents e(&previewArea);
  fast.button("",e);
  if ( ed && fast.hovering ) {
   if ( fast.clicking ) {
    switch ( pointActive.value ) {
     default:
     case 1: ed->TL.x=fast.point.x; ed->TL.y=fast.point.y; break;
     case 2: ed->TR.x=fast.point.x; ed->TR.y=fast.point.y; break;
     case 3: ed->BR.x=fast.point.x; ed->BR.y=fast.point.y; break;
     case 4: ed->BL.x=fast.point.x; ed->BL.y=fast.point.y; break;
    }
   } else if ( fast.clicked ) {
    switch ( pointActive.value ) {
     default:
     case 1: ed->TL.x=fast.point.x; ed->TL.y=fast.point.y; pointActive=2; break;
     case 2: ed->TR.x=fast.point.x; ed->TR.y=fast.point.y; pointActive=3; break;
     case 3: ed->BR.x=fast.point.x; ed->BR.y=fast.point.y; pointActive=4; break;
     case 4: ed->BL.x=fast.point.x; ed->BL.y=fast.point.y; pointActive=1; break;
    }
   }
  }
  {
   Area(crayons.Pick(black),&rect);
   Rectangle(crayons.jami(0,24,24,32,255),&rect);
   DrawGrid(&rect,crayons.jami(0,16,24,16,255),10,10);
  }
  if ( fast.hovering ) {
   Cartesiand cursor;
   cursor.Set(fast.point.x*(double)e.w,fast.point.y*(double)e.h);
   DrawLine(crayons.Pick(blue255),e.x,e.y+(int)cursor.y,e.x+e.w,e.y+(int)cursor.y);
   DrawLine(crayons.Pick(blue255),e.x+(int)cursor.x,e.y,e.x+(int)cursor.x,e.y+e.h);
   fast.text(FORMAT("%d,%d",(int)(fast.point.x*3180.0),(int)(fast.point.y*2160.0)).c_str(),x+(int)cursor.x+8,y+(int)cursor.y-32);
  }
//  Rectangle(crayons.Pick(blue255),&rect);
  e.Set(&controlsArea);
  Crayon color_chestnut(chestnut);
  Rectangle(color_chestnut,&rect);
  color_chestnut.Multiply(0.5f);
  color_chestnut.Alpha(1.0f);
  Rectangle(color_chestnut,rect.x+rect.w/4,rect.y+rect.h/4,rect.x+rect.w/4+rect.w/2,rect.y+rect.h/4+rect.h/2);
  RenderPreview();
 }
 void RenderPreview() {
  Crayon color_grey; color_grey.Int(72,72,72,255);
  Crayon color_cyan; color_cyan.Int(72,200,200,255);
  SwabdYInvert(quadOrientationImage,crayons.Pick(alabaster),none,
   previewArea.x+ed->TL.x*512.0,
   previewArea.y+ed->TL.y*512.0,
   previewArea.x+ed->TR.x*512.0,
   previewArea.y+ed->TR.y*512.0,
   previewArea.x+ed->BR.x*512.0,
   previewArea.y+ed->BR.y*512.0,
   previewArea.x+ed->BL.x*512.0,
   previewArea.y+ed->BL.y*512.0
  );
  Cartesian pen;
  if ( pointActive ==1 ) {
   Cartesiand point(
    (double)previewArea.x+(double)ed->TL.x*(double)previewArea.w,
    (double)previewArea.y+(double)ed->TL.y*(double)previewArea.h
   );
   Area(color_cyan,point.x-8,point.y-8,point.x+8,point.y+8);
   fast.text("1",(int)point.x,(int)point.y,true,crayons.Pick(black));
  } else {
   Cartesiand point(
    (double)previewArea.x+(double)ed->TL.x*(double)previewArea.w,
    (double)previewArea.y+(double)ed->TL.y*(double)previewArea.h
   );
   color_cyan.gl();
   DrawCircle(point.x,point.y,8.0,4);
  }
  if ( pointActive ==2 ) {
   Cartesiand point(
    (double)previewArea.x+(double)ed->TR.x*(double)previewArea.w,
    (double)previewArea.y+(double)ed->TR.y*(double)previewArea.h
   );
   Area(color_cyan,point.x-8,point.y-8,point.x+8,point.y+8);
   fast.text("2",(int)point.x,(int)point.y,true,crayons.Pick(black));
  } else {
   Cartesiand point(
    (double)previewArea.x+(double)ed->TR.x*(double)previewArea.w,
    (double)previewArea.y+(double)ed->TR.y*(double)previewArea.h
   );
   color_cyan.gl();
   DrawCircle(point.x,point.y,8.0,4);
  }
  if ( pointActive ==3 ) {
   Cartesiand point(
    (double)previewArea.x+(double)ed->BR.x*(double)previewArea.w,
    (double)previewArea.y+(double)ed->BR.y*(double)previewArea.h
   );
   Area(color_cyan,point.x-8,point.y-8,point.x+8,point.y+8);
   fast.text("3",(int)point.x,(int)point.y,true,crayons.Pick(black));
  } else {
   Cartesiand point(
    (double)previewArea.x+(double)ed->BR.x*(double)previewArea.w,
    (double)previewArea.y+(double)ed->BR.y*(double)previewArea.h
   );
   color_cyan.gl();
   DrawCircle(point.x,point.y,8.0,4);
  }
  if ( pointActive ==4 ) {
   Cartesiand point(
    (double)previewArea.x+(double)ed->BL.x*(double)previewArea.w,
    (double)previewArea.y+(double)ed->BL.y*(double)previewArea.h
   );
   Area(color_cyan,point.x-8,point.y-8,point.x+8,point.y+8);
   fast.text("4",(int)point.x,(int)point.y,true,crayons.Pick(black));
  } else {
   Cartesiand point(
    (double)previewArea.x+(double)ed->BL.x*(double)previewArea.w,
    (double)previewArea.y+(double)ed->BL.y*(double)previewArea.h
   );
   color_cyan.gl();
   DrawCircle(point.x,point.y,8.0,4);
  }
  Vertices path;
  pen.Set(controlsArea.x+8,controlsArea.y+32);
  ed->TL.x=fast.decimal("TL x",ed->TL.x*1024.0,0.0,1024.0,1.0,pen.x,pen.y,32,16)/1024.0;
  ed->TL.y=fast.decimal("TL y",ed->TL.y*1024.0,0.0,1024.0,1.0,pen.x+96,pen.y,32,16)/1024.0;
  pen.MoveBy(0,32);
  ed->TR.x=fast.decimal("TR x",ed->TR.x*1024.0,0.0,1024.0,1.0,pen.x,pen.y,32,16)/1024.0;
  ed->TR.y=fast.decimal("TR y",ed->TR.y*1024.0,0.0,1024.0,1.0,pen.x+96,pen.y,32,16)/1024.0;
  pen.MoveBy(0,32);
  ed->BR.x=fast.decimal("BR x",ed->BR.x*1024.0,0.0,1024.0,1.0,pen.x,pen.y,32,16)/1024.0;
  ed->BR.y=fast.decimal("BR y",ed->BR.y*1024.0,0.0,1024.0,1.0,pen.x+96,pen.y,32,16)/1024.0;
  pen.MoveBy(0,32);
  ed->BL.x=fast.decimal("BL x",ed->BL.x*1024.0,0.0,1024.0,1.0,pen.x,pen.y,32,16)/1024.0;
  ed->BL.y=fast.decimal("BL y",ed->BL.y*1024.0,0.0,1024.0,1.0,pen.x+96,pen.y,32,16)/1024.0;
  pen.MoveBy(0,32);
  if ( fast.button(fast.rotateLeft,pen.x,pen.y,32,32) ) {
   Vertexd temp1,temp2,temp3,temp4;
   temp1.Set(&ed->TL);
   temp2.Set(&ed->TR);
   temp3.Set(&ed->BR);
   temp4.Set(&ed->BL);
   ed->TL.Set(&temp2);
   ed->TR.Set(&temp3);
   ed->BR.Set(&temp4);
   ed->BL.Set(&temp1);
  }
  if ( fast.button(fast.rotateRight,pen.x+32+8,pen.y,32,32) ) {
   Vertexd temp1,temp2,temp3,temp4;
   temp1.Set(&ed->TL);
   temp2.Set(&ed->TR);
   temp3.Set(&ed->BR);
   temp4.Set(&ed->BL);
   ed->TL.Set(&temp4);
   ed->TR.Set(&temp1);
   ed->BR.Set(&temp2);
   ed->BL.Set(&temp3);
  }
 }
MANY(QuadEditor,QuadEditorHandle,QuadEditorHandles,"QuadEditor",QuadEditors,{})
DONE(QuadEditor);