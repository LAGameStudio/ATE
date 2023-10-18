#pragma once

#include "Workspace.h"
#include "Player.h"

extern Zint stashed_circle64tenth,stashed_circle64,stashed_starmapgrid;

class StarmapViewer : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<StarmapProfile> location;
 Zdouble time;
 Zbool firstTime;
 Zp<StarmapProfile> firstDestination;
 Zp<StarmapProfile> selectedDestination;
 Zp<GLImage> crosshair;
 Zdouble lockedDelay;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  if ( stashed_circle64tenth == 0 ) stashed_circle64tenth=art.stash.GenCircleThickness(0.0,1.0,256,crayons.Pick(alabaster),0.1);
  if ( stashed_circle64 == 0 ) stashed_circle64=art.stash.GenCircle();
  if ( stashed_starmapgrid == 0 ) stashed_starmapgrid=art.stash.GenGrid(10,10,0.005);
  Repopulate();
  firstDestination=starmapProfiles.firstNamed("phenoktus");
  crosshair=library.Load("data/images/icon3/shiphud_2.png");
 }
 void Repopulate() {
  starmapProfiles.AllVisible();
  Zstring cloud;
  EACH(player->cloud.first,String,s) { cloud+=s->s.c_str(); cloud+=" "; }
  starmapProfiles.Cloud(cloud.c_str());
  starmapProfiles.Sort();
  if ( player->starmapLocation.length == 0 ) {
   if ( !location ) {
    firstTime=true;
    location=starmapProfiles.firstNamed("earth_system");
    if ( !location ) location=(StarmapProfile *) starmapProfiles.first;
   }
  } else {
   location=starmapProfiles.firstNamed(player->starmapLocation.c_str());
   if ( !location ) {
    firstTime=true;
    location=starmapProfiles.firstNamed("earth_system");
    if ( !location ) location=(StarmapProfile *) starmapProfiles.first;
   }
  }
 }
 Zbool wasHoveringOn;
 Cartesiand offset;
 Cartesiand oldMouse;
 void Between() {
  if ( lockedDelay > 0 ) lockedDelay-=FRAMETIME;
  time+=FRAMETIME;
  if ( !wasHoveringOn ) {
   if ( input->left ) {
    offset.MoveBy(input->mxi-input->oldmxi,input->myi-input->oldmyi);
   } else {
    if ( input->mxi < 100 ) offset.MoveBy(1,0); else if ( input->mxi > w-100 ) offset.MoveBy(-1,0);
    if ( input->myi < 100 ) offset.MoveBy(0,1); else if ( input->myi > h-100 ) offset.MoveBy(0,-1);
   }
  }
 }
 void Render() {
  if ( !visible || this->destroyMe == true ) return;
  fast.SaveState();
  art.PushViewport(w,h);
  Cartesiand area;
  if ( !location ) area.SetRect((w-h)/2,0,h,h);
  else {
   double scale=h*5.0;
   if ( offset.x > scale-w/2 ) offset.x=scale-w/2;
   if ( offset.y > scale-h/2 ) offset.y=scale-h/2;
   if ( offset.x < -w ) offset.x=-w;
   if ( offset.y < -h ) offset.y=-h;
   area.SetRect(offset.x-location->position.x*scale+h/2,offset.y-location->position.y*scale+h/2,scale,scale);
  }
  Crayon gt(red255);
  gt.ScaleByAlphaf(0.12f);
  art.Element(gt,transparency,stashed_starmapgrid,area.x,area.y,area.w,area.h);
  wasHoveringOn=false;
  EACH(starmapProfiles.first,StarmapProfile,sp) if ( !sp->hidden ) {
   //mulligan
   if ( sp->image ) {
    if ( sp->spin ) sp->spun+=sp->spinRate*sp->timescale;
    Crayon t(sp->tint1);
    double ratio=sp->tween1?(double)sp->tween1->tweend(time*sp->timescale):1.0;
    t.MultiplyWrapRGB(ratio);
    art.Pivot(sp->image,t,sp->blend,area.x+sp->position.x*area.w,area.y+sp->position.y*area.h,0.0,0.0,area.h*sp->position.h/2.0,area.w*sp->position.w/2.0,sp->angle+sp->spun);
   }
   if ( sp->image2 ) {
    if ( sp->spin2 ) sp->spun2+=sp->spinRate2*sp->timescale2;
    Crayon t(sp->tint2);
    double ratio=sp->tween2?(double)sp->tween2->tweend(time*sp->timescale2):1.0;
    t.MultiplyWrapRGB(ratio);
    art.Pivot(sp->image2,t,sp->blend,area.x+sp->position.x*area.w,area.y+sp->position.y*area.h,0.0,0.0,area.h*sp->position.h/2.0,area.w*sp->position.w/2.0,sp->angle2+sp->spun2);
   }
   Cartesiand region;
   region.SetRect(area.x+sp->position.x*area.w-area.w*sp->position.w/2.0,area.y+sp->position.y*area.h-area.h*sp->position.h/2.0,area.w*sp->position.w,area.h*sp->position.h);
   if ( sp == location ) {
    Crayon t(red255);
    t.Alpha(fmod(time,3.0)/3.0);
    art.Element(t,transparency,stashed_circle64,(int)(area.x+sp->position.x*area.w)-16, (int)(area.y+sp->position.y*area.h)-10,32,32);
   }
   if ( sp != location && region.w < w/2 && fast.hoverarea((int)region.x,(int)region.y,(int)region.w,(int)region.h) ) {
    if ( fast.clicked ) {
     if (firstTime) {
      if (sp == firstDestination) {
       selectedDestination=sp;
       PlaySelectSound();
      }
     } else {
      selectedDestination=sp;
      PlaySelectSound();
     }
    }
   }
   if ( sp == selectedDestination ) {    
    Crayon t(green255);
    t.Alpha(0.35);
    art.Element(t,transparency,stashed_circle64tenth,region.x,region.y,region.w,region.h);
    if ( location ) {
     Line line((area.x+sp->position.x*area.w), (int)(area.y+sp->position.y*area.h), (area.x+location->position.x*area.w), (int)(area.y+location->position.y*area.h));
     Vertexd point; line.Point(0.1+0.8*(1.0-(fmod(time,1.0))),&point);
     double ang=rad2deg(line.LineAngle());
     art.Pivot(fast.rightArrow,t,additive,point.x,point.y,0.0,0.0,16.0,16.0,ang+180.0);
     fast.fontSize.x=16;
     fast.fontSize.y=22;
     line.Set(sp->position.x,sp->position.y,location->position.x,location->position.y);
     int fuel=(int)UMAX(1.0,line.d/0.1);
     if ( lockedDelay <= 0.0 ) {
      if ( fuel <= player->jumpFuel ) {
       if ( fast.button(digicaps,"Engage Hyperdrive",w/2-768/2,h-192,768,64) ) {
        ///
        LockCourseAndReturn();
       }
      } else {
       fast.deactivated.Pick(red255);
       fast.dbutton(digicaps,"Not Enough Fuel",w/2-256,h-192,512,64);
      }
     }
     fast.boxedtext( FORMAT("Fuel Required: %d", (int)(fuel) ).c_str(), w/2,h-64,true );
     fast.fontSize.x=16;
     fast.fontSize.y=22;
     fast.boxedtext( sp->named.c_str(), (int)(area.x+sp->position.x*area.w), (int)(area.y+sp->position.y*area.h) );
    }
   }
   if ( sp != location && fast.hovering && (!firstTime || sp == firstDestination) ) {
    if ( region.w < w/2 ) {
     wasHoveringOn=true;
     Crayon t(green255);
     t.Alpha(0.25);
     art.Element(t,transparency,stashed_circle64tenth,region.x,region.y,region.w,region.h);
    }
    fast.fontSize.x=16;
    fast.fontSize.y=22;
    fast.boxedtext( sp->named.c_str(), (int)(area.x+sp->position.x*area.w), (int)(area.y+sp->position.y*area.h) );
   } else if ( sp == location ) {
    fast.fontSize.x=8;
    fast.fontSize.y=11;
    fast.boxedtext( sp->named.c_str(), (int)(area.x+sp->position.x*area.w), (int)(area.y+sp->position.y*area.h) );
   }
  }
  if ( fast.button(crosshair,w-64,h-64,48,48) ) {
   offset.Set(0.0,0.0);
  }
  if ( fast.hovering ) wasHoveringOn=true;
  fast.fontSize.x=8;
  fast.fontSize.y=11;
  fast.boxedtext( FORMAT("Jump Fuel: %d unit%c", (int)(player->jumpFuel.value), player->jumpFuel == 1 ? ' ' : 's' ).c_str(), 64,64, false, player->jumpFuel > 0 ? crayons.Pick(green255) : crayons.Pick(red255) );
  if ( fast.button(digicaps,"CANCEL",16,h-64,256,48) ) {
   back_to->visible=true;
   this->destroyMe=true;
   return;
  }
  if ( fast.hovering ) wasHoveringOn=true;
  art.ResetViewport();
  fast.RecallState();
 }
 void PlaySelectSound() {
  audio.manager.CueAndPlay("data/sounds/UI/hyper1.wav",1.0,1.0);
  lockedDelay=1.5;
 }
 void LockCourseAndReturn();
};

