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

class DeformerShader : public GLSLShader {
public:
 float rgbFactor[3];
 float mouse[2]; // normalized 0-1
 float time;
 float position[2];
 float resolution[2];
 float textureOffset[2];
 GLuint *pTex0;
  // Uniform locations
 GLint UrgbFactor, Uposition, Uresolution, Umouse, Utime, UtextureOffset, Utex0, Utex1;
 
 void Init() {
  rgbFactor[0]=1.0f;
  rgbFactor[1]=1.0f;
  rgbFactor[2]=1.0f;
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  textureOffset[0]=0.0f;
  textureOffset[1]=0.0f;
  time=0.0f;
  pTex0=null;
  OUTPUT("DeformerShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// Plane Deformer\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec3 rgbFactor;\n")+
   string("uniform vec2 mouse;\n")+
   string("uniform float time;\n")+
   string("uniform vec2 textureOffset;\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 p = -1.0 + 2.0 * (position.xy+gl_FragCoord.xy) / resolution.xy;\n")+
   string("    vec2 m = -1.0 + 2.0 * mouse.xy;\n")+
   string("    float a1 = atan(p.y-m.y,p.x-m.x);\n")+
   string("    float r1 = sqrt(dot(p-m,p-m));\n")+
   string("    float a2 = atan(p.y+m.y,p.x+m.x);\n")+
   string("    float r2 = sqrt(dot(p+m,p+m));\n")+
   string("    vec2 uv;\n")+
   string("    uv.x = 0.2*time + (r1-r2)*0.25;\n")+
   string("    uv.y = sin(2.0*(a1-a2));\n")+
   string("    float w = r1*r2*0.8;\n")+
   string("    vec3 col = texture2D(tex0,uv+textureOffset).xyz;\n")+
   string("    gl_FragColor = vec4(rgbFactor*vec3(col/(.1+w)),1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  UrgbFactor=glGetUniformLocation(program,"rgbFactor");
  Utime=glGetUniformLocation(program,"time");
  Uposition=glGetUniformLocation(program,"position");
  Uresolution=glGetUniformLocation(program,"resolution");
  Umouse=glGetUniformLocation(program,"mouse");
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  UtextureOffset=glGetUniformLocation(program, "textureOffset");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(UrgbFactor, rgbFactor[0], rgbFactor[1], rgbFactor[2]);
  glUniform1f(Utime, time);
  glUniform2f(Umouse, mouse[0], mouse[1]);
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform2f(UtextureOffset, textureOffset[0], textureOffset[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern DeformerShader deformerShader;