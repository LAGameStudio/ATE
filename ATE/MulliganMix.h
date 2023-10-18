#pragma once

#include "Mulligan.h"

ONE(MulliganMixNode, { scaleX = 1.0; scaleY = 1.0; })
 Mulligan mulligan;
 Zdouble startTime, endTime, cutLeft, cutRight;
 Zdouble x, y, vx, vy, scaleX, scaleY;
 Proce55ors implementation;
 KEYWORDS({
  SSUBWORD("mulligan",mulligan)
  NUMWORD("startTime",startTime)
  NUMWORD("endTime",endTime)
  NUMWORD("cutLeft",cutLeft)
  NUMWORD("cutRight",cutRight)
  NUMWORD("x",x)
  NUMWORD("y",y)
  NUMWORD("vx",vx)
  NUMWORD("vy",vy)
  NUMWORD("scaleX",scaleX)
  NUMWORD("scaleY",scaleY)
 })
 WORDKEYS({
  SSUBKEY("mulligan",mulligan)
  NUMBKEY("startTime",startTime)
  NUMBKEY("endTime",endTime)
  NUMBKEY("cutLeft",cutLeft)
  NUMBKEY("cutRight",cutRight)
  NUMBKEY("x",x)
  NUMBKEY("y",y)
  NUMBKEY("vx",vx)
  NUMBKEY("vy",vy)
  NUMBKEY("scaleX",scaleX)
  NUMBKEY("scaleY",scaleY)
 })
 void Implement( FBOColorDepth *scratch, double wasFrameTime, double frameTime) {
  if (wasFrameTime <= startTime && frameTime >= startTime) {
   mulligan.Implement(&implementation,(scratch->w/2)+(int)x, (scratch->h/2)+(int)y,vx,vy,scaleX,scaleY);
  }
 }
 void Between() { implementation.Between(); implementation.DropDone(); }
 void Render() { implementation.Render(); }
 void Render(double currentTime) {
  if (implementation.count > 0
   && startTime <= currentTime
   && (endTime <= startTime
    || currentTime < endTime)
   ) {
   implementation.Render();
  }
 }
 void Reset() { implementation.Clear(); }
 void CaptureTo(FBOColorDepth *out) {
  out->background.Pick(clear);
  out->Bind();
  art.PushViewport(out->w, out->h);
  implementation.Render();
  art.PopViewport();
  out->Unbind();
 }
MANY(MulliganMixNode, MulliganMixNodeHandle, MulliganMixNodeHandles, "MulliganMixNode", MulliganMixNodes, {})
 KEYSWORDSGroups("MulliganMixNodes",MulliganMixNode)
 CALLEACH(MulliganMixNode, Between);
 CALLEACH(MulliganMixNode, Render);
 CALLEACH(MulliganMixNode, Reset);
 void Render(double time) { FOREACH(MulliganMixNode, mn) mn->Render(time); }
 void Implement(FBOColorDepth *scratch, double wasFrameTime, double frameTime) { FOREACH(MulliganMixNode, mn) mn->Implement(scratch,wasFrameTime, frameTime); }
 void CaptureTo(FBOColorDepth *out) {
  out->background.Pick(clear);
  out->Bind();
  art.PushViewport(out->w, out->h);
  Render();
  art.PopViewport();
  out->Unbind();
 }
 void CaptureTo(double time, FBOColorDepth *out) {
  out->background.Pick(clear);
  out->Bind();
  art.PushViewport(out->w, out->h);
  Render(time);
  art.PopViewport();
  out->Unbind();
 }
DONE(MulliganMixNode);

enum MulliganMixOutSizes {
 mmos_16x16=0,
 mmos_32x32=1,
 mmos_48x48=2,
 mmos_64x64=3,
 mmos_128x128=4,
 mmos_256x256=5,
 mmos_512x512=6,
 mmos_1024x1024=7,
 mmos_2048x2048=8
};

ONE(MulliganMix, { framesPerSecond = 60.0; scratch = &fbo512d;  out = &fbo512c; outSize = mmos_512x512; })
 MulliganMixNodes nodes;
 Zdouble endTime, framesPerSecond;
 Zbool limitFrames;
 Zint startFrame, endFrame;
 MulliganMixOutSizes outSize;
 KEYWORDS({
  KEYWORD("name",{name = w;})
  SSUBWORD("nodes",nodes)
  NUMWORD("framesPerSecond",framesPerSecond)
  NUMWORD("endTime",endTime)
  TAGWORD("limitFrames",limitFrames)
  NUMWORD("startFrame",startFrame)
  NUMWORD("endFrame",endFrame)
  ENUMWORD("outSize",outSize,MulliganMixOutSizes)
 })
 WORDKEYS({
  WORDKEY("name",name)
  SSUBKEY("nodes",nodes)
  NUMBKEY("framesPerSecond",framesPerSecond)
  NUMBKEY("endTime",endTime)
  TAGKEY("limitFrames",limitFrames)
  NUMBKEY("startFrame",startFrame)
  NUMBKEY("endFrame",endFrame)
  ENUMKEY("outSize",outSize)
 })
 void Rewind() {
  nodes.Reset();
  frame = 0;
  expired = 0.0;
 }
 Zint frame;
 Zdouble expired;
 Zp<FBOColorDepth> scratch,out;
 void Advance(int frames, double frameTimeDelta = FRAMETIME) {
  for (int i = 0; i < frames; i++) {
   double wasFrameTime = expired;
   frame++;
   expired += frameTimeDelta;
   nodes.Implement(scratch, wasFrameTime, expired);
   nodes.Between();
   nodes.CaptureTo(expired,scratch);
  }
 }
 int TotalParticleCount() {
  int total = 0;
  EACH(nodes.first, MulliganMixNode, node) total += node->implementation.count.value;
  return total;
 }
 int ImplementationsLeft() {
  int total = 0;
  EACH(nodes.first, MulliganMixNode, node) {
   if (expired - FRAMETIME <= node->startTime && expired >= node->startTime) total++;
  }
  return total;
 }
MANY(MulliganMix, MulliganMixHandle, MulliganMixHandles, "MulliganMix", MulliganMixes, {})
 KEYSWORDSGroups("MulliganMixes",MulliganMix)
DONE(MulliganMix);