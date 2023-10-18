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

void ShadedStencilR( GLuint *in, GLuint *stencilR, FBOColor *out );
void ShadedStencilRG( GLuint *in, GLuint *stencilR, GLuint *stencilG, FBOColor *out );
void ShadedStencilRGB( GLuint *in, GLuint *stencilR, GLuint *stencilG, GLuint *stencilB, FBOColor *out );
void ShadedStencilRGBA( GLuint *in, GLuint *stencilR, GLuint *stencilG, GLuint *stencilB, GLuint *stencilA, FBOColor *out );

class StencilShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;
 GLint Utex0, Utex1;
 void Init() {
  pTex0=null;
  pTex1=null;
  OUTPUT("StencilShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" gl_FragColor= fr*f0.r;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilShader stencilShader;

class Stencil2Shader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2;
 GLint Utex0, Utex1, Utex2;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  OUTPUT("StencilShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" gl_FragColor= fr*f0.r +fg*f0.g;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern Stencil2Shader stencil2Shader;

class Stencil3Shader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLint Utex0, Utex1, Utex2, Utex3;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  OUTPUT("Stencil3Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex3,gl_TexCoord[0].xy);\n")+
   string(" gl_FragColor= fr*f0.r +fg*f0.g +fb*f0.b;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern Stencil3Shader stencil3Shader;

class Stencil4Shader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 GLint Utex0, Utex1, Utex2, Utex3, Utex4;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  pTex4=null;
  OUTPUT("Stencil4Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("uniform sampler2D tex4; // A value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex3,gl_TexCoord[0].xy);\n")+
   string(" vec4 fa=texture2D(tex4,gl_TexCoord[0].xy);\n")+
   string(" gl_FragColor= fr*f0.r +fg*f0.g +fb*f0.b +fa*f0.a;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utex4=glGetUniformLocation(program,"tex4");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern Stencil4Shader stencil4Shader;

class StencilAddShader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1;
 GLint Utex0, Utex1, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  OUTPUT("StencilAddShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= fr*f0.r;\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilAddShader stencilAddShader;

class StencilAdd2Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2;
 GLint Utex0, Utex1, Utex2, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  OUTPUT("StencilAdd2Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= fr*f0.r +fg*f0.g;\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilAdd2Shader stencilAdd2Shader;

class StencilAdd3Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLint Utex0, Utex1, Utex2, Utex3, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  OUTPUT("StencilAdd3Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex3,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= fr*f0.r +fg*f0.g +fb*f0.b;\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilAdd3Shader stencilAdd3Shader;

class StencilAdd4Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 GLint Utex0, Utex1, Utex2, Utex3, Utex4, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  pTex4=null;
  OUTPUT("StencilAdd4Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;     // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("uniform sampler2D tex4; // A value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex3,gl_TexCoord[0].xy);\n")+
   string(" vec4 fa=texture2D(tex4,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= fr*f0.r +fg*f0.g +fb*f0.b +fa*f0.a;\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utex4=glGetUniformLocation(program,"tex4");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilAdd4Shader stencilAdd4Shader;

class StencilAvg2Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2;
 GLint Utex0, Utex1, Utex2, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  OUTPUT("StencilAvg2Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r +fg*f0.g)/2.0;\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilAvg2Shader stencilAvg2Shader;

class StencilAvg3Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLint Utex0, Utex1, Utex2, Utex3, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  OUTPUT("StencilAvg3Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex3,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r +fg*f0.g +fb*f0.b)/3.0;\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilAvg3Shader stencilAvg3Shader;

class StencilAvg4Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 GLint Utex0, Utex1, Utex2, Utex3, Utex4, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  pTex4=null;
  OUTPUT("StencilAvg4Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("uniform sampler2D tex4; // A value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex3,gl_TexCoord[0].xy);\n")+
   string(" vec4 fa=texture2D(tex4,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r +fg*f0.g +fb*f0.b +fa*f0.a)/4.0;\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utex4=glGetUniformLocation(program,"tex4");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilAvg4Shader stencilAvg4Shader;

class StencilMult2Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2;
 GLint Utex0, Utex1, Utex2, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  OUTPUT("StencilMult2Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r) * (fg*f0.g);\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilMult2Shader stencilMult2Shader;

class StencilMult3Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLint Utex0, Utex1, Utex2, Utex3, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  OUTPUT("StencilMult3Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex3,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r)*(fg*f0.g)*(fb*f0.b);\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilMult3Shader stencilMult3Shader;

class StencilMult4Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 GLint Utex0, Utex1, Utex2, Utex3, Utex4, Ucolor;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  pTex4=null;
  OUTPUT("StencilMult4Shader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("uniform sampler2D tex4; // A value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fr=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" vec4 fg=texture2D(tex2,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex3,gl_TexCoord[0].xy);\n")+
   string(" vec4 fa=texture2D(tex4,gl_TexCoord[0].xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r)*(fg*f0.g)*(fb*f0.b)*(fa*f0.a);\n")+
   string(" gl_FragColor=stenciled*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utex4=glGetUniformLocation(program,"tex4");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilMult4Shader stencilMult4Shader;

// Where tex0 is multiplied by tex1 preserving the alpha channel (0 on either = transparent)
class StencilMult2AlphaShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;
 GLint Utex0, Utex1;
 void Init() {
  pTex0=null;
  pTex1=null;
  OUTPUT("StencilMult2AlphaShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D tex0; // A\n")+
   string("uniform sampler2D tex1; // B\n")+
   string("void main() {\n")+
   string(" vec4 fa=texture2D(tex0,gl_TexCoord[0].xy);\n")+
   string(" vec4 fb=texture2D(tex1,gl_TexCoord[0].xy);\n")+
   string(" gl_FragColor=fa*fb;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
//  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilMult2AlphaShader stencilMult2AlphaShader;