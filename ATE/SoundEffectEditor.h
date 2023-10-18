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
#include "GridSplitter.h"
#include "FileDialog.h"
#include "SoundEffect.h"
#include "Workspace.h"

C_ONE(SoundEffectEditor,MoveableEditorWindow,{})
 Zp<Workspace> workspace;
 Zp<SoundEffect> ed;
 FileDialog fd;
 Zstring filepicker;
 Zint selected,scrolled;
 Zstring lastPlayedMessage;
 Zbool hideAll,hideLoop,hideSingleton,hideSporadic;
 Cartesian window;
 SoundEffectEditor( Workspace *c, SoundEffect *s ) : MoveableEditorWindow() {
  ed=s;
  workspace=c;
  selected=-1;
  window.SetRect(512,512,512,512);
 }
 SoundEffectEditor( SoundEffect *s ) : MoveableEditorWindow() {
  ed=s;
  selected=-1;
  window.SetRect(512,512,512,512);
 }
 void OnSetup() {
  clip=false;
  v.alwaysOnTop=true;
  v.SizeAndPosition(window.x,window.y,window.w,window.h);
  v.minimumSize.Set(512,512);
  v.maximumSize.Set(1920,1080-64);
  v.noMove=false;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noClose=false;
  v.noClipTitle=true;
  v.background.x_Black();
  Name("SOUND");
  fd.description="Choose audio, WAV or OGG, must not be longer than 8 or 9 seconds.";
  fd.path=(workspace?workspace->path:string(""))+string("sounds");
  fd.includedLocations.Add("data/sounds/");
  fd.background.x_DarkSlateBlue();
  fd.bound=&filepicker.value;
  fd.OnLoad();
 }
 void Between() {
 }
 void RenderWindow() {
  fast.member=&v;
  int clicked=fast.list(termFont,"Sound Files",&ed->sounds,x+16,y+24,458,8,&selected.value,&scrolled.value,false);
  if ( clicked >= 0 ) {   
   float tempPitch = (float)(uniform()*(ed->pitchHigh-ed->pitchLow)+ed->pitchLow);
   float tempGain = (float)(uniform()*(ed->gainHigh-ed->gainLow)+ed->gainLow);
   if ( ed->playback == sfx_Impulse || ed->playback == sfx_OnLoopedUntilOff ) {
    audio.manager.CueAndPlay(ed->sounds.Get(clicked).c_str(), tempPitch,tempGain );
   } else {
    audio.manager.CueAndPlayIfNotPlaying(ed->sounds.Get(clicked).c_str(), tempPitch,tempGain );
   }
   lastPlayedMessage=FORMAT("Played at %1.1f pitch %1.1f gain",tempPitch,tempGain);
  }
  if ( hideAll ) {
  } else {
   Strings playbackOptions;
   if ( !hideSingleton ) playbackOptions.Append(new String("singleton",(int)sfx_Singleton));
   if ( !hideLoop ) playbackOptions.Append(new String("looping",(int)sfx_OnLoopedUntilOff));
   playbackOptions.Append(new String("impulse",(int)sfx_Impulse));
   if ( !hideSporadic ) playbackOptions.Append(new String("sporadic",(int)sfx_Sporadic));
   ed->playback=(SoundEffectPlaybackHints)fast.enumerated(&playbackOptions,"Playback",(int)ed->playback,x+32,y+h-48,64,32,true);
  }
  if ( fast.button("Reset",x+w-128,y+h-128+8,100,24) ) {
   ed->pitchLow=0.9;
   ed->pitchHigh=1.0;
   ed->gainLow=0.9;
   ed->gainHigh=1.0;
  }
  fd.MoveTo(x+16,y+256+16+8);
  fd.Render();
  if ( fd.changed ) {
   ed->sounds.Add(filepicker.c_str());
   fd.changed=false;
  }
  if ( fast.button("Remove Selected",x+32+128,y+256+16+8,196,32) ) {
   String *removing=(String *) ed->sounds.Element(selected);
   if ( removing ) {
    ed->sounds.Remove(removing);
    delete removing;
   }
   selected=-1;
  }
  crayons.Pick(macNcheese).gl();
  Text(lastPlayedMessage.c_str(),x+16,y+256+16,fast.fontW,fast.fontH,false);
  fast.hrange("PITCH",&ed->pitchLow.value,&ed->pitchHigh.value,0.5,4.0,x+64,y+256+64+8+24,w-128);
  fast.hrange("GAIN",&ed->gainLow.value,&ed->gainHigh.value,0.1,2.0,x+64,y+256+64+64+24,w-128);
 }
MANY(SoundEffectEditor,SoundEffectEditorHandle,SoundEffectEditorHandles,"SoundEffectEditor",SoundEffectEditors,{})
DONE(SoundEffectEditor);