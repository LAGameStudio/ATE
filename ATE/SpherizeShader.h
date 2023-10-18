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

class SpherizeShader : public GLSLShader {
public:
 float resolution[2];
 float time;
 GLuint *pTex0;

  // Uniform locations
 GLint Uresolution, Utime,Utex0;
 
 void Init() {
  resolution[0]=display.wf;
  resolution[1]=display.hf;
  //resolution[0]=5000.0f;
  //resolution[1]=5000.0f;
  time=1.0f;
  pTex0=null;
  OUTPUT("SpherizeShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("/*\n")+
   string("	Cubic Lens Distortion GLSL Shader\n")+
   string("	http://vvvv.org/contribution/ft-cubic-lens-distortion \n")+
   string("	r2 = image_aspect*image_aspect*u*u + v*v\n")+
   string("	f = 1 + r2*(k + kcube*sqrt(r2))\n")+
   string("	u' = f*u\n")+
   string("	v' = f*v\n")+
   string("*/\n")+
   string("uniform sampler2D tex0;\n")+
   string("float k = 0.2, kcube = 0.3, scale = 0.85, dispersion = 0.01, blurAmount=1.0;\n")+
   string("vec2 rand(vec2 co) //needed for fast noise based blurring\n")+
   string("{\n")+
   string("	float noise1 =  (fract(sin(dot(co ,vec2(12.9898,78.233))) * 43758.5453));\n")+
   string("	float noise2 =  (fract(sin(dot(co ,vec2(12.9898,78.233)*2.0)) * 43758.5453));\n")+
   string("	return clamp(vec2(noise1,noise2),0.0,1.0);\n")+
   string("}\n")+
   string("vec3 blur(vec2 coords) {\n")+
   string("	vec2 noise = rand(gl_TexCoord[0].xy);\n")+
   string("	float tolerance = 0.2;\n")+
   string("	float vignette_size = 0.5;\n")+
   string("	vec2 powers = pow(abs(vec2(gl_TexCoord[3].s - 0.5,gl_TexCoord[3].t - 0.5)),vec2(2.0));\n")+
   string("	float radiusSqrd = pow(vignette_size,2.0);\n")+
   string("	float gradient = smoothstep(radiusSqrd-tolerance, radiusSqrd+tolerance, powers.x+powers.y);\n")+
   string("	vec4 col = vec4(0.0);\n")+
   string("	float X1 = coords.x + blurAmount * noise.x*0.004 * gradient;\n")+
   string("	float Y1 = coords.y + blurAmount * noise.y*0.004 * gradient;\n")+
   string("	float X2 = coords.x - blurAmount * noise.x*0.004 * gradient;\n")+
   string("	float Y2 = coords.y - blurAmount * noise.y*0.004 * gradient;\n")+
   string("	float invX1 = coords.x + blurAmount * ((1.0-noise.x)*0.004) * (gradient * 0.5);\n")+
   string("	float invY1 = coords.y + blurAmount * ((1.0-noise.y)*0.004) * (gradient * 0.5);\n")+
   string("	float invX2 = coords.x - blurAmount * ((1.0-noise.x)*0.004) * (gradient * 0.5);\n")+
   string("	float invY2 = coords.y - blurAmount * ((1.0-noise.y)*0.004) * (gradient * 0.5);\n")+
   string("	col += texture2D(tex0, vec2(X1, Y1))*0.1;\n")+
   string("	col += texture2D(tex0, vec2(X2, Y2))*0.1;\n")+
   string("	col += texture2D(tex0, vec2(X1, Y2))*0.1;\n")+
   string("	col += texture2D(tex0, vec2(X2, Y1))*0.1;	\n")+
   string("	col += texture2D(tex0, vec2(invX1, invY1))*0.15;\n")+
   string("	col += texture2D(tex0, vec2(invX2, invY2))*0.15;\n")+
   string("	col += texture2D(tex0, vec2(invX1, invY2))*0.15;\n")+
   string("	col += texture2D(tex0, vec2(invX2, invY1))*0.15;	\n")+
   string("	return col.rgb;\n")+
   string("}\n")+
   string("void main(void) {\n")+
   string("	//index of refraction of each color channel, causing chromatic dispersion\n")+
   string("	vec3 eta = vec3(1.0+dispersion*0.9, 1.0+dispersion*0.6, 1.0+dispersion*0.3);\n")+
   string("	//texture coordinates\n")+
   string("	vec2 texcoord = gl_TexCoord[0].st;\n")+
   string("	//canvas coordinates to get the center of rendered viewport\n")+
   string("	vec2 cancoord = gl_TexCoord[0].st;\n")+
   string("	//vec2 cancoord = gl_TexCoord[3].st;\n")+
   string("	float r2 = (cancoord.x-0.5) * (cancoord.x-0.5) + (cancoord.y-0.5) * (cancoord.y-0.5);\n")+
   string("	float f = 1.0 + r2 * (k + kcube * sqrt(r2));\n")+
   string("	// get the right pixel for the current position	\n")+
   string("	vec2 rCoords = (f*eta.r)*scale*(texcoord.xy-0.5)+0.5;\n")+
   string("	vec2 gCoords = (f*eta.g)*scale*(texcoord.xy-0.5)+0.5;\n")+
   string("	vec2 bCoords = (f*eta.b)*scale*(texcoord.xy-0.5)+0.5;\n")+
   string("	vec2 aCoords = (f*eta.b)*scale*(texcoord.xy-0.5)+0.5;\n")+
   string("	vec4 inputDistort = vec4(\n")+
   string("	 texture2D(tex0,rCoords).r,	 texture2D(tex0,gCoords).g,\n")+
   string("	 texture2D(tex0,bCoords).b,	 texture2D(tex0,bCoords).a\n")+
   string("	);\n")+
   string("	/*\n")+
   string("	{ // blur\n")+
   string("		inputDistort.r = blur(rCoords).r;\n")+
   string("		inputDistort.g = blur(gCoords).g;\n")+
   string("		inputDistort.b = blur(bCoords).b;\n")+
   string("		inputDistort.b = blur(bCoords).a;\n")+
   string("	}\n")+
   string("	*/\n")+
   string("	vec4 Color = inputDistort;\n")+
   string("//    float d=distance(gl_TexCoord[0].xy,vec2(0.5,0.5));\n")+
   string("//    Color.rgba *= smoothstep(0.4,0.3,d);\n")+
   string(" gl_FragColor = Color;\n")+
   string("}\n")+
   string("/*\n")+
   string("#define EPSILON 0.000011\n")+
   string("uniform vec2 resolution; // Screen resolution (target resolution)\n")+
   string("uniform float time; // time in seconds\n")+
   string("uniform sampler2D tex0; // scene buffer (input FBO of screen)\n")+
   string("void main(void) {\n")+
   string(" vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;\n")+
   string(" vec2 uv;\n")+
   string(" float a = atan(p.y,p.x);\n")+
   string(" float r = sqrt(dot(p,p));\n")+
   string(" uv.x = .15*time+.1/r;\n")+
   string(" uv.y = .1*time + a/(3.1416);\n")+
   string(" vec4 Color=texture2D(tex0,uv);\n")+
   string(" float d=distance(gl_TexCoord[0].xy,vec2(0.5,0.5));\n")+
   string(" Color.rgb *= smoothstep(0.4,0.3,d);\n")+
   string(" gl_FragColor = Color;\n")+
   string("}\n")+
   string("*/\n")+
   string("/*\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("  gl_FragColor = texture2D(tex0,uv);\n")+
   string("}\n")+
   string("*/\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uresolution=glGetUniformLocation(program,"resolution");
  Utime=glGetUniformLocation(program,"time");
  Utex0=glGetUniformLocation(program, "tex0");
 }

 void UpdateUniforms() {
  glUseProgram(program);

  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform1f(Utime, time);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern SpherizeShader spherizeShader;


class SpherizeShader2 : public GLSLShader {
public:
// float tolerance;
 float dispersion;
// float blurAmount;
 float k,kcube,/*vignette_size,*/scale;
 float center[2], canvas[2], disperseComponents[3];
 GLuint *pTex0;

  // Uniform locations
 GLint Utolerance,Udispersion,UblurAmount,Uk,Ukcube,Uvignette_size,Uscale,Ucenter,Ucanvas,UdisperseComponents,Utex0;
 
 void Reset() {  
//  tolerance=0.2f;
  dispersion=0.01f;
//  blurAmount=1.0f;
  k=0.2f;
  kcube=0.3f;
//  vignette_size=0.5f;
  scale=0.85f;
  center[0]=0.5f;
  center[1]=0.5f;
  canvas[0]=1.0f;
  canvas[1]=1.0f;
  disperseComponents[0]=0.9f;
  disperseComponents[1]=0.6f;
  disperseComponents[2]=0.3f;
  pTex0=null;
 }

 void Init() {
  Reset();
  OUTPUT("SpherizeShader2::Loading\n");
  string sphere=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("//	Cubic Lens Distortion GLSL Shader\n")+
   string("//	http://vvvv.org/contribution/ft-cubic-lens-distortion\n")+
   string("//	r2 = image_aspect*image_aspect*u*u + v*v\n")+
   string("//	f = 1 + r2*(k + kcube*sqrt(r2))\n")+
   string("//	u' = f*u\n")+
   string("//	v' = f*v\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform float k;\n")+
   string("uniform float kcube;\n")+
   string("uniform float dispersion;\n")+
   string("uniform vec2 center; // default was 0.5,0.5\n")+
   string("uniform vec2 canvas; // default was 0.5,0.5\n")+
   string("uniform vec3 disperseComponents; // default was 0.9, 0.6, 0.3\n")+
   string("uniform float scale;\n")+
   string("//float k = 0.2, kcube = 0.3, scale = 0.85, dispersion = 0.01\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("	//index of refraction of each color channel, causing chromatic dispersion\n")+
   string("	vec3 eta = vec3(1.0+dispersion*disperseComponents.r, 1.0+dispersion*disperseComponents.g, 1.0+dispersion*disperseComponents.b);\n")+
   string("	//texture coordinates\n")+
   string("	vec2 texcoord = gl_TexCoord[0].st;\n")+
   string("	//canvas coordinates to get the center of rendered viewport\n")+
   string("	vec2 cancoord = gl_TexCoord[0].st;\n")+
   string("	//vec2 cancoord = gl_TexCoord[3].st;\n")+
   string("	float cx=center.x+(cancoord.x-canvas.x);\n")+
   string("	float cy=center.y+(cancoord.y-canvas.y);\n")+
   string("	float r2 =  cx*cx+cy*cy;\n")+
   string("	float f = 1.0 + r2 * (k + kcube * sqrt(r2));\n")+
   string("	// get the right pixel for the current position\n")+
   string("	vec2 rCoords = (f*eta.r)*scale*(texcoord.xy-0.5)+0.5;\n")+
   string("	vec2 gCoords = (f*eta.g)*scale*(texcoord.xy-0.5)+0.5;\n")+
   string("	vec2 bCoords = (f*eta.b)*scale*(texcoord.xy-0.5)+0.5;\n")+
   string("	vec2 aCoords = (f*eta.b)*scale*(texcoord.xy-0.5)+0.5;\n")+
   string("	vec4 inputDistort = vec4(\n")+
   string("	 texture2D(tex0,rCoords).r,\n")+
   string("	 texture2D(tex0,gCoords).g,\n")+
   string("	 texture2D(tex0,bCoords).b,\n")+
   string("	 texture2D(tex0,bCoords).a\n")+
   string("	); \n")+
   string("	vec4 Color = inputDistort;\n")+
   string(" gl_FragColor = Color;\n")+
   string("}\n");
  loadShaders(sphere.c_str());
  InitUniforms();
 }

 void InitUniforms() {
//  Utolerance=glGetUniformLocation(program, "tolerance");
  Udispersion=glGetUniformLocation(program, "dispersion");
//  UblurAmount=glGetUniformLocation(program, "blurAmount");
  Uk=glGetUniformLocation(program, "k");
  Ukcube=glGetUniformLocation(program, "kcube");
//  Uvignette_size=glGetUniformLocation(program, "vignette_size");
  Uscale=glGetUniformLocation(program, "scale");
  Ucenter=glGetUniformLocation(program, "center");
  Ucanvas=glGetUniformLocation(program, "canvas");
  UdisperseComponents=glGetUniformLocation(program, "disperseComponents");
  Utex0=glGetUniformLocation(program, "tex0");
 }

 void UpdateUniforms() {
  glUseProgram(program);

//  glUniform1f(Utolerance, tolerance);
  glUniform1f(Udispersion, dispersion);
//  glUniform1f(UblurAmount, blurAmount);
  glUniform1f(Uk, k);
  glUniform1f(Ukcube, kcube);
//  glUniform1f(Uvignette_size, vignette_size);
  glUniform1f(Uscale, scale);
  glUniform2f(Ucenter, center[0], center[1]);
  glUniform2f(Ucanvas, canvas[0], canvas[1]);
  glUniform3f(UdisperseComponents, disperseComponents[0], disperseComponents[1], disperseComponents[2]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern SpherizeShader2 spherizeShader2;

#include "TextureLibrary.h"
#include "UglyFont.h"

class SpherizeShader2Test : public GLWindow {
public:
 GLImage *test;
// float tolerance;
 float dispersion;
// float blurAmount;
 float k,kcube,/*vignette_size,*/scale;
 float center[2], canvas[2], disperseComponents[3];
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(red255);
  Reset();
  test=library.Load("data/images/encyclopedia/NASA_70s_colony_lrg.jpg");
 }
 void Reset() {
//  tolerance=0.2f;
  dispersion=0.01f;
//  blurAmount=1.0f;
  k=0.2f;
  kcube=0.3f;
//  vignette_size=0.5f;
  scale=0.85f;
  center[0]=0.5f;
  center[1]=0.5f;
  canvas[0]=1.0f;
  canvas[1]=1.0f;
  disperseComponents[0]=0.9f;
  disperseComponents[1]=0.6f;
  disperseComponents[2]=0.3f;
 }
 void Render() {
//  spherizeShader2.tolerance=tolerance;
  spherizeShader2.dispersion=dispersion;
//  spherizeShader2.blurAmount=blurAmount;
  spherizeShader2.k=k;
  spherizeShader2.kcube=kcube;
  spherizeShader2.scale=scale;
//  spherizeShader2.vignette_size=vignette_size;
  spherizeShader2.center[0]=center[0];
  spherizeShader2.center[1]=center[1];
  spherizeShader2.canvas[0]=canvas[0];
  spherizeShader2.canvas[1]=canvas[1];
  spherizeShader2.disperseComponents[0]=disperseComponents[0];
  spherizeShader2.disperseComponents[1]=disperseComponents[1];
  spherizeShader2.disperseComponents[2]=disperseComponents[2];
  spherizeShader2.pTex0=&test->texture;
  spherizeShader2.UpdateUniforms();
  FullscreenQuad();
  spherizeShader2.Disable();
  char buf[80];
  FORMAT(buf,80,"center: %f, %f", (float) center[0], (float) center[1]);
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"canvas: %f, %f", (float) canvas[0], (float) canvas[1]);
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+20.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+20.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"dispersion %f", (float) dispersion); //"tolerance= %f, dispersion=%f", tolerance, dispersion );//, blurAmount=%f", tolerance, dispersion, blurAmount );
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+40.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+40.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"k= %f, kcube=%f", (float) k, (float) kcube);
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+60.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+60.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"scale: %f", (float) scale ); // vignette: %f", scale, vignette_size);
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+80.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+80.0-1.0,10.0,8.0,false);
  FORMAT(buf,80,"RGB: %f - %f - %f", (float) disperseComponents[0], (float) disperseComponents[1], (float) disperseComponents[2] );
  glColor3f(1.0,0.0,0.0);
  Text(buf,10,display->hd/2.0+100.0,10.0,8.0,false);
  glColor3f(1.0,1.0,0.0);
  Text(buf,11,display->hd/2.0+100.0-1.0,10.0,8.0,false);
 }
 void Between() {
  int mx,my;
  MousePosition(&mx,&my);
  if ( input->right ) {
   center[0]=iratiof(mx,display->w)*2.0f-1.0f;
   center[1]=iratiof(my,display->h)*2.0f-1.0f;
  }
  if ( input->left ) {
   canvas[0]=iratiof(mx,display->w)*2.0f-1.0f;
   canvas[1]=iratiof(my,display->h)*2.0f-1.0f;
  }
  if ( input->KeyDown(DX_R) ) {
   disperseComponents[0]-=FRAMETIME; if ( disperseComponents[0] < 0.0f ) disperseComponents[0]=5.0f;
  }
  if ( input->KeyDown(DX_G) ) {
   disperseComponents[1]-=FRAMETIME; if ( disperseComponents[1] < 0.0f ) disperseComponents[1]=5.0f;
  }
  if ( input->KeyDown(DX_B) ) {
   disperseComponents[2]-=FRAMETIME; if ( disperseComponents[2] < 0.0f ) disperseComponents[2]=5.0f;
  }
  if ( input->KeyDown(DX_A) ) {
   k=iratiof(mx,display->w);
  }
  /*
  if ( input->KeyDown(DX_Z) ) {
   vignette_size=iratiof(my,display->h)*2.0f-1.0f;
  }
  if ( input->KeyDown(DX_S) ) {
   tolerance=iratiof(my,display->h)*2.0f-1.0f;
  }
  */
  if ( input->KeyDown(DX_X) ) {
   kcube=iratiof(my,display->h)*400.0f-200.0f;
  }
  if ( input->KeyDown(DX_D) ) {
   dispersion=iratiof(my,display->h)*20.0f-10.0f;
  }
  /*
  if ( input->KeyDown(DX_C) ) {
   blurAmount=iratiof(my,display->h)*2.0f-1.0f;
  }
  */
  if ( input->KeyDown(DX_V) ) {
   scale=iratiof(my,display->h)*100.0f;
  }
  if ( input->KeyDown(DX_SPACE) ) {
   Reset();
  }
 }
};