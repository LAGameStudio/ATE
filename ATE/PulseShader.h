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

class PulseShader : public GLSLShader {
public:
 float time;
 float position[2];
 float resolution[2];
 GLuint *pTex0;

  // Uniform locations
 GLint Uposition, Uresolution, Umouse, Utime, Utex0;
 
 void Init() {
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  time=0.0f;
  pTex0=null;
  OUTPUT("PulseShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform float time;\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform sampler2D tex0;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 h2 = resolution.xy/2.0;\n")+
   string("    vec2 cPos = (position.xy+gl_FragCoord.xy);\n")+
   string("   	vec2 pos=cPos;\n")+
   string("    cPos.x -= 0.5*h2.x*sin(time/2.0)+0.3*h2.x*cos(time)+h2.x;\n")+
   string("    cPos.y -= 0.4*h2.y*sin(time/5.0)+0.3*h2.y*cos(time)+h2.y;\n")+
   string("    float cLength = length(cPos);\n")+
   string("    vec2 uv = pos.xy/resolution.xy+(cPos/cLength)*sin(cLength/30.0-time*10.0)/25.0;\n")+
   string("    vec3 col = texture2D(tex0,uv).xyz*50.0/cLength;\n")+
   string("    gl_FragColor = vec4(col,1.0);\n")+
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
 }

 void UpdateUniforms() {
  glUseProgram(program);  
 
  glUniform1f(Utime,       time);
  glUniform2f(Uposition, position[0], position[1]);
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

extern PulseShader pulseShader;