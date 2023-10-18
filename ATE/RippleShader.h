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

class RippleShader : public GLSLShader {
public:
 float time;
 float scale;
 float resolution[2];
 GLuint *pTex0;

  // Uniform locations
 GLint Utime, Uscale, Uresolution, Utex0;
 
 void Init() {
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  time=0.0f;
  scale=12.0f;
  pTex0=null;
  OUTPUT("RippleShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec2 resolution; // Screen resolution\n")+
   string("uniform float time; // time in seconds\n")+
   string("uniform float scale; // scale (default: 12.0)\n")+
   string("uniform sampler2D tex0; // thing to ripple\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("  vec2 tc = gl_TexCoord[0].xy;\n")+
   string("  vec2 p = -1.0 + 2.0 * tc;\n")+
   string("  float len = length(p);\n")+
   string("  vec2 uv = tc + (p/len)*cos(len*scale-time*4.0)*0.03;\n")+
   string("  vec3 col = texture2D(tex0,uv).xyz;\n")+
   string("  gl_FragColor = vec4(col,1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  Uscale=glGetUniformLocation(program,"scale");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utex0=glGetUniformLocation(program, "tex0");
 }

 void UpdateUniforms() {
  glUseProgram(program); 
  glUniform1f(Utime, time);
  glUniform1f(Uscale, scale);
  glUniform2f(Uresolution, resolution[0], resolution[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0); glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern RippleShader rippleShader;