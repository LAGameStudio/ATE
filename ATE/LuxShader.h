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

class LuxShader : public GLSLShader {
public:
 float position[2];
 float resolution[2];
 float rgbFactor[3];
 float time;

  // Uniform locations
 GLint UrgbFactor, Utime, Uresolution, Uposition;
 
 void Init() {
  rgbFactor[0]=4.0f;
  rgbFactor[1]=1.0f;
  rgbFactor[2]=2.0f;
  time=0.0f;
  OUTPUT("LuxShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform float time;\n")+
   string("uniform vec3 rgbFactor;\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 pos = (position.xy+gl_FragCoord.xy);\n")+
   string("    vec3 s[4];\n")+
   string("    s[0]=vec3(0);\n")+
   string("    s[3]=vec3(sin(time),cos(time),0);\n")+
   string("    s[1]=s[3].zxy;\n")+
   string("    s[2]=s[3].zzx;\n")+
   string("    float t,b,c,h=0.0;\n")+
   string("    vec3 m=vec3(0),n;\n")+
   string("    vec3 p=vec3(.2);\n")+
   string("    vec3 d=normalize(.001*gl_FragCoord.rgb-p);\n")+
   string("    for(int i=0;i<4;i++)\n")+
   string("    {\n")+
   string("        t=2.0;\n")+
   string("        for(int i=0;i<4;i++)\n")+
   string("        {\n")+
   string("            b=dot(d,n=s[i]-p);\n")+
   string("            c=b*b+.2-dot(n,n);\n")+
   string("            if(b-c<t)\n")+
   string("            if(c>0.0)\n")+
   string("            {\n")+
   string("                m=s[i];t=b-c;\n")+
   string("            }\n")+
   string("        }\n")+
   string("    p+=t*d;\n")+
   string("    d=reflect(d,n=normalize(p-m));\n")+
   string("    h+=pow(n.x*n.x,44.)+n.x*n.x*.2;\n")+
   string("    }\n")+
   string("    gl_FragColor=vec4(pow(h,rgbFactor.x),pow(h,rgbFactor.y),pow(h,rgbFactor.z),1);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uposition=glGetUniformLocation(program,"position");
  Uresolution=glGetUniformLocation(program,"resolution");
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  UrgbFactor=glGetUniformLocation(program,"rgbFactor");
  Utime=glGetUniformLocation(program,"time");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture (see Apply)
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform3f(UrgbFactor, rgbFactor[0], rgbFactor[1], rgbFactor[2]);
  glUniform1f(Utime,       time);
 }

 void Disable() {
  glUseProgram(0);
 }
};

