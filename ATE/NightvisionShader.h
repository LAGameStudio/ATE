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

class NightvisionShader : public GLSLShader {
public:
 Crayon color;
 float time;
 float luminance;
 float amplification;
 float coverage;
 GLuint *pTex0,*pTex1,*pTex2; // scene, noise, mask
 GLint Ucolor,Utime,Uluminance,Uamplification,Ucoverage,Utex0,Utex1,Utex2;
 void Init() {
  color.Float(0.1f,95.0f,0.2f,1.0f);
  time=0.0f;
  pTex0=pTex1=pTex2=null;
  luminance=0.2f;
  amplification=0.4f;
  coverage=0.5f;
  OUTPUT("NightvisionShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D sceneBuffer;\n")+
   string("uniform sampler2D noiseTex;\n")+
   string("uniform sampler2D maskTex;\n")+
   string("uniform float elapsedTime; // seconds\n")+
   string("uniform float luminanceThreshold; // 0.2\n")+
   string("uniform float colorAmplification; // 4.0\n")+
   string("uniform vec3 visionColor; // = vec3(0.1, 0.95, 0.2); \"green\"\n")+
   string("void main ()\n")+
   string("{\n")+
   string("  vec4 finalColor;\n")+
   string("  {\n")+
   string("    vec2 uv;\n")+
   string("    uv.x = 0.4*sin(elapsedTime*50.0);\n")+
   string("    uv.y = 0.4*cos(elapsedTime*50.0);\n")+
   string("    float m = texture2D(maskTex, gl_TexCoord[0].st).r;\n")+
   string("    vec3 n = texture2D(noiseTex,\n")+
   string("                 (gl_TexCoord[0].st*3.5) + uv).rgb;\n")+
   string("    vec3 c = texture2D(sceneBuffer, gl_TexCoord[0].st\n")+
   string("                               + (n.xy*0.005)).rgb;\n")+
   string("    float lum = dot(vec3(0.30, 0.59, 0.11), c);\n")+
   string("    if (lum < luminanceThreshold)\n")+
   string("      c *= colorAmplification;\n")+
   string("    finalColor.rgb = (c + (n*0.2)) * visionColor * m;\n")+
   string("   }\n")+
   string("  gl_FragColor.rgb = finalColor.rgb;\n")+
   string("  gl_FragColor.a = 1.0;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"visionColor");
  Utime=glGetUniformLocation(program,"elapsedTime");
  Uluminance=glGetUniformLocation(program,"luminanceThreshold");
  Uamplification=glGetUniformLocation(program,"colorAmplification");
  Ucoverage=glGetUniformLocation(program,"effectCoverage");
  Utex0=glGetUniformLocation(program,"sceneBuffer");
  Utex1=glGetUniformLocation(program,"noiseTex");
  Utex2=glGetUniformLocation(program,"maskTex");
 }
 void UpdateUniforms() {
  glUseProgram(program); 
  glUniform1f(Utime,  time);
  glUniform3f(Ucolor, color.rf, color.gf, color.bf);
  glUniform1f(Uamplification, amplification);
  glUniform1f(Uluminance, luminance);
  glUniform1f(Ucoverage, coverage);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern NightvisionShader nightvisionShader;