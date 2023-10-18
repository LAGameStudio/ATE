#pragma once

#include "Crayon.h"
#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"
#include "TextureLibrary.h"

ONE(ASCIIProfile,{
 res.Set(1.0,1.0);
})
 Cartesiand res;
  Zp<GLImage> tex0;
 KEYWORDS({
  SUBWORD("res",res)
  else TILEWORD("tex0",tex0)
 })
 WORDKEYS({
  SSUBKEY("res",res)
  IMGKEY("tex0",tex0)
 })
MANY(ASCIIProfile,ASCIIProfileHandle,ASCIIProfileHandles,"ASCII",ASCIIProfiles,{})
 KEYWORDSGroup("ASCII",ASCIIProfile);
 WORDKEYSGroup("ASCII",ASCIIProfile);
DONE(ASCIIProfile);

class ASCIIShader : public GLSLShader {
public:
  Cartesiand res;
  Zp<GLuint> tex0;
 GLuint Ures,Utex0;
 void Init() {
  OUTPUT("ASCIIShader::Loading\n");
  string vert=
    string("#version 120\n")
   +string("#ifdef GL_ES\n")
   +string("precision highp float;  //precision mediump float;\n")
   +string("#endif\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("\n")
   +string("void main() {\n")
   +string(" gl_Position = ftransform();\n")
   +string(" gl_TexCoord[0]=gl_MultiTexCoord0;\n")
   +string(" v_vTexcoord = gl_TexCoord[0].st;\n")
   +string("}")
  ;
  string frag=
    string("varying vec2 v_vTexcoord;\n")
   +string("uniform vec2 res;\n")
   +string("uniform sampler2D tex0;\n")
   +string("float character(float n, vec2 p) {\n")
   +string("  p = floor(p*vec2(4.0, -4.0) + 1.5);\n")
   +string("  if (clamp(p.x,0.0, 4.0) == p.x && clamp(p.y, 0.0, 4.0) == p.y) {\n")
   +string("  if (int(mod(n/exp2(p.x + 5.0*p.y), 2.0)) == 1) return 1.0;\n")
   +string(" } else return 0.0;\n")
   +string("}\n")
   +string("void main() {\n")
   +string(" vec2 uv=v_vTexcoord*res;\n")
   +string(" vec3 col = texture2D(tex0,floor(uv/8.0)*8.0/res.xy).rgb; \n")
   +string(" float gray = (col.r + col.g + col.b)/3.0;	\n")
   +string(" float n =  65536.0;              // .\n")
   +string(" if (gray >= 0.2) n = 65600.0;    // :\n")
   +string(" if (gray >= 0.3) n = 332772.0;   // *\n")
   +string(" if (gray >= 0.4) n = 15255086.0; // o \n")
   +string(" if (gray >= 0.6) n = 23385164.0; // &\n")
   +string(" if (gray >= 0.7) n = 15252014.0; // 8\n")
   +string(" if (gray >= 0.8) n = 13199452.0; // @\n")
   +string(" if (gray >= 0.9) n = 11512810.0; // #	\n")
   +string(" vec2 p = mod(uv/4.0, 2.0) - vec2(1.0);\n")
   +string(" col = col*character(n, p);	\n")
   +string(" gl_FragColor = vec4(col, 1.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ures=glGetUniformLocation(program, "res");
  Utex0=glGetUniformLocation(program, "tex0");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Ures,(GLfloat)res.x,(GLfloat)res.y);
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex0.pointer));
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( ASCIIProfile *in ) {
  res.Set(&in->res);
  if ( in->tex0 ) tex0 = &in->tex0->texture;
 }
 void Render( FBOColor *out, ASCIIProfile *in );
};

extern ASCIIShader asciiShader;

void ShadedASCII( FBOColor *out, ASCIIProfile *in );
void ShadedASCII( FBOColor *out, ASCIIProfile *in, GLuint *input );


ONE(P2xProfile,{
 res.Set(1.0,1.0);
 rate=5.0;
})
 Cartesiand res;
 Zdouble rate;
  Zp<GLImage> tex0;
 KEYWORDS({
  SUBWORD("res",res)
  else TILEWORD("tex0",tex0)
  else NUMWORD("rate",rate)
 })
 WORDKEYS({
  SSUBKEY("res",res)
  IMGKEY("tex0",tex0)
 })
MANY(P2xProfile,P2xProfileHandle,P2xProfileHandles,"P2x",P2xProfiles,{})
 KEYWORDSGroup("P2x",P2xProfile);
 WORDKEYSGroup("P2x",P2xProfile);
DONE(P2xProfile);

class P2xShader : public GLSLShader {
public:
 Cartesiand res;
 Zp<GLuint> tex0;
 Zdouble rate;
 GLuint Ures,Utex0,Urate;
 void Init() {
  OUTPUT("P2xShader::Loading\n");
  string vert=
    string("#version 120\n")
   +string("#ifdef GL_ES\n")
   +string("precision highp float;  //precision mediump float;\n")
   +string("#endif\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("\n")
   +string("void main() {\n")
   +string(" gl_Position = ftransform();\n")
   +string(" gl_TexCoord[0]=gl_MultiTexCoord0;\n")
   +string(" v_vTexcoord = gl_TexCoord[0].st;\n")
   +string("}")
  ;
  string frag=
    string("varying vec2 v_vTexcoord;\n")
   +string("uniform vec2 res;\n")
   +string("uniform sampler2D tex0;\n")
   +string("uniform float rate;\n")
   +string("void main() {\n")
   +string(" vec2 uv=v_vTexcoord*res;\n")
   +string(" vec3 col = texture2D(tex0,floor(uv/rate)*rate/res.xy).rgb; \n")
   +string(" gl_FragColor = vec4(col, 1.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ures=glGetUniformLocation(program, "res");
  Utex0=glGetUniformLocation(program, "tex0");
  Urate=glGetUniformLocation(program, "rate");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Ures,(GLfloat)res.x,(GLfloat)res.y);
  glUniform1f(Urate,(GLfloat)rate);
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex0.pointer));
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( P2xProfile *in ) {
  res.Set(&in->res);
  rate=in->rate;
  if ( in->tex0 ) tex0 = &in->tex0->texture;
 }
 void Render( FBOColor *out, P2xProfile *in );
};

extern P2xShader p2xShader;

void ShadedP2x( FBOColor *out, P2xProfile *in );
void ShadedP2x( FBOColor *out, P2xProfile *in, GLuint *input );