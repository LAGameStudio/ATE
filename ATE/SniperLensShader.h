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

class SniperLensShader : public GLSLShader {
public:
 float lensRadius[2];
 GLuint *pTex0;
 GLint Ulens, Utex0;
 void Init() {
  lensRadius[0]=0.45f;
  lensRadius[1]=0.38f;
  pTex0=null;  
  OUTPUT("SniperLensShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D sceneTex; // FBO texture input (what to distort)\n")+
   string("uniform vec2 lensRadius; // 0.45, 0.38\n")+
   string("void main()\n")+
   string("{\n")+
   string("  vec4 Color = texture2D(sceneTex, gl_TexCoord[0].xy);\n")+
   string("  float dist = distance(gl_TexCoord[0].xy, vec2(0.5,0.5));\n")+
   string("  Color.rgb *= smoothstep(lensRadius.x, lensRadius.y, dist);\n")+
   string("  gl_FragColor = Color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ulens=glGetUniformLocation(program,"lensRadius");
  Utex0=glGetUniformLocation(program, "tex0");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Ulens, lensRadius[0], lensRadius[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }

};

extern SniperLensShader sniperlensShader;