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

class SwirlShader : public GLSLShader {
public:
 float time, radius, angle;
 float resolution[2];
 float center[2];
 GLuint *pTex0;
 GLint Utime,Uradius,Uangle,Uresolution,Ucenter,Utex0;
 void Init() {
  time=0.0f;
  radius=200.0f;
  angle=0.8f;
  center[0]=400.0f;
  center[1]=300.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  pTex0=null;
  OUTPUT("SwirlShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0;  // Scene buffer\n")+
   string("uniform float time;  // time value\n")+
   string("uniform vec2 resolution;  // render target\n")+
   string("// Swirl effect parameters\n")+
   string("uniform float radius;// = 200.0;\n")+
   string("uniform float angle;// = 0.8;\n")+
   string("uniform vec2 center;// = vec2(400.0, 300.0);\n")+
   string("vec4 PostFX(sampler2D tex, vec2 uv, float time)\n")+
   string("{\n")+
   string("  vec2 texSize = vec2(resolution.x, resolution.y);\n")+
   string("  vec2 tc = uv * texSize;\n")+
   string("  tc -= center;\n")+
   string("  float dist = length(tc);\n")+
   string("  if (dist < radius)\n")+
   string("  {\n")+
   string("    float percent = (radius - dist) / radius;\n")+
   string("    float theta = percent * percent * angle * 8.0;\n")+
   string("    float s = sin(theta);\n")+
   string("    float c = cos(theta);\n")+
   string("    tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));\n")+
   string("  }\n")+
   string("  tc += center;\n")+
   string("  vec3 color = texture2D(tex0, tc / texSize).rgb;\n")+
   string("  return vec4(color, 1.0);\n")+
   string("}\n")+
   string("void main (void)\n")+
   string("{\n")+
   string("  vec2 uv = gl_TexCoord[0].st;\n")+
   string("  gl_FragColor = PostFX(tex0, uv, time);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Uradius=glGetUniformLocation(program, "radius");
  Uangle=glGetUniformLocation(program, "angle");
  Uresolution=glGetUniformLocation(program, "resolution");
  Ucenter=glGetUniformLocation(program, "center");
  Utex0=glGetUniformLocation(program, "tex0");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1f(Utime, time);
  glUniform1f(Uradius, radius);
  glUniform1f(Uangle, angle);
  glUniform2f(Uresolution, resolution[0],resolution[1]);
  glUniform2f(Ucenter, center[0],center[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern SwirlShader swirlShader;