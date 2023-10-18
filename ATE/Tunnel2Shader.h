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

class Tunnel2Shader : public GLSLShader {
public:
 float resolution[2];
 float time;
 GLuint *pTex0;
 GLint Utime,Uresolution,Utex0;
 void Init() {
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  time=0.0f;
  pTex0=null;
  OUTPUT("Tunnel2Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec2 resolution; // Screen resolution (target resolution)\n")+
   string("uniform float time; // time in seconds\n")+
   string("uniform sampler2D tex0; // scene buffer (input FBO of screen)\n")+
   string("void main(void) {\n")+
   string(" vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;\n")+
   string(" vec2 uv;\n")+
   string(" float a = atan(p.y,p.x);\n")+
   string(" float r = sqrt(dot(p,p));\n")+
   string(" uv.x = .15*time+.1/r;\n")+
   string(" uv.y = .1*time + a/(3.1416);\n")+
   string(" vec4 Color=texture2D(tex0,uv);\n")+
   string(" float d=distance(gl_TexCoord[0].xy,vec2(0.5,0.5));\n")+
   string(" Color.rgb *= smoothstep(0.4,0.3,d);\n")+
   string(" gl_FragColor = Color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
 }

 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utex0=glGetUniformLocation(program, "tex0");
 }
 
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1f(Utime,       time);
  glUniform2f(Uresolution, resolution[0], resolution[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }

};

extern Tunnel2Shader tunnel2Shader;