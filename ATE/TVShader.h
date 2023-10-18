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
#include "TextureLibrary.h"

class TVShader : public GLSLShader {
public:
 float time;
 float size;
 float position[2];
 float resolution[2];
 GLuint *pTex0;

  // Uniform locations
 GLint Uposition, Uresolution, Utime, Usize, Utex0;
 
 void Init() {
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  size=512.0f;
  time=0.0f;
  pTex0=null;
  OUTPUT("TVShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform float time;\n")+
   string("uniform float size;\n")+
   string("uniform sampler2D tex0;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 q = (position.xy+gl_FragCoord.xy) / resolution.xy;\n")+
   string("    vec2 uv = 0.5 + (q-0.5)*(0.9 + 0.1*sin(0.2*time));\n")+
   string("    vec3 oricol = texture2D(tex0,vec2(q.x,q.y)).xyz;\n")+
   string("    vec3 col;\n")+
   string("    col.r = texture2D(tex0,vec2(uv.x+0.003,-uv.y)).x;\n")+
   string("    col.g = texture2D(tex0,vec2(uv.x+0.000,-uv.y)).y;\n")+
   string("    col.b = texture2D(tex0,vec2(uv.x-0.003,-uv.y)).z;\n")+
   string("    col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);\n")+
   string("    col *= 0.9+0.1*sin(10.0*time+uv.y*size);\n")+
   string("    gl_FragColor = vec4(col,1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  Usize=glGetUniformLocation(program,"size");
  Uposition=glGetUniformLocation(program,"position");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utex0=glGetUniformLocation(program, "tex0");
 }

 void UpdateUniforms() {
  glUseProgram(program);
 
  glUniform1f(Utime, time);
  glUniform1f(Usize, size);
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};
extern TVShader tvShader;

ONE(OldTVProfile,{
 vertJerkOpt    = 1.0;
 vertMovementOpt= 1.0;
 bottomStaticOpt= 1.0;
 scalinesOpt    = 1.0;
 rgbOffsetOpt   = 1.0;
 horzFuzzOpt    = 1.0;
 resolution.Set(1.0,1.0);
})
 Zdouble time,vertJerkOpt,vertMovementOpt,bottomStaticOpt,scalinesOpt,rgbOffsetOpt,horzFuzzOpt;
  Zp<GLuint> tex;
 Cartesiand resolution;
 KEYWORDS({
  KEYWORD("vertJerkOpt",{vertJerkOpt=w;})
  else KEYWORD("vertMovementOpt",{vertMovementOpt=w;})
  else KEYWORD("bottomStaticOpt",{bottomStaticOpt=w;})
  else KEYWORD("scalinesOpt",{scalinesOpt=w;})
  else KEYWORD("rgbOffsetOpt",{rgbOffsetOpt=w;})
  else KEYWORD("horzFuzzOpt",{horzFuzzOpt=w;})
  else SUBWORD("resolution",resolution)
 })
 WORDKEYS({
  NUMBKEY("vertJerkOpt",vertJerkOpt)
  NUMBKEY("vertMovementOpt",vertMovementOpt)
  NUMBKEY("bottomStaticOpt",bottomStaticOpt)
  NUMBKEY("scalinesOpt",scalinesOpt)
  NUMBKEY("rgbOffsetOpt",rgbOffsetOpt)
  NUMBKEY("horzFuzzOpt",horzFuzzOpt)
  SSUBKEY("resolution",resolution)
 })
MANY(OldTVProfile,OldTVProfileHandle,OldTVProfileHandles,"OldTV",OldTVProfiles,{})
 KEYWORDSGroup("OldTV",OldTVProfile);
 WORDKEYSGroup("OldTV",OldTVProfile);
DONE(OldTVProfile);

class OldTVShader : public GLSLShader {
public:
  Zdouble time,vertJerkOpt,vertMovementOpt,bottomStaticOpt,scalinesOpt,rgbOffsetOpt,horzFuzzOpt;
  Zp<GLuint> tex;
  Cartesiand resolution;
 GLuint Utime,UvertJerkOpt,UvertMovementOpt,UbottomStaticOpt,UscalinesOpt,UrgbOffsetOpt,UhorzFuzzOpt,Utex,Uresolution;
 void Init() {
  OUTPUT("OldTVShader::Loading\n");
  string vert=
    string("#version 120\n")
   +string("#ifdef GL_ES\n")
   +string("precision highp float;  //precision mediump float;\n")
   +string("#endif\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("varying vec4 v_vColour;\n")
   +string("\n")
   +string("void main() {\n")
   +string(" gl_Position = ftransform();\n")
   +string(" gl_TexCoord[0]=gl_MultiTexCoord0;\n")
   +string(" v_vColour = gl_Color;\n")
   +string(" v_vTexcoord = gl_TexCoord[0].st;\n")
   +string("}")
  ;
  string frag=
    string("#version 120\n")
   +string("#ifdef GL_ES\n")
   +string("precision highp float;  //precision mediump float;\n")
   +string("#endif\n")
   +string("// change these values to 0.0 to turn off individual effects\n")
   +string("uniform float time;\n")
   +string("uniform float vertJerkOpt;     // = 1.0;\n")
   +string("uniform float vertMovementOpt; // = 1.0;\n")
   +string("uniform float bottomStaticOpt; // = 1.0;\n")
   +string("uniform float scalinesOpt;     // = 1.0;\n")
   +string("uniform float rgbOffsetOpt;    // = 1.0;\n")
   +string("uniform float horzFuzzOpt;     // = 1.0;\n")
   +string("uniform sampler2D tex;\n")
   +string("uniform vec2 resolution;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }\n")
   +string("float snoise(vec2 v) {\n")
   +string("  const vec4 C = vec4(0.211324865405187,0.366025403784439,-0.577350269189626,0.024390243902439); \n")
   +string("  vec2 i  = floor(v + dot(v, C.yy) );\n")
   +string("  vec2 x0 = v -   i + dot(i, C.xx);\n")
   +string("  vec2 i1;\n")
   +string("  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\n")
   +string("  vec4 x12 = x0.xyxy + C.xxzz;\n")
   +string("  x12.xy -= i1;\n")
   +string("  i = mod289(i); // Avoid truncation effects in permutation\n")
   +string("  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) + i.x + vec3(0.0, i1.x, 1.0 ));\n")
   +string("  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\n")
   +string("  m = m*m ;\n")
   +string("  m = m*m ;\n")
   +string("  vec3 x = 2.0 * fract(p * C.www) - 1.0;\n")
   +string("  vec3 h = abs(x) - 0.5;\n")
   +string("  vec3 ox = floor(x + 0.5);\n")
   +string("  vec3 a0 = x - ox;\n")
   +string("  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\n")
   +string("  vec3 g;\n")
   +string("  g.x  = a0.x  * x0.x  + h.x  * x0.y;\n")
   +string("  g.yz = a0.yz * x12.xz + h.yz * x12.yw;\n")
   +string("  return 130.0 * dot(m, g);\n")
   +string("}\n")
   +string("float staticV(vec2 uv) {\n")
   +string("    float staticHeight = snoise(vec2(9.0,time*1.2+3.0))*0.3+5.0;\n")
   +string("    float staticAmount = snoise(vec2(1.0,time*1.2-6.0))*0.1+0.3;\n")
   +string("    float staticStrength = snoise(vec2(-9.75,time*0.6-3.0))*2.0+2.0;\n")
   +string("	return (1.0-step(snoise(vec2(5.0*pow(time,2.0)+pow(uv.x*7.0,1.2),pow((mod(time,100.0)+100.0)*uv.y*0.3+3.0,staticHeight))),staticAmount))*staticStrength;\n")
   +string("}\n")
   +string("void main() {\n")
   +string("	vec2 uv =  v_vTexcoord.xy/resolution.xy;\n")
   +string("	float jerkOffset = (1.0-step(snoise(vec2(time*1.3,5.0)),0.8))*0.05;\n")
   +string("	float fuzzOffset = snoise(vec2(time*15.0,uv.y*80.0))*0.003;\n")
   +string("	float largeFuzzOffset = snoise(vec2(time*1.0,uv.y*25.0))*0.004;\n")
   +string("    float vertMovementOn = (1.0-step(snoise(vec2(time*0.2,8.0)),0.4))*vertMovementOpt;\n")
   +string("    float vertJerk = (1.0-step(snoise(vec2(time*1.5,5.0)),0.6))*vertJerkOpt;\n")
   +string("    float vertJerk2 = (1.0-step(snoise(vec2(time*5.5,5.0)),0.2))*vertJerkOpt;\n")
   +string("    float yOffset = abs(sin(time)*4.0)*vertMovementOn+vertJerk*vertJerk2*0.3;\n")
   +string("    float y = mod(uv.y+yOffset,1.0);\n")
   +string("	float xOffset = (fuzzOffset + largeFuzzOffset) * horzFuzzOpt;\n")
   +string("    float staticVal = 0.0;\n")
   +string("    for (float y = -1.0; y <= 1.0; y += 1.0) {\n")
   +string("        float maxDist = 5.0/200.0;\n")
   +string("        float dist = y/200.0;\n")
   +string("    	staticVal += staticV(vec2(uv.x,uv.y+dist))*(maxDist-abs(dist))*1.5;\n")
   +string("    }\n")
   +string("    staticVal *= bottomStaticOpt;\n")
   +string("	float red 	=   texture2D(	tex, 	vec2(uv.x + xOffset -0.01*rgbOffsetOpt,y)).r+staticVal;\n")
   +string("	float green = 	texture2D(	tex, 	vec2(uv.x + xOffset,	  y)).g+staticVal;\n")
   +string("	float blue 	=	texture2D(	tex, 	vec2(uv.x + xOffset +0.01*rgbOffsetOpt,y)).b+staticVal;\n")
   +string("	vec3 color = vec3(red,green,blue);\n")
   +string("	float scanline = sin(uv.y*800.0)*0.04*scalinesOpt;\n")
   +string("	color -= scanline;\n")
   +string("	gl_FragColor = vec4(color,1.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  UvertJerkOpt=glGetUniformLocation(program, "vertJerkOpt");
  UvertMovementOpt=glGetUniformLocation(program, "vertMovementOpt");
  UbottomStaticOpt=glGetUniformLocation(program, "bottomStaticOpt");
  UscalinesOpt=glGetUniformLocation(program, "scalinesOpt");
  UrgbOffsetOpt=glGetUniformLocation(program, "rgbOffsetOpt");
  UhorzFuzzOpt=glGetUniformLocation(program, "horzFuzzOpt");
  Utex=glGetUniformLocation(program, "tex");
  Uresolution=glGetUniformLocation(program, "resolution");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1f(Utime,(GLfloat)time);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform1f(UvertJerkOpt,(GLfloat)vertJerkOpt);
  glUniform1f(UvertMovementOpt,(GLfloat)vertMovementOpt);
  glUniform1f(UbottomStaticOpt,(GLfloat)bottomStaticOpt);
  glUniform1f(UscalinesOpt,(GLfloat)scalinesOpt);
  glUniform1f(UrgbOffsetOpt,(GLfloat)rgbOffsetOpt);
  glUniform1f(UhorzFuzzOpt,(GLfloat)horzFuzzOpt);
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex.pointer));
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( OldTVProfile *in ) {
  time=in->time;
  vertJerkOpt=in->vertJerkOpt;
  vertMovementOpt=in->vertMovementOpt;
  bottomStaticOpt=in->bottomStaticOpt;
  scalinesOpt=in->scalinesOpt;
  rgbOffsetOpt=in->rgbOffsetOpt;
  horzFuzzOpt=in->horzFuzzOpt;
  tex=in->tex;
  resolution.Set(&in->resolution);
 }
 void Render( FBOColor *out, OldTVProfile *in );
};

extern OldTVShader oldtvShader;

void ShadedOldTV( FBOColor *out, OldTVProfile *in );

