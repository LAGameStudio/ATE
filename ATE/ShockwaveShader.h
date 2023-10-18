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

class ShockwaveShader : public GLSLShader {
public:
 float center[2]; // in texture coord space 0-1
 float time;
 float shock[3]; // curvature A, curvature B, lens radius
 GLuint *pTex0;
 GLint Ucenter,Utime,Ushock,Utex0;
 void Init() {
  center[0]=0.5f;
  center[1]=0.5f;
  time=0.0f;
  shock[0]=10.0f;
  shock[1]=0.8f;
  shock[2]=0.1f;
  pTex0=null;
  OUTPUT("ShockwaveShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D sceneTex; //  the final scene image.\n")+
   string("uniform vec2 center; // center (in texture coord space: [0 ; 1]); Mouse position\n")+
   string("uniform float time; // effect elapsed time (s)\n")+
   string("uniform vec3 shockParams; // 10.0, 0.8, 0.1\n")+
   string("void main(){\n")+
   string("  vec2 uv = gl_TexCoord[0].xy;\n")+
   string("  vec2 texCoord = uv;\n")+
   string("  float distance = distance(uv, center);\n")+
   string("  if ( (distance <= (time + shockParams.z)) && (distance >= (time - shockParams.z)) ) {\n")+
   string("   float diff = (distance - time);\n")+
   string("   float powDiff = 1.0 - pow(abs(diff*shockParams.x), shockParams.y);\n")+
   string("   float diffTime = diff  * powDiff;\n")+
   string("   vec2 diffUV = normalize(uv - center);\n")+
   string("   texCoord = uv + (diffUV * diffTime);\n")+
   string("  }\n")+
   string("  gl_FragColor = texture2D(sceneTex, texCoord);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ucenter=glGetUniformLocation(program,"center");
  Utime=glGetUniformLocation(program,"time");
  Ushock=glGetUniformLocation(program,"shockParams");
  Utex0=glGetUniformLocation(program,"sceneTex");
 }
 void UpdateUniforms() {
  glUseProgram(program); 
  glUniform1f(Utime,       time);
  glUniform2f(Ucenter, center[0], center[1]);
  glUniform3f(Ushock, shock[0], shock[1], shock[2]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern ShockwaveShader shockwaveShader;