#include "PresentationEditor.h"

Cartesiand copiedCoordinate;
Line copiedLine;

PresentationProfiles presentationProfiles;

Zdis<AnimationElement> copiedAnimationElement;
Zdis<SlideProfile> copiedSlideProfile;

void fx_NextGenVisual::Setup() {
 x=(int)A.x;
 y=(int)A.y;
 w=(int)A.w;
 h=(int)A.h;
 TL.Set(A.x,A.y,B.x,B.y);
 BR.Set(A.x2,A.y2,B.x2,B.y2);
 angle=startAngle;
 diffAngle=startAngle-endAngle;
 frame=1;
 frames=(int)(duration/FRAMETIME);
 soundDelay.Reset();
 mulliganDelay.Reset();
 if ( presentation.length > 0 ) {
  PresentationProfile *pp=presentationProfiles.firstNamed(presentation.c_str());
  if ( pp ) {
   presi.Recycle(pp->Create(false,recursionDepth+1));
   if ( presi ) {
    presi->fbo=&fbo1080p3;
    presi->Init(false);
   }
  }
 }
} 