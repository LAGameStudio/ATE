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

void ShadedVortex( GLuint *in, GLuint *in2, FBOColor *out );

class VortexShader : public GLSLShader {
public:
 float time;
 float position[2];
 float resolution[2];
 GLuint *pTex0,*pTex1;

  // Uniform locations
 GLint Uposition, Uresolution, Utime, Utex0, Utex1;
 
 void Init() {
  Uposition = Uresolution = Utime = Utex0 = Utex1 = 0;
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  time=0.0f;
  pTex0=pTex1=null;
  OUTPUT("VortexShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform float time;\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main(void) {\n")+
   string("    vec2 uv;\n")+
   string("    vec2 p = -1.0 + 2.0 * (position.xy+gl_FragCoord.xy) / resolution.xy;\n")+
   string("    float a = atan(p.y,p.x);\n")+
   string("    float r = sqrt(dot(p,p));\n")+
   string("    float s = r * (1.0+0.8*cos(time*1.0));\n")+
   string("    uv.x =          .02*p.y+.03*cos(-time+a*3.0)/s;\n")+
   string("    uv.y = .1*time +.02*p.x+.03*sin(-time+a*3.0)/s;\n")+
   string("    float w = .9 + pow(max(1.5-r,0.0),4.0);\n")+
   string("    w*=0.6+0.4*cos(time+3.0*a);\n")+
   string("    vec3 col =  texture2D(tex0,uv).xyz;\n")+
   string("    gl_FragColor = vec4(col*w,1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  Uposition=glGetUniformLocation(program,"position");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);  
 
  glUniform1f(Utime,       time);
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern VortexShader vortexShader;