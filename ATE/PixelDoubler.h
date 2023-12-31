#pragma once
#include "Strings.h"
#include "TextureLibrary.h"
#include "FBO.h"
#include "PixelateShader.h"
#include "ConvolverShader.h"
#include "TVShader.h"
#include "BlendShader.h"
#include "BloomShader.h"
#include "GammaShader.h"
#include "ParallaxShader.h"

// Doubles 890x640 to 1920x1280
// Input is assumed to be fbo1080p
// Output FBO is assumed to be fboScreen and uses all four fboScreen fbos

enum PixelDoublerTypes {
 pdt_Default=0,
 pdt_HQ4X=1,
 pdt_Nearest=2,
 pdt_Additive=3,
 pdt_BloomGamma=4,
 pdt_Multiply=5,
 pdt_BloomNearest=6,
 pdt_SharpenAdd=7,
 pdt_Emboss=8,
 pdt_SuperSharp=9
};

class PixelDoubler {
public:
 PixelDoublerTypes mode;
 Zdouble time;
 OldTVProfile tvProfile;
 Zp<FBOColor> screen,screen2,screen3,source;
 static void DeployTypes( Strings*out ) {
  out->Add("Default",0);
  out->Add("HQ4X",1);
  out->Add("Nearest",2);
  out->Add("Contrast\nDetail",3);
  out->Add("Bloom\nGamma",4);
  out->Add("Muted",5);
  out->Add("Aliased",6);
  out->Add("Sharpen\nAdd",7);
  out->Add("Emboss",8);
  out->Add("SuperSharp",9);
 }
 PixelDoubler() {
  mode=pdt_Default;
  Go1080toScreen();
 }
 void Go512to1080() {
  screen=&fbo1080p;
  screen2=&fbo1080p2;
  screen3=&fbo1080p3;
  source=&fboCDS512;
 }
 void Go512to1024() {
  screen=&fboCDS1024;
  screen2=&fboCDS1024b;
  screen3=&fboCDS1024c;
  source=&fboCDS512;
 }
 void Go1080toScreen() {
  screen=&fboScreen;
  screen2=&fboScreen2;
  screen3=&fboScreen3;
  source=&fbo1080p;
 }
 void Off() { mode=pdt_Nearest; }
 void Between() {
  time+=FRAMETIME;
 }
 FBOColor *Double() {
  screen->tint.Pick(alabaster);
  screen->background.Pick(clear);
  screen2->tint.Pick(alabaster);
  screen2->background.Pick(clear);
  source->tint.Pick(alabaster);
  source->background.Pick(clear);
  switch ( mode ) {
   case pdt_Default: {
    screen->Bind();
    source->RenderUpsideDown(transparency,0,0,screen->w*2,screen->h*2);
    screen->Unbind();
    return screen;
   } break;
   case pdt_BloomGamma: {
    screen->Bind();
    source->RenderUpsideDown(transparency,0,0,screen->w*2,screen->h*2);
    screen->Unbind();
    ShadedSharpen(&screen->texture,screen2);
    screen3->tint.Pick(alabaster);
    screen3->background.Pick(clear);
    screen3->Bind();
    source->RenderUpsideDown(transparency,0,0,screen3->w*2,screen3->h*2);
    screen2->RenderUpsideDown(multiply,0,0,screen3->w,screen3->h);
    screen3->Unbind();
    ShadedBloom(&screen3->texture,screen2);
    ShadedGamma(&screen2->texture,1.5f,screen3);
    return screen3;
   } break;
   case pdt_HQ4X: {
    ShadedHQ4X(source,screen);
    return screen;
   } break;
   case pdt_Nearest: {
    screen->Bind();
    source->RenderUpsideDown(transparency,0,0,screen->w*2,screen->h*2);
    screen->Unbind();
    return screen;
   } break;
   case pdt_Additive: {
    screen->Bind();
    source->RenderUpsideDown(transparency,0,0,screen->w*2,screen->h*2);
    screen->Unbind();
    screen2->background.Pick(clear);
    ShadedSharpen(&screen->texture,screen2);
    screen3->Bind();
    screen2->tint.Pick(alabaster);
    screen2->RenderUpsideDown(none,0,0,screen3->w,screen3->h);
    source->tint.Double(1.0,1.0,1.0,0.1);
    source->RenderUpsideDown(additive,0,0,screen3->w*2,screen3->h*2);
    screen3->Unbind();
    ShadedGamma(&screen3->texture,0.75f,screen);
    return screen;
   } break;
   case pdt_Multiply: {
    screen3->Bind();
    source->RenderUpsideDown(transparency,0,0,screen3->w*2,screen3->h*2);
    screen3->Unbind();
    ShadedSharpen(&screen3->texture,screen2);
    screen->background.Pick(clear);
    ShadedBlendAlphaMult(&source->texture,&screen2->texture,screen);
    return screen;
   } break;
   case pdt_BloomNearest: {
    screen2->Bind();
    source->RenderUpsideDown(transparency,0,0,screen2->w*2,screen2->h*2);
    screen2->Unbind();
    ShadedBloom(&screen2->texture,screen);
    return screen;
   } break;
   case pdt_SharpenAdd: {
    screen->Bind();
    source->RenderUpsideDown(transparency,0,0,screen->w*2,screen->h*2);
    screen->Unbind();
    ShadedSharpen(&screen->texture,screen2);
    screen->tint.Pick(alabaster);
    screen->background.Pick(clear);
    screen->Bind();
    source->RenderUpsideDown(transparency,0,0,screen->w*2,screen->h*2); 
    screen2->tint.Double(1.0,1.0,1.0,0.154);
    screen2->RenderUpsideDown(additive,0,0,screen->w,screen->h);
    screen->Unbind();
    return screen;
   } break;
   case pdt_Emboss: {
    screen->Bind();
    fbo1080p.RenderUpsideDown(transparency,0,0,screen->w*2,screen->h*2);
    screen->Unbind();
    ShadedSharpen(&screen->texture,screen2);
    screen2->tint.Pick(alabaster);
    screen2->background.Pick(clear);
    ShadedEmbossPreserveA(&screen2->texture,screen);
    return screen;
   } break;
   case pdt_SuperSharp: {
    screen2->Bind();
    source->RenderUpsideDown(transparency,0,0,screen2->w*2,screen2->h*2);
    screen2->Unbind();
    ShadedSharpen(&screen2->texture,screen);
    return screen;
   } break;
   default: OUTPUT("PixelDoublerTypes: Invalid value: %d\n",(int)mode); return screen; break;
  }
 }
};

extern PixelDoubler pixelDoubler;