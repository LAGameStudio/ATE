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

/*
 * Uses a lot of GPU bandwidth, clampable by modifying the resolution settings.
 * Use for "hyperspace"
 * When changing direction to up from down (and perhaps right to left), reverse time.
 * Produces a wide-range of lava-lamp-like effects.
 */

class NautilusShader : public GLSLShader {
public:
 float position[2];
 float resolution[2];
 float time;
 float mouse[2];

  // Uniform locations
 GLint Uposition, Uresolution, Utime, Umouse;
 
 void Init() {
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  mouse[0]=(resolution[0]/2.0f);
  mouse[1]=(resolution[1]/2.0f);
  time=0.0f;
  OUTPUT("NautilusShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform float time;\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform vec2 mouse;\n")+
   string("float e(vec3 c)\n")+
   string("{\n")+
   string("    c=cos(vec3(cos(c.r+time/6.0)*c.r-cos(c.g*3.0+time/5.0)*c.g, cos(time/4.0)*c.b/3.0*c.r-cos(time/7.0)*c.g, c.r+c.g+c.b+time));\n")+
   string("    return dot(c*c,vec3(1.0))-1.0;\n")+
   string("}\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 c=-1.0+2.0*(position.xy+gl_FragCoord.xy)/resolution.xy;\n")+
   string("    vec3 o=vec3(c.r,c.g,0.0),g=vec3(c.r,c.g,1.0)/64.0,v=vec3(0.5);\n")+
   string("//    float m = 0.4;\n")+
   string("    float m = 1.0-1.5*mouse.x/resolution.x;\n")+
   string("    for(int r=0;r<100;r++)\n")+
   string("    {\n")+
   string("      float h=e(o)-m;\n")+
   string("      if(h<0.0)break;\n")+
   string("      o+=h*10.0*g;\n")+
   string("      v+=h*0.02;\n")+
   string("    }\n")+
   string("    // light (who needs a normal?)\n")+
   string("    v+=e(o+0.1)*vec3(0.4,0.7,1.0);\n")+
   string("    // ambient occlusion\n")+
   string("    float a=0.0;\n")+
   string("    for(int q=0;q<100;q++)\n")+
   string("    {\n")+
   string("       float l = e(o+0.5*vec3(cos(1.1*float(q)),cos(1.6*float(q)),cos(1.4*float(q))))-m;\n")+
   string("       a+=clamp(4.0*l,0.0,1.0);\n")+
   string("    }\n")+
   string("    v*=a/100.0;\n")+
   string("    gl_FragColor=vec4(v,1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uposition=glGetUniformLocation(program,"position");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utime=glGetUniformLocation(program,"time");
  Umouse=glGetUniformLocation(program,"mouse");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform1f(Utime, time);
  glUniform2f(Umouse, mouse[0], mouse[1]);
 }

 void Disable() {
  glUseProgram(0);
 }
};

extern NautilusShader nautilusShader;