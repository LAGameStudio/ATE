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

#include "Slide.h"
#include "Presentation.h"

class Consulate;

class PresentationScriptable;
class ScriptableSlide : public Slide {
public:
 Zp<PresentationScriptable> owner;
 Zp<Consulate> consulate;
 Zdisposable<Consulate> scripts;
 ScriptableSlide();
 void fromString( const char *in );
 void OnComplete();
};

class PresentationScriptable : public Presentation {
public:
 Zp<ScriptableSlide> slidePointer;
 Zp<Consulate> consulate;
 PresentationScriptable( Consulate *es );
 void fromString( const char *in );
 bool OnComplete(void) {
  if ( current ) current->sounds.Stop(); 
  if ( loop ) {
   Rewind(this->paused);
  }
  return false;
 }
};