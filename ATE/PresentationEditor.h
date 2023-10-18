#pragma once

#include "EditGUI.h"
#include "FormUI.h"

#include "fx_Film.h"
#include "fx_UglyText.h"
#include "fx_MixedBag.h"
#include "fx_TexFont.h"
#include "fx_Glow.h"
#include "VisualAid.h"
#include "Slide.h"
#include "Presentation.h"
#include "fx_Tweakables.h"
#include "Mulligan.h"
#include "MulliganEditor.h"
#include "SoundEffectEditor.h"
#include "CurveEditor.h"

class fx_Mulligan : public Proce55or {
public:
 Cartesiand rect;
 Zp<Mulligan> mulligan;
 Zbool linear;  // instead of rectangular region
 Zbool circular;
 Delay interval; // How fast they are fired off.
 void Setup() {}
 void Render() {
 }
 void Render( GLWindow *surface ) {
 }
};

class fx_SoundEffect : public Proce55or {
public:
 void Between() {
 }
};

class fx_NextGenVisual : public Proce55or {
public:
 Zstring presentation;
 Zp<GLImage> image1,image2,image3,image4;
 Zp<Film> film; Zp<GLImage> filmFrame;
 Crayon tint1,tint2,tint3,tint4,filmTint;
 Blends blend1,blend2,blend3,blend4,filmBlend;
 Cartesiand A,B,pivot,mulliganArea;
 Zbool isotrap,mute,circular,edgeGenerationOnly,stopped,outward,useCurves,curves2x,useQuad,placeBehind;
 Vertices curves;
 Quad q,qNew;
 Cartesiand velocity;
 Zdouble startAngle,endAngle,diffAngle,angle,inTime,outTime;
 Zp<Tween> inTween,midTween,outTween;
 Mulligan mulligan;
 Delay mulliganDelay;
 SoundEffect sound;
 Zdouble volume;
 Zdouble filmTimeDilation,sonicAttenuation;
 Delay soundDelay;
 Zstring content;
 Cartesiand fontSize,fontSizeB;
 Zp<TexFont> font;
 Zint charSpace,lineSpace;
 ///
 Line TL,BR;
 Zint frame,frames;
 Cartesiand newExtents;
 Crayon t1,t2,t3,t4,ft;
 Zdouble inPerc,outPerc,midPerc;
 Zbool soundPlayedOnce;
 Lines curveLines;
 Zint recursionDepth; // so you don't end up with an infinite loop when creating a presi
 Zdis<Presentation> presi;
 Zdouble perc; // instant
 fx_NextGenVisual() : Proce55or() {
  sonicAttenuation=1.0;
  font=glowTermFont;
  fontSize.SetRect(0,0,40,65);
  fontSizeB.SetRect(0,0,40,65);
  charSpace=-4;
  lineSpace=2;
 }
 void Setup();
 void Between() {
  if ( !stopped ) {
   proce55ors->DropDone();
   if ( expired < delay+duration ) {
   expired+=FRAMETIME;
   } else expired=delay+duration;
   if ( !soundPlayedOnce ) {
    soundDelay.Between();
    if ( !soundDelay ) {
     soundPlayedOnce=true;
     switch ( sound.playback ) {
      case sfx_Singleton:
        sound.Impulse(sonicAttenuation);
       break;
      case sfx_Sporadic:
       break;
      case sfx_Impulse:
        sound.Impulse(sonicAttenuation);
       break;
      case sfx_OnLoopedUntilOff:
       break;
     }
    }
    if ( presi ) {
     presi->BetweenFrames();
     presi->Between();
    }
   }
   perc=(double)((expired)/duration);
   angle=startAngle+diffAngle*perc;
   int currentFrame=(int)abs((double)frame*filmTimeDilation);
   if ( film ) filmFrame=film->frames.element(currentFrame%film->frames.count);
   frame++;
   if ( useCurves && curveLines.count > 0 ) {
    angle+=180.0;
    newExtents.SetRect(0,0,ilerp((int)A.w,(int)B.w,(float)perc),ilerp((int)A.h,(int)B.h,(float)perc));
    Vertexd vert;
    curveLines.GetPoint(&vert,perc);
    if ( curves2x ) {
     vert.x*=(double)fbo1080p.w*2.0;
     vert.y*=(double)fbo1080p.h*2.0;
     vert.x-=(double)fbo1080p.w/2.0;
     vert.y-=(double)fbo1080p.h/2.0;
    } else {
     vert.x*=(double)fbo1080p.w;
     vert.y*=(double)fbo1080p.h;
    }
    newExtents.SetRect(vert.x,vert.y,newExtents.w,newExtents.h);
   } else {
    Vertexd tl,br;
    TL.Point(perc,&tl);
    BR.Point(perc,&br);
    newExtents.SetCorners(tl.x,tl.y,br.x,br.y);
   }
   SetExtents((int)newExtents.x,(int)newExtents.y,(int)newExtents.w,(int)newExtents.h);
   mulliganArea.Set(&newExtents);
   if ( curving ) {
    mulliganArea.MoveBy(-newExtents.w/2,-newExtents.h/2);
   } else {
    mulliganArea.MoveBy(-newExtents.w/2,-newExtents.h/2);
   }
   inPerc=UMIN((double)expired/inTime,1.0);
   midPerc=UMIN(((double)expired-inTime)/(outTime-inTime),1.0);
   outPerc=UMIN(((double)expired-outTime)/((double)duration-outTime),1.0);
   if ( image1 || presi || content.length > 0 ) {
    t1.fromCrayon(tint1);
    if ( blend1 == additive ) {
     if ( midPerc <= 0 ) t1.Multiply((float)(inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) t1.Multiply((float)(midTween?midTween->tweend(midPerc):midPerc));
     else t1.Multiply((float)(outTween?outTween->tweend(outPerc):outPerc));
    } else {
     if ( midPerc <= 0 ) t1.Alpha((inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) t1.Alpha((midTween?midTween->tweend(midPerc):midPerc));
     else t1.Alpha((outTween?outTween->tweend(outPerc):outPerc));
    }
   }
   if ( image2 ) {
    t2.fromCrayon(tint2);
    if ( blend2 == additive ) {
     if ( midPerc <= 0 ) t2.Multiply((float)(inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) t2.Multiply((float)(midTween?midTween->tweend(midPerc):midPerc));
     else t2.Multiply((float)(outTween?outTween->tweend(outPerc):outPerc));
    } else {
     if ( midPerc <= 0 ) t2.Alpha((inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) t2.Alpha((midTween?midTween->tweend(midPerc):midPerc));
     else t2.Alpha((outTween?outTween->tweend(outPerc):outPerc));
    }
   }
   if ( image3 ) {
    t3.fromCrayon(tint3);
    if ( blend3 == additive ) {
     if ( midPerc <= 0 ) t3.Multiply((float)(inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) t3.Multiply((float)(midTween?midTween->tweend(midPerc):midPerc));
     else t3.Multiply((float)(outTween?outTween->tweend(outPerc):outPerc));
    } else {
     if ( midPerc <= 0 ) t3.Alpha((inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) t3.Alpha((midTween?midTween->tweend(midPerc):midPerc));
     else t3.Alpha((outTween?outTween->tweend(outPerc):outPerc));
    }
   }
   if ( image4 ) {
    t4.fromCrayon(tint4);
    if ( blend4 == additive ) {
     if ( midPerc <= 0 ) t4.Multiply((float)(inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) t4.Multiply((float)(midTween?midTween->tweend(midPerc):midPerc));
     else t4.Multiply((float)(outTween?outTween->tweend(outPerc):outPerc));
    } else {
     if ( midPerc <= 0 ) t4.Alpha((inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) t4.Alpha((midTween?midTween->tweend(midPerc):midPerc));
     else t4.Alpha((outTween?outTween->tweend(outPerc):outPerc));
    }
   }
   if ( film ) {
    ft.fromCrayon(filmTint);
    if ( filmBlend == additive ) {
     if ( midPerc <= 0 ) ft.Multiply((float)(inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) ft.Multiply((float)(midTween?midTween->tweend(midPerc):midPerc));
     else ft.Multiply((float)(outTween?outTween->tweend(outPerc):outPerc));
    } else {
     if ( midPerc <= 0 ) ft.Alpha((inTween?inTween->tweend(inPerc):inPerc));
     else if ( outPerc <= 0 ) ft.Alpha((midTween?midTween->tweend(midPerc):midPerc));
     else ft.Alpha((outTween?outTween->tweend(outPerc):outPerc));
    }
   }
   mulliganDelay.Between();
   if ( useQuad ) {
    qNew.Set(&q);
    qNew.TL.Multiply(newExtents.x2,newExtents.y2,0.0); qNew.TL.Add(newExtents.x,newExtents.y,0.0);
    qNew.TR.Multiply(newExtents.x2,newExtents.y2,0.0); qNew.TR.Add(newExtents.x,newExtents.y,0.0);
    qNew.BR.Multiply(newExtents.x2,newExtents.y2,0.0); qNew.BR.Add(newExtents.x,newExtents.y,0.0);
    qNew.BL.Multiply(newExtents.x2,newExtents.y2,0.0); qNew.BL.Add(newExtents.x,newExtents.y,0.0); 
   }
  }
 }
 void Draw( GLWindow *surface ) { Render(); }
 void Render( GLWindow *surface ) { Render(); }
 Zbool curving;
 void Render() {
  if ( !hidden && expired < duration ) {
   if ( presi ) {
    presi->Render();
    fbo1080p3.blend=blend1;
    fbo1080p3.tint.fromCrayon(t1);
    if ( useQuad ) fbo1080p3.RenderQuad(&qNew);
    else {
     if ( isotrap ) {
      fbo1080p3.RenderRotatedIsotrap(newExtents.x,newExtents.y,A.h,A.w,B.w,angle);
     } else {
      curving=( useCurves && curveLines.count > 0 );
      if ( pivot.x != 0 || pivot.y != 0 ) {
       fbo1080p3.RenderPivotedRotatedRect(
        !curving ? (newExtents.x-newExtents.w/2.0) : (newExtents.x),
        !curving ? (newExtents.y-newExtents.h/2.0) : (newExtents.y),
        pivot.x,pivot.y,(newExtents.h/2.0),(newExtents.w/2.0),angle
       );
      } else {
       fbo1080p3.RenderRotatedRect(
        !curving ? (newExtents.x-newExtents.w/2.0) : (newExtents.x),
        !curving ? (newExtents.y-newExtents.h/2.0) : (newExtents.y),
        (newExtents.h/2.0),(newExtents.w/2.0),angle
       );
      }
     }
    }
   }
   if ( useQuad ) {
    if ( image1 ) { ImageColorBlend(image1,t1,blend1); qNew.RenderYInvert(); glDisable(GL_TEXTURE_2D); }
    if ( image2 ) { ImageColorBlend(image2,t2,blend2); qNew.RenderYInvert(); glDisable(GL_TEXTURE_2D); }
    if ( image3 ) { ImageColorBlend(image3,t3,blend3); qNew.RenderYInvert(); glDisable(GL_TEXTURE_2D); }
    if ( image4 ) { ImageColorBlend(image4,t4,blend4); qNew.RenderYInvert(); glDisable(GL_TEXTURE_2D); }
    if ( filmFrame ) { ImageColorBlend(filmFrame,ft,filmBlend); qNew.RenderYInvert(); glDisable(GL_TEXTURE_2D); }
   } else if ( isotrap ) {
    if ( image1 ) RotatedIsotrapf(image1,t1,blend1,(float)newExtents.x,(float)newExtents.y,(float)A.h,(float)A.w,(float)B.w,(float)angle);
    if ( image2 ) RotatedIsotrapf(image2,t2,blend2,(float)newExtents.x,(float)newExtents.y,(float)A.h,(float)A.w,(float)B.w,(float)angle);
    if ( image3 ) RotatedIsotrapf(image3,t3,blend3,(float)newExtents.x,(float)newExtents.y,(float)A.h,(float)A.w,(float)B.w,(float)angle);
    if ( image4 ) RotatedIsotrapf(image4,t4,blend4,(float)newExtents.x,(float)newExtents.y,(float)A.h,(float)A.w,(float)B.w,(float)angle);
    if ( filmFrame ) RotatedIsotrapf(filmFrame,ft,filmBlend,(float)newExtents.x,(float)newExtents.y,(float)A.h,(float)A.w,(float)B.w,(float)angle);
   } else {
    curving=( useCurves && curveLines.count > 0 );
    ///
     if ( image1 ) art.Pivot(image1,t1,blend1,
      !curving ? (newExtents.x-newExtents.w/2.0) : (newExtents.x),
      !curving ? (newExtents.y-newExtents.h/2.0) : (newExtents.y),
      pivot.x,pivot.y,(newExtents.h/2.0),(newExtents.w/2.0),angle-180.0
     );
     if ( image2 ) art.Pivot(image2,t2,blend2,
      !curving ? (newExtents.x-newExtents.w/2.0) : (newExtents.x),
      !curving ? (newExtents.y-newExtents.h/2.0) : (newExtents.y),
      pivot.x,pivot.y,(newExtents.h/2.0),(newExtents.w/2.0),angle-180.0
     );
     if ( image3 ) art.Pivot(image3,t3,blend3,
      !curving ? (newExtents.x-newExtents.w/2.0) : (newExtents.x),
      !curving ? (newExtents.y-newExtents.h/2.0) : (newExtents.y),
      pivot.x,pivot.y,(newExtents.h/2.0),(newExtents.w/2.0),angle-180.0
     );
     if ( image4 ) art.Pivot(image4,t4,blend4,
      !curving ? (newExtents.x-newExtents.w/2.0) : (newExtents.x),
      !curving ? (newExtents.y-newExtents.h/2.0) : (newExtents.y),
      pivot.x,pivot.y,(newExtents.h/2.0),(newExtents.w/2.0),angle-180.0
     );
     if ( filmFrame ) art.Pivot(filmFrame,ft,filmBlend,
      !curving ? (newExtents.x-newExtents.w/2.0) : (newExtents.x),
      !curving ? (newExtents.y-newExtents.h/2.0) : (newExtents.y),
      pivot.x,pivot.y,(newExtents.h/2.0),(newExtents.w/2.0),angle-180.0
     );
   }
   if ( showingDebug ) {
    glLineWidth(3.0);
    Rectangle(crayons.Pick(orange),
     (newExtents.x+(curving?-newExtents.w/2.0:-newExtents.w)),
     (newExtents.y+(curving?-newExtents.h/2.0:-newExtents.h)),
     (newExtents.x+(curving?-newExtents.w/2.0:-newExtents.w))+newExtents.w,
     (newExtents.y+(curving?-newExtents.h/2.0:-newExtents.h))+newExtents.h
    );
    DrawCross(crayons.Pick(green255),(int)(newExtents.x),(int)(newExtents.y),32);
    DrawCross(crayons.Pick(schoolBusYellow),
     (int)(newExtents.x+(curving?0.0:(-newExtents.w))),
     (int)(newExtents.y+(curving?0.0:(-newExtents.h))),
     32
    );
    crayons.Pick(red255).gl();
    DrawCircle(
     (int)(newExtents.x+(curving?0.0:(-newExtents.w/2.0))),
     (int)(newExtents.y+(curving?0.0:(-newExtents.h/2.0))),
     (int)(newExtents.w/2.0),24
    );
    if ( curving ) {
     DrawCross(crayons.Pick(magenta),(int)(newExtents.x+pivot.x),(int)(newExtents.y+pivot.y),32);
    }
    if ( !curving ) {
     glLineWidth(2.0);
     DrawLine(crayons.Pick(brown2),TL.x,TL.y,TL.x2,TL.y2);
     DrawLine(crayons.Pick(brown2),BR.x,BR.y,BR.x2,BR.y2);
    } 
    glLineWidth(1.0);
   }
   if ( !mulliganDelay ) {
    mulliganDelay.Reset();
    int mulx=0;
    int muly=0;
    if ( circular ) {
     if ( edgeGenerationOnly ) {
      Circle c(mulliganArea.x+(curving?(mulliganArea.w/2):(0)),mulliganArea.y+(curving?(mulliganArea.h/2):(0)),mulliganArea.w/2);
      Vertexd vert;
      c.RandomPointOn(&vert);
      mulx=(int)vert.x;
      muly=(int)vert.y;
     } else {
      Circle c(mulliganArea.x+(curving?(mulliganArea.w/2):(0)),mulliganArea.y+(curving?(mulliganArea.h/2):(0)),mulliganArea.w/2);
      Vertexd vert;
      c.RandomPoint(&vert);
      mulx=(int)vert.x;
      muly=(int)vert.y;
     }
    } else {
     if ( edgeGenerationOnly ) {
      Lines lines;
      lines.Add(mulliganArea.x,mulliganArea.y,mulliganArea.x2,mulliganArea.y);
      lines.Add(mulliganArea.x,mulliganArea.y,mulliganArea.x,mulliganArea.y2);
      lines.Add(mulliganArea.x,mulliganArea.y2,mulliganArea.x2,mulliganArea.y2);
      lines.Add(mulliganArea.x2,mulliganArea.y,mulliganArea.x2,mulliganArea.y2);
      lines.RandomizeOrder();
      EACH(lines.first,Line,L) L->MoveBy(curving?(0):mulliganArea.w/2,curving?(0):mulliganArea.h/2);
      Line *f=(Line *) lines.first;
      Vertexd vert;
      f->Point(uniform(),&vert);
      mulx=(int)vert.x;
      muly=(int)vert.y;
     } else {
      mulx=number_range((int)mulliganArea.x,(int)mulliganArea.x2)+(curving?0:((int)mulliganArea.w/2));
      muly=number_range((int)mulliganArea.y,(int)mulliganArea.y2)+(curving?0:((int)mulliganArea.h/2));
     }
    }
    double velx=velocity.x;
    double vely=velocity.y;
    if ( outward ) {
     Line line;
     circular
      ? line.Set(mulliganArea.x,mulliganArea.y,(double)mulx,(double)muly)
      : line.Set(mulliganArea.x+mulliganArea.w/2.0,mulliganArea.y+mulliganArea.h/2.0,(double)mulx,(double)muly);
     velocity.Rotate(rad2deg(180.0+line.LineAngle()));
    }
    mulligan.Implement(proce55ors,mulx,muly,velx,vely,1.0,1.0,mute?0.0f:(float)volume,placeBehind?this:null);
   }
   if ( content.length > 0 && font ) {
    Cartesiand flerp,fsA,fsB;
    flerp.Lerp2d(&fontSize,&fontSizeB,perc);
    fast.text(font,content.c_str(),(int)newExtents.x,(int)newExtents.y,(int)flerp.w,(int)flerp.h,charSpace,lineSpace,t1,blend1);
   }
  }
 }
};

extern Cartesiand copiedCoordinate;
extern Line copiedLine;

enum AnimationElementTypes {
 aet_Film=0,
 aet_FilmIsotrapf=1,
 aet_FilmLineFader=2,
 aet_FilmLoop=3,
 aet_FilmLoopFader=4,
 aet_MultiFilm=5,
 aet_Glow=6,
 aet_Glow2=7,
 aet_Glow3=8,
 aet_Image=9,
 aet_Box=10,
 aet_Circle=11,
 aet_FadeIn=12,
 aet_FadeOut=13,
 aet_MovingLine=14,
 aet_MatrixText=15,
 aet_Text=16,
 aet_WriteText=17,
 aet_TexFont=18,
 aet_TexFontFader=19,
 aet_Presentation=20,
 aet_XFade=21,
 aet_Aid=22,
 aet_DriftingRotatingVisualAid=23,
 aet_DriftingStretchingRotatingVisualAid=24,
 aet_DriftingStretchingVisualAid=25,
 aet_DriftingVisualAid=26,
 aet_Visual=27,
 aet_VisualAid=28,
 aet_VisualAid2=29,
 aet_VisualAidAdditive=30,
 aet_VisualRotated=31,
 aet_VisualSkewed=32,
 aet_WanderingVisualAid=33,
 aet_WobblingVisualAid=34,
 aet_Mulligan=35,
 aet_NextGenVisual=36,
 aet_SoundEffect=37
};

ONE(AnimationElement,{
 type=aet_NextGenVisual;
 inTween=tweens.find("Sine 0-1");
 outTween=tweens.find("Sine 1-0");
 midTween=tweens.one;
 maxOp=255.0f;
 centered=true;
 lines=1;
 fontSize.SetRect(8.5,11.0,45.0,60.0);
 fontSizeB.SetRect(8.5,11.0,45.0,60.0);
 blend=additive;
 blend2=additive;
 blend3=additive;
 blend4=additive;
 blend5=additive;
 segments=12;
 maxChars=128;
 lineSpace=2;
 style=uglyWire;
 tint.Pick(alabaster);
 tint2.Pick(alabaster);
 tint3.Pick(alabaster);
 tint4.Pick(alabaster);
 opacityMid=255;
 tintIn.Pick(alabaster);
 angle=0.0;
 spin=0.0;
 deltaDuration=1.0;
 inTime=1.0;
 outTime=2.0;
 duration=3.0;
})
 AnimationElementTypes type;
 Zp<Film> film;
 Zp<GLImage> image,image2,image3,image4;
 Crayon tint,tint2,tint3,tint4,tintOut,tintIn,tintStart,tintEnd,interior;
 Blends blend,blend2,blend3,blend4,blend5;
 Zdouble duration,inTime,outTime,delay,deltaDelay,deltaDuration;
 Zbool xreversed,erratic_flaresource,tweenW,tweenH;
 Zdouble tweenLR,tweenUD,maxOp;
 Cartesiand extents,delta,B,C,D; // dx,dy,w2,h2
 Zp<Tween> inTween,outTween,midTween;
 ScreenPosition start,end;
 Zint segments;
 Zfloat thickness,pulseSpeed;
 Zstring presentationName,slideName;
 Zp<TexFont> font;
 Zstring content;
 Cartesiand fontSize,fontSizeB;
 Zint maxChars,lineSpace,lines,a,b,c,d;
 Quad q;
 Zbool useRelativeGlyphWidths,centered,useQuad;
 Zfloat wanderSpeed,wanderSpeedMinimum,wanderSpeedLimit,wanderAccel,tinting,rate,angle,spin;
 Cartesian wrapLimit;
 PointController2d location;
 AngleController rotation;
 SizeController2d size;
 Zint opacityStart,opacityMid,opacityEnd;
 Zbool flickerOut,perpetuate,repeats,placeBehind;
 Cartesian pivot;
 Cartesian
  sA,sB,sC,sD,
  pA,pB,pC,pD,
  dA,dB,dC,dD;
 UglyDrawStyle style;
 Zbool filled;
 Mulligan mulligan;
 SoundEffect sound;
 Zdouble interval;
 Zbool fireOnce;
 Zbool rectangular,circular;
 CurveDefinitions curves;
 CLONE(AnimationElement,{
  DUPE(type);
  DUPE(film);
  DUPE(image);
  DUPE(image2);
  DUPE(image3);
  DUPE(image4);
  a->tint.fromCrayon(tint);
  a->tint2.fromCrayon(tint2);
  a->tint3.fromCrayon(tint3);
  a->tint4.fromCrayon(tint4);
  a->tintOut.fromCrayon(tintOut);
  a->tintIn.fromCrayon(tintIn);
  a->tintStart.fromCrayon(tintStart);
  a->tintEnd.fromCrayon(tintEnd);
  a->interior.fromCrayon(interior);
  DUPE(blend);
  DUPE(blend2);
  DUPE(blend3);
  DUPE(blend4);
  DUPE(blend5);
  DUPE(duration);
  DUPE(inTime);
  DUPE(outTime);
  DUPE(delay);
  DUPE(deltaDelay);
  DUPE(deltaDuration);
  DUPE(xreversed);
  DUPE(erratic_flaresource);
  DUPE(tweenW);
  DUPE(tweenH);
  DUPE(tweenLR);
  DUPE(tweenUD);
  DUPE(maxOp);
  a->extents.Set(&extents);
  a->delta.Set(&delta);
  a->B.Set(&B);
  a->C.Set(&C);
  a->D.Set(&D);
  DUPE(inTween);
  DUPE(outTween);
  DUPE(midTween);
  a->start.Set(&start);
  a->end.Set(&end);
  DUPE(segments);
  DUPE(thickness);
  DUPE(pulseSpeed);
  DUPE(presentationName);
  DUPE(name);
  DUPE(slideName);
  DUPE(font);
  DUPE(content);
  a->fontSize.Set(&fontSize);
  DUPE(maxChars);
  DUPE(lineSpace);
  DUPE(lines);
  a->a=this->a;
  DUPE(b);
  DUPE(c);
  DUPE(d);
  a->q.Set(&q);
  DUPE(useRelativeGlyphWidths);
  DUPE(centered);
  DUPE(useQuad);
  DUPE(wanderSpeed);
  DUPE(wanderSpeedLimit);
  DUPE(wanderSpeedMinimum);
  DUPE(wanderAccel);
  DUPE(tinting);
  DUPE(rate);
  DUPE(angle);
  DUPE(spin);
  a->wrapLimit.Set(&wrapLimit);
  a->location.Set(&location);
  a->rotation.Set(&rotation);
  a->size.Set(&size);
  DUPE(opacityStart);
  DUPE(opacityMid);
  DUPE(opacityEnd);
  DUPE(flickerOut);
  DUPE(perpetuate);
  DUPE(repeats);
  DUPE(placeBehind);
  a->pivot.Set(&pivot);
  a->sA.Set(&sA);
  a->sB.Set(&sB);
  a->sC.Set(&sC);
  a->sD.Set(&sD);
  a->pA.Set(&pA);
  a->pB.Set(&pB);
  a->pC.Set(&pC);
  a->pD.Set(&pD);
  a->dA.Set(&dA);
  a->dB.Set(&dB);
  a->dC.Set(&dC);
  a->dD.Set(&dD);
  DUPE(style);
  DUPE(filled);
  a->mulligan.Set(&mulligan);
  a->sound.Set(&sound);
  DUPE(interval);
  DUPE(fireOnce);
  DUPE(rectangular);
  DUPE(circular);
  a->curves.Duplicate(&curves);
 })
 KEYWORDS({
  TXTWORD("name",name)
  else KEYWORD("type",{type=(AnimationElementTypes)atoi(w);})
  else FILMWORD("film",film)
  else IMGWORD("im",image)
  else IMGWORD("im2",image2)
  else IMGWORD("im3",image3)
  else IMGWORD("im4",image4)
  else TINTWORD("t",tint)
  else TINTWORD("t2",tint2)
  else TINTWORD("t3",tint3)
  else TINTWORD("t4",tint4)
  else TINTWORD("tIn",tintIn)
  else TINTWORD("tOut",tintOut)
  else TINTWORD("tStart",tintStart)
  else TINTWORD("tEnd",tintEnd)
  else TINTWORD("interior",interior)
  else KEYWORD("bl",{blend=w;})
  else KEYWORD("bl2",{blend2=w;})
  else KEYWORD("bl3",{blend3=w;})
  else KEYWORD("bl4",{blend4=w;})
  else KEYWORD("b5",{blend5=w;})
  else KEYWORD("duration",{duration=w;})
  else KEYWORD("inTime",{inTime=w;})
  else KEYWORD("outTime",{outTime=w;})
  else KEYWORD("delay",{delay=w;})
  else KEYWORD("dDel",{deltaDelay=w;})
  else KEYWORD("dDur",{deltaDuration=w;})
  else KEYWORD("twLR",{tweenLR=w;})
  else KEYWORD("twUD",{tweenUD=w;})
  else KEYWORD("maxOp",{maxOp=w;})
  else TAGWORD("xrev",xreversed)
  else TAGWORD("erra",erratic_flaresource)
  else TAGWORD("twW",tweenW)
  else TAGWORD("twH",tweenH)
  else SUBWORD("extents",extents)
  else SUBWORD("delt",delta)
  else SUBWORD("B",B)
  else SUBWORD("C",C)
  else SUBWORD("D",D)
  else KEYWORD("inTw",{inTween=tweens.find(w);})
  else KEYWORD("outTw",{outTween=tweens.find(w);})
  else KEYWORD("midTw",{midTween=tweens.find(w);})
  else SUBWORD("start",start)
  else SUBWORD("end",end)
  else KEYWORD("segs",{segments=w;})
  else KEYWORD("thick",{thickness=w;})
  else KEYWORD("pulse",{pulseSpeed=w;})
  else TXTWORD("presi",presentationName)
  else TXTWORD("to",slideName)
  else KEYWORD("font",{font=texfonts.find(w);})
  else TXTWORD("content",content)
  else SUBWORD("fontSize",fontSize)
  else SUBWORD("fontSizeB",fontSizeB)
  else KEYWORD("maxchars",{maxChars=w;})
  else KEYWORD("ls",{lineSpace=w;})
  else KEYWORD("lines",{lines=w;})
  else KEYWORD("aa",{a=w;})
  else KEYWORD("bb",{b=w;})
  else KEYWORD("cc",{c=w;})
  else KEYWORD("dd",{d=w;})
  else SUBWORD("q",q)
  else TAGWORD("use",useRelativeGlyphWidths)
  else TAGWORD("centered",centered)
  else TAGWORD("useQ",useQuad)
  else TAGWORD("fireOnce",fireOnce)
  else TAGWORD("rect",rectangular)
  else TAGWORD("circ",circular)
  else TAGWORD("flick",flickerOut)
  else TAGWORD("perp",perpetuate)
  else TAGWORD("rep",repeats)
  else TAGWORD("behind",placeBehind)
  else TAGWORD("fill",filled)
  else KEYWORD("wanSpd",{wanderSpeed=w;})
  else KEYWORD("wanSpdMi",{wanderSpeedMinimum=w;})
  else KEYWORD("wanSpdMa",{wanderSpeedLimit=w;})
  else KEYWORD("wanAcc",{wanderAccel=w;})
  else KEYWORD("ting",{tinting=w;})
  else KEYWORD("ra",{rate=w;})
  else KEYWORD("ang",{angle=w;})
  else KEYWORD("spin",{spin=w;})
  else KEYWORD("opS",{opacityStart=w;})
  else KEYWORD("opMid",{opacityMid=w;})
  else KEYWORD("opE",{opacityEnd=w;})
  else SUBWORD("wrap",wrapLimit)
  else SUBWORD("loc",location)
  else SUBWORD("rot",rotation)
  else SUBWORD("sz",size)
  else SUBWORD("piv",pivot)
  else SUBWORD("sa",sA)
  else SUBWORD("sb",sB)
  else SUBWORD("sc",sC)
  else SUBWORD("sd",sD)
  else SUBWORD("pa",pA)
  else SUBWORD("pb",pB)
  else SUBWORD("pc",pC)
  else SUBWORD("pd",pD)
  else SUBWORD("da",dA)
  else SUBWORD("db",dB)
  else SUBWORD("dc",dC)
  else SUBWORD("dd",dD)
  else SUBWORD("mul",mulligan)
  else SUBWORD("snd",sound)
  else SUBWORD("cur",curves)
  else KEYWORD("cyc",{interval=w;})
  else KEYWORD("ug",{style=(UglyDrawStyle)atoi(w);})
  else BADKEY("AnimationElement")
 })
 WORDKEYS({
  TXTKEY("name",name)
  WORDKEY("type",I2S((int)type))
  FILMKEY("film",film)
  IMGKEY("im",image)
  IMGKEY("im2",image2)
  IMGKEY("im3",image3)
  IMGKEY("im4",image4)
  TINTKEY("t",tint)
  TINTKEY("t2",tint2)
  TINTKEY("t3",tint3)
  TINTKEY("t4",tint4)
  TINTKEY("tIn",tintIn)
  TINTKEY("tOut",tintOut)
  TINTKEY("tStart",tintStart)
  TINTKEY("tEnd",tintEnd)
  TINTKEY("interior",interior)
  BLENDKEY("bl",blend)
  BLENDKEY("bl2",blend2)
  BLENDKEY("bl3",blend3)
  BLENDKEY("bl4",blend4)
  BLENDKEY("b5",blend5)
  WORDKEY("duration",duration)
  WORDKEY("inTime",inTime)
  WORDKEY("outTime",outTime)
  WORDKEY("delay",delay)
  WORDKEY("dDel",deltaDelay)
  WORDKEY("dDur",deltaDuration)
  WORDKEY("twLR",tweenLR)
  WORDKEY("twUD",tweenUD)
  WORDKEY("maxOp",maxOp)
  TAGKEY("xrev",xreversed)
  TAGKEY("erra",erratic_flaresource)
  TAGKEY("twW",tweenW)
  TAGKEY("twH",tweenH)
  SSUBKEY("extents",extents)
  SSUBKEY("delt",delta)
  SSUBKEY("B",B)
  SSUBKEY("C",C)
  SSUBKEY("D",D)
  if ( inTween )  { TWEENKEY("inTw",inTween)   }
  if ( outTween ) { TWEENKEY("outTw",outTween) }
  if ( midTween ) { TWEENKEY("midTw",midTween) }
  SSUBKEY("end",end)
  SSUBKEY("start",start)
  WORDKEY("segs",segments)
  WORDKEY("thick",thickness)
  WORDKEY("pulse",pulseSpeed)
  TXTKEY("presi",presentationName)
  TXTKEY("to",slideName)
  if ( font ) WORDKEY("font",font->name)
  TXTKEY("content",content)
  SSUBKEY("fontSize",fontSize)
  SSUBKEY("fontSizeB",fontSizeB)
  WORDKEY("maxchars",maxChars)
  WORDKEY("ls",lineSpace)
  WORDKEY("lines",lines)
  NUMBKEY("aa",a)
  NUMBKEY("bb",b)
  NUMBKEY("cc",c)
  NUMBKEY("dd",d)
  SSUBKEY("q",q)
  TAGKEY("use",useRelativeGlyphWidths)
  TAGKEY("centered",centered)
  TAGKEY("useQ",useQuad)
  TAGKEY("fireOnce",fireOnce)
  TAGKEY("rect",rectangular)
  TAGKEY("circ",circular)
  TAGKEY("flick",flickerOut)
  TAGKEY("perp",perpetuate)
  TAGKEY("rep",repeats)
  TAGKEY("behind",placeBehind)
  WORDKEY("wanSpd",wanderSpeed)
  WORDKEY("wanSpdMi",wanderSpeedMinimum)
  WORDKEY("wanSpdMa",wanderSpeedLimit)
  WORDKEY("wanAcc",wanderAccel)
  WORDKEY("ting",tinting)
  WORDKEY("ra",rate)
  WORDKEY("ang",angle)
  WORDKEY("spin",spin)
  WORDKEY("opS",opacityStart)
  WORDKEY("opMid",opacityMid)
  WORDKEY("opE",opacityEnd)
  TAGKEY("fill",filled)
  SSUBKEY("wrap",wrapLimit)
  SSUBKEY("loc",location)
  SSUBKEY("rot",rotation)
  SSUBKEY("sz",size)
  SSUBKEY("piv",pivot)
  SSUBKEY("sa",sA)
  SSUBKEY("sb",sB)
  SSUBKEY("sc",sC)
  SSUBKEY("sd",sD)
  SSUBKEY("pa",pA)
  SSUBKEY("pb",pB)
  SSUBKEY("pc",pC)
  SSUBKEY("pd",pD)
  SSUBKEY("da",dA)
  SSUBKEY("db",dB)
  SSUBKEY("dc",dC)
  SSUBKEY("dd",dD)
  SSUBKEY("mul",mulligan)
  SSUBKEY("snd",sound)
  SSUBKEY("cur",curves)
  WORDKEY("cyc",interval)
  WORDKEY("ug",I2S(style))
 })
 void AddToSlide( Presentation *p, Slide *s, bool debugMode=false, int recursionDepth=0 ) {
  switch ( type ) {
   case aet_Film:
     {
      fx_Film *e=new fx_Film();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->film=film;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_FilmIsotrapf:
     {
      fx_FilmIsotrapf *e=new fx_FilmIsotrapf();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->film=film;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_FilmLineFader:
     {
      fx_FilmLineFader *e=new fx_FilmLineFader();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->film=film;
      e->in=inTween;
      e->out=outTween;
      e->inTime=inTime;
      e->outTime=outTime;
      e->blend=blend;
      e->tint.fromCrayon(tint);
      e->w2=end.x;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_FilmLoop:
     {
      fx_FilmLoop *e=new fx_FilmLoop();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->film=film;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_FilmLoopFader:
     {
      fx_FilmLoopFader *e=new fx_FilmLoopFader();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->film=film;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_MultiFilm:
     {
     }
    break;
   case aet_Glow:
     {
      fx_Glow *e=new fx_Glow();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->flare=image;
      e->flare2=image2;
      e->flare3=image3;
      e->erratic_flaresource=erratic_flaresource;
      e->tweenW=tweenW;
      e->tweenH=tweenH;
      e->tweenLR=tweenLR;
      e->tweenUD=tweenUD;
      e->tint.fromCrayon(tint);
      e->dx=delta.x;
      e->dy=delta.y;
      e->w2=delta.w;
      e->h2=delta.h;
      e->maxOp=maxOp;      
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Glow2:
     {
      fx_Glow2 *e=new fx_Glow2();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->flare=image;
      e->flare2=image2;
      e->flare3=image3;
      e->erratic_flaresource=erratic_flaresource;
      e->tweenW=tweenW;
      e->tweenH=tweenH;
      e->tweenLR=tweenLR;
      e->tweenUD=tweenUD;
      e->tint.fromCrayon(tint);
      e->dx=delta.x;
      e->dy=delta.y;
      e->w2=delta.w;
      e->h2=delta.h;
      e->maxOp=maxOp;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Glow3:
     {
      fx_Glow3 *e=new fx_Glow3();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->flare=image;
      e->tint.fromCrayon(tint);
      e->dx=delta.x;
      e->dy=delta.y;
      e->w2=delta.w;
      e->h2=delta.h;
      e->maxOp=maxOp;
      e->blend=blend;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Image:
     {
      fx_Image *e=new fx_Image();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->image=image;
      e->blend=blend;
      e->tint.fromCrayon(tint);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Box:
     {
      fx_Box *e=new fx_Box();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->thickness=(int)thickness;
      e->pulsespeed=pulseSpeed;
      e->tint_outside.fromCrayon(tintOut);
      e->tint_inside.fromCrayon(tintIn);
      e->tint_start.fromCrayon(tintStart);
      e->tint_end.fromCrayon(tintEnd);
      e->interior.fromCrayon(interior);
      e->filled=filled;
//      e->t.fromCrayon(tint);
//      e->u.fromCrayon(tint2);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Circle:
     {
      fx_Circle2d*e=new fx_Circle2d();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->radius=delta.w;
      e->tint.fromCrayon(tint);
      e->blend=blend;
      e->segments=segments;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_FadeIn:
     {
      fx_FadeIn *e=new fx_FadeIn();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->tint.fromCrayon(tint);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_FadeOut:
     {
      fx_FadeOut *e=new fx_FadeOut();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->tint.fromCrayon(tint);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_MovingLine:
     {
      fx_MovingLine *e=new fx_MovingLine();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->tint.fromCrayon(tint);
      e->xdx=(int)delta.x;
      e->xdy=(int)delta.y;
      e->x2dx2=(int)delta.w;
      e->y2dy2=(int)delta.h;
      e->a=a;
      e->b=b;
      e->c=c;
      e->d=d;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_MatrixText:
     {
      fx_MatrixText *e=new fx_MatrixText();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Text:
     {
     }
    break;
   case aet_WriteText:
     {
     }
    break;
   case aet_TexFont:
     {
      fx_TexFont *e=new fx_TexFont();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->font=font;
      e->fw=fontSize.x;
      e->fh=fontSize.y;
      e->len=(int)content.length;
      e->maxChars=maxChars;
      e->deltaIn=inTime;
      e->deltaOut=outTime;
      e->deltaDelay=deltaDelay;
      e->color.fromCrayon(tint);
      e->UseRelativeGlyphWidths=useRelativeGlyphWidths;
      e->lineSpace=lineSpace;
      e->lines=linefeeds(content.c_str()); if ( e->lines < 1 ) e->lines=1;
      e->blend=blend;
      if ( centered ) e->Center(content.c_str(),p,(int)extents.x,(int)extents.y,(int)fontSize.x,(int)fontSize.y,lineSpace);
      else e->Text(content.c_str(),p);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_TexFontFader:
     {
      fx_TexFontFader *e=new fx_TexFontFader();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->font=font;
      e->fw=fontSize.x;
      e->fh=fontSize.y;
      e->len=(int)content.length;
      e->maxChars=maxChars;
      e->deltaDuration=deltaDuration;
      e->deltaIn=inTime;
      e->deltaOut=outTime;
      e->deltaDelay=deltaDelay;
      e->color.fromCrayon(tint);
      e->UseRelativeGlyphWidths=useRelativeGlyphWidths;
      e->lineSpace=lineSpace;
      e->lines=linefeeds(content.c_str()); if ( e->lines < 1 ) e->lines=1;
      e->blend=blend;
      if ( centered ) e->Center(content.c_str(),p,(int)extents.x,(int)extents.y,(int)fontSize.x,(int)fontSize.y,lineSpace);
      else e->Text(content.c_str(),p);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Presentation:
     {
//      fx_Presentation *e=new fx_Presentation();
//      s->fx->Add(e,p);
     }
    break;
   case aet_XFade:
     {
      fx_XFade *e=new fx_XFade();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Aid:
     {
      Aid *e=new Aid();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->image=image;
      e->tint.fromCrayon(tint);
      e->blend=blend;
      e->xreversed=xreversed;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_DriftingRotatingVisualAid:
     {
      DriftingRotatingVisualAid2 *e=new DriftingRotatingVisualAid2();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->sx=extents.x;
      e->sy=extents.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w2=delta.w;
      e->h2=delta.h;
      e->widthMod=tweenW;
      e->heightMod=tweenH;
      e->xDirection=delta.x;
      e->yDirection=delta.y;
      e->blend=blend;
      e->wanderSpeed=wanderSpeed;
      e->tinting=tinting;
      e->delta=rate;
      e->tint_start.fromCrayon(tintStart);
      e->tint_end.fromCrayon(tintEnd);
      e->angle=angle;
      e->deltaAngle=spin;
      e->image=image;
      e->blend=blend;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_DriftingStretchingRotatingVisualAid:
     {
      DriftingStretchingRotatingVisualAid2 *e=new DriftingStretchingRotatingVisualAid2();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->sx=extents.x;
      e->sy=extents.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w2=delta.w;
      e->h2=delta.h;
      e->widthMod=tweenW;
      e->heightMod=tweenH;
      e->xDirection=delta.x;
      e->yDirection=delta.y;
      e->blend=blend;
      e->wanderSpeed=wanderSpeed;
      e->tinting=tinting;
      e->delta=rate;
      e->tint_start.fromCrayon(tintStart);
      e->tint_end.fromCrayon(tintEnd);
//      e->angle=angle;
      e->image=image;
      e->blend=blend;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_DriftingStretchingVisualAid:
     {
      DriftingStretchingVisualAid2 *e=new DriftingStretchingVisualAid2();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->sx=extents.x;
      e->sy=extents.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w2=delta.w;
      e->h2=delta.h;
      e->widthMod=tweenW;
      e->heightMod=tweenH;
      e->xDirection=delta.x;
      e->yDirection=delta.y;
      e->blend=blend;
      e->wanderSpeed=wanderSpeed;
      e->tinting=tinting;
      e->delta=rate;
      e->tint_start.fromCrayon(tintStart);
      e->tint_end.fromCrayon(tintEnd);
      e->image=image;
      e->blend=blend;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_DriftingVisualAid:
     {
      DriftingVisualAid2 *e=new DriftingVisualAid2();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->sx=extents.x;
      e->sy=extents.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w2=delta.w;
      e->h2=delta.h;
      e->widthMod=tweenW;
      e->heightMod=tweenH;
      e->xDirection=delta.x;
      e->yDirection=delta.y;
      e->blend=blend;
      e->wanderSpeed=wanderSpeed;
      e->tinting=tinting;
      e->delta=rate;
      e->tint_start.fromCrayon(tintStart);
      e->tint_end.fromCrayon(tintEnd);
      e->image=image;
      e->blend=blend;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Visual:
     {
      Visual *e=new Visual();
      e->image=image;
      e->blend=blend;
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_VisualAid:
     {
      VisualAid *e=new VisualAid();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->image=image;
      e->mask=image2;
      e->solarize=image3;
      e->filter=image4;
      e->sx=extents.x;
      e->sy=extents.y;
      e->dx=delta.x;
      e->dy=delta.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w2=delta.w;
      e->h2=delta.h;
      e->in=inTime;
      e->out=outTime;
      e->blend=blend;
      e->tint_start.fromCrayon(tintStart);
      e->tint_mid.fromCrayon(tint);
      e->tint_end.fromCrayon(tintEnd);
      e->start=opacityStart;
      e->mid=opacityMid;
      e->end=opacityEnd;
      e->flicker_out=flickerOut;
      e->perpetuate=perpetuate;
      e->repeats=repeats;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_VisualAid2:
     {
      VisualAid2 *e=new VisualAid2();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->image=image;
      e->mask=image2;
      e->solarize=image3;
      e->filter=image4;
      e->sx=extents.x;
      e->sy=extents.y;
      e->dx=delta.x;
      e->dy=delta.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w2=delta.w;
      e->h2=delta.h;
      e->in=inTime;
      e->out=outTime;
      e->blend=blend;
      e->tint_start.fromCrayon(tintStart);
      e->tint_mid.fromCrayon(tint);
      e->tint_end.fromCrayon(tintEnd);
      e->start=opacityStart;
      e->mid=opacityMid;
      e->end=opacityEnd;
      e->flicker_out=flickerOut;
      e->perpetuate=perpetuate;
      e->repeats=repeats;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_VisualAidAdditive:
     {
      VisualAidAdditive *e=new VisualAidAdditive();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_VisualRotated:
     {
      VisualRotated *e=new VisualRotated();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->angle=(float)angle;
      e->spin=(float)spin;
      e->image=image;
      e->mask=image2;
      e->solarize=image3;
      e->filter=image4;
      e->sx=extents.x;
      e->sy=extents.y;
      e->dx=delta.x;
      e->dy=delta.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w2=delta.w;
      e->h2=delta.h;
      e->in=inTime;
      e->out=outTime;
      e->blend=blend;
      e->tint_start.fromCrayon(tintStart);
      e->tint_mid.fromCrayon(tint);
      e->tint_end.fromCrayon(tintEnd);
      e->start=opacityStart;
      e->mid=opacityMid;
      e->end=opacityEnd;
      e->flicker_out=flickerOut;
      e->perpetuate=perpetuate;
      e->repeats=repeats;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_VisualSkewed:
     {
      VisualSkewed *e=new VisualSkewed();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->sA.Set(&sA);
      e->sB.Set(&sB);
      e->sC.Set(&sC);
      e->sD.Set(&sD);
      e->pA.Set(&pA);
      e->pB.Set(&pB);
      e->pC.Set(&pC);
      e->pD.Set(&pD);
      e->dA.Set(&dA);
      e->dB.Set(&dB);
      e->dC.Set(&dC);
      e->dD.Set(&dD);
      e->image=image;
      e->mask=image2;
      e->solarize=image3;
      e->filter=image4;
      e->in=inTime;
      e->out=outTime;
      e->blend=blend;
      e->tint_start.fromCrayon(tintStart);
      e->tint_mid.fromCrayon(tint);
      e->tint_end.fromCrayon(tintEnd);
      e->start=opacityStart;
      e->mid=opacityMid;
      e->end=opacityEnd;
      e->flicker_out=flickerOut;
      e->perpetuate=perpetuate;
      e->repeats=repeats;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_WanderingVisualAid:
     {
      WanderingVisualAid2 *e=new WanderingVisualAid2();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->sx=start.x;
      e->sy=start.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w=delta.w;
      e->h=delta.h;
      e->widthMod=tweenW;
      e->heightMod=tweenH;
      e->wanderSpeed=wanderSpeed;
      e->wanderSpeedLimit=wanderSpeedLimit;
      e->wanderAccel=wanderAccel;
      e->blend=blend;
      e->image=image;
      e->tint_start.fromCrayon(tintStart);
      e->tint_end.fromCrayon(tintEnd);
      e->wrapLimit=&wrapLimit;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_WobblingVisualAid:
     {
      WobblingVisualAid2 *e=new WobblingVisualAid2();
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->sx=start.x;
      e->sy=start.y;
      e->width=extents.w;
      e->height=extents.h;
      e->w=delta.w;
      e->h=delta.h;
      e->widthMod=tweenW;
      e->heightMod=tweenH;
      e->wanderSpeed=wanderSpeed;
      e->wanderSpeedLimit=wanderSpeedLimit;
      e->wanderAccel=wanderAccel;
      e->blend=blend;
      e->image=image;
      e->tint_start.fromCrayon(tintStart);
      e->tint_end.fromCrayon(tintEnd);
      e->wrapLimit=&wrapLimit;
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);
      s->fx->Add(e,p);
     }
    break;
   case aet_Mulligan:
     {
      fx_Mulligan *e=new fx_Mulligan();
      e->showingDebug=debugMode;
      e->duration=(float)duration;
      e->delay=(float)(delay<0.05?0.0:delay);
      e->SetExtents((int)extents.x,(int)extents.y,(int)extents.w,(int)extents.h);      
      s->fx->Add(e,p);
     }
    break;
   case aet_NextGenVisual:
    {
     fx_NextGenVisual *e=new fx_NextGenVisual;
     e->font=this->font;
     e->fontSize.Set(&fontSize);
     e->fontSizeB.Set(&fontSizeB);
     e->presentation=presentationName;
     e->recursionDepth=recursionDepth;
     e->useCurves=flickerOut;
     if ( e->useCurves ) {
      Vertices path;      
      this->curves.Compute(&path);
      e->curveLines.Add(&path,false);
      e->curveLines.TallyLength();
     }
     e->placeBehind=placeBehind;
     e->useQuad=useQuad;
     e->content=content;
     Vertexd outputSize((double)fbo1080p3.w,(double)fbo1080p3.h,0.0);
     e->q.Set(&q);
     e->curves2x=xreversed;
     curves.Compute(&e->curves,true);
     e->showingDebug=debugMode;
     e->duration=(float)duration;
     e->delay=(float)(delay<0.05?0.0:delay);
     e->image1=image;
     e->blend1=blend;
     e->tint1.fromCrayon(tint);
     e->image2=image2;
     e->tint2.fromCrayon(tint2);
     e->blend2=blend2;
     e->image3=image3;
     e->tint3.fromCrayon(tint3);
     e->blend3=blend3;
     e->image4=image4;
     e->tint4.fromCrayon(tint4);
     e->blend4=blend4;
     e->film=film;
     e->filmTint.fromCrayon(tintIn);
     e->filmBlend=blend5;
     e->A.Set(&extents);
     e->B.Set(&delta);
     e->velocity.Set(&this->B);
     e->outward=this->erratic_flaresource;
     e->mulliganDelay.Set((float)interval);
     e->mulligan.Set(&mulligan);
     e->pivot.Set((double)pivot.x,(double)pivot.y);
     e->mulliganArea.Set(&this->C);
     e->isotrap=this->fireOnce;
     e->circular=circular;
     e->edgeGenerationOnly=this->filled;
     e->startAngle=angle;
     e->endAngle=spin;
     e->inTime=this->inTime;
     e->outTime=this->outTime;
     e->inTween=inTween;
     e->midTween=midTween;
     e->outTween=outTween;
     e->volume=this->thickness;
     e->filmTimeDilation=this->deltaDuration;
     e->sound.Set(&sound);
     e->soundDelay.Set((float)this->deltaDelay);
     s->fx->Add(e,p);
    }
    break;
   case aet_SoundEffect:
     {
      
     }
    break;
  }
 }
MANY(AnimationElement,AnimationElementHandle,AnimationElementHandles,"AnimEle",AnimationElements,{})
 KEYWORDSGroup("ele",AnimationElement)
 WORDKEYSGroup("ele",AnimationElement)
 static void DeployTypes( Strings *s ) {
  s->Add("Film");///                                  
  s->Add("Film Isotrap");///
  s->Add("Film Line Fader");
  s->Add("Film Loop");///
  s->Add("Film Loop Fader");///
  s->Add("Multi-Film");///
  s->Add("Glow v1");
  s->Add("Glow v2");
  s->Add("Glow v3");
  s->Add("Static Image");
  s->Add("Line Box");
  s->Add("Static Circle");
  s->Add("Slide Fade In");
  s->Add("Slide Fade Out");
  s->Add("Moving Line");
  s->Add("Matrix Text");///
  s->Add("Text");///
  s->Add("Write Text");///
  s->Add("TexFont");
  s->Add("TexFont Fader");
  s->Add("Presentation");
  s->Add("Crossfaded Visuals");
  s->Add("Simple Visual Aid");
  s->Add("Drift-Rotate Visual");
  s->Add("Drift-Rotate-Stretch Visual");
  s->Add("Drift-Stretch Visual");
  s->Add("Multi-Visual");
  s->Add("Visual");
  s->Add("Visual Aid");
  s->Add("Visual Aid 2");
  s->Add("Additive Visual");
  s->Add("Rotated Visual");
  s->Add("Skewed Visual");
  s->Add("Wandering Visual");
  s->Add("Wobbling Visual");
  s->Add("Mulligan and Sound");
  s->Add("Scaled Rotated Visual");
 }
DONE(AnimationElement);

extern Zdis<AnimationElement> copiedAnimationElement;

//class SoundSequencerEditor : public FormEditorWindow {
//public:
// SoundSequencerEditor( SoundSequencer *sounds, MoveableWindowManager *mwm ) : FormEditorWindow(mwm) {
// }
//};

////////////////
// Slide Profile
////////////////
ONE(SlideProfile,{ duration=3.0; })
// SoundSequencer sounds;
 AnimationElements elements;
 Zdouble duration;
 Zint seek;
 CLONE(SlideProfile,{
  DUPE(duration)
  DUPE(seek)
  DUPESLIST(elements,AnimationElement)
 })
 KEYWORDS({
  TXTWORD("name",name)
  else KEYWORD("duration",{duration=w;})
  else KEYWORD("seek",{seek=w;})
  else SUBWORD("elements",elements)
  else BADKEY("SlideProfile")
 })
 WORDKEYS({
  TXTKEY("name",name)
  NUMBKEY("duration",duration)
  NUMBKEY("seek",seek)
  SSUBKEY("elements",elements)
 })
 void CreateSlide(Presentation *p,bool debugMode=false, int recursionDepth=0){ 
  Slide *s=new Slide;
  s->duration=duration;
  s->seek=seek;
  p->Add(s);
  EACH(elements.first,AnimationElement,e) {
   e->AddToSlide(p,s,debugMode,recursionDepth);
  }
 }
 Presentation *CreateSlideAsPresentation( bool debugMode=false, int recursionDepth=0 ) {
  Presentation *p=new Presentation();
  p->doNotPauseWhenNotVisible=false;
  p->duration=duration;
  p->paused=false;
  p->fbo=null;
  CreateSlide(p,debugMode,recursionDepth);
  return p;
 }
MANY(SlideProfile,SlideProfileHandle,SlideProfileHandles,"SlideProfile",SlideProfiles,{})
 KEYWORDSGroup("Slide",SlideProfile)
 WORDKEYSGroup("Slide",SlideProfile)
DONE(SlideProfile);

extern Zdis<SlideProfile> copiedSlideProfile;

C_ONE(SlideProfileEditor,FormEditorWindow,{})
 Zp<SlideProfile> ed;
 Zp<few_Reserved> layerSelectArea,playbackArea,copyButtonArea,layerEditArea,moveRectArea,selectFullscreenButton,lineModeToggle,copyLayerButton,pasteLayerButton;
 Zp<FBOColorDepth> fbo;
 Zstring path;
 Zp<GLImage> selectFullscreenIcon;
 Zp<AnimationElement> selectedLayer;
 Zdis<Presentation> presi;
 Zint layerScrolled,layerSelected,wasLayerSelected;
 Strings layers;
 Zstring fImage1,fImage2,fImage3,fImage4;
 FileDialog fdImage1,fdImage2,fdImage3,fdImage4;
 Zbool lineMode;
 Zbool clickedForRect;
 Zbool clickedForLine;
 Line clickedLine,virtualLine;
 ScreenPosition clickedRect;
 Cartesiand mousePos,viewPos,virtualPos,virtualRect; 
 Zint layerAreaScrolled;
 Zdouble filmPickInstant;
 Zint filmPickScrolled;
 Zstring va,fi;
 SlideProfileEditor( SlideProfile *sp, MoveableWindowManager *mwm, const char *path="data/workspaces/presentations/" ) : FormEditorWindow(mwm) {
  this->path=path;
  ed=sp;
  AnimationElements::DeployTypes(&typeNames);
  v.Associate(sp);
  PopulateWidgets();
//  startingSeek=Bind(&prpr->startingSeek.value);
//  f->label="Seek Slide #";
  selectFullscreenIcon=library.Load("data/images/icon3/select_full.png");
  fbo=&fbo1080p2;
  fdImage1.description="Choose the first image for this animation element.";
  fdImage1.includedLocations.Add("data/images/");
  fdImage1.path=path;
  fdImage1.background.x_DarkSlateBlue();
  fdImage1.bound=&fImage1.value;
  fdImage2.description="Choose the second image for this animation element.";
  fdImage2.includedLocations.Add("data/images/");
  fdImage2.path=path;
  fdImage2.background.x_DarkSlateBlue();
  fdImage2.bound=&fImage2.value;
  fdImage3.description="Choose the third image for this animation element.";
  fdImage3.includedLocations.Add("data/images/");
  fdImage3.path=path;
  fdImage3.background.x_DarkSlateBlue();
  fdImage3.bound=&fImage3.value;
  fdImage4.description="Choose the fourth image for this animation element.";
  fdImage4.includedLocations.Add("data/images/");
  fdImage4.path=path;
  fdImage4.background.x_DarkSlateBlue();
  fdImage4.bound=&fImage4.value;
 }
 Strings typeNames;
 void PopulateWidgets() {
  widgets.Clear();
  playbackArea=Reserve(0,512,(int)(512.0*0.75));
  layerEditArea=Reserve(1,1024,512+32);
  layerSelectArea=Reserve(2,128-16,512+32);
  moveRectArea=Reserve(3,96,96);
  selectFullscreenButton=Reserve(4,32,32);
  copyButtonArea=Reserve(5,64+8,16+2);
  lineModeToggle=Reserve(6,128,16);
  copyLayerButton=Reserve(7,128,16);
  pasteLayerButton=Reserve(8,128,16);
  FormEditorWidget *f=null;
  f=(FormEditorWidget *) Bind(&ed->name.value);
  f->label="Slide Name";
  few_DecimalTuner *f_dt=Bind(&ed->duration.value);
  f_dt->label="Slide Duration (seconds)";
  f_dt->delta=1.0;
  f_dt->deltaCtrl=0.1;
  f_dt->deltaAlt=5.0;
  f_dt->deltaShift=10.0;
  f=(FormEditorWidget *) Bind(&ed->seek.value);
  f->label="At End, Go To Slide #";
  v.Resize(256,256);
  Arrange();
  mwm->vgui.Arrange2Rows(32.0,64.0);
  mwm->vgui.SlideTo(&v);
  Setup();
  v.Calculate();
  Repopulate();
 }
 void WhenSetup() {
  clip=false;
  v.noMove=false;
  v.noMinimize=false;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noTitleBar=false;
  v.noBacking=false;
  v.alwaysOnTop=true;
  v.noClose=false;
  v.noClipTitle=true;
  v.minimumSize.Set(128,64);
  v.maximumSize.Set(1280,1024);
 }
 void Repopulate() {
  if ( selectedLayer ) {
   if ( selectedLayer->film ) {
    FilmLibrary *library=selectedLayer->film->GetLibrary();
    if ( library ) {
     va=library->title;
     fi=selectedLayer->film->title;
    } else {va=""; fi=""; }
   } else { va=""; fi=""; }
  }
  layerSelected=-1;
  layers.Clear();
  int i=0;
  EACH(ed->elements.first,AnimationElement,e) { i++; layers.Add(I2S(i)); }
 }
 void RenderRulerOverlay() {
  lineMode=fast.toggle("Line Mode Ruler",lineMode,lineModeToggle->x,lineModeToggle->y,16,16);
  fast.GetState(playbackArea->x,playbackArea->y,playbackArea->w,playbackArea->h);
  Crayon outer,inner,darkGrey;
  outer.Pick(red255);
  inner.Pick(green255);
  darkGrey.Int(98,98,98,255);
  if ( fast.hovering ) {
   Cartesiand rect;
   mousePos.Set(playbackArea->w*fast.mouse.x,playbackArea->h*fast.mouse.y);
   viewPos.Set(mousePos.x+playbackArea->x,mousePos.y+playbackArea->y);
   virtualPos.Set(fbo->w*fast.mouse.x,fbo->h*fast.mouse.y);
   if ( !lineMode ) {
    if ( input.left && !input.wasLeft ) {
     clickedForRect=true;
     clickedRect.Set(virtualPos.x,virtualPos.y,0.0,0.0);
    } else if ( input.left ) {
     clickedRect.Corners(
      virtualPos.x > (double)clickedRect.x ? (double)clickedRect.x : virtualPos.x,
      virtualPos.y > (double)clickedRect.y ? (double)clickedRect.y : virtualPos.y,
      virtualPos.x > (double)clickedRect.x ? virtualPos.x : (double)clickedRect.x,
      virtualPos.y > (double)clickedRect.y ? virtualPos.y : (double)clickedRect.y
     );
    } else if ( !input.left && input.wasLeft ) {
     clickedRect.Corners(
      virtualPos.x > (double)clickedRect.x ? (double) clickedRect.x : virtualPos.x,
      virtualPos.y > (double)clickedRect.y ? (double) clickedRect.y : virtualPos.y,
      virtualPos.x > (double)clickedRect.x ? virtualPos.x : (double) clickedRect.x,
      virtualPos.y > (double)clickedRect.y ? virtualPos.y : (double) clickedRect.y
     );
     virtualRect.SetRect(clickedRect.x,clickedRect.y,clickedRect.w,clickedRect.h);
    } else {
     clickedForRect=false;
    }
    rect.SetCorners((double)playbackArea->x,viewPos.y,viewPos.x,viewPos.y);
    DrawLine2Color(outer,inner,rect.x,rect.y,rect.x2,rect.y2);
    rect.SetCorners(viewPos.x,viewPos.y,(double)playbackArea->x2,viewPos.y);
    DrawLine2Color(inner,outer,rect.x,rect.y,rect.x2,rect.y2);
    rect.SetCorners(viewPos.x,(double)playbackArea->y,viewPos.x,viewPos.y);
    DrawLine2Color(outer,inner,rect.x,rect.y,rect.x2,rect.y2);
    rect.SetCorners(viewPos.x,viewPos.y,viewPos.x,(double)playbackArea->y2);
    DrawLine2Color(inner,outer,rect.x,rect.y,rect.x2,rect.y2);
    Text( FORMAT("%d,%d",(int)virtualPos.x, (int) virtualPos.y ).c_str(),
     viewPos.x+20.0,viewPos.y+20.0, fast.fontSize.x, fast.fontSize.y, false
    );
   } else {
    if ( input.left && !input.wasLeft ) {
     clickedForLine=true;
     clickedLine.Set(virtualPos.x,virtualPos.y,0.0,0.0);
    } else if ( input.left ) {
     clickedLine.Set(clickedLine.x,clickedLine.y,virtualPos.x,virtualPos.y);     
    } else if ( !input.left && input.wasLeft ) {
     clickedLine.Set(clickedLine.x,clickedLine.y,virtualPos.x,virtualPos.y);
     virtualLine.Set(clickedLine.x,clickedLine.y,clickedLine.x2,clickedLine.y2);      
    } else {
     clickedForLine=false;
    }
    outer.Divide(2.0f);
    inner.Divide(2.0f);
    rect.SetCorners((double)playbackArea->x,viewPos.y,viewPos.x,viewPos.y);
    DrawLine2Color(outer,inner,rect.x,rect.y,rect.x2,rect.y2);
    rect.SetCorners(viewPos.x,viewPos.y,(double)playbackArea->x2,viewPos.y);
    DrawLine2Color(inner,outer,rect.x,rect.y,rect.x2,rect.y2);
    rect.SetCorners(viewPos.x,(double)playbackArea->y,viewPos.x,viewPos.y);
    DrawLine2Color(outer,inner,rect.x,rect.y,rect.x2,rect.y2);
    rect.SetCorners(viewPos.x,viewPos.y,viewPos.x,(double)playbackArea->y2);
    DrawLine2Color(inner,outer,rect.x,rect.y,rect.x2,rect.y2);
    Text( FORMAT("%d,%d",(int)virtualPos.x, (int) virtualPos.y ).c_str(),
     viewPos.x+20.0,viewPos.y+20.0, fast.fontSize.x, fast.fontSize.y, false
    );
    outer.Pick(red255);
    inner.Pick(green255);
   }
  }
  if ( !lineMode ) {
   crayons.jami(0,32,32,32,255).gl();
   DrawCircle(
    playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
    iratiod((int)clickedRect.w,fbo->w)*playbackArea->w,
    32
   );
   Rectangle(darkGrey,
    playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
    playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)clickedRect.y2,fbo->h)*playbackArea->h)
   );
   DrawLine(darkGrey,
    playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
    playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)clickedRect.y2,fbo->h)*playbackArea->h)
   );
   Circle(
    playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
    playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w)
   );
   if ( clickedForRect )
    Rectangle(outer,
     playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
     playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedRect.y2,fbo->h)*playbackArea->h)
   );
   Text( FORMAT("%d,%d\n%d,%d\n%d x %d\n%s",
     (int)clickedRect.x, (int) clickedRect.y,
     (int)clickedRect.x2, (int) clickedRect.y2,
     (int) clickedRect.w, (int) clickedRect.h,
     ( virtualRect.Equals(&copiedCoordinate) ? "copied" : "" )
    ).c_str(),
    fast.color,
    transparency,
    (double)playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*(double)playbackArea->w)+8.0,
    (double)playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*(double)playbackArea->h), 
    fast.fontSize.x, fast.fontSize.y, 2.0, fast.lineSpace, false
   );
  } else {
   DrawLine2Color(inner,outer,
    playbackArea->x+(iratiod((int)clickedLine.x,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)clickedLine.y,fbo->h)*playbackArea->h),
    playbackArea->x+(iratiod((int)clickedLine.x2,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)clickedLine.y2,fbo->h)*playbackArea->h)
   );
   DrawLine(darkGrey,
    playbackArea->x+(iratiod((int)virtualLine.x,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)virtualLine.y,fbo->h)*playbackArea->h),
    playbackArea->x+(iratiod((int)virtualLine.x2,fbo->w)*playbackArea->w),
    playbackArea->y+(iratiod((int)virtualLine.y2,fbo->h)*playbackArea->h)
   );
   Text( "A", 
    (double)playbackArea->x+(iratiod((int)virtualLine.x,fbo->w)*playbackArea->w),
    (double)playbackArea->y+(iratiod((int)virtualLine.y,fbo->h)*playbackArea->h-16.0),
    11.0,8.0,true
   );
   Text( "B", 
    (double)playbackArea->x+(iratiod((int)virtualLine.x2,fbo->w)*playbackArea->w),
    (double)playbackArea->y+(iratiod((int)virtualLine.y2,fbo->h)*playbackArea->h-16.0),
    11.0,8.0,true
   );
   virtualLine.CalculateSlope();
   clickedLine.CalculateSlope();
   Text( FORMAT("%d,%d\n%d,%d\nAngle: %1.1f deg\n%s",
     (int)virtualLine.x, (int) virtualLine.y,
     (int)virtualLine.x2, (int) virtualLine.y2,
     (double)(virtualLine.angleDeg),
     ( virtualLine.Equals(&copiedLine) ? "copied" : "" )
    ).c_str(),
    fast.color,
    transparency,
    (double)playbackArea->x+(iratiod((int)clickedLine.x2,fbo->w)*(double)playbackArea->w)+8.0+32.0,
    (double)playbackArea->y+(iratiod((int)clickedLine.y,fbo->h)*(double)playbackArea->h), fast.fontSize.x, fast.fontSize.y, fast.charSpace, fast.lineSpace, false
   );
  }
  if ( input.leftReleased() ) { clickedForLine=false; clickedForRect=false; }
  bool playbackAreaHovering=fast.hovering;
  int movement=fast.arrows("",moveRectArea->x+32,moveRectArea->y+32,24);
  if ( (playbackAreaHovering && input.KeyDown(DX_X))
    || fast.button(selectFullscreenIcon,selectFullscreenButton->x+1,selectFullscreenButton->y+1,selectFullscreenButton->w-2,selectFullscreenButton->h-2) ) {
   virtualRect.SetRect(0,0,fbo->w,fbo->h);
   clickedRect.Set(0,0,fbo->w,fbo->h);
  }
  if ( fast.button("Copy [\\]",copyButtonArea->x+4,copyButtonArea->y+1,copyButtonArea->w-8,copyButtonArea->h-2)
    || ( playbackAreaHovering && input.KeyDown(DX_C) ) ) {
   if ( !lineMode ) copiedCoordinate.Set(&virtualRect);
   else copiedLine.Set(&virtualLine);
  }
 }
 void RenderAfterWindow() {
  if ( v.moving || v.holder->arranging || v.holder->sliding ) return;
  if ( v.wasMoving || v.holder->wasSliding ) return;
  if ( selectedLayer && fast.button("Copy Layer",copyLayerButton->x,copyLayerButton->y,copyLayerButton->w,copyLayerButton->h) ) {
    copiedAnimationElement.Recycle(selectedLayer->Duplicate());
    return;
  }
  if ( copiedAnimationElement && fast.button("Paste After",pasteLayerButton->x,pasteLayerButton->y,pasteLayerButton->w,pasteLayerButton->h) ) {
   AnimationElement *ae=copiedAnimationElement->Duplicate();
   ed->elements.InsertAfter(ae,selectedLayer);
   Repopulate();
   selectedLayer=ae;
   v.holder->DestroyAttached(&v);     
   return;
  }
  Cartesian rect;
  { // Slide Preview
   rect.SetRect(playbackArea->x+4,playbackArea->y+4,playbackArea->w-8,playbackArea->h-8);
   if ( presi ) {
    bool stopped=!presi || !presi->current || presi->paused || fast.button("",rect.x,rect.y,rect.w,rect.h);
    fbo->background.Pick(black);
    fbo->Bind();
    presi->Between();
    presi->BetweenFrames();
    presi->Render();
    fbo->Unbind();
    fbo->blend=none;
    fbo->tint.Pick(alabaster);
    fbo->RenderUpsideDown(rect.x,rect.y,rect.x2,rect.y2);
    Area(crayons.Pick(red255),rect.x,rect.y,rect.x2,rect.y+2);
    if ( presi->current ) Area(crayons.Pick(green255),rect.x,rect.y,rect.x+(int)((double)rect.w*(double)((presi->current->duration-presi->current->remaining)/presi->current->duration)),rect.y+2);
    if ( stopped ) {
     presi.Recycle(ed->CreateSlideAsPresentation(true));
     presi->SetPositionAndSize(0,0,fbo->w,fbo->h);
   	 presi->Init(false);
    }
   } else {
    presi.Recycle(ed->CreateSlideAsPresentation(true));
    presi->SetPositionAndSize(0,0,fbo->w,fbo->h);
  	 presi->Init(false);
   }
   if ( presi ) {
    if ( selectedLayer ) {
     switch ( selectedLayer->type ) {
      case aet_NextGenVisual:
        {
         AnimationElement *AE=selectedLayer;
         Cartesiand r1,r2;
         if ( !AE->xreversed && !AE->fireOnce ) {
          r2.SetRect(
           playbackArea->x+playbackArea->w/4.0,
           playbackArea->y+playbackArea->h/4.0,
           playbackArea->w/2.0,
           playbackArea->h/2.0
          );
          Rectangle(crayons.Pick(darkBrown),&r2);
         }
         r1.SetRect(
          playbackArea->x+(AE->extents.x/fbo->w)*playbackArea->w,
          playbackArea->y+(AE->extents.y/fbo->h)*playbackArea->h,
          playbackArea->w*(AE->extents.w/fbo->w),
          playbackArea->h*(AE->extents.h/fbo->h)
         );
         Rectangle(crayons.Pick(green255),&r1);
         r2.SetRect(
          playbackArea->x+(AE->delta.x/fbo->w)*playbackArea->w,
          playbackArea->y+(AE->delta.y/fbo->h)*playbackArea->h,
          playbackArea->w*(AE->delta.w/fbo->w),
          playbackArea->h*(AE->delta.h/fbo->h)
         );
         Rectangle(crayons.Pick(red255),&r2);
         r2.SetRect(
          playbackArea->x+(AE->delta.x/fbo->w)*playbackArea->w,
          playbackArea->y+(AE->delta.y/fbo->h)*playbackArea->h,
          playbackArea->w*(AE->delta.w/fbo->w),
          playbackArea->h*(AE->delta.h/fbo->h)
         );
         Rectangle(crayons.Pick(red255),&r2);
        }
       break;
     }
    }
    crayons.Pick(gray).gl();
    fast.text(FORMAT("%-1.2fs %1.0f%%",
     (float)(presi->duration-presi->remaining),
     (float)((presi->duration-presi->remaining)/presi->duration)*100.0f).c_str(),
     playbackArea->x2,playbackArea->y+24
    );
   }
  }
  {
   rect.SetRect(layerSelectArea->x,layerSelectArea->y,layerSelectArea->w,layerSelectArea->h);
   wasLayerSelected=layerSelected;
   int selected=fast.list(termFont,"Layers",&layers,layerSelectArea->x,layerSelectArea->y+20,layerSelectArea->w,layerSelectArea->h/(16+4),&layerSelected.value,&layerScrolled.value);
   if ( selected >= 0 && selected != wasLayerSelected.value ) {
    v.holder->DestroyAttached(&v);     
    wasLayerSelected=selected;
    if ( selected >= 0 ) selectedLayer=(AnimationElement *)ed->elements.Element(layerSelected);
    else selectedLayer=null;
    if ( selectedLayer ) {
     if ( selectedLayer->film ) {
      FilmLibrary *library=selectedLayer->film->GetLibrary();
      va=library?library->title:"";
      fi=selectedLayer->film->title;
     } else {
      va="";
      fi="";
     }
    }
   }
   if ( fast.button("+",layerSelectArea->x,layerSelectArea->y2-20,20,20) ) {
    v.holder->DestroyAttached(&v);     
    selectedLayer=new AnimationElement;
    ed->elements.Append(selectedLayer);
    Repopulate();
    layerSelected=ed->elements.count-1;
    wasLayerSelected=layerSelected;
   }
   if ( selectedLayer ) { // Draw the Layer Edit widgets
    if ( selectedLayer->prev && fast.button(fast.upArrow,layerSelectArea->x+24,layerSelectArea->y2-20,20,20) ) {
     ed->elements.sendUp(selectedLayer);
     layerSelected=ed->elements.IndexOf(selectedLayer);
    }
    if ( selectedLayer->next && fast.button(fast.downArrow,layerSelectArea->x+48,layerSelectArea->y2-20,20,20) ) {
     ed->elements.sendDown(selectedLayer);
     layerSelected=ed->elements.IndexOf(selectedLayer);
    }
    AnimationElement *AE=selectedLayer;
    if ( AE->type == aet_NextGenVisual ) {
     RenderNextGenVisualControls(AE);
    } else {
     RenderLegacyWidgetControls(AE);
 // Zp<Film> film;
 // Zstring presentationName,slideName;
 // ScreenPosition start,end;
 // Zp<TexFont> font;
 // Zstring content;
 // PointController2d location;
 // AngleController rotation;
 // SizeController2d size;
 // UglyDrawStyle style;
 // Mulligan mulligan;
    } ///
   }
  }
  RenderRulerOverlay();
 }
 void RenderNextGenVisualControls( AnimationElement * AE ) {
  double alphaWas=0.0,alph=0.0;
  double number_delta=fast.number_delta();
  int integer_delta=fast.integer_delta();
  Cartesian pen; pen.Set(layerEditArea->x,layerEditArea->y);
  // Reactivate if needed.. only thing truly not available is the line-film
//  AE->type=(AnimationElementTypes)fast.enumerated(&typeNames,"",(int)AE->type,pen.x,pen.y-16,32,16);
  pen.MoveBy(0,-24);
  {
   AE->image=fast.imagepick(AE->image,"",&fdImage1,fImage1.self(),AE->tint,pen.x,pen.y,64,64);
   pen.MoveBy(72,0);
   alphaWas=(double)AE->tint.af;
   AE->tint=fast.colorPickerTiny(AE->tint,pen.x,pen.y,64,32);
   pen.MoveBy(0,32);
   AE->tint.Alpha(alphaWas);
   alph=(double)(AE->tint.af);
   alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
   AE->tint.Alpha(alph);
   pen.MoveBy(-8,20);
   AE->blend=fast.blend("",AE->blend,pen.x,pen.y,32,16);
   pen.MoveBy(96,-52);
  }
  {
   AE->image2=fast.imagepick(AE->image2,"",&fdImage2,fImage2.self(),AE->tint2,pen.x,pen.y,64,64);
   pen.MoveBy(72,0);
   alphaWas=(double)AE->tint2.af;
   AE->tint2=fast.colorPickerTiny(AE->tint2,pen.x,pen.y,64,32);
   pen.MoveBy(0,32);
   AE->tint2.Alpha(alphaWas);
   alph=(double)(AE->tint2.af);
   alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
   AE->tint2.Alpha(alph);
   pen.MoveBy(-8,20);
   AE->blend2=fast.blend("",AE->blend2,pen.x,pen.y,32,16);
   pen.MoveBy(96,-52);
  }
  {
   AE->image3=fast.imagepick(AE->image3,"",&fdImage3,fImage3.self(),AE->tint3,pen.x,pen.y,64,64);
   pen.MoveBy(72,0);
   alphaWas=(double)AE->tint3.af;
   AE->tint3=fast.colorPickerTiny(AE->tint3,pen.x,pen.y,64,32);
   pen.MoveBy(0,32);
   AE->tint3.Alpha(alphaWas);
   alph=(double)(AE->tint3.af);
   alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
   AE->tint3.Alpha(alph);
   pen.MoveBy(-8,20);
   AE->blend3=fast.blend("",AE->blend3,pen.x,pen.y,32,16);
   pen.MoveBy(96,-52);
  }
  {
   AE->image4=fast.imagepick(AE->image4,"",&fdImage4,fImage4.self(),AE->tint4,pen.x,pen.y,64,64);
   pen.MoveBy(72,0);
   alphaWas=(double)AE->tint4.af;
   AE->tint4=fast.colorPickerTiny(AE->tint4,pen.x,pen.y,64,32);
   pen.MoveBy(0,32);
   AE->tint4.Alpha(alphaWas);
   alph=(double)(AE->tint4.af);
   alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
   AE->tint4.Alpha(alph);
   pen.MoveBy(-8,20);
   AE->blend4=fast.blend("",AE->blend4,pen.x,pen.y,32,16);
   pen.MoveBy(108,-52);
  }
  pen.MoveBy(0,-8);
  AE->delay=fast.decimal("delay (s)",AE->delay,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  AE->deltaDuration=fast.decimal("Film Dilation Multiple",AE->deltaDuration,0.01,1000.0,1.0,pen.x+128,pen.y,32,16);
  pen.MoveBy(0,32);
  AE->duration=fast.decimal("duration (s)",AE->duration,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  AE->deltaDelay=fast.decimal("Sound FX Delay (s)",AE->deltaDelay,0.01,1000.0,1.0,pen.x+128,pen.y,32,16);
  pen.MoveBy(0,32);
  AE->inTime=fast.decimal("fade in length (s)",AE->inTime,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  AE->interval=fast.decimal("Mulligan Interval (s)",AE->interval,0.01,100.0,1.0,pen.x+128,pen.y,32,16);
  pen.MoveBy(0,32);
  AE->outTime=fast.decimal("fade out start (s)",AE->outTime,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  AE->blend=fast.blend("font",AE->blend,pen.x+128+8,pen.y,32,16);
  pen.MoveBy(0,48);
  AE->fontSize.w=fast.decimal("font w",AE->fontSize.w,0.0,10000.0,number_delta,pen.x-256+32,pen.y,32,16);
  AE->fontSize.h=fast.decimal("h",AE->fontSize.h,0.0,10000.0,number_delta,pen.x-256+128+32,pen.y,32,16);
  Blends old=fast.tblend; fast.tblend=AE->blend;
  AE->font=fast.fontSelect(AE->font,pen.x-256+32,pen.y-32,13*7,16);
  fast.tblend=old;
  AE->angle=fast.angleDegrees(AE->angle,pen.x+24+16,pen.y);
  AE->spin=fast.angleDegrees(AE->spin,pen.x+24+16+96,pen.y);
  pen.MoveBy(0,32);
  AE->fontSizeB.w=fast.decimal("to font w",AE->fontSizeB.w,0.0,10000.0,number_delta,pen.x-256+32,pen.y,32,16);
  AE->fontSizeB.h=fast.decimal("h",AE->fontSizeB.h,0.0,10000.0,number_delta,pen.x-256+128+32,pen.y,32,16);
  fast.text("<a",pen.x,pen.y-16-(int)fast.fontSize.y);
  if ( fast.button("Reset",pen.x,pen.y,48,24) ) AE->angle=0.0;
  if ( fast.button("=",pen.x+48,pen.y,24,24) ) AE->angle=AE->spin;
  fast.text("<b",pen.x+96,pen.y-16-(int)fast.fontSize.y);
  if ( fast.button("Reset",pen.x+96,pen.y,48,24) ) AE->spin=0.0;
  if ( fast.button("=",pen.x+96+48,pen.y,24,24) ) AE->spin=AE->angle;
  pen.MoveBy(-256+48-32,32); 
  fast.xywhi(&AE->extents,pen.x,pen.y,-10000,10000,"E | Start");
  if ( fast.vwindows->focused == &v && input.KeyDown(DX_E) ) AE->extents.Set(&copiedCoordinate);
  pen.MoveBy(0,40);
  fast.xywhi(&AE->delta,pen.x,pen.y,-10000,10000,"F | Ending");
  if ( fast.vwindows->focused == &v && input.KeyDown(DX_F) ) AE->delta.Set(&copiedCoordinate);
  pen.MoveBy(0,32+8);
  fast.arrows("Pivot",&AE->pivot,pen.x+24,pen.y+24,16);
  fast.arrows("Velocity",&AE->B,pen.x+24+108,pen.y+24,16);
  if ( fast.squarebutton(fast.particlesIcon,fast.rounded,pen.x+24+108+108,pen.y+8,32,32,fast.color,fast.excited,fast.deactivated) ) {
   VirtualWindowHandle *existing=v.holder->Editing(&AE->sound);
   if ( existing ) {
    v.holder->SlideTo(existing->p);
   } else {
    MulliganEditor *me=new MulliganEditor(&AE->mulligan);
    v.holder->Add(&me->v);
    me->window.SetRect( x+w+16,y-v.titlebar.h,768,768 );
    me->v.Editing(&AE->mulligan);
    me->Setup();
    me->Name((this->v.title+string(" > Mulligan")).c_str());
    me->v.Attach(&v,pen.x+24+108+108+8-x,pen.y+8-y);
    v.holder->SlideTo(&me->v);
    return;
   }
  }
  if ( fast.squarebutton(fast.soundIcon,fast.rounded,pen.x+24+108+108+32+4,pen.y+8,32,32,fast.color,fast.excited,fast.deactivated) ) {
   VirtualWindowHandle *existing=v.holder->Editing(&AE->sound);
   if ( existing ) {
    v.holder->SlideTo(existing->p);
   } else {
    SoundEffectEditor *sfe=new SoundEffectEditor(&AE->sound);
    v.holder->Add(&sfe->v);
    sfe->v.Editing(&AE->sound);
    sfe->window.SetRect( x-512-16,y,512,512 );
    sfe->Setup();
    sfe->Name((this->v.title+string(" > Sound")).c_str());
    sfe->hideAll=true;
    sfe->v.Attach(&v,pen.x+24+108+108+32+4+8-x,pen.y+8-y);
    v.holder->SlideTo(&sfe->v);
    return;
   }
  }
  if ( fast.squarebutton(fast.curveIcon,fast.rounded,pen.x+24+108+108+32+4+32+4,pen.y+8,32,32,fast.color,fast.excited,fast.deactivated) ) {
   VirtualWindowHandle *existing=v.holder->Editing(&AE->curves);
   if ( existing ) {
    v.holder->SlideTo(existing->p);
   } else {
    CurveEditor *ce=new CurveEditor(&AE->curves);
    v.holder->Add(&ce->v);
    ce->v.Editing(&AE->curves);
    ce->window.SetRect( x-786-8,y,768,512+32 );
    ce->Setup();
    ce->Name( (this->v.title+string(" ")+string(I2S(ed->elements.IndexOf(selectedLayer)))+string(" > Curves Path")).c_str() );
    ce->v.Attach(&v,pen.x+24+108+108+32+4+8-x,pen.y+8-y);
    v.holder->SlideTo(&ce->v);
    return;
   }
  }
  if ( fast.squarebutton(fast.quadIcon,fast.rounded,pen.x+24+108+108+64+8+32+4,pen.y+8,32,32,fast.color,fast.excited,fast.deactivated) ) {
   VirtualWindowHandle *existing=v.holder->Editing(&AE->q);
   if ( existing ) {
    v.holder->SlideTo(existing->p);
   } else {
    QuadEditor *qe=new QuadEditor(&AE->q);
    v.holder->Add(&qe->v);
    qe->v.Editing(&AE->q);
    qe->window.SetRect( x-786-8,y,768,512+32 );
    qe->Setup();
    qe->Name( (this->v.title+string(" ")+string(I2S(ed->elements.IndexOf(selectedLayer)))+string(" > Quad Area")).c_str() );
    qe->v.Attach(&v,pen.x+24+108+108+32+4+8-x,pen.y+8-y);
    v.holder->SlideTo(&qe->v);
    return;
   }
  }
  if ( fast.squarebutton(fast.animIcon,fast.rounded,pen.x+24+108+108+128+16,pen.y+8,32,32,fast.color,fast.excited,fast.deactivated) ) {
   VirtualWindowHandle *existing=v.holder->Editing(AE);
   if ( existing ) {
    v.holder->SlideTo(existing->p);
   } else {
    ZstringEditor *qe=new ZstringEditor(AE,AE->presentationName.self());
    v.holder->Add(&qe->v);
    qe->v.Editing(AE);
    qe->window.SetRect( x-786-8,y,512,96 );
    qe->Setup();
    qe->Name( (this->v.title+string(" ")+string(I2S(ed->elements.IndexOf(selectedLayer)))+string(" > Presentation Name Reference")).c_str() );
    qe->v.Attach(&v,pen.x+24+108+108+32+4+8-x,pen.y+8-y);
    v.holder->SlideTo(&qe->v);
    return;
   }
  }
  if ( fast.squarebutton(fast.textIcon,fast.rounded,pen.x+24+108+108+128+32+16,pen.y+8,32,32,fast.color,fast.excited,fast.deactivated) ) {
   VirtualWindowHandle *existing=v.holder->Editing(AE);
   if ( existing ) {
    v.holder->SlideTo(existing->p);
   } else {
    ZstringEditor *qe=new ZstringEditor(AE,AE->content.self());
    v.holder->Add(&qe->v);
    qe->v.Editing(AE);
    qe->lines=5;
    qe->window.SetRect( x-786-8,y,512+256,96+32*4 );
    qe->Setup();
    qe->Name( (this->v.title+string(" ")
     +string(I2S(ed->elements.IndexOf(selectedLayer)))
     +string(" > Text")).c_str() 
    );
    qe->v.Attach(&v,pen.x+24+108+108+32+4+4*32+16-x,pen.y+16-y);
    v.holder->SlideTo(&qe->v);
    return;
   }
  }
  pen.MoveBy(0,48+8);
  AE->fireOnce=fast.toggle("Isotrap",AE->fireOnce,pen.x,pen.y,16,16);
  AE->erratic_flaresource=fast.toggle("Outward",AE->erratic_flaresource,pen.x+88,pen.y,16,16);
  AE->circular=fast.toggle("Circular",AE->circular,pen.x+88*2,pen.y,16,16);
  AE->filled=fast.toggle("Edges",AE->filled,pen.x+88*3,pen.y,16,16);
  AE->flickerOut=fast.toggle("Path",AE->flickerOut,pen.x+88*3+64,pen.y,16,16);
  AE->useQuad=fast.toggle("Quad",AE->useQuad,pen.x+88*3+64,pen.y+24,16,16);
  AE->xreversed=fast.toggle("2X",AE->xreversed,pen.x+88*3+64*2,pen.y,16,16);
  AE->placeBehind=fast.toggle("Under",AE->placeBehind,pen.x+88*3+64*2,pen.y+24,16,16);
  pen.MoveBy(0,24);
  AE->inTween=fast.tween(AE->inTween,pen.x,pen.y,96,16);
  AE->midTween=fast.tween(AE->midTween,pen.x+96,pen.y,96,16);
  AE->outTween=fast.tween(AE->outTween,pen.x+96*2,pen.y,96,16);
   ///
  pen.Set(layerEditArea->x,layerEditArea->y);
  pen.MoveBy(0,96-32-10); 
  AE->film=fast.FilmPicker(termFont,&va.value,&fi.value,&filmPickInstant.value,&filmPickScrolled.value,pen.x+16,pen.y+32+16,384,232);
  Crayon b; b.Int(32,32,48,255);
  AE->tintIn=fast.colorPickerTiny(AE->tintIn,pen.x+16+128+64,pen.y+256+16,96,24);
  AE->blend5=fast.blend("",AE->blend5,pen.x+16+128+64+96-12,pen.y+256+16,32,16);
  { // Draw the layered timeline widgetry
   Cartesian rect;
   rect.Corners((int)layerEditArea->x+4,(int)layerEditArea->y2-128,(int)layerEditArea->x2,(int)layerEditArea->y2);
   Area(b,&rect);
   b.Int(127,127,127,255);
   int slideWidth=rect.w-64;
   int slideEndX=rect.x+rect.w-48;
   DrawLine(b,slideEndX,rect.y,slideEndX,rect.y2);
   Text( "S\nL\nI\nD\nE\n\nE\nN\nD", (double) rect.x2-16+4.0, (double)(rect.y+16), 11.0, 8.0, 5.0, false );
   if ( selectedLayer ) {
    fast.hrange("\nin-out times",&selectedLayer->inTime.value,&selectedLayer->outTime.value,0.0,selectedLayer->duration,rect.x+16,rect.y-48,slideWidth,16,true);
    selectedLayer->delay=fast.hslider("",selectedLayer->delay.value,0.0,ed->duration.value,rect.x+16,rect.y-24,slideWidth,24,true);
   }
   if ( ed->elements.count > 7 ) {
    if ( layerAreaScrolled > 0 && fast.button(fast.upArrow,rect.x2-16,rect.y,16,16) ) {
     layerAreaScrolled--;
    }
    if ( layerAreaScrolled <= UMAX(ed->elements.count-7,0) && fast.button(fast.downArrow,rect.x2-16,rect.y2-16,16,16) ) {
     layerAreaScrolled++;
    }
   } else layerAreaScrolled=0;
   int i=0;
   int sy=rect.y;
   int sh=16;
   EACHN(ed->elements.first,AnimationElement,AE,{
    if ( i < layerAreaScrolled ) { i++; continue; }
    if ( fast.button("X",(int)rect.x,sy,sh,sh,selectedLayer==AE)) {
     v.holder->DestroyAttached(&v);     
     ed->elements.Remove(selectedLayer);
     selectedLayer.Delete();
     selectedLayer=null;
     layerSelected=-1;
     Repopulate();
     return;
    }
    if ( fast.button("",(int)rect.x+sh,sy,slideWidth,sh,selectedLayer==AE) ) {
     selectedLayer=AE;
     layerSelected=ed->elements.IndexOf(selectedLayer);
    }
    if ( fast.button(fast.upArrow,(int)rect.x2-sh*3,sy,sh,sh) ) {
     ed->elements.sendUp(AE,true);
     layerSelected=ed->elements.IndexOf(selectedLayer);
     if ( ed->elements.last == AE ) layerAreaScrolled = ed->elements.count-7;
     else layerAreaScrolled--;
     return;
    }
    if ( layerAreaScrolled >= 0 && fast.button(fast.downArrow,(int)rect.x2-sh*2,sy,sh,sh) ) {
     ed->elements.sendDown(AE,true);
     layerSelected=ed->elements.IndexOf(selectedLayer);
     return;
    }
    glLineWidth(2.0);
    int animWidth=(int)((double)slideWidth*(AE->duration/ed->duration));
    int animOffset=(int)((double)slideWidth*(AE->delay/ed->duration));
    Rectangle(crayons.Pick(orange),rect.x+sh+animOffset,sy+2,UMIN(rect.x+sh+animOffset+animWidth,rect.x2),sy+sh-2);
    glLineWidth(1.0);
    sy+=sh;
    i++;
    if ( sy+sh > (int) layerEditArea->y2 ) break;
   });
  }
 }
 void RenderLegacyWidgetControls( AnimationElement *AE ) {
  double alphaWas=0.0,alph=0.0;
  double number_delta=fast.number_delta();
  int integer_delta=fast.integer_delta();
  Cartesian pen; pen.Set(layerEditArea->x,layerEditArea->y);
  AE->type=(AnimationElementTypes)fast.enumerated(&typeNames,"",(int)AE->type,pen.x,pen.y-16,32,16);
  pen.MoveBy(0,20);
  {
   AE->image=fast.imagepick(AE->image,"",&fdImage1,fImage1.self(),AE->tint,pen.x,pen.y,64,64);
   pen.MoveBy(72,0);
   alphaWas=(double)AE->tint.af;
   AE->tint=fast.colorPickerTiny(AE->tint,pen.x,pen.y,64,32);
   pen.MoveBy(0,32);
   AE->tint.Alpha(alphaWas);
   alph=(double)(AE->tint.af);
   alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
   AE->tint.Alpha(alph);
   pen.MoveBy(-8,20);
   AE->blend=fast.blend("",AE->blend,pen.x,pen.y,32,16);
   pen.MoveBy(-64,64+16-32);
  }
  {
   if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed || AE->type == aet_Glow2 )
    AE->image2=fast.imagepick(AE->image2,"",&fdImage2,fImage2.self(),AE->tint2,pen.x,pen.y,64,64);
   pen.MoveBy(72,0);
   alphaWas=(double)AE->tint2.af;
   AE->tint2=fast.colorPickerTiny(AE->tint2,pen.x,pen.y,64,32);
   pen.MoveBy(0,32);
   AE->tint2.Alpha(alphaWas);
   alph=(double)(AE->tint2.af);
   alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
   AE->tint2.Alpha(alph);
   pen.MoveBy(-8,20);
   AE->blend2=fast.blend("",AE->blend2,pen.x,pen.y,32,16);
   pen.MoveBy(-64,64+16-32);
  }
  {
   if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed || AE->type == aet_Glow2 )
    AE->image3=fast.imagepick(AE->image3,"",&fdImage3,fImage3.self(),AE->tint3,pen.x,pen.y,64,64);
   pen.MoveBy(72,0);
   alphaWas=(double)AE->tint3.af;
   AE->tint3=fast.colorPickerTiny(AE->tint3,pen.x,pen.y,64,32);
   pen.MoveBy(0,32);
   AE->tint3.Alpha(alphaWas);
   (double)(AE->tint3.af);
   alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
   AE->tint3.Alpha(alph);
   pen.MoveBy(-8,20);
   if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed || AE->type == aet_Glow2 )
    AE->blend3=fast.blend("",AE->blend3,pen.x,pen.y,32,16);
   pen.MoveBy(-64,64+16-32);
  }
  {
   if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed || AE->type == aet_Glow2 )
    AE->image4=fast.imagepick(AE->image4,"",&fdImage4,fImage4.self(),AE->tint4,pen.x,pen.y,64,64);
   pen.MoveBy(72,0);
   alphaWas=(double)AE->tint4.af;
   AE->tint4=fast.colorPickerTiny(AE->tint4,pen.x,pen.y,64,32);
   pen.MoveBy(0,32);
   AE->tint4.Alpha(alphaWas);
   alph=(double)(AE->tint4.af);
   alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
   AE->tint4.Alpha(alph);
   pen.MoveBy(-8,20);
   if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed )
    AE->blend4=fast.blend("",AE->blend4,pen.x,pen.y,32,16);
   pen.MoveBy(-64,64+16-32);
  }
  alphaWas=(double)AE->tintOut.af;
  AE->tintOut=fast.colorPickerTiny(AE->tintOut,pen.x,pen.y,64,20);
  pen.MoveBy(72,0);
  AE->tintOut.Alpha(alphaWas);
  alph=(double)(AE->tintOut.af);
  alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
  AE->tintOut.Alpha(alph);
  pen.MoveBy(-72,24);
  alphaWas=(double)AE->tintIn.af;
  AE->tintIn=fast.colorPickerTiny(AE->tintIn,pen.x,pen.y,64,20);
  pen.MoveBy(72,0);
  AE->tintIn.Alpha(alphaWas);
  alph=(double)(AE->tintIn.af);
  alph=fast.hslider("",alph,0.0,1.0,pen.x,pen.y,64,16,true);
  AE->tintIn.Alpha(alph);
  pen.MoveBy(-72,24);
  if ( AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid || AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed || AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid )
   AE->tintStart=fast.colorPickerTiny(AE->tintStart,pen.x,pen.y,64,20);
  pen.MoveBy(0,24);
  if ( AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid || AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed || AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid )
   AE->tintEnd=fast.colorPickerTiny(AE->tintEnd,pen.x,pen.y,64,20);
  pen.MoveBy(0,24);
  AE->interior=fast.colorPickerTiny(AE->interior,pen.x,pen.y,64,20);
  pen.Set(layerEditArea->x+64+96+6,layerEditArea->y);
  if ( AE->type == aet_Aid )
   AE->xreversed=fast.toggle("x-reversed",AE->xreversed,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_Glow || AE->type == aet_Glow2 )
   AE->erratic_flaresource=fast.toggle("erratic flare",AE->erratic_flaresource,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_Glow || AE->type == aet_Glow2 || AE->type == aet_DriftingRotatingVisualAid || AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type==aet_DriftingVisualAid || AE->type==aet_WanderingVisualAid || AE->type==aet_WobblingVisualAid )
   AE->tweenW=fast.toggle("mod width",AE->tweenW,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_Glow || AE->type == aet_Glow2 || AE->type == aet_DriftingRotatingVisualAid || AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type==aet_DriftingVisualAid || AE->type==aet_WanderingVisualAid || AE->type==aet_WobblingVisualAid )
   AE->tweenH=fast.toggle("mod height",AE->tweenH,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_WriteText || AE->type == aet_MatrixText || AE->type == aet_Text || AE->type==aet_TexFont || AE->type==aet_TexFontFader )
   AE->centered=fast.toggle("center text",AE->centered,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed )
   AE->flickerOut=fast.toggle("flicker out",AE->flickerOut,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed )
   AE->perpetuate=fast.toggle("perpetuate",AE->perpetuate,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated || AE->type == aet_VisualSkewed )
   AE->repeats=fast.toggle("repeats",AE->repeats,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  AE->fireOnce=fast.toggle("fire once",AE->fireOnce,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_Mulligan )
   AE->rectangular=fast.toggle("rectangular",AE->rectangular,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_Mulligan )
   AE->circular=fast.toggle("circular",AE->circular,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_Box || AE->type == aet_Circle )
   AE->filled=fast.toggle("filled",AE->filled,pen.x,pen.y,12,12);
  pen.MoveBy(0,20);
  if ( AE->type == aet_WriteText || AE->type == aet_MatrixText || AE->type == aet_Text || AE->type==aet_TexFont || AE->type==aet_TexFontFader )
   AE->useRelativeGlyphWidths=fast.toggle("relative glyphs",AE->useRelativeGlyphWidths,pen.x,pen.y,12,12);
  pen.MoveBy(0,32);
  if ( AE->type == aet_FilmLineFader|| AE->type == aet_FilmIsotrapf || AE->type == aet_FilmLoop || AE->type == aet_FilmLoopFader ) 
   AE->inTween=fast.tween(AE->inTween,pen.x,pen.y,64,64);
  pen.MoveBy(64+2,0);
  if ( AE->type == aet_FilmLineFader|| AE->type == aet_FilmIsotrapf || AE->type == aet_FilmLoop || AE->type == aet_FilmLoopFader ) 
   AE->outTween=fast.tween(AE->outTween,pen.x,pen.y,64,64);
  pen.Set(layerEditArea->x+64+16+256-32,layerEditArea->y);
  AE->tweenLR=fast.decimal("left-right",AE->tweenLR,-1024,1024,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  AE->tweenUD=fast.decimal("up-down",AE->tweenUD,-1024,1024,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_Glow || AE->type == aet_Glow2 || AE->type == aet_Glow3 )
   AE->maxOp=fast.decimal("maxOp",AE->maxOp,-1024,1024,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  AE->duration=fast.decimal("duration",AE->duration,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_FilmLineFader || AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated )
   AE->inTime=fast.decimal("inTime",AE->inTime,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_FilmLineFader || AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated )
   AE->outTime=fast.decimal("outTime",AE->outTime,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  AE->delay=fast.decimal("delay",AE->delay,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_TexFontFader || AE->type == aet_TexFont )
   AE->deltaDelay=fast.decimal("deltaDelay",AE->deltaDelay,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_TexFontFader || AE->type == aet_TexFont )
   AE->deltaDuration=fast.decimal("deltaDuration",AE->deltaDuration,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_Circle ) 
   AE->segments=fast.integer("segments",AE->segments,1,128,1,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_Box || AE->type == aet_Circle || AE->type == aet_WriteText || AE->type == aet_MatrixText || AE->type == aet_Text || AE->type==aet_TexFont || AE->type==aet_TexFontFader )
   AE->thickness=(float)fast.decimal("thickness",(double)AE->thickness,0.0,100000.0,1.0,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
///    AE->interval=fast.decimal("interval",AE->interval,0.0,100000.0,1.0,pen.x,pen.y,32,16);
  pen.MoveBy(0,32-8);
  fast.xywh(&AE->extents,pen.x,pen.y,-10000.0,10000.0,"(E)xtents");
  if ( fast.vwindows->focused == &v && input.KeyDown(DX_E) ) AE->extents.Set(&copiedCoordinate);
  pen.MoveBy(0,32+4);
///    fast.xywh(&AE->B,pen.x,pen.y,-10000.0,10000.0,"point B");
  pen.MoveBy(0,32+4);
///    fast.xywh(&AE->C,pen.x,pen.y,-10000.0,10000.0,"point C");
  pen.MoveBy(0,32+4);
///    fast.xywh(&AE->D,pen.x,pen.y,-10000.0,10000.0,"point D");
  pen.Set(layerEditArea->x+64+16+256+128-64,layerEditArea->y);
  if ( AE->type == aet_Box )
   AE->pulseSpeed=(float)fast.decimal("pulse speed",(double)AE->pulseSpeed,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_DriftingRotatingVisualAid || AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid || AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid )
   AE->wanderSpeed=(float)fast.decimal("wander speed",(double)AE->wanderSpeed,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_DriftingRotatingVisualAid || AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid || AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid )
   AE->wanderSpeedMinimum=(float)fast.decimal("wander min",(double)AE->wanderSpeedMinimum,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_DriftingRotatingVisualAid || AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid || AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid )
   AE->wanderSpeedLimit=(float)fast.decimal("wander limit",(double)AE->wanderSpeedLimit,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_DriftingRotatingVisualAid || AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid || AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid )
   AE->wanderAccel=(float)fast.decimal("wander accel",(double)AE->wanderAccel,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  AE->tinting=(float)fast.decimal("tinting",(double)AE->tinting,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid )
   AE->rate=(float)fast.decimal("rate",(double)AE->rate,0.0,100000.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_DriftingRotatingVisualAid || AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid || AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid || AE->type == aet_VisualRotated )
   AE->angle=(float)fast.angleDegrees((double)AE->angle,pen.x,pen.y);
  pen.MoveBy(0,32);
  if ( AE->type == aet_DriftingRotatingVisualAid || AE->type == aet_DriftingStretchingRotatingVisualAid || AE->type == aet_DriftingStretchingVisualAid || AE->type == aet_DriftingVisualAid || AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid || AE->type == aet_VisualRotated )
   AE->spin=(float)fast.decimal("spin",(double)AE->spin,-180.0,180.0,number_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated )
   AE->opacityStart=fast.integer("opacityStart",AE->opacityStart,0,255,integer_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated )
   AE->opacityMid=fast.integer("opacityMid",AE->opacityMid,0,255,integer_delta,pen.x,pen.y,32,16);
  pen.MoveBy(0,32);
  if ( AE->type == aet_VisualAid || AE->type == aet_VisualAid2 || AE->type == aet_VisualRotated )
   AE->opacityEnd=fast.integer("opacityEnd",AE->opacityEnd,0,255,integer_delta,pen.x,pen.y,32,16);
  pen.Set(layerEditArea->x+512-16,layerEditArea->y-16);
  fast.xywh(&AE->delta,pen.x,pen.y,-10000.0,10000.0,"del(T)a size");
  if ( fast.vwindows->focused == &v && input.KeyDown(DX_T) ) AE->delta.Set(&copiedCoordinate);
  pen.MoveBy(0,32+4);
  fast.xy(&AE->pivot,pen.x,pen.y,-10000,10000,"pivot");
  pen.MoveBy(0,32+4);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->sA,pen.x,pen.y,-10000,10000,"sA (S+A)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_S) && input.KeyDown(DX_A) ) AE->sA.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(164,0);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->sB,pen.x,pen.y,-10000,10000,"sB (S+B)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_S) && input.KeyDown(DX_B) ) AE->sB.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(-164,32+4);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->sC,pen.x,pen.y,-10000,10000,"sC (S+C)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_S) && input.KeyDown(DX_C) ) AE->sC.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(164,0);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->sD,pen.x,pen.y,-10000,10000,"sD (S+D)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_S) && input.KeyDown(DX_D) ) AE->sD.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(-164,32+4);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->pA,pen.x,pen.y,-10000,10000,"pA (P+A)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_P) && input.KeyDown(DX_A) ) AE->pA.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(164,0);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->pB,pen.x,pen.y,-10000,10000,"pB (P+B)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_P) && input.KeyDown(DX_B) ) AE->pB.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(-164,32+4);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->pC,pen.x,pen.y,-10000,10000,"pC (P+C)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_P) && input.KeyDown(DX_C) ) AE->pC.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(164,0);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->pD,pen.x,pen.y,-10000,10000,"pD (P+D)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_P) && input.KeyDown(DX_D) ) AE->pD.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(-164,32+4);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->dA,pen.x,pen.y,-10000,10000,"dA (D+A)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_D) && input.KeyDown(DX_A) ) AE->dA.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(164,0);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->dB,pen.x,pen.y,-10000,10000,"dB (D+B)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_D) && input.KeyDown(DX_B) ) AE->dB.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(-164,32+4);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->dC,pen.x,pen.y,-10000,10000,"dC (D+C)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_D) && input.KeyDown(DX_C) ) AE->dC.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(164,0);
  if ( AE->type == aet_VisualSkewed ) {
   fast.xy(&AE->dD,pen.x,pen.y,-10000,10000,"dD (D+F)");
   if ( fast.vwindows->focused == &v && input.KeyDown(DX_D) && input.KeyDown(DX_F) ) AE->dD.SetRect((int)copiedCoordinate.x,(int)copiedCoordinate.y,(int)copiedCoordinate.w,(int)copiedCoordinate.h);
  }
  pen.MoveBy(-164,32+4);
  fast.xywh(&AE->fontSize,pen.x,pen.y,0.0,1000.0,"font size");
  pen.MoveBy(0,32+4);
  if ( AE->type == aet_WanderingVisualAid || AE->type == aet_WobblingVisualAid )
   fast.xywh(&AE->wrapLimit,pen.x,pen.y,-5000,5000,"edge wrap");
  pen.MoveBy(0,32+16);
  if ( AE->type == aet_MovingLine ) {
   AE->a=fast.integer("A",AE->a,-5000,5000,1,pen.x,pen.y,32,16);
   AE->b=fast.integer("B",AE->b,-5000,5000,1,pen.x+80,pen.y,32,16);
   AE->c=fast.integer("C",AE->c,-5000,5000,1,pen.x+80*2,pen.y,32,16);
   AE->d=fast.integer("D",AE->d,-5000,5000,1,pen.x+80*3,pen.y,32,16);
  }
  pen.MoveBy(96*2,32+4);
  if ( AE->type == aet_WriteText || AE->type == aet_MatrixText || AE->type == aet_Text || AE->type==aet_TexFont || AE->type==aet_TexFontFader )
   AE->maxChars=fast.integer("max chars",AE->maxChars,0,5000,1,pen.x,pen.y,32,16);
  pen.MoveBy(0,32+4);
  if ( AE->type == aet_WriteText || AE->type == aet_MatrixText || AE->type == aet_Text || AE->type==aet_TexFont || AE->type==aet_TexFontFader )
   AE->lineSpace=fast.integer("line space",AE->lineSpace,0,5000,1,pen.x,pen.y,32,16);
  pen.MoveBy(0,32+4);
  if ( AE->type == aet_WriteText || AE->type == aet_MatrixText || AE->type == aet_Text || AE->type==aet_TexFont || AE->type==aet_TexFontFader )
   AE->lines=fast.integer("lines",AE->lines,0,5000,1,pen.x,pen.y,32,16);
  pen.Set(layerEditArea->x+512+96*3,layerEditArea->y+32+4);
  if ( input.KeyDown(DX_F8) ) {
   fbo1080p3.Render(0,0,512,512);
  }
 }
 void Between() {
 }
MANY(SlideProfileEditor,SlideProfileEditorHandle,SlideProfileEditorHandles,"SlideProfileEditor",SlideProfileEditors,{})
DONE(SlideProfileEditor);

///////////////////////
// Presentation Profile
///////////////////////
ONE(PresentationProfile,{
 y1=0.25;
 timescale=1.0;
 duration=1.0;
 name="New";
 background.Pick(black);
 extents.SetRect(0,0,3840,2160);
 blend=transparency;
})
 SlideProfiles slideProfiles;
 Crayon background;
 Blends blend;
 Cartesian extents;
 Zstring path;
 Zdouble duration;
 Zbool doNotPauseWhenNotVisible;
 Zbool looping,freezeLast;
 Zbool pauses;
 Zbool letterbox;
 Zdouble y1;
 Zdouble timescale;
 Zbool startPaused;
 Zstring seekByName; // Seek by slide #
 KEYWORDS({
  TXTWORD("name",name)
  else SUBWORD("slides",slideProfiles)
  else TINTWORD("bg",background)
  else KEYWORD("bl",{blend=w;})
  else SUBWORD("ext",extents)
  else KEYWORD("path",{path=w;})
  else NUMWORD("duration",duration)
  else TAGWORD("nopause",doNotPauseWhenNotVisible)
  else TAGWORD("loop",looping)
  else TAGWORD("freeze",freezeLast)
  else TAGWORD("pauses",pauses)
  else TAGWORD("LB",letterbox)
  else NUMWORD("y1",y1)
  else NUMWORD("timescale",timescale)
  else TAGWORD("startpaused",startPaused)
  else TXTWORD("to",seekByName)
  else BADKEY("PresentationProfile")
 })
 WORDKEYS({
  TXTKEY("name",name)
  SSUBKEY("slides",slideProfiles)
  TINTKEY("bg",background)
  BLENDKEY("bl",blend)
  SSUBKEY("ext",extents)
  WORDKEY("path",path)
  NUMBKEY("duration",duration)
  TAGKEY("nopause",doNotPauseWhenNotVisible)
  TAGKEY("loop",looping)
  TAGKEY("freeze",freezeLast)
  TAGKEY("pauses",pauses)
  TAGKEY("LB",letterbox)
  NUMBKEY("y1",y1)
  NUMBKEY("timescale",timescale)
  TAGKEY("startpaused",startPaused)
  TXTKEY("to",seekByName)
 })
 Presentation *Create( bool debugMode=false, int recursionDepth=0 ) {
  if ( recursionDepth > 5 ) return null;
  Presentation *p=new Presentation;
  p->background.fromCrayon(background);
  p->fbo=null;
  p->SetPositionAndSize(extents.x,extents.y,extents.w,extents.h);
  p->doNotPauseWhenNotVisible=doNotPauseWhenNotVisible;
  p->duration=duration;
  p->y1=y1;
  p->timescale=timescale;
  p->paused=startPaused;
  p->Seek(seekByName.integer());
  EACH(slideProfiles.first,SlideProfile,sp) sp->CreateSlide(p,debugMode,recursionDepth);
  return p;
 }
 void Save( const char *filepath ) {
  Zstring output=toString();
  output("ele {","\n#----\nele {");
  output("Slide {","\n#----------------\nslide {");
  string_as_file(output.c_str(),filepath);
 }
MANY(PresentationProfile,PresentationProfileHandle,PresentationProfileHandles,"Presentation",PresentationProfiles,{})
 KEYWORDSGroup("Presi",PresentationProfile)
 WORDKEYSGroup("Presi",PresentationProfile)
 void Save(const char *path) {
  int i=0;
  FOREACH(PresentationProfile,p) {
   p->Save((string(path)+string("presi_")+I2S(i)+string(".txt")).c_str());
   i++;
  }
 }
 void Load(const char *path) {
  Zdis<Strings> files;
  files.Recycle(ls(path));
  EACH(files->first,String,f) {
   PresentationProfile *pp=new PresentationProfile;
   pp->fromString(file_as_string(f->s.c_str()).c_str());
   Append(pp);
  }
 }
DONE(PresentationProfile);

extern PresentationProfiles presentationProfiles;

C_ONE(PresentationFullscreenPreview,GLWindow,{})
 Zp<GLWindow> back_to;
 Zp<PresentationProfile> presiprofi;
 Zp<Presentation> presi;
 Zp<FBOColorDepth> fbo;
 PresentationFullscreenPreview(PresentationProfile *p, GLWindow *back_to) : GLWindow() {
  this->back_to=back_to;
  presi=p->Create();
  presi->loop=p->looping;
  presi->freezeLast=p->freezeLast;
  presiprofi=p;
  back_to->visible=false;
  fbo=&fbo1080p;
 }
 void OnLoad() {
  Fullscreen();
  background.fromCrayon(presiprofi->background);
  presi->Init(false);
 }
 void OnDestroy() {
  if ( presi ) {
   presi.Delete();
   presi=null;
  }
 }
 void Between() {
  if ( input->KeyDown(DX_ESCAPE) ) {
   back_to->visible=true;
   destroyMe=true;
   return;
  }
  presi->Between();
  presi->BetweenFrames();
  if ( presi->looped > 0 ) {
   presi.Delete();
   presi=presiprofi->Create();
   presi->loop=presiprofi->looping;
   presi->freezeLast=presiprofi->freezeLast;
   presi->Init(false);
  }
 }
 void Render() {
  fbo->background.Pick(clear);
  fbo->Bind();
  presi->Render();
  fbo->Unbind();
  fbo->blend=presiprofi->blend;
  fbo->tint.Pick(alabaster);
  fbo->RenderUpsideDown(x,y,w,h);
 }
MANY(PresentationFullscreenPreview,PresentationFullscreenPreviewHandle,PresentationFullscreenPreviewHandles,"PresentationFullscreenPreview",PresentationFullscreenPreviews,{})
DONE(PresentationFullscreenPreview);


C_ONE(PresentationProfileEditor,FormEditorWindow,{})
 Zdis<Presentation> presi;
 Zp<PresentationProfile> ed;
 Zp<SlideProfile> edSlide;
 Zp<few_Reserved> slidePickerArea,playbackArea,resetTimeScaleButton,playButton,rewindButton,selectFullscreenButton,playFullscreenButton,copyButtonArea,moveRectArea,clearBGArea,blackBG,slideDuplicateButton,slideDeleteButton,copySlideButton,pasteSlideButton;
 Zstring path;
 Zp<GLImage> playIcon,rewindIcon,stopIcon,playFullscreenIcon,selectFullscreenIcon;
 Zp<FBOColorDepth> fbo;
 Zbool lineMode;
 Zbool clickedForRect,showRulerOverlay;
 Zbool clickedForLine;
 Line clickedLine,virtualLine;
 ScreenPosition clickedRect;
 Cartesiand mousePos,viewPos,virtualPos,virtualRect; 
// Zp<FormEditorWidget> startingSeek; // handle so we can update the max by the slide count
 PresentationProfileEditor( PresentationProfile *pp, MoveableWindowManager *mwm, const char *path="data/workspaces/presentations/" ) : FormEditorWindow(mwm) {
  this->path=path;
  showRulerOverlay=true;
  ed=pp;
  playIcon=library.Load("data/images/icon3/play.png");
  rewindIcon=library.Load("data/images/icon3/rewind.png");
  stopIcon=library.Load("data/images/icon3/stop.png");
  playFullscreenIcon=library.Load("data/images/icon3/play_full.png");
  selectFullscreenIcon=library.Load("data/images/icon3/select_full.png");
  fbo=&fbo1080p2;
  PopulateWidgets();
//  startingSeek=Bind(&prpr->startingSeek.value);
//  f->label="Seek Slide #";
 }
 void PopulateWidgets() {
  v.Associate(ed);
  widgets.Clear();
  playbackArea=Reserve(1,800,600);
  slidePickerArea=Reserve(2,800,96);
  slideDuplicateButton=Reserve(12,128,16);
  slideDeleteButton=Reserve(13,128,16);
  FormEditorWidget *f=null;
  f=(FormEditorWidget *) Bind(true,&ed->y1.value,0.1,100.0,true);       f->label="Letterbox coverage %";
  f=(FormEditorWidget *) Bind(&ed->letterbox.value);                    f->label="Letterbox";
  f=(FormEditorWidget *) Bind(&ed->looping.value);                      f->label="Looping";
  f=(FormEditorWidget *) Bind(&ed->freezeLast.value);                   f->label="Pause At End";
  f=(FormEditorWidget *) Bind(&ed->startPaused.value);                  f->label="Start Paused";
  f=(FormEditorWidget *) Bind(&ed->pauses.value);                       f->label="Pauses";
  f=(FormEditorWidget *) Bind(&ed->doNotPauseWhenNotVisible.value);     f->label="Continuous Offscreen";
  f=(FormEditorWidget *) Bind(&ed->name.value);                         f->label="Name";
  f=(FormEditorWidget *) Bind(&ed->seekByName.value);                   f->label="Starting Slide";
  f=(FormEditorWidget *) Bind(true,&ed->timescale.value,0.1,10.0,true); f->label="Time scale (1.0 = realtime)";
  resetTimeScaleButton=Reserve(10,48,32);
  f=(FormEditorWidget *) Bind(&ed->background,false,true);              f->label="BG";
  f=(FormEditorWidget *) Bind(&lineMode.value);                         f->label="Ruler Line Mode";
  f=(FormEditorWidget *) Bind(&showRulerOverlay.value);                 f->label="Show Ruler Tool";
  playButton=Reserve(3,32,32);
  rewindButton=Reserve(4,32,32);
  playFullscreenButton=Reserve(5,32,32);
  selectFullscreenButton=Reserve(6,32,32);
  clearBGArea=Reserve(7,64+16+8,16+2);
  blackBG=Reserve(8,64+16+8,16+2);
  copyButtonArea=Reserve(8,64+8,16+2);
  moveRectArea=Reserve(11,96,96);
  copySlideButton=Reserve(14,128,16);
  pasteSlideButton=Reserve(15,128,16);
  Setup();
  v.Calculate();
  v.Resize(1600,860);
  Arrange();//1600,860);
  mwm->vgui.Arrange2Rows(32.0,64.0);
  mwm->vgui.SlideTo(&v);
  Repopulate();
 }
 void WhenSetup() {
  clip=false;
  v.noMove=false;
  v.noMinimize=false;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noTitleBar=false;
  v.noBacking=false;
  v.alwaysOnTop=false;
  v.noClose=false;
  v.noClipTitle=true;
  v.minimumSize.Set(1600,860);
  v.maximumSize.Set(1600,860);
  v.SizeAndPosition(32,32,1600,860);
 }
 void RenderAfterWindow() {
  if ( v.moving || v.wasMoving || v.holder->arranging || v.holder->sliding || v.holder->wasSliding ) return;
  if ( fast.button(edSlide?"Paste After":"Paste",pasteSlideButton->x,pasteSlideButton->y,pasteSlideButton->w,pasteSlideButton->h) ) {
   SlideProfile *s=copiedSlideProfile->Duplicate();
   if ( edSlide ) ed->slideProfiles.InsertAfter(s,edSlide);
   else ed->slideProfiles.Append(s);
   ed->duration+=s->duration;
   edSlide=s;
   v.holder->DestroyAttached(&v);     
   return;
  }
  if ( edSlide ) {
   if ( fast.button("Copy Slide",copySlideButton->x,copySlideButton->y,copySlideButton->w,copySlideButton->h) ) {
    copiedSlideProfile.Recycle(edSlide->Duplicate());
    return;
   }
   if ( fast.button("Duplicate Slide",slideDuplicateButton->x,slideDuplicateButton->y,slideDuplicateButton->w,slideDuplicateButton->h) ) {
    v.holder->DestroyAttached(&v);     
    SlideProfile *slide=edSlide->Duplicate();
    ed->slideProfiles.InsertBefore(slide,edSlide);
    ed->duration+=slide->duration;
    edSlide=slide;
    return;
   }
   if ( fast.button("X Delete Slide",slideDeleteButton->x,slideDeleteButton->y,slideDeleteButton->w,slideDeleteButton->h) ) {
    v.holder->DestroyAttached(&v);     
    ed->slideProfiles.Remove(edSlide);
    ed->duration-=edSlide->duration;
    edSlide.Delete();
    edSlide=null;
    return;
   }
  }
  Cartesian rect;
  {
   rect.SetRect(resetTimeScaleButton->x,resetTimeScaleButton->y,resetTimeScaleButton->w,resetTimeScaleButton->h);
   if ( fast.button("100%",rect.x+4,rect.y+4,resetTimeScaleButton->w-8,resetTimeScaleButton->h-8) ) {
    ed->timescale=1.0;
   }
  }
  {
   rect.SetRect(slidePickerArea->x,slidePickerArea->y,slidePickerArea->w,slidePickerArea->h);
   glLineWidth(2.0);
   if ( fast.button("+SLIDE",rect.x2-48-2,rect.y2-24-2-128,64,24) ) {
    ed->slideProfiles.Append(edSlide=new SlideProfile);
    return;
   }
   glLineWidth(1.0);
   int sx=rect.x+8;
   int dx=16+32+16+16+4;
   int dy=24+4;
   int ofsx=16+16+8,ofsy=12+rect.y-(int)v.interior.y;
   int sy=rect.y;
   int i=0;
   EACHN(ed->slideProfiles.first,SlideProfile,p,{
    if ( p->prev && fast.button(fast.leftArrow,sx,sy,16,16) ) {
     ed->slideProfiles.sendUp(p,true);
     return;
    }
    if ( p->next && fast.button(fast.rightArrow,sx+16+32+16,sy,16,16) ) {
     ed->slideProfiles.sendDown(p,true);
     return;
    }
    if ( fast.button(I2S(i),sx+16,sy,32,24,edSlide==p) ) {
     edSlide=p;
    } 
    if ( fast.button("E",sx+16+32,sy,16,16) ) {
     if ( !mwm->vgui.SlideToIfAssociated(p)) {
      SlideProfileEditor *e=new SlideProfileEditor(p,mwm,path.c_str());
      e->v.Title(
       FORMAT("Slide %s: %s",
       I2S(ed->slideProfiles.IndexOf(p)),
       ed->name.c_str()).c_str()
      );
      e->v.Attach(&v,true,ofsx,ofsy);
      e->v.attachmentRadius=16.0;
     }
    }
    i++;
    sx+=dx;
    ofsx+=dx;
    if ( sx+32 > rect.x2-8 ) {
     sx=rect.x+8;
     sy+=dy;
     ofsx+=dy;
     ofsx=0;
    }
   });
  }
  rect.SetRect(playbackArea->x,playbackArea->y,playbackArea->w,playbackArea->h);
  if ( presi ) {
   fbo->Bind();
   presi->BetweenFrames();
   presi->Between();
   presi->Render();
   fbo->Unbind();
   fbo->blend=ed->blend;
   fbo->background.fromCrayon(ed->background);
   fbo->RenderUpsideDown(rect.x,rect.y,rect.x2,rect.y2);
   bool stopping=presi&&(!presi->paused&&presi->current);
   if ( fast.button((stopping?stopIcon:playIcon),playButton->x+1,playButton->y+1,playButton->w-2,playButton->h-2) ) {
    if ( stopping ) {
     presi->Pause();
    } else {
     if ( presi->current && presi->paused ) presi->Pause();
     else {
      presi.Recycle(ed->Create(true));
      presi->SetPositionAndSize(0,0,fbo->w,fbo->h);
    	 presi->Init(false);
     }
    }
   }
   if ( fast.button(rewindIcon,rewindButton->x+1,rewindButton->y+1,rewindButton->w-2,rewindButton->h-2) ) {
    presi.Recycle(ed->Create(true));
    presi->SetPositionAndSize(0,0,fbo->w,fbo->h);
  	 presi->Init(false);
   }
  } else {
   Blending(ed->blend);
   Area(ed->background,&rect);
   if ( fast.button(playIcon,playButton->x+1,playButton->y+1,playButton->w-2,playButton->h-2) ) {
    presi.Recycle(ed->Create(true));
    presi->SetPositionAndSize(0,0,fbo->w,fbo->h);
  	 presi->Init(false);
   }
  }
  if ( fast.button(playFullscreenIcon,playFullscreenButton->x+1,playFullscreenButton->y+1,playFullscreenButton->w-2,playFullscreenButton->h-2) ) {
   windows.Add(new PresentationFullscreenPreview(ed,v.holder->glWindow));
   return;
  }
  fast.GetState(playbackArea->x,playbackArea->y,playbackArea->w,playbackArea->h);
  if ( showRulerOverlay ) {
   Crayon outer,inner,darkGrey;
   outer.Pick(red255);
   inner.Pick(green255);
   darkGrey.Int(98,98,98,255);
   if ( fast.hovering ) {
    Cartesiand rect;
    mousePos.Set(playbackArea->w*fast.mouse.x,playbackArea->h*fast.mouse.y);
    viewPos.Set(mousePos.x+playbackArea->x,mousePos.y+playbackArea->y);
    virtualPos.Set(fbo->w*fast.mouse.x,fbo->h*fast.mouse.y);
    if ( !lineMode ) {
     if ( input.left && !input.wasLeft ) {
      clickedForRect=true;
      clickedRect.Set(virtualPos.x,virtualPos.y,0.0,0.0);
     } else if ( input.left ) {
      clickedRect.Corners(
       virtualPos.x > (double)clickedRect.x ? (double)clickedRect.x : virtualPos.x,
       virtualPos.y > (double)clickedRect.y ? (double)clickedRect.y : virtualPos.y,
       virtualPos.x > (double)clickedRect.x ? virtualPos.x : (double)clickedRect.x,
       virtualPos.y > (double)clickedRect.y ? virtualPos.y : (double)clickedRect.y
      );
     } else if ( !input.left && input.wasLeft ) {
      clickedRect.Corners(
       virtualPos.x > (double)clickedRect.x ? (double)clickedRect.x : virtualPos.x,
       virtualPos.y > (double)clickedRect.y ? (double)clickedRect.y : virtualPos.y,
       virtualPos.x > (double)clickedRect.x ? virtualPos.x : (double)clickedRect.x,
       virtualPos.y > (double)clickedRect.y ? virtualPos.y : (double)clickedRect.y
      );
      virtualRect.SetRect(clickedRect.x,clickedRect.y,clickedRect.w,clickedRect.h);
     } else {
      clickedForRect=false;
     }
     rect.SetCorners((double)playbackArea->x,viewPos.y,viewPos.x,viewPos.y);
     DrawLine2Color(outer,inner,rect.x,rect.y,rect.x2,rect.y2);
     rect.SetCorners(viewPos.x,viewPos.y,(double)playbackArea->x2,viewPos.y);
     DrawLine2Color(inner,outer,rect.x,rect.y,rect.x2,rect.y2);
     rect.SetCorners(viewPos.x,(double)playbackArea->y,viewPos.x,viewPos.y);
     DrawLine2Color(outer,inner,rect.x,rect.y,rect.x2,rect.y2);
     rect.SetCorners(viewPos.x,viewPos.y,viewPos.x,(double)playbackArea->y2);
     DrawLine2Color(inner,outer,rect.x,rect.y,rect.x2,rect.y2);
     Text( FORMAT("%d,%d",(int)virtualPos.x, (int) virtualPos.y ).c_str(),
      viewPos.x+20.0,viewPos.y+20.0, fast.fontSize.x, fast.fontSize.y, false
     );
    } else {
     if ( input.left && !input.wasLeft ) {
      clickedForLine=true;
      clickedLine.Set(virtualPos.x,virtualPos.y,0.0,0.0);
     } else if ( input.left ) {
      clickedLine.Set(clickedLine.x,clickedLine.y,virtualPos.x,virtualPos.y);     
     } else if ( !input.left && input.wasLeft ) {
      clickedLine.Set(clickedLine.x,clickedLine.y,virtualPos.x,virtualPos.y);
      virtualLine.Set(clickedLine.x,clickedLine.y,clickedLine.x2,clickedLine.y2);      
     } else {
      clickedForLine=false;
     }
     outer.Divide(2.0f);
     inner.Divide(2.0f);
     rect.SetCorners((double)playbackArea->x,viewPos.y,viewPos.x,viewPos.y);
     DrawLine2Color(outer,inner,rect.x,rect.y,rect.x2,rect.y2);
     rect.SetCorners(viewPos.x,viewPos.y,(double)playbackArea->x2,viewPos.y);
     DrawLine2Color(inner,outer,rect.x,rect.y,rect.x2,rect.y2);
     rect.SetCorners(viewPos.x,(double)playbackArea->y,viewPos.x,viewPos.y);
     DrawLine2Color(outer,inner,rect.x,rect.y,rect.x2,rect.y2);
     rect.SetCorners(viewPos.x,viewPos.y,viewPos.x,(double)playbackArea->y2);
     DrawLine2Color(inner,outer,rect.x,rect.y,rect.x2,rect.y2);
     Text( FORMAT("%d,%d",(int)virtualPos.x, (int) virtualPos.y ).c_str(),
      viewPos.x+20.0,viewPos.y+20.0, fast.fontSize.x, fast.fontSize.y, false
     );
     outer.Pick(red255);
     inner.Pick(green255);
    }
   }
   if ( !lineMode ) {
    crayons.jami(0,32,32,32,255).gl();
    DrawCircle(
     playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
     iratiod((int)clickedRect.w,fbo->w)*playbackArea->w,
     32
    );
    Rectangle(darkGrey,
     playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
     playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedRect.y2,fbo->h)*playbackArea->h)
    );
    DrawLine(darkGrey,
     playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
     playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedRect.y2,fbo->h)*playbackArea->h)
    );
    Circle(
     playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
     playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w)
    );
    if ( clickedForRect )
     Rectangle(outer,
      playbackArea->x+(iratiod((int)clickedRect.x,fbo->w)*playbackArea->w),
      playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h),
      playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w),
      playbackArea->y+(iratiod((int)clickedRect.y2,fbo->h)*playbackArea->h)
    );
    Text( FORMAT("%d,%d\n%d,%d\n%d x %d\n%s",
      (int)clickedRect.x, (int) clickedRect.y,
      (int)clickedRect.x2, (int) clickedRect.y2,
      (int) clickedRect.w, (int) clickedRect.h,
      ( virtualRect.Equals(&copiedCoordinate) ? "copied" : "" )
     ).c_str(),
     playbackArea->x+(iratiod((int)clickedRect.x2,fbo->w)*playbackArea->w)+8.0,
     playbackArea->y+(iratiod((int)clickedRect.y,fbo->h)*playbackArea->h), fast.fontSize.x, fast.fontSize.y, fast.lineSpace, false
    );
   } else {
    DrawLine2Color(inner,outer,
     playbackArea->x+(iratiod((int)clickedLine.x,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedLine.y,fbo->h)*playbackArea->h),
     playbackArea->x+(iratiod((int)clickedLine.x2,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)clickedLine.y2,fbo->h)*playbackArea->h)
    );
    DrawLine(darkGrey,
     playbackArea->x+(iratiod((int)virtualLine.x,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)virtualLine.y,fbo->h)*playbackArea->h),
     playbackArea->x+(iratiod((int)virtualLine.x2,fbo->w)*playbackArea->w),
     playbackArea->y+(iratiod((int)virtualLine.y2,fbo->h)*playbackArea->h)
    );
    Text( "A", 
     (double)playbackArea->x+(iratiod((int)virtualLine.x,fbo->w)*playbackArea->w),
     (double)playbackArea->y+(iratiod((int)virtualLine.y,fbo->h)*playbackArea->h-16.0),
     11.0,8.0,true
    );
    Text( "B", 
     (double)playbackArea->x+(iratiod((int)virtualLine.x2,fbo->w)*playbackArea->w),
     (double)playbackArea->y+(iratiod((int)virtualLine.y2,fbo->h)*playbackArea->h-16.0),
     11.0,8.0,true
    );
    virtualLine.CalculateSlope();
    clickedLine.CalculateSlope();
    Text( FORMAT("%d,%d\n%d,%d\nAngle: %1.1f deg\n%s",
      (int)virtualLine.x, (int) virtualLine.y,
      (int)virtualLine.x2, (int) virtualLine.y2,
      (double)(virtualLine.angleDeg),
      ( virtualLine.Equals(&copiedLine) ? "copied" : "" )
     ).c_str(),
     playbackArea->x+(iratiod((int)clickedLine.x2,fbo->w)*playbackArea->w)+8.0+32.0,
     playbackArea->y+(iratiod((int)clickedLine.y,fbo->h)*playbackArea->h), fast.fontSize.x, fast.fontSize.y, fast.lineSpace, false
    );
   }
   if ( input.leftReleased() ) { clickedForLine=false; clickedForRect=false; }
   bool playbackAreaHovering=fast.hovering;
   int movement=fast.arrows("",moveRectArea->x+32,moveRectArea->y+32,24);
   if ( (playbackAreaHovering && input.KeyDown(DX_X))
     || fast.button(selectFullscreenIcon,selectFullscreenButton->x+1,selectFullscreenButton->y+1,selectFullscreenButton->w-2,selectFullscreenButton->h-2) ) {
    virtualRect.SetRect(0,0,fbo->w,fbo->h);
    clickedRect.Set(0,0,fbo->w,fbo->h);
   }
   if ( fast.button("Copy [\\]",copyButtonArea->x+4,copyButtonArea->y+1,copyButtonArea->w-8,copyButtonArea->h-2)
     || ( playbackAreaHovering && input.KeyDown(DX_C) ) ) {
    if ( !lineMode ) copiedCoordinate.Set(&virtualRect);
    else copiedLine.Set(&virtualLine);
   }
  }
  if ( fast.button("Transparent",clearBGArea->x+4,clearBGArea->y+1,clearBGArea->w-8,clearBGArea->h-2) ) {
   ed->background.Pick(clear);
  }
  if ( fast.button("Darkness",blackBG->x+4,blackBG->y+1,blackBG->w-8,blackBG->h-2) ) {
   ed->background.Pick(black);
  }
  Text(
   FORMAT("Slide: %d %2.0f%% %1.2fs Expired: %1.2fs",
    (presi->current?presi->slides->IndexOf(presi->current):-1),    
    (presi->current?(double)(1.0-presi->current->remaining/presi->current->duration)*100.0:0.0),
    (presi->current?(double)presi->current->remaining:0.0),
    (float)presi->expired).c_str(),
   (double)(playbackArea->x+playbackArea->w/2),(double)playbackArea->y-10.0,8.0,11.0,false
  );
 }
 void Between() {
  v.Title(ed->name.c_str());
 }
MANY(PresentationProfileEditor,PresentationProfileEditorHandle,PresentationProfileEditorHandles,"PresentationProfileEditor",PresentationProfileEditors,{})
DONE(PresentationProfileEditor);

class PresentationSelector : public MoveableEditorWindow {
public:
 PresentationSelector( MoveableWindowManager *mwm, const char *path="data/workspaces/presentations/" ) : MoveableEditorWindow(mwm) {
  repopulateDelay.Set(10.0f);
  repopulateDelay.Reset();
  this->path=path;
 }
 Zstring path;
 Strings list;
 Delay repopulateDelay;
 void OnSetup() {
  clip=false;
  v.background.Pick(black);
  v.noMove=false;
  v.noMinimize=false;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noTitleBar=false;
  v.noBacking=false;
  v.alwaysOnTop=true;
  v.noClose=true;
  v.noCenter=false;
  v.noClipTitle=true;
  v.minimumSize.Set(128,64);
  v.maximumSize.Set(512,1024);
  v.SizeAndPosition(8,8,512,1024);
  Name("Presentations");
  Repopulate();
 }
 void Repopulate() {
  list.Clear();
  EACH(presentationProfiles.first,PresentationProfile,p) {
   list.Add(p->name.c_str());
  }
 }
 Zint scrolled;
 void RenderWindow() {
  if ( !repopulateDelay ) {
   Repopulate();
   repopulateDelay.Reset();
  } else repopulateDelay.Between();
  int selection=fast.list(digicaps,"",&list,
   (int)v.position.x+8,(int)v.position.y+24,
   (int)v.position.w-16,(int)((v.position.h-256.0)/(fast.fontSize.y+4.0)),
   &scrolled.value
  );
  if ( selection >= 0 ) {
   PresentationProfile *p=(PresentationProfile *) presentationProfiles.Element(selection);
   if ( !mwm->vgui.SlideToIfAssociated(p) ) {
    new PresentationProfileEditor(p,mwm,path.c_str());
   }
  }
  if ( fast.button("Create New",(int)v.position.x+9,(int)v.position.y2-64,128,24) ) {
   PresentationProfile *p=new PresentationProfile;
   presentationProfiles.Append(p);
   new PresentationProfileEditor(p,mwm,path.c_str());
   Repopulate();
   v.isMinimized=true;
  }
//  if ( fast.button("<- Go Back",32,1000,256,32,false) ) {
//  }
 }
};

class PresentationEditor : public MoveableWindowManager {
public:
 Zp<GLWindow> back_to;
 Zstring path,statefile;
 PresentationEditor( const char *path ) : MoveableWindowManager() {
  this->path=path;
  statefile=specialPaths.historic("presi.editor.windows.txt");
 }
 void LoadEditors() {
  Add(new PresentationSelector(this,path));
  vgui.Arrange2Rows();
  if ( file_exists(statefile.c_str()) ) vgui.LoadState(statefile.c_str());
  else vgui.SaveState(statefile.c_str());
 }
 void RenderAfterWindow() {
  art.PushViewport(display->w,display->h);
  vgui.drawWindowControls=input->KeyDown(DX_LCTRL) || input->KeyDown(DX_RCTRL);
  if ( back_to ) {
   fast.member=null;
   if ( fast.button("<-",16,96,32,16) ) {
    back_to->visible=true;
    destroyMe=true;
    return;
   }
   if ( fast.button("SAVE",48,96,48,16) ) {
    presentationProfiles.Save(path);
    vgui.SaveState(statefile.c_str());
   }
   if ( fast.clicking ) fast.text("Saving.",16+32+8+32+8,32+16);
  }
  art.PopViewport();
 }
};

