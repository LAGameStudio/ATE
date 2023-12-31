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

class Frosted2Shader : public GLSLShader {
public:
 float offset;
 float factor; // rnd_factor
 float scale; // rnd_scale
 float v1[2];
 float v2[2];
 GLuint *pTex0;

  // Uniform locations
 GLint Uoffset,Ufactor,Uscale,Uv1,Uv2,Utex0;
 
 void Init() {
  offset=0.0f;
  factor=0.5f;
  scale=5.1f;
  v1[0]=92.0f;
  v1[1]=80.0f;
  v2[0]=41.0f;
  v2[1]=62.0f;
  pTex0=null;
  OUTPUT("Frosted2Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("// sequel to frosted.frag\n")+
   string("//sceneTex (sampler2D): the final scene image.\n")+
   string("//vx_offset (float): x position of the vertical red line\n")+
   string("//rnd_factor (float): effect param, default value: 0.05\n")+
   string("//rnd_scale (float): effect param, default value: 5.1\n")+
   string("//v1 (vec2): effect param, default value: vec2(92.,80.)\n")+
   string("//v2 (vec2): effect param, default value: vec2(41.,62.)\n")+
   string("uniform sampler2D sceneTex; // 0\n")+
   string("uniform float vx_offset;\n")+
   string("uniform float rnd_factor;// = 0.05;\n")+
   string("uniform float rnd_scale;// = 5.1;\n")+
   string("uniform vec2 v1;// = vec2(92.,80.);\n")+
   string("uniform vec2 v2;// = vec2(41.,62.);\n")+
   string("float rand(vec2 co)\n")+
   string("{\n")+
   string("  return fract(sin(dot(co.xy ,v1)) + cos(dot(co.xy ,v2)) * rnd_scale);\n")+
   string("}\n")+
   string("void main()\n")+
   string("{\n")+
   string("  vec2 uv = gl_TexCoord[0].xy;\n")+
   string("  vec3 tc = vec3(1.0, 0.0, 0.0);\n")+
   string("  if (uv.x < (vx_offset-0.005))\n")+
   string("  {\n")+
   string("    vec2 rnd = vec2(rand(uv.xy),rand(uv.yx));\n")+
   string("    tc = texture2D(sceneTex, uv+rnd*rnd_factor).rgb;\n")+
   string("  }\n")+
   string("  else if (uv.x>=(vx_offset+0.005))\n")+
   string("  {\n")+
   string("    tc = texture2D(sceneTex, uv).rgb;\n")+
   string("  }\n")+
   string("  gl_FragColor = vec4(tc, 1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uoffset=glGetUniformLocation(program,"vx_offset");
  Ufactor=glGetUniformLocation(program,"rnd_factor");
  Uscale=glGetUniformLocation(program,"rnd_scale");
  Uv1=glGetUniformLocation(program, "v1");
  Uv2=glGetUniformLocation(program, "v2");
  Utex0=glGetUniformLocation(program, "tex0");
 }

 void UpdateUniforms() {
  glUseProgram(program);
 
  glUniform1f(Uscale, scale);
  glUniform1f(Ufactor, factor);
  glUniform1f(Uoffset, offset);
  glUniform2f(Uv1, v1[0],v1[1]);
  glUniform2f(Uv2, v2[0],v2[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern Frosted2Shader frosted2Shader;