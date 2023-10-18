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

class ClodShader : public GLSLShader {
public:
 float undulation[3];
 float position[2];
 float resolution[2];
 float time;

  // Uniform locations
 GLint Uresolution, Utime,Uposition,Uundulation;
 
 void Init() {
  undulation[0]=0.1f;
  undulation[1]=0.05f;
  undulation[2]=0.1f;
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=5000.0f;
  resolution[1]=5000.0f;
  time=0.0f;
  OUTPUT("ClodShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec3 undulation;\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform float time;\n")+
   string("float f(vec3 o)\n")+
   string("{\n")+
   string("    float a=(sin(o.x)+o.y*.25)*.35;\n")+
   string("    o=vec3(cos(a)*o.x-sin(a)*o.y,sin(a)*o.x+cos(a)*o.y,o.z);\n")+
   string("    return dot(cos(o)*cos(o),vec3(1))-1.2;\n")+
   string("}\n")+
   string("vec3 s(vec3 o,vec3 d)\n")+
   string("{\n")+
   string("    float t=0.0;\n")+
   string("    float dt = 0.2;\n")+
   string("    float nh = 0.0;\n")+
   string("    float lh = 0.0;\n")+
   string("    for(int i=0;i<50;i++)\n")+
   string("    {\n")+
   string("        nh = f(o+d*t);\n")+
   string("        if(nh>0.0) { lh=nh; t+=dt; }\n")+
   string("    }\n")+
   string("    if( nh>0.0 ) return vec3(.93,.94,.85);\n")+
   string("    t = t - dt*nh/(nh-lh);\n")+
   string("    vec3 e=vec3(.1,0.0,0.0);\n")+
   string("    vec3 p=o+d*t;\n")+
   string("    vec3 n=-normalize(vec3(f(p+e),f(p+e.yxy),f(p+e.yyx))+vec3((sin(p*75.)))*.01);\n")+
   string("    return vec3( mix( ((max(-dot(n,vec3(.577)),0.) + 0.125*max(-dot(n,vec3(-.707,-.707,0)),0.)))*(mod\n")+
   string("(length(p.xy)*20.,2.)<1.0?vec3(.71,.85,.25):vec3(.79,.93,.4)),vec3(.93,.94,.85), vec3(pow(t/9.,5.)) ) );\n")+
   string("}\n")+
   string("void main()\n")+
   string("{\n")+
   string("    vec2 p = -1.0 + 2.0 * (position.xy+gl_FragCoord.xy) / resolution.xy;\n")+
   string("    gl_FragColor=vec4(s(vec3(sin(time*undulation.x)*.05,cos(time)*undulation.y,undulation.z*sin(time)), normalize(vec3(p.xy,1.0))),1.0);\n")+
   string("}")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uundulation=glGetUniformLocation(program,"undulation");
  Uposition=glGetUniformLocation(program,"position");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utime=glGetUniformLocation(program,"time");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture (see Apply)
  glUniform3f(Uundulation, undulation[0], undulation[1],undulation[2]);
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform1f(Utime, time);
 }

 void Disable() {
  glUseProgram(0);
 }
};

extern ClodShader clodShader;