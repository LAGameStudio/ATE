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


void ShadedGamma( GLuint *in, float gamma, FBOColor *out );

class GammaShader : public GLSLShader {
public:
 float gamma;
 GLuint *pTex0;
 GLint Ugamma,Utex0;
 void Init() {
  gamma=0.5f;  
  OUTPUT("GammaShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D sceneBuffer;\n")+
   string("uniform float gamma;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("  vec2 uv = gl_TexCoord[0].xy;\n")+
   string("  vec3 color = texture2D(sceneBuffer, uv).rgb;\n")+
   string("  gl_FragColor.rgb = pow(color, vec3(1.0 / gamma));\n")+
   string("  gl_FragColor.a = 1.0;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ugamma=glGetUniformLocation(program,"gamma");
  Utex0=glGetUniformLocation(program,"sceneBuffer");
 }
 void UpdateUniforms() {
  glUseProgram(program); 
  glUniform1f(Ugamma, gamma);
  
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern GammaShader gammaShader;