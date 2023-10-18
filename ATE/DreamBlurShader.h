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

class DreamBlurShader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0;
 GLint Ucolor,Utex0;
 void Init() {
  color.Pick(alabaster);
  pTex0=null;
  OUTPUT("DreamBlurShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D sceneTex; // 0\n")+
   string("uniform vec4 color;\n")+
   string("void main ()\n")+
   string("{\n")+
   string("  vec2 uv = gl_TexCoord[0].xy;\n")+
   string("  vec4 c = texture2D(sceneTex, uv);\n")+
   string("  c += texture2D(sceneTex, uv+0.001);\n")+
   string("  c += texture2D(sceneTex, uv+0.003);\n")+
   string("  c += texture2D(sceneTex, uv+0.005);\n")+
   string("  c += texture2D(sceneTex, uv+0.007);\n")+
   string("  c += texture2D(sceneTex, uv+0.009);\n")+
   string("  c += texture2D(sceneTex, uv+0.011);\n")+
   string("  c += texture2D(sceneTex, uv-0.001);\n")+
   string("  c += texture2D(sceneTex, uv-0.003);\n")+
   string("  c += texture2D(sceneTex, uv-0.005);\n")+
   string("  c += texture2D(sceneTex, uv-0.007);\n")+
   string("  c += texture2D(sceneTex, uv-0.009);\n")+
   string("  c += texture2D(sceneTex, uv-0.011);\n")+
   string("  c.rgb = vec3((c.r+c.g+c.b)/3.0);\n")+
   string("  c = c / 9.5;\n")+
   string("  gl_FragColor = c*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"sceneTex");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform4f(Ucolor, color.rf, color.gf, color.bf, color.af);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
}
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern DreamBlurShader dreamblurShader;

class DreamBlurRGBShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLint Ucolor,Utex0,Ua,Ub,Uc,Ud,Ue,Uf,Ufactor,Uoffset;
 float a,b,c,d,e,f,factor;
 float offset[2];
 void Init() {
  a=0.001f;
  b=0.003f;
  c=0.005f;
  d=0.007f;
  e=0.009f;
  f=0.011f;
  factor=9.5f;
  offset[0]=0.0f;//16.0f;
  offset[1]=0.0f;//-16.0f;
  pTex0=null;
  OUTPUT("DreamBlurRGBShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D sceneTex; // 0\n")+
   string("uniform float a,b,c,d,e,f,factor;\n")+
   string("uniform vec2 offset;\n")+
   string("void main () {\n")+
   string("  vec2 uv = gl_TexCoord[0].xy+offset.xy;\n")+
   string("  vec4 color = texture2D(sceneTex, uv);\n")+
   string("  color += texture2D(sceneTex, uv+a);\n")+
   string("  color += texture2D(sceneTex, uv+b);\n")+
   string("  color += texture2D(sceneTex, uv+c);\n")+
   string("  color += texture2D(sceneTex, uv+d);\n")+
   string("  color += texture2D(sceneTex, uv+e);\n")+
   string("  color += texture2D(sceneTex, uv+f);\n")+
   string("  color += texture2D(sceneTex, uv-a);\n")+
   string("  color += texture2D(sceneTex, uv-b);\n")+
   string("  color += texture2D(sceneTex, uv-c);\n")+
   string("  color += texture2D(sceneTex, uv-d);\n")+
   string("  color += texture2D(sceneTex, uv-e);\n")+
   string("  color += texture2D(sceneTex, uv-f);\n")+
   string("  gl_FragColor = color / vec4(factor);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"sceneTex");
  Ua=glGetUniformLocation(program,"a");
  Ub=glGetUniformLocation(program,"b");
  Uc=glGetUniformLocation(program,"c");
  Ud=glGetUniformLocation(program,"d");
  Ue=glGetUniformLocation(program,"e");
  Uf=glGetUniformLocation(program,"f");
  Ufactor=glGetUniformLocation(program,"factor");
  Uoffset=glGetUniformLocation(program,"offset");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1f(Ua,a);
  glUniform1f(Ub,b);
  glUniform1f(Uc,c);
  glUniform1f(Ud,d);
  glUniform1f(Ue,e);
  glUniform1f(Uf,f);
  glUniform1f(Ufactor,factor);
  glUniform2fv(Uoffset,1,offset);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
}
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern DreamBlurRGBShader dreamblurrgbShader;