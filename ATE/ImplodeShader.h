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

class ImplodeShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 void Init() {
  OUTPUT("ImplodeShader::Loading\n");
  pTex0=null;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D tex;\n")+
   string("void main(){\n")+
   string("vec2 cen = vec2(0.5,0.5) - gl_TexCoord[0].xy;\n")+
   string("vec2 mcen = 0.07*log(length(cen))*normalize(cen);\n")+
   string("gl_FragColor = texture2D(tex, gl_TexCoord[0].xy+mcen);\n")+
   string("}")
   ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex");
 }
 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};


extern ImplodeShader implodeShader;