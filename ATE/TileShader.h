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

// This shader has no purpose.

class TileShader : public GLSLShader {
public:
 float tilesx,tilesy;
 GLuint *pTex0;
 GLint Utx,Uty,Utex0;
 void Init() {
  pTex0=null;
  tilesx=tilesy=1.0f;
  string frag=
   string("#version 120\n")+   
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform float tx,ty;\n")+
   string("uniform sampler2D tex0;\n")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y));\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utx=glGetUniformLocation(program, "tx");
  Uty=glGetUniformLocation(program, "ty");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1f(Utx,tilesx);
  glUniform1f(Uty,tilesy);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern TileShader tileShader;