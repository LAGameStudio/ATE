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

#include "GLSetup.h"
#include "GLWindow.h"
#include "Proce55or.h"
#include "pBuffer.h"
#include "FBO.h"

class fx_pBufferSwitcher : public Proce55or {
public:
 Zbool DesiredState;
 Zpointer<pBuffer> pbuffer;
 PROGRAMMABLE(fx_pBufferSwitcher,StateChangePtr,change,Change);
 fx_pBufferSwitcher() {
  Constructor();
  change=&fx_pBufferSwitcher::NoChange;
 }
 void NoChange() {
 }
 void Enable() {
  pbuffer->MakeCurrent();
 }
 void Disable() {
  display.MakeCurrent();
 }
 void Render() {
  Change();
 }
 void Draw(GLWindow *surface) {
  Render();
 }
};

class fx_FBOSwitcher : public Proce55or {
public:
 Zbool DesiredState;
 Zpointer<FBOColor> fbo;
 PROGRAMMABLE(fx_FBOSwitcher,StateChangePtr,change,Change);
 fx_FBOSwitcher() : Proce55or() {
  change=&fx_FBOSwitcher::NoChange;
 }
 void NoChange() {
 }
 void Enable() {
  fbo->Bind();
 }
 void Disable() {
  fbo->Unbind();
 }
 void Render() {
  Change();
 }
 void Draw(GLWindow *surface) {
  Render();
 }
};