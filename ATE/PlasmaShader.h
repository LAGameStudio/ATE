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

#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"

class PlasmaShader : public GLSLShader {
public:
 float position[2];
 float resolution[2];
 float scale[2];
 float factor[4];
 float factor2[2];
 float time;

  // Uniform locations
 GLint Uposition, Uresolution, Utime, Uscale, Ufactor, Ufactor2;
 
 void Reset() {
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  scale[0]=0.2f;
  scale[1]=0.2f;
  factor[0]=1000.0f;
  factor[1]=100.0f;
  factor[2]=40.0f;
  factor[3]=2.0f;
  factor2[0]=3.0f;
  factor2[1]=2.0f;
  time=0.0f;
 }

 void Init() {
  Uposition=Uresolution=Utime=Uscale=Ufactor=Ufactor2=0;
  Reset();
  OUTPUT("PlasmaShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform vec2 scale;\n")+
   string("uniform vec4 factor;\n")+
   string("uniform vec2 factor2;\n")+
   string("uniform float time;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("   float x = position.x+gl_FragCoord.x;\n")+
   string("   float y = position.y+gl_FragCoord.y;\n")+
   string("   float mov0 = x+y+cos(sin(time)*factor.w)*factor2.y+sin(x/factor.y)*factor.x;\n")+
   string("   float mov1 = y / resolution.y / scale.y + time;\n")+
   string("   float mov2 = x / resolution.x / scale.x;\n")+
   string("   float c1 = abs(sin(mov1+time)/factor.w+mov2/factor.w-mov1-mov2+time);\n")+
   string("   float c2 = abs(sin(c1+sin(mov0/factor.x+time)+sin(y/factor.z+time)+sin((x+y)/factor.y)*factor2.x));\n")+
   string("   float c3 = abs(sin(c2+cos(mov1+mov2+c2)+cos(mov2)+sin(x/factor.x)));\n")+
   string("   gl_FragColor = vec4( c1,c2,c3,1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uniform(&Uposition,"position");
  Uniform(&Uresolution,"resolution");
  Uniform(&Utime,"time");
  Uniform(&Uscale,"scale");
  Uniform(&Ufactor,"factor");
  Uniform(&Ufactor2,"factor2");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform1f(Utime, time);
  glUniform2f(Uscale, scale[0], scale[1]);
  glUniform4f(Ufactor, factor[0], factor[1], factor[2], factor[3]);
  glUniform2f(Ufactor2, factor2[0], factor2[1]);
 }

 void Disable() {
  glUseProgram(0);
 }
};

extern PlasmaShader plasmaShader;