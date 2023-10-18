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

class FBOColor;

void ShadedBend( GLuint *sampler, GLuint *mixer, FBOColor *out );
void ShadedBendR( GLuint *sampler, GLuint *mixer, FBOColor *out );
void ShadedBendG( GLuint *sampler, GLuint *mixer, FBOColor *out );
void ShadedBendB( GLuint *sampler, GLuint *mixer, FBOColor *out );
void ShadedBendRG( GLuint *sampler, GLuint *mixer, FBOColor *out );
void ShadedBendGB( GLuint *sampler, GLuint *mixer, FBOColor *out );
void ShadedBendBR( GLuint *sampler, GLuint *mixer, FBOColor *out );

class BendShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;  // input, bend map
 GLuint Utex0,Utex1;
 void Init() {
  OUTPUT("BendShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0,tex1;\n")+
   string("void main(void) {\n")+
   string(" vec4 b=texture2D(tex1,gl_TexCoord[0].st);\n")+
   string(" float wx=gl_TexCoord[0].x+((b.r+b.g+b.b)/3.0)*0.05;\n")+
   string(" float wy=gl_TexCoord[0].y+((b.r+b.g+b.b)/3.0)*0.05;\n")+
   string(" gl_FragColor= texture2D(tex0, vec2(wx,wy));\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern BendShader bendShader;

class BendRShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;  // input, bend map
 GLuint Utex0,Utex1;
 void Init() {
  OUTPUT("BendRShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0,tex1;\n")+
   string("void main(void) {\n")+
   string(" vec4 b=texture2D(tex1,gl_TexCoord[0].st);\n")+
   string(" float wx=gl_TexCoord[0].x+b.r*0.05;\n")+
   string(" float wy=gl_TexCoord[0].y+b.r*0.05;\n")+
   string(" gl_FragColor= texture2D(tex0, vec2(wx,wy));\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern BendRShader bendRShader;

class BendGShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;  // input, bend map
 GLuint Utex0,Utex1;
 void Init() {
  OUTPUT("BendGShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0,tex1;\n")+
   string("void main(void) {\n")+
   string(" vec4 b=texture2D(tex1,gl_TexCoord[0].st);\n")+
   string(" float wx=gl_TexCoord[0].x+b.g*0.05;\n")+
   string(" float wy=gl_TexCoord[0].y+b.g*0.05;\n")+
   string(" gl_FragColor= texture2D(tex0, vec2(wx,wy));\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern BendGShader bendGShader;

class BendBShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;  // input, bend map
 GLuint Utex0,Utex1;
 void Init() {
  OUTPUT("BendBShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0,tex1;\n")+
   string("void main(void) {\n")+
   string(" vec4 b=texture2D(tex1,gl_TexCoord[0].st);\n")+
   string(" float wx=gl_TexCoord[0].x+b.r*0.05;\n")+
   string(" float wy=gl_TexCoord[0].y+b.r*0.05;\n")+
   string(" gl_FragColor= texture2D(tex0, vec2(wx,wy));\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern BendBShader bendBShader;

class BendRGShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;  // input, bend map
 GLuint Utex0,Utex1;
 void Init() {
  OUTPUT("BendRGShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0,tex1;\n")+
   string("void main(void) {\n")+
   string(" vec4 b=texture2D(tex1,gl_TexCoord[0].st);\n")+
   string(" float wx=gl_TexCoord[0].x+((b.r+b.g)/2.0)*0.05;\n")+
   string(" float wy=gl_TexCoord[0].y+((b.r+b.g)/2.0)*0.05;\n")+
   string(" gl_FragColor= texture2D(tex0, vec2(wx,wy));\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern BendRGShader bendRGShader;

class BendGBShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;  // input, bend map
 GLuint Utex0,Utex1;
 void Init() {
  OUTPUT("BendGBShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0,tex1;\n")+
   string("void main(void) {\n")+
   string(" vec4 b=texture2D(tex1,gl_TexCoord[0].st);\n")+
   string(" float wx=gl_TexCoord[0].x+((b.g+b.b)/2.0)*0.05;\n")+
   string(" float wy=gl_TexCoord[0].y+((b.g+b.b)/2.0)*0.05;\n")+
   string(" gl_FragColor= texture2D(tex0, vec2(wx,wy));\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern BendGBShader bendGBShader;

class BendBRShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;  // input, bend map
 GLuint Utex0,Utex1;
 void Init() {
  OUTPUT("BendBRShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0,tex1;\n")+
   string("void main(void) {\n")+
   string(" vec4 b=texture2D(tex1,gl_TexCoord[0].st);\n")+
   string(" float wx=gl_TexCoord[0].x+((b.r+b.b)/2.0)*0.05;\n")+
   string(" float wy=gl_TexCoord[0].y+((b.r+b.b)/2.0)*0.05;\n")+
   string(" gl_FragColor= texture2D(tex0, vec2(wx,wy));\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern BendBRShader bendBRShader;