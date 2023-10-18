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

extern Interface input;

class FBOColor;
void ShadedBloom( GLuint *in, FBOColor *out );

class BloomShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0,Ua,Ub,Uc,Ud,Ue,Uf,Ug,Uk1,Uk2;
 float a,b,c,d,e,f,g;
 int kernel1,kernel2;
 Zstring frag;
 void Init() {
  a=0.0004f;
  b=0.012f;
  c=0.009f;
  d=0.0075f;
  e=0.3f;
  f=0.5f;
  g=0.25f;
  kernel1=4;
  kernel2=3;
  pTex0=null;
  OUTPUT("BloomShader::Loading\n");
  frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform float a,b,c,d,e,f,g;\n")+
   string("uniform int k1,k2;\n")+
   string("uniform sampler2D tex0;\n")+
   string("void main(){\n")+
   string(" vec4 sum = vec4(0);\n")+
   string(" vec2 texcoord = vec2(gl_TexCoord[0]);\n")+
   string(" int j,i;\n")+
   string(" for( i= -k1 ;i < k1; i++) { for (j = -k2; j < k2; j++) {\n")+
   string("   sum += texture2D(tex0, texcoord + vec2(j, i)*a) * g;\n")+
   string("  }\n")+
   string(" }\n")+
   string(" if (texture2D(tex0, texcoord).r < e) {\n")+
   string("  gl_FragColor = sum*sum*b + texture2D(tex0, texcoord);\n")+
   string(" } else {\n")+
   string("  if (texture2D(tex0, texcoord).r < f) {\n")+
   string("   gl_FragColor = sum*sum*c + texture2D(tex0, texcoord);\n")+
   string("  } else {\n")+
   string("   gl_FragColor = sum*sum*d + texture2D(tex0, texcoord);\n")+
   string("  }\n")+
   string(" }\n")+
   string("}")
   ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Ua=glGetUniformLocation(program, "a");
  Ub=glGetUniformLocation(program, "b");
  Uc=glGetUniformLocation(program, "c");
  Ud=glGetUniformLocation(program, "d");
  Ue=glGetUniformLocation(program, "e");
  Uf=glGetUniformLocation(program, "f");
  Ug=glGetUniformLocation(program, "g");
  Uk1=glGetUniformLocation(program, "k1");
  Uk2=glGetUniformLocation(program, "k2");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1i(Uk1,kernel1);
  glUniform1i(Uk2,kernel2);
  glUniform1f(Ua,a);
  glUniform1f(Ub,b);
  glUniform1f(Uc,c);
  glUniform1f(Ud,d);
  glUniform1f(Ue,e);
  glUniform1f(Uf,f);
  glUniform1f(Ug,g);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 Zdouble keyDelay;
 void DebugControls() {
  if ( keyDelay > 0.0 ) keyDelay-=FRAMETIME;
  else
  if ( input.KeyDown(DX_1) ) {
   if ( input.KeyDown(DX_NUMPADPLUS) ) a+=FRAMETIME;
   else if ( input.KeyDown(DX_NUMPADMINUS) ) a-=FRAMETIME;
   keyDelay=0.2;
  } else if ( input.KeyDown(DX_2) ) {
   if ( input.KeyDown(DX_NUMPADPLUS) ) b+=FRAMETIME;
   else if ( input.KeyDown(DX_NUMPADMINUS) ) b-=FRAMETIME;
   keyDelay=0.2;
  } else if ( input.KeyDown(DX_3) ) {
   if ( input.KeyDown(DX_NUMPADPLUS) ) c+=FRAMETIME;
   else if ( input.KeyDown(DX_NUMPADMINUS) ) c-=FRAMETIME;
   keyDelay=0.2;
  } else if ( input.KeyDown(DX_4) ) {
   if ( input.KeyDown(DX_NUMPADPLUS) ) d+=FRAMETIME;
   else if ( input.KeyDown(DX_NUMPADMINUS) ) d-=FRAMETIME;
   keyDelay=0.2;
  } else if ( input.KeyDown(DX_5) ) {
   if ( input.KeyDown(DX_NUMPADPLUS) ) e+=FRAMETIME;
   else if ( input.KeyDown(DX_NUMPADMINUS) ) e-=FRAMETIME;
   keyDelay=0.2;
  } else if ( input.KeyDown(DX_6) ) {
   if ( input.KeyDown(DX_NUMPADPLUS) ) f+=FRAMETIME;
   else if ( input.KeyDown(DX_NUMPADMINUS) ) f-=FRAMETIME;
   keyDelay=0.2;
  } else if ( input.KeyDown(DX_7) ) {
   if ( input.KeyDown(DX_NUMPADPLUS) ) g+=FRAMETIME;
   else if ( input.KeyDown(DX_NUMPADMINUS) ) g-=FRAMETIME;
   keyDelay=0.2;
  } else {
   if ( input.KeyDown(DX_LSHIFT) ) {
    if ( input.KeyDown(DX_NUMPADPLUS) ) kernel1+=1;
    else if ( input.KeyDown(DX_NUMPADMINUS) ) kernel1-=1;
    keyDelay=0.2;
   } else if ( input.KeyDown(DX_RSHIFT) ) {
    if ( input.KeyDown(DX_NUMPADPLUS) ) kernel2+=1;
    else if ( input.KeyDown(DX_NUMPADMINUS) ) kernel2-=1;
    keyDelay=0.2;
   }
  }
 }
};


extern BloomShader bloomShader;