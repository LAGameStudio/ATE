#pragma once

#include "GLWindow.h"
#include "ScriptSection.h"


class RecruitScreen : public GLWindow {
public:
 Zp<GLWindow> back_to;
 ScriptSections scriptSections;
 void ExecuteScript( const char *script ) {
  scriptSections.Merge(script);
  EACH(scriptSections.first,ScriptSection,ss) {
   EACH(ss->lines.first,ScriptLine,sl) {
    Zstring one(sl->Word1());
    if ( one.length > 0 ) {
     if ( one == "stop" || one == "break" ) { break; }
    }
   }
  }
 }
 void Between() {
  if ( !visible || this->destroyMe == true ) return;
 }
 void Render() {
  if ( !visible || this->destroyMe == true ) return;
 }
};

