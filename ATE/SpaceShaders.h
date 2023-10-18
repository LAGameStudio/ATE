#pragma once
#include "Crayon.h"
#include "Vertex.h"
#include "GLSetup.h"
#include "GLSL2Shader.h"
#include "FBO.h"
#include "TextureLibrary.h"


// ----------------------------------------------------
ONE(WaterAndGroundProfile,{
 waveParams.Set(0.5,4.0,6.0); waveParams.w=5.0;
 waterLight.Set(15.0,120.0);
 particleAmount=70.0;
 waterColor1.Double(0.43,0.6,0.66,1.0);
 waterColor2.Double(0.06,0.07,0.11,1.0);
 waterSpec.Set(1.3,1.3,0.9);
 light.Set(0.0,20.0,10.0);
 LEVEL.Set(0.525,0.54,0.5); LEVEL.w=0.1;
 noiseTex=library.Load("data/images/textures/noise4.png",tllto_Tiling);
 groundTex=library.Load("data/images/textures/stone.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
 landColor.Double(0.7,0.55,0.4,1.0);
})
 Cartesiand waveParams,waterLight,waterSpec,light,pos2d,LEVEL,resolution;
 Zdouble particleAmount,time;
 Crayon waterColor1,waterColor2,landColor;
  Zp<GLImage> noiseTex,groundTex;
 void Set( WaterAndGroundProfile *in ) {
  waveParams.Set(&in->waveParams);
  waterLight.Set(&in->waterLight);
  particleAmount=in->particleAmount;
  waterColor1.fromCrayon(in->waterColor1);
  waterColor2.fromCrayon(in->waterColor2);
  landColor.fromCrayon(in->landColor);
  waterSpec.Set(&in->waterSpec);
  light.Set(&in->light);
  LEVEL.Set(&in->LEVEL);
  noiseTex=in->noiseTex;
  groundTex=in->groundTex;
  resolution.Set(&in->resolution);
 }
 BINARY_IN({
  B_IN_SSUB(waveParams)  B_IN_SSUB(waterLight)    B_IN_SSUB(waterSpec)
  B_IN_SSUB(light)       B_IN_SSUB(pos2d)         B_IN_SSUB(LEVEL)
  B_IN_SSUB(resolution)  B_IN_NUM(particleAmount) B_IN_NUM(time)
  B_IN_TINT(waterColor1) B_IN_TINT(waterColor2)   B_IN_TINT(landColor)
  B_IN_TILE(noiseTex)    B_IN_TILE(groundTex)
 })
 BINARY_OUT({
  B_OUT_SSUB(waveParams)  B_OUT_SSUB(waterLight)    B_OUT_SSUB(waterSpec)
  B_OUT_SSUB(light)       B_OUT_SSUB(pos2d)         B_OUT_SSUB(LEVEL)
  B_OUT_SSUB(resolution)  B_OUT_NUM(particleAmount) B_OUT_NUM(time)
  B_OUT_TINT(waterColor1) B_OUT_TINT(waterColor2)   B_OUT_TINT(landColor)
  B_OUT_TILE(noiseTex)    B_OUT_TILE(groundTex)
 })
 KEYWORDS({
  SUBWORD("waveParams",waveParams)
  else SUBWORD("waterLight",waterLight)
  else SUBWORD("waterSpec",waterSpec)
  else SUBWORD("light",light)
  else SUBWORD("pos2d",pos2d)
  else SUBWORD("LEVEL",LEVEL)
  else SUBWORD("resolution",resolution)
  else NUMWORD("particleAmount",particleAmount)
  else NUMWORD("time",time)
  else TINTWORD("waterColor1",waterColor1)
  else TINTWORD("waterColor2",waterColor2)
  else TINTWORD("landColor",landColor)
  else TILEWORD("noiseTex",noiseTex)
  else TILEWORD("groundTex",groundTex)
 })
 WORDKEYS({
  SSUBKEY("waveParams",waveParams)
  SSUBKEY("waterLight",waterLight)
  SSUBKEY("waterSpec",waterSpec)
  SSUBKEY("light",light)
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("LEVEL",LEVEL)
  SSUBKEY("resolution",resolution)
  NUMBKEY("particleAmount",particleAmount)
  NUMBKEY("time",time)
  TINTKEY("waterColor1",waterColor1)
  TINTKEY("waterColor2",waterColor2)
  TINTKEY("landColor",landColor)
  IMGKEY("noiseTex",noiseTex)
  IMGKEY("groundTex",groundTex)
 })
MANY(WaterAndGroundProfile,WaterAndGroundProfileHandle,WaterAndGroundProfileHandles,"WaterAndGround",WaterAndGroundProfiles,{})
 KEYWORDSGroup("WaterAndGround",WaterAndGroundProfile);
 WORDKEYSGroup("WaterAndGround",WaterAndGroundProfile);
 BINARY_INOUT(WaterAndGroundProfile);
DONE(WaterAndGroundProfile);

class WaterAndGroundShader : public GLSLShader {
public:
  Cartesiand waveParams,waterLight,waterSpec,light,resolution,pos2d,LEVEL;
  Zdouble particleAmount,time;
  Crayon waterColor1,waterColor2,landColor;
  Zp<GLuint> noiseTex,groundTex;
 GLuint Uresolution,UwaveParams,UwaterLight,UwaterSpec,Ulight,Upos2d,ULEVEL,UparticleAmount,Utime,UwaterColor1,UwaterColor2,UnoiseTex,UgroundTex,UlandColor;
 void Init() {
  OUTPUT("WaterAndGroundShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec4 waveParams;  // = vec4(0.5,4.,6.,.5) largeH,largeSz,smallH,smallSz\n")
   +string("uniform vec2 waterLight;  // = vec2(15.,120.)  range 1-200, x < y, x=soft,y=glossy factors\n")
   +string("uniform float particleAmount; // = 70.\n")
   +string("uniform vec3 waterColor1;  // = vec3(0.43,0.6,0.66)  transparent low-water color RGB1\n")
   +string("uniform vec3 waterColor2;  // = vec3(0.06,0.07,0.11)  RGB1\n")
   +string("uniform vec3 waterSpec;    // = vec3(1.3,1.3,0.9)     Water Specular Factor\n")
   +string("uniform vec3 light;        // = vec3(0,20,10); // light position : vec3(-0., sin(iGlobalTime*0.5)*.5 + .35, 2.8); // position of the sun\n")
   +string("uniform sampler2D noiseTex;\n")
   +string("uniform sampler2D groundTex;\n")
   +string("uniform vec3 landColor;   // vec3(0.7, 0.55, 0.4) \n")
   +string("uniform vec2 pos2d;        // 0-1.0 normalized Position 2d vector\n")
   +string("uniform vec2 resolution;   // 0-1.0 normalized Position 2d vector\n")
   +string("uniform vec4 LEVEL;        // = 0.525,0.54,0.5,0.1 threshold of what is considered flat / coast, water level, deep water level, coastalFade 0.0-2.0\n")
   +string("uniform float time;        // time (global time)\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("float hash( float n )\n")
   +string("{\n")
   +string(" return fract(sin(n)*43758.5453123);\n")
   +string("}\n")
   +string("float noise1( in vec2 x )\n")
   +string("{\n")
   +string(" vec2 p  = floor(x);\n")
   +string(" vec2 f  = smoothstep(0.0, 1.0, fract(x));\n")
   +string(" float n = p.x + p.y*57.0;\n")
   +string(" return mix( mix( hash(n+  0.0), hash(n+  1.0),f.x), mix(hash(n+ 57.0), hash(n+ 58.0), f.x ), f.y);\n")
   +string("}\n")
   +string("float noise(vec2 p)\n")
   +string("{\n")
   +string(" return texture2D(noiseTex,p*vec2(1.0/256.0)).x;\n")
   +string("}\n")
   +string("float height_map( vec2 p )\n")
   +string("{\n")
   +string("  mat2 m = mat2( 0.9563*1.4,  -0.2924*1.4,  0.2924*1.4,  0.9563*1.4 );\n")
   +string("  p = p+ pos2d*10.0;\n")
   +string("  p = p*6.;\n")
   +string("  float f = 0.6000*noise1( p );\n")
   +string("  p = m*p*1.1;\n")
   +string("  f += 0.2500*noise1( p );\n")
   +string("  p = m*p*1.32;\n")
   +string("  f += 0.1666*noise1( p );\n")
   +string("  p = m*p*1.11;\n")
   +string("  f += 0.0834*noise( p );\n")
   +string("  p = m*p*1.12;\n")
   +string("  f += 0.0634*noise( p );\n")
   +string("  p = m*p*1.13;\n")
   +string("  f += 0.0444*noise( p );\n")
   +string("  p = m*p*1.14;\n")
   +string("  f += 0.0274*noise( p );\n")
   +string("  p = m*p*1.15;\n")
   +string("  f += 0.0134*noise( p );\n")
   +string("  p = m*p*1.16;\n")
   +string("  f += 0.0104*noise( p );\n")
   +string("  p = m*p*1.17;\n")
   +string("  f += 0.0084*noise( p );\n")
   +string("  if (f<LEVEL.x)\n")
   +string("      f = f;\n")
   +string("  else\n")
   +string("      f = pow((f-LEVEL.x)/(1.-LEVEL.x), 2.)*(1.-LEVEL.x)*2.0+LEVEL.x; // makes a smooth coast-increase\n")
   +string("  return clamp(f, 0., 10.);\n")
   +string("}\n")
   +string("vec3 terrain_map( vec2 p )\n")
   +string("{\n")
   +string("  p = p+ pos2d*10.0;\n")
   +string("  return landColor+texture2D(groundTex, p*2.0).rgb*0.5; // test-terrain is simply 'sandstone'\n")
   +string("}\n")
   +string("const mat2 m = mat2( 0.72, -1.60,  1.60,  0.72 );\n")
   +string("float water_map( vec2 p, float height )\n")
   +string("{\n")
   +string("  p = p+ pos2d*120.0;\n")
   +string("  vec2 p2 = p*waveParams.y;\n")
   +string("  vec2 shift1 = 0.001*vec2( time*160.0*2.0, time*120.0*2.0 );\n")
   +string("  vec2 shift2 = 0.001*vec2( time*190.0*2.0, -time*130.0*2.0 );\n")
   +string("\n")
   +string("  // coarse crossing 'ocean' waves...\n")
   +string("  float f = 0.6000*noise( p );\n")
   +string("  f += 0.2500*noise( p*m );\n")
   +string("  f += 0.1666*noise( p*m*m );\n")
   +string("  float wave = sin(p2.x*0.622+p2.y*0.622+shift2.x*4.269)*waveParams.x*f*height*height ;\n")
   +string("\n")
   +string("  p *= waveParams.w;\n")
   +string("  f = 0.;\n")
   +string("  float amp = 1.0;\n")
   +string("  float s = 0.5;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string("  p = m * p * 0.947;\n")
   +string("  f -= amp*abs(sin((noise( p+shift1*s )-0.5)*2.0));\n")
   +string("  amp = amp*0.59;\n")
   +string("  s*=-1.329;\n")
   +string(" \n")
   +string("  return wave+f*waveParams.z;\n")
   +string("}\n")
   +string("\n")
   +string("float nautic(vec2 p)\n")
   +string("{\n")
   +string("  p = p+ pos2d*10.0;\n")
   +string("  p *= 18.0;\n")
   +string("  float f = 0.0;\n")
   +string("  float amp = 1.0;\n")
   +string("  float s = 0.5;\n")
   +string("  p = m * p * 1.2;\n")
   +string("  f += amp*abs(smoothstep(0.0, 1.0, noise( p+time*s ))-0.5);\n")
   +string("  amp = amp * 0.5;\n")
   +string("  s*=-1.227;\n")
   +string("  p = m * p * 1.2;\n")
   +string("  f += amp*abs(smoothstep(0.0, 1.0, noise( p+time*s ))-0.5);\n")
   +string("  amp = amp * 0.5;\n")
   +string("  s*=-1.227;\n")
   +string("  p = m * p * 1.2;\n")
   +string("  f += amp*abs(smoothstep(0.0, 1.0, noise( p+time*s ))-0.5);\n")
   +string("  amp = amp * 0.5;\n")
   +string("  s*=-1.227;\n")
   +string("  p = m * p * 1.2;\n")
   +string("  f += amp*abs(smoothstep(0.0, 1.0, noise( p+time*s ))-0.5);\n")
   +string("  amp = amp * 0.5;\n")
   +string("  s*=-1.227;\n")
   +string("  return pow(1.0-f, 5.0);\n")
   +string("}\n")
   +string("\n")
   +string("float particles(vec2 p)\n")
   +string("{\n")
   +string("  p = p+ vec2(-pos2d.x,pos2d.y);\n")
   +string("  p *= 200.;\n")
   +string("  float f = 0.;\n")
   +string("  float amp = 1.0;\n")
   +string("  float s = 1.5;\n")
   +string("  p = m * p * 1.2;\n")
   +string("  f += amp*noise( p+time*s );\n")
   +string("  amp = amp*0.5;\n")
   +string("  s*=-1.227;\n")
   +string("  p = m * p * 1.2;\n")
   +string("  f += amp*noise( p+time*s );\n")
   +string("  amp = amp*0.5;\n")
   +string("  s*=-1.227;\n")
   +string("  p = m * p * 1.2;\n")
   +string("  f += amp*noise( p+time*s );\n")
   +string("  amp = amp*0.5;\n")
   +string("  s*=-1.227;\n")
   +string("  p = m * p * 1.2;\n")
   +string("  f += amp*noise( p+time*s );\n")
   +string("  amp = amp*0.5;\n")
   +string("  s*=-1.227;\n")
   +string("  return pow(f*0.35, 7.0)*particleAmount;\n")
   +string("}\n")
   +string("\n")
   +string("\n")
   +string("float test_shadow( vec2 xy, float height)\n")
   +string("{\n")
   +string("    vec3 r0 = vec3(xy, height);\n")
   +string("    vec3 rd = normalize( light - r0 );\n")
   +string("    \n")
   +string("    float hit = 1.0;\n")
   +string("    float t   = 0.001;\n")
   +string("    for (int j=1; j<25; j++)\n")
   +string("    {\n")
   +string("        vec3 p = r0 + t*rd;\n")
   +string("        float h = height_map( p.xy );\n")
   +string("        float height_diff = p.z - h;\n")
   +string("        if (height_diff<0.0)\n")
   +string("        {\n")
   +string("            return 0.0;\n")
   +string("        }\n")
   +string("        t += 0.01+height_diff*.02;\n")
   +string("        hit = min(hit, 2.0*height_diff/t); // soft shaddow   \n")
   +string("    }\n")
   +string("    return hit;\n")
   +string("}\n")
   +string("\n")
   +string("vec3 CalcTerrain(vec2 uv, float height)\n")
   +string("{\n")
   +string("  vec3 col = terrain_map( uv );\n")
   +string("  float h1 = height_map(uv-vec2(0.0, 0.01));\n")
   +string("  float h2 = height_map(uv+vec2(0.0, 0.01));\n")
   +string("  float h3 = height_map(uv-vec2(0.01, 0.0));\n")
   +string("  float h4 = height_map(uv+vec2(0.01, 0.0));\n")
   +string("  vec3 norm = normalize(vec3(h3-h4, h1-h2, 1.0));\n")
   +string("  vec3 r0 = vec3(uv, height);\n")
   +string("  vec3 rd = normalize( light - r0 );\n")
   +string("  float grad = dot(norm, rd);\n")
   +string("  col *= grad+pow(grad, 8.0);\n")
   +string("  float terrainshade = test_shadow( uv, height );\n")
   +string("  col = mix(col*0.25, col, terrainshade);\n")
   +string("  return col;\n")
   +string("}\n")
   +string("\n")
   +string("\n")
   +string("void main() {\n")
   +string("	vec2 uv =(v_vTexcoord.xy / resolution.xy - vec2(-0.12, +0.25));\n")
   +string("    float deepwater_fadedepth   = LEVEL.z + LEVEL.w;\n")
   +string("    float height = height_map( uv );\n")
   +string("    vec3 col = vec3(0.0,0.0,0.0);\n")
   +string("    float waveheight = clamp(LEVEL.y*3.0-1.5, 0.0, 1.0);\n")
   +string("    float level = LEVEL.y + 0.2*water_map(uv*15.0 + vec2(time*0.1), waveheight);\n")
   +string("    if (height > level)\n")
   +string("    {\n")
   +string("        col = CalcTerrain(uv, height);\n")
   +string("    }\n")
   +string("    if (height <= level)\n")
   +string("    {\n")
   +string("        vec2 dif = vec2(0.0, 0.01);\n")
   +string("        vec2 pos = uv*15. + vec2(time*0.01);\n")
   +string("        float h1 = water_map(pos-dif,waveheight);\n")
   +string("        float h2 = water_map(pos+dif,waveheight);\n")
   +string("        float h3 = water_map(pos-dif.yx,waveheight);\n")
   +string("        float h4 = water_map(pos+dif.yx,waveheight);\n")
   +string("        vec3 normwater = normalize(vec3(h3-h4, h1-h2, 0.125)); // norm-vector of the 'bumpy' water-plane\n")
   +string("        uv += normwater.xy*.002*(level-height);\n")
   +string("        col = CalcTerrain(uv, height);\n")
   +string("        float coastfade = clamp((level-height)/LEVEL.w, 0.0, 1.0);\n")
   +string("        float coastfade2= clamp((level-height)/LEVEL.z, 0.0, 1.0);\n")
   +string("        float intensity = col.r*0.2126+col.g*0.7152+col.b*0.0722;\n")
   +string("		vec3 watercolor = waterColor1;\n")
   +string("        watercolor = mix(watercolor*intensity, waterColor2, smoothstep(0.0, 1.0, coastfade2));\n")
   +string("        vec3 r0 = vec3(uv, LEVEL.y);\n")
   +string("        vec3 rd = normalize( light - r0 ); // ray-direction to the light from water-position\n")
   +string("        float grad     = dot(normwater, rd); // dot-product of norm-vector and light-direction\n")
   +string("        float specular = pow(grad, waterLight.x);  // used for soft highlights                          \n")
   +string("        float specular2= pow(grad, waterLight.y); // used for glossy highlights\n")
   +string("        float gradpos  = dot(vec3(0.0, 0.0, 1.0), rd);\n")
   +string("        float specular1= smoothstep(0.0, 1.0, pow(gradpos, 5.0));  // used for diffusity (some darker corona around light's specular reflections...)                          \n")
   +string("        float wshade  = test_shadow( uv, level );\n")
   +string("        watercolor *= 2.2+wshade;\n")
   +string("   		watercolor += (0.2+0.8*wshade) * ((grad-1.0)*0.5+specular) * 0.25;\n")
   +string("   		watercolor /= (1.0+specular1*1.25);\n")
   +string("   		watercolor += wshade*specular2*waterSpec;\n")
   +string("        watercolor += wshade*coastfade*(1.0-coastfade2)*(vec3(0.5, 0.6, 0.7)*nautic(uv)+vec3(1.0, 1.0, 1.0)*particles(uv));\n")
   +string("        col = mix(col, watercolor, coastfade);\n")
   +string("    }\n")
   +string("	gl_FragColor = vec4(col, 1.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  UwaveParams=glGetUniformLocation(program, "waveParams");
  UwaterLight=glGetUniformLocation(program, "waterLight");
  UwaterSpec=glGetUniformLocation(program, "waterSpec");
  Ulight=glGetUniformLocation(program, "light");
  Upos2d=glGetUniformLocation(program, "pos2d");
  ULEVEL=glGetUniformLocation(program, "LEVEL");
  Uresolution=glGetUniformLocation(program, "resolution");
  UparticleAmount=glGetUniformLocation(program, "particleAmount");
  Utime=glGetUniformLocation(program, "time");
  UwaterColor1=glGetUniformLocation(program, "waterColor1");
  UwaterColor2=glGetUniformLocation(program, "waterColor2");
  UlandColor=glGetUniformLocation(program, "landColor");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  UgroundTex=glGetUniformLocation(program, "groundTex");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform4f(UwaveParams,(GLfloat)waveParams.x,(GLfloat)waveParams.y,(GLfloat)waveParams.z,(GLfloat)waveParams.w);
  glUniform2f(UwaterLight,(GLfloat)waterLight.x,(GLfloat)waterLight.y);
  glUniform3f(UwaterSpec,(GLfloat)waterSpec.x,(GLfloat)waterSpec.y,(GLfloat)waterSpec.z);
  glUniform3f(Ulight,(GLfloat)light.x,(GLfloat)light.y,(GLfloat)light.z);
  glUniform2f(Upos2d,
   (GLfloat)(pos2d.x),
   (GLfloat)(pos2d.y)
  );
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform4f(ULEVEL,(GLfloat)LEVEL.x,(GLfloat)LEVEL.y,(GLfloat)LEVEL.z,(GLfloat)LEVEL.w);
  glUniform3f(UwaterColor1,(GLfloat)waterColor1.rf,(GLfloat)waterColor1.gf,(GLfloat)waterColor1.bf);
  glUniform3f(UwaterColor2,(GLfloat)waterColor2.rf,(GLfloat)waterColor2.gf,(GLfloat)waterColor2.bf);
  glUniform3f(UlandColor,(GLfloat)landColor.rf,(GLfloat)landColor.gf,(GLfloat)landColor.bf);
  glUniform1f(UparticleAmount,(GLfloat)particleAmount);
  glUniform1f(Utime,(GLfloat)time);
  glEnable(GL_TEXTURE_2D);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
  glUniform1i(UgroundTex, 1);  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *groundTex);
 }
 void Disable() {
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);
 }
 void Set( WaterAndGroundProfile *in ) {
  waveParams.Set(&in->waveParams);
  waterLight.Set(&in->waterLight);
  waterSpec.Set(&in->waterSpec);
  light.Set(&in->light);
  pos2d.Set(&in->pos2d);
  resolution.Set(&in->resolution);
  LEVEL.Set(&in->LEVEL);
  particleAmount=in->particleAmount;
  time=in->time;
  waterColor1.fromCrayon(in->waterColor1);
  waterColor2.fromCrayon(in->waterColor2);
  landColor.fromCrayon(in->landColor);
  noiseTex=&in->noiseTex->texture;
  groundTex=&in->groundTex->texture;
 }
};


extern WaterAndGroundShader waterandgroundShader;

void ShadedWaterAndGround( FBOColor *out, WaterAndGroundProfile *in );

// ----------------------------------------------------

ONE(EmissionCloudsProfile,{
 noiseTex=library.Load("data/images/noise3.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
})
  Zp<GLImage> noiseTex;
  Cartesiand pos2d,resolution;
  Zdouble time;
  void Set( EmissionCloudsProfile *in ) {
   noiseTex=in->noiseTex;
   pos2d.Set(&in->pos2d);
   resolution.Set(&in->resolution);
   time=in->time;
  }
 BINARY_IN({
  B_IN_TILE(noiseTex)
  B_IN_SSUB(pos2d)
  B_IN_SSUB(resolution)
  B_IN_NUM(time)
 })
 BINARY_OUT({
  B_OUT_TILE(noiseTex)
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(resolution)
  B_OUT_NUM(time)
 })
 KEYWORDS({
  SUBWORD("pos2d",pos2d)
  else TILEWORD("noise",noiseTex)
  else SUBWORD("resolution",resolution)
  else KEYWORD("time",{time=w;})
 })
 WORDKEYS({
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("resolution",resolution)
  NUMBKEY("time",time)
  IMGKEY("noise",noiseTex)
 })
MANY(EmissionCloudsProfile,EmissionCloudsProfileHandle,EmissionCloudsProfileHandles,"EmissionClouds",EmissionCloudsProfiles,{})
 KEYWORDSGroup("EmissionClouds",EmissionCloudsProfile);
 WORDKEYSGroup("EmissionClouds",EmissionCloudsProfile);
 BINARY_INOUT(EmissionCloudsProfile)
DONE(EmissionCloudsProfile);

class EmissionCloudsShader : public GLSLShader {
public:
 Cartesiand pos2d,resolution;
 Zdouble time;
 Zp<GLuint> noiseTex;
 GLuint Upos2d,Uresolution,Utime,UnoiseTex;
 void Init() {
  OUTPUT("EmissionClouds::Loading\n");
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
   +string("precision mediump float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec2 pos2d;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform float time;\n")
   +string("uniform sampler2D noiseTex;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("const int MAX_RAY_STEPS = 64;\n")
   +string("float noise( vec3 x )\n")
   +string("{\n")
   +string(" vec3 p = floor(x);\n")
   +string(" vec3 f = fract(x);\n")
   +string(" f = f*f*(3.0-2.0*f);\n")
   +string(" vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;\n")
   +string(" vec2 rg = texture2D( noiseTex, (uv+ 0.5)/512.0, -100.0 ).yx;\n")
   +string(" return -1.0+2.0*mix( rg.x, rg.y, f.z );\n")
   +string("}\n")
   +string("void main() {\n")
   +string(" vec2 screenPos = ((v_vTexcoord.xy) / resolution.xy) * 2.0 - 1.0;\n")
   +string(" vec3 cameraDir = vec3(0.0, 0.0, 1.4);\n")
   +string(" vec3 cameraPlaneU = vec3(1.0, 0.0, 0.0);\n")
   +string(" vec3 cameraPlaneV = vec3(0.0, 1.0, 0.0) * resolution.y / resolution.x;\n")
   +string(" vec3 rayDir = cameraDir + screenPos.x * cameraPlaneU + screenPos.y * cameraPlaneV;\n")
   +string(" vec3 rayPos = vec3(pos2d, 0.0);\n")
   +string(" rayDir = normalize(rayDir);\n")
   +string(" float dis = 0.0;\n")
   +string(" float t1 = 2.0;\n")
   +string(" vec3 dir = vec3(0.0,1.0,0.0);\n")
   +string(" float val;\n")
   +string(" vec3 col = vec3(0);\n")
   +string(" for(int i=0;i<MAX_RAY_STEPS;i++){\n")
   +string("  vec3 q = rayPos - dir * t1;\n")
   +string("  val  = 0.50000 * noise( q * 0.05 );\n")
   +string("  q = q*2.0 - dir * t1;\n")
   +string("  val += 0.25000 * noise( q * 0.05  );\n")
   +string("  q = q*2.0 - dir * t1;\n")
   +string("  val += 0.12500 * noise( q * 0.05  );\n")
   +string("  q = q*2.0 - dir * t1;\n")
   +string("  val += 0.06250 * noise( q * 0.05  );\n")
   +string("  q = q*2.5 - dir * t1;\n")
   +string("  val += 0.03125 * noise( q * 0.8  );\n")
   +string("  float t = max(5.0 * val - 0.9, 0.0);\n")
   +string("  col += sqrt(dis) * 0.1 * vec3(0.5 * t * t * t, 0.6 * t * t, 0.7 * t);\n")
   +string("  dis += 1.0 / float(MAX_RAY_STEPS);\n")
   +string("  rayPos += rayDir * 1.0/ (0.4);\n")
   +string(" }\n")
   +string(" col = min(col, 1.0) - 0.34 * (log(col + 1.0));\n")
   +string(" gl_FragColor = vec4(sqrt(col.rgb), 1.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Upos2d=glGetUniformLocation(program, "pos2d");
  Uresolution=glGetUniformLocation(program, "resolution");
  Utime=glGetUniformLocation(program, "time");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( EmissionCloudsProfile *in ) {
  pos2d.Set(&in->pos2d);
  resolution.Set(&in->resolution);
  time=in->time;
  noiseTex=&in->noiseTex->texture;
 }
};

extern EmissionCloudsShader emissioncloudsShader;

void ShadedEmissionClouds( FBOColor *out, EmissionCloudsProfile *in );


// ----------------------------------------------------

ONE(ElectroCloudsProfile,{
 toffs = -154.0;
 complexity = 10;
 noiseTex=library.Load("data/images/textures/noise2.jpg",tllto_Tiling);
 resolution.Set(1.0,1.0);
 tint.Pick(alabaster);
})
 Zp<GLImage> noiseTex;
 Zdouble time,toffs,complexity;
 Cartesiand position,resolution;
 Crayon tint;
 void Set( ElectroCloudsProfile *in ) {
  noiseTex=in->noiseTex;
  time=in->time;
  toffs=in->toffs;
  complexity=in->complexity;
  position.Set(&in->position);
  resolution.Set(&in->resolution);
  tint.fromCrayon(in->tint);
 }
 BINARY_IN({
  B_IN_TILE(noiseTex)
  B_IN_NUM(time)
  B_IN_NUM(toffs)
  B_IN_NUM(complexity)
  B_IN_SSUB(position)
  B_IN_SSUB(resolution)
  B_IN_TINT(tint)
 })
 BINARY_OUT({
  B_OUT_TILE(noiseTex)
  B_OUT_NUM(time)
  B_OUT_NUM(toffs)
  B_OUT_NUM(complexity)
  B_OUT_SSUB(position)
  B_OUT_SSUB(resolution)
  B_OUT_TINT(tint)
 })
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("toffs",{toffs=w;})
  else KEYWORD("complexity",{complexity=w;})
  else SUBWORD("position",position)
  else SUBWORD("resolution",resolution)
  else TINTWORD("tint",tint)
 })
 WORDKEYS({
  NUMBKEY("time",time)
  NUMBKEY("toffs",toffs)
  NUMBKEY("complexity",complexity)
  SSUBKEY("position",position)
  SSUBKEY("resolution",resolution)
  TINTKEY("tint",tint)
 })
MANY(ElectroCloudsProfile,ElectroCloudsProfileHandle,ElectroCloudsProfileHandles,"ElectroClouds",ElectroCloudsProfiles,{})
 KEYWORDSGroup("ElectroClouds",ElectroCloudsProfile);
 WORDKEYSGroup("ElectroClouds",ElectroCloudsProfile);
 BINARY_INOUT(ElectroCloudsProfile)
DONE(ElectroCloudsProfile);

class ElectroCloudsShader : public GLSLShader {
public:
 Zp<GLuint> noiseTex;
 Zdouble time,toffs,complexity;
 Cartesiand position,resolution;
 Crayon tint;
 GLuint Utime,Utoffs,Ucomplexity,Uposition,Uresolution,Utint,UnoiseTex;
 void Init() {
  OUTPUT("ElectroCloudsShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform float time;\n")
   +string("uniform sampler2D noiseTex;\n")
   +string("uniform vec2 position;\n")
   +string("uniform vec2 resolution;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("uniform float toffs;        // = -154.0;\n")
   +string("uniform float complexity;   // 10 - 100\n")
   +string("uniform vec3 tint;\n")
   +string("// Nebula Clouds\n")
   +string("vec4 noise(float p){return texture2D(noiseTex,vec2(p*float(1.0/256.0),.0));}\n")
   +string("vec4 noise(vec2 p){return texture2D(noiseTex,p*vec2(1.0/256.0));}\n")
   +string("vec4 noise(vec3 p){float m = mod(p.z,1.0);float s = p.z-m; float sprev = s-1.0;if (mod(s,2.0)==1.0) { s--; sprev++; m = 1.0-m; };return mix(texture2D(noiseTex,p.xy*vec2(1.0/256.0) + noise(sprev).yz*21.421),texture2D(noiseTex,p.xy*vec2(1.0/256.0) + noise(s).yz*14.751),m);}\n")
   +string("vec4 noise(vec4 p){float m = mod(p.w,1.0);float s = p.w-m; float sprev = s-1.0;if (mod(s,2.0)==1.0) { s--; sprev++; m = 1.0-m; };return mix(noise(p.xyz+noise(sprev).wyx*3531.123420),	noise(p.xyz+noise(s).wyx*4521.5314),	m);}\n")
   +string("mat2 rotate_2D(float a){float sa = sin(a); float ca = cos(a); return mat2(ca,sa,-sa,ca);}\n")
   +string("mat3 rotate_x(float a){float sa = sin(a); float ca = cos(a); return mat3(1.0,0.0,0.0,    0.0,ca,sa,   0.0,-sa,ca);}\n")
   +string("mat3 rotate_y(float a){float sa = sin(a); float ca = cos(a); return mat3(ca,0.0,sa,    0.0,1.0,0.0,   -sa,0.0,ca);}\n")
   +string("mat3 rotate_z(float a){float sa = sin(a); float ca = cos(a); return mat3(ca,sa,0.0,    -sa,ca,0.0,  0.0,0.0,1.0);}\n")
   +string("float density(vec3 p) {	vec4 d = noise(p*0.5)*noise(p.xz*0.044)*noise(p.xy*0.26)*noise(p.yz*0.21);float fd = dot(d,vec4(1.4));fd = fd*fd*fd*fd*fd;return max(0.0,fd);}\n")
   +string("void main() {\n")
   +string("    float t = time + noise(v_vTexcoord.xy).y/(24.0-24.0/(time+1.0)) + toffs;\n")
   +string("    vec2 uv = -3.0+position.xy/resolution.xy*6.0 + v_vTexcoord.xy / resolution.yy/2.0 -vec2(0.9,0.5);\n")
   +string("	vec2 m = position.xy/ resolution.xy;\n")
   +string("	//rotation matrix for the camera\n")
   +string("	mat3 rotmat = rotate_y((t-toffs)*.07)*rotate_x((t-toffs)*.031);\n")
   +string("	//p is ray position\n")
   +string("	vec3 p = vec3(0.0,0.0,-30.0); p*=rotmat;\n")
   +string("	p += vec3(sin(t),cos(t),sin(t*0.25)*29.0+t*7.0-22.0-4.0/((t-toffs)*0.01+0.01));\n")
   +string("	//d is ray direction\n")
   +string("	vec3 d = normalize(vec3(uv*(sin(t*0.17)*02+0.8),1.0-length(uv)*0.2));\n")
   +string("	d*=rotmat;\n")
   +string("	p+=d*noise(v_vTexcoord.xy).x*0.9;\n")
   +string("	//some accumulators\n")
   +string("	float a = 0.0;\n")
   +string("	float ai = 0.0;\n")
   +string("	vec3 color = vec3(0.0);\n")
   +string("	for (float i=0.0; i<complexity; i += 1.0)\n")
   +string("	{\n")
   +string("		p+=d*0.9;\n")
   +string("		vec3 n = noise(p.xz*0.25+vec2(t*0.1)).xyz*12.0*noise(p.zy*0.1+vec2(t*0.1)).xyz;\n")
   +string("		float de = density(p+n);\n")
   +string("		a += de; // a is alpha, as the ray traverses the density function the\n")
   +string("		a = min(1.0,a); //a > 1.0 makes no sence and produces bugs\n")
   +string("		vec4 c2 = noise(p.yz*0.03).xyzw;\n")
   +string("		vec3 c = c2.xyz*1.7;\n")
   +string("		//lame illumiation\n")
   +string("		float occ = min((de-density(p+vec3(0.7+n))),1.0);\n")
   +string("		//occ = min(occ,(de-density(p+vec3(3.7)+n)));\n")
   +string("		//occ = min(occ,(de-density(p+vec3(5.7)+n)));\n")
   +string("		color += max(.0,occ)*(1.0-a)*c;\n")
   +string("		if (a>1.0) break;\n")
   +string("	}\n")
   +string("	color +=noise(uv).xyz*0.08;\n")
   +string("	color -= length(uv)*0.12;\n")
   +string("	color = max(vec3(0.0),color);\n")
   +string("	color  = mix(color,vec3(length(color)),length(color)*1.7-0.4);\n")
   +string("	color  = pow(color,vec3(0.6));\n")
   +string("	color *= 1.0+1.0/(t-toffs+0.01);\n")
   +string("	color *= tint;\n")
   +string("	gl_FragColor = vec4(color,(color.r,color.g,color.b)/3.0);\n")
   +string("}\n")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Utoffs=glGetUniformLocation(program, "toffs");
  Ucomplexity=glGetUniformLocation(program, "complexity");
  Uposition=glGetUniformLocation(program, "position");
  Uresolution=glGetUniformLocation(program, "resolution");
  Utint=glGetUniformLocation(program, "tint");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uposition,(GLfloat)position.x,(GLfloat)position.y);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform3f(Utint,(GLfloat)tint.r,(GLfloat)tint.g,(GLfloat)tint.b);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Utoffs,(GLfloat)toffs);
  glUniform1f(Ucomplexity,(GLfloat)complexity);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( ElectroCloudsProfile *in ) {
  time=in->time;
  toffs=in->toffs;
  complexity=in->complexity;
  position.Set(&in->position);
  resolution.Set(&in->resolution);
  tint.fromCrayon(in->tint);
  noiseTex=&in->noiseTex->texture;
 }
};

extern ElectroCloudsShader electrocloudsShader;

void ShadedElectroClouds( FBOColor *out, ElectroCloudsProfile *in );

// ----------------------------------------------------

ONE(NebulazProfile,{
 structure_size=1.9;
 background_radiation1=0.2;
 noiseTex=library.Load("data/images/noise4.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
})
 Zdouble time;
 Zdouble structure_size,background_radiation1;
 Cartesiand position,resolution;
 Zp<GLImage> noiseTex;
 Crayon blueColor,yellowColor,redColor,orangeColor,brownColor,blackColor,coolestColor,middleColor,hottestColor;
 void Set( NebulazProfile *in ) {
  time=in->time;
  structure_size=in->structure_size;
  background_radiation1=in->background_radiation1;
  position.Set(&in->position);
  noiseTex=in->noiseTex;
 }
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_NUM(structure_size)
  B_IN_NUM(background_radiation1)
  B_IN_SSUB(position)
  B_IN_SSUB(resolution)
  B_IN_TILE(noiseTex)
  B_IN_TINT(blueColor)
  B_IN_TINT(yellowColor)
  B_IN_TINT(redColor)
  B_IN_TINT(orangeColor)
  B_IN_TINT(brownColor)
  B_IN_TINT(blackColor)
  B_IN_TINT(coolestColor)
  B_IN_TINT(middleColor)
  B_IN_TINT(hottestColor)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_NUM(structure_size)
  B_OUT_NUM(background_radiation1)
  B_OUT_SSUB(position)
  B_OUT_SSUB(resolution)
  B_OUT_TILE(noiseTex)
  B_OUT_TINT(blueColor)
  B_OUT_TINT(yellowColor)
  B_OUT_TINT(redColor)
  B_OUT_TINT(orangeColor)
  B_OUT_TINT(brownColor)
  B_OUT_TINT(blackColor)
  B_OUT_TINT(coolestColor)
  B_OUT_TINT(middleColor)
  B_OUT_TINT(hottestColor)
 })
 KEYWORDS({
  SUBWORD("position",position)
  else SUBWORD("resolution",resolution)
  else TILEWORD("noiseTex",noiseTex)
  else TINTWORD("blueColor",blueColor)
  else TINTWORD("yellowColor",yellowColor)
  else TINTWORD("redColor",redColor)
  else TINTWORD("orangeColor",orangeColor)
  else TINTWORD("brownColor",brownColor)
  else TINTWORD("blackColor",blackColor)
  else TINTWORD("coolestColor",coolestColor)
  else TINTWORD("middleColor",middleColor)
  else TINTWORD("hottestColor",hottestColor)
 })
 WORDKEYS({
  SSUBKEY("position",position)
  SSUBKEY("resolution",resolution)
  IMGKEY("noiseTex",noiseTex)
  TINTKEY("blueColor",blueColor)
  TINTKEY("yellowColor",yellowColor)
  TINTKEY("redColor",redColor)
  TINTKEY("orangeColor",orangeColor)
  TINTKEY("brownColor",brownColor)
  TINTKEY("blackColor",blackColor)
  TINTKEY("coolestColor",coolestColor)
  TINTKEY("middleColor",middleColor)
  TINTKEY("hottestColor",hottestColor)
 })
MANY(NebulazProfile,NebulazProfileHandle,NebulazProfileHandles,"Nebulaz",NebulazProfiles,{})
 KEYWORDSGroup("Nebulaz",NebulazProfile);
 WORDKEYSGroup("Nebulaz",NebulazProfile);
 BINARY_INOUT(NebulazProfile)
DONE(NebulazProfile);

class NebulazShader : public GLSLShader {
public:
 Zdouble time;
 Cartesiand position,resolution;
 Zp<GLuint> noiseTex;
 Zdouble structure_size,background_radiation1;
 Crayon blueColor,yellowColor,redColor,orangeColor,brownColor,blackColor,coolestColor,middleColor,hottestColor;
 GLuint Utime,Uposition,Uresolution,UnoiseTex,UblueColor,UyellowColor,UredColor,UorangeColor,UbrownColor,UblackColor,UcoolestColor,UmiddleColor,UhottestColor,Ustructure_size,Ubackground_radiation1;
 void Init() {
  OUTPUT("NebulazShader::Loading\n");
  string vert=
    string("#version 120\n")
   +string("#ifdef GL_ES\n")
   +string("precision highp float;  //precision mediump float;\n")
   +string("#endif\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("varying vec4 v_vColour;\n")
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec2 position;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform sampler2D noiseTex;\n")
   +string("uniform vec3 blueColor;   //= vec3(0.635, 0.827, 0.996);\n")
   +string("uniform vec3 yellowColor; //= vec3(1.000, 0.823, 0.459);\n")
   +string("uniform vec3 redColor;    //= vec3(0.945, 0.584, 0.502);\n")
   +string("uniform vec3 orangeColor; //= vec3(0.553, 0.231, 0.086);\n")
   +string("uniform vec3 brownColor;  //= vec3(0.360, 0.137, 0.062);\n")
   +string("uniform vec3 blackColor;  //= vec3(0.095, 0.023, 0.043);\n")
   +string("uniform vec3 coolestColor; // = vec3(0.615, 0.706, 1.000);\n")
   +string("uniform vec3 middleColor;  // = vec3(0.984, 0.972, 1.000);\n")
   +string("uniform vec3 hottestColor; // = vec3(1.000, 0.733, 0.482);\n")
   +string("uniform float structure_size; //= 1.9;\n")
   +string("uniform float background_radiation1; // = 0.2;\n")
   +string("uniform float time; // = 0.2;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("const int drift_count = 5; // decrease for more fps\n")
   +string("const float step_size = 0.23; // increase for more fps\n")
   +string("const float pi = 3.1415926;\n")
   +string("const float field_of_view = 1.4;\n")
   +string("const float camera_radius = 7.0;\n")
   +string("const float nebula_radius = 6.0;\n")
   +string("float noise3d(vec3 x) {\n")
   +string("    vec3 p = floor(x);\n")
   +string("    vec3 f = fract(x);\n")
   +string("    f = f*f*(3.0-2.0*f);\n")
   +string("    vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;\n")
   +string("    vec2 rg = texture2D(noiseTex, (uv + 0.5)/256.0, -100.0).xy;\n")
   +string("    return mix(rg.y, rg.x, f.z);\n")
   +string("}\n")
   +string("vec2 noise3d2(vec3 x) {\n")
   +string("    vec3 p = floor(x);\n")
   +string("    vec3 f = fract(x);\n")
   +string("    f = f*f*(3.0-2.0*f);\n")
   +string("    vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;\n")
   +string("    vec4 rg = texture2D(noiseTex, (uv + 0.5)/256.0, -100.0);\n")
   +string("    return vec2(mix(rg.y, rg.x, f.z), mix(rg.w, rg.z, f.z));\n")
   +string("}\n")
   +string("\n")
   +string("float fbm3d_low(vec3 x) {\n")
   +string("    float f = 0.0;\n")
   +string("    f += 0.50000*noise3d(x); x = x*2.01;\n")
   +string("    f += 0.25000*noise3d(x); x = x*2.02;\n")
   +string("    f += 0.12500*noise3d(x); x = x*2.03;\n")
   +string("    f += 0.06250*noise3d(x);\n")
   +string("    return f;\n")
   +string("}\n")
   +string("float fbm3d(vec3 x) {\n")
   +string("    float f = 0.0;\n")
   +string("    f += 0.50000*noise3d(x); x = x*2.01;\n")
   +string("    f += 0.25000*noise3d(x); x = x*2.02;\n")
   +string("    f += 0.12500*noise3d(x); x = x*2.03;\n")
   +string("    f += 0.06250*noise3d(x); x = x*2.04;\n")
   +string("    f += 0.03125*noise3d(x); x = x*2.01;\n")
   +string("    f += 0.01562*noise3d(x);\n")
   +string("    return f;\n")
   +string("}\n")
   +string("vec4 noise3d4_discontinuous(vec3 x) {\n")
   +string("    vec3 p = floor(x);\n")
   +string("    vec3 f = fract(x);\n")
   +string("    f = f*f*(3.0-2.0*f);\n")
   +string("    vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;\n")
   +string("    return texture2D(noiseTex, (uv + 0.5)/256.0, -100.0);\n")
   +string("}\n")
   +string("vec4 noise2d4(vec2 x) {\n")
   +string("    vec2 p = floor(x);\n")
   +string("    vec2 f = fract(x);\n")
   +string("    f = f*f*(3.0-2.0*f);\n")
   +string("    return texture2D(noiseTex, (p + f + 0.5)/256.0, -100.0);\n")
   +string("}\n")
   +string("vec3 star_color(float n) {\n")
   +string("    // based on: http://www.vendian.org/mncharity/dir3/starcolor/ \n")
   +string("    // constant distribution and linear interpolation\n")
   +string("    return mix(mix(coolestColor, middleColor, smoothstep(0.0, 0.5, n)),\n")
   +string("               mix(middleColor, hottestColor, smoothstep(0.5, 1.0, n)), step(0.5, n));\n")
   +string("}\n")
   +string("vec3 only_mix(vec3 src, vec3 dst, float start, float end, float amount) {\n")
   +string("    return mix(src, dst, smoothstep(start, end, amount)) * step(start, amount) * step(-end, -amount);\n")
   +string("}\n")
   +string("vec3 nebula_color(float noise) {\n")
   +string("    // colors sampled from the Keyhole Nebula\n")
   +string("    // https://commons.wikimedia.org/wiki/File:Keyhole_Nebula_-_Hubble_1999.jpg#/media/File:Keyhole_Nebula_-_Hubble_1999.jpg \n")
   +string("    noise = clamp(noise, 0.0, 1.0);\n")
   +string("    return max(max(max(max(\n")
   +string("           only_mix(blueColor, yellowColor, 0.0, 0.4, noise),\n")
   +string("           only_mix(yellowColor,  redColor, 0.4, 0.5, noise)),\n")
   +string("           only_mix(redColor,  orangeColor, 0.5, 0.7, noise)),\n")
   +string("           only_mix(orangeColor,brownColor, 0.7, 0.8, noise)),\n")
   +string("           only_mix(brownColor, blackColor, 0.8, 1.0, noise));\n")
   +string("}\n")
   +string("float star_brightness = 0.0;\n")
   +string("vec3 star_pos = vec3(0.0,0.0,0.0);\n")
   +string("vec3 star_field( vec3 x, float grid_size ) {\n")
   +string("    // a tiled randomly positioned dot, looks like stars.\n")
   +string("    vec3 grid = floor(x * grid_size);\n")
   +string("    vec3 pos = fract(x * grid_size);\n")
   +string("    vec4 noise = noise3d4_discontinuous(grid);\n")
   +string("    vec3 center = noise.xxy * 0.5 + 0.25;\n")
   +string("    vec3 to_center = center - pos;\n")
   +string("    vec3 out_of_plane = x * dot(to_center, x);\n")
   +string("    float len = length(to_center - out_of_plane);\n")
   +string("    float brightness = noise.w;\n")
   +string("    float radius = mix(0.003, 0.009, pow(brightness, 9.0)) * grid_size;\n")
   +string("    float show = step(0.8, noise.y);\n")
   +string("    \n")
   +string("    star_pos = (grid + center) / grid_size;\n")
   +string("    star_brightness = show * brightness;\n")
   +string("    return 2.0 * star_color(noise.z) * show * smoothstep(radius, 0.0, len);\n")
   +string("}\n")
   +string("vec2 screen_space(vec3 x, vec3 vx, vec3 vy, vec3 vz) {\n")
   +string("    vec3 rescaled = field_of_view / dot(vz, x) * x;\n")
   +string("    return vec2(dot(vx, rescaled), dot(vy, rescaled));\n")
   +string("}\n")
   +string("vec3 lens_flare(vec2 x, vec2 center, float brightness) {\n")
   +string("    // renders a lens flare at center\n")
   +string("    // quantization might be unnecessary, it prevents flickering\n")
   +string("    const float quantization = 500.0;\n")
   +string("    const float flare_size = 0.5;\n")
   +string("    vec2 to_x = (floor((x - center) * quantization) + 0.5) / quantization;\n")
   +string("    float shape = max(0.0, 0.005 / pow(abs(to_x.x * to_x.y), flare_size) - 0.3);\n")
   +string("    float radial_fade = smoothstep(0.04, 0.0, length(to_x));\n")
   +string("    float brightness_fade = smoothstep(0.75, 1.0, brightness);\n")
   +string("    return vec3(1.0) * shape * radial_fade * brightness_fade;\n")
   +string("}\n")
   +string("vec3 haze(vec3 x, vec3 background) {\n")
   +string("    // add grainy star background (illusion of infinite stars)\n")
   +string("    float base_structure = fbm3d_low(x * structure_size);\n")
   +string("    float star_structure = mix(smoothstep(0.35, 0.8, base_structure), 1.0, background_radiation1);\n")
   +string("    vec3 haze_color = 3.0 * vec3(0.058, 0.047, 0.096);\n")
   +string("    float grain = mix(2.0, 2.0*noise3d(x * 800.0), 0.5);\n")
   +string("    vec3 haze = haze_color * grain * smoothstep(0.1, 0.9, base_structure);\n")
   +string("    return star_structure * background + haze;\n")
   +string("}\n")
   +string("vec3 drift_field(vec3 x) {\n")
   +string("    // provide a velocity field to drift the nebula (makes it streaky)\n")
   +string("    // generate a divergence free field to make it look like fluid flow\n")
   +string("    x = x * pi / nebula_radius;\n")
   +string("    vec3 outwards = normalize(x) * 0.2;\n")
   +string("    vec3 div_free = vec3(0.0);\n")
   +string("    div_free += 0.50*sin(1.00*x+7.85).yzx;\n")
   +string("    div_free += 0.25*cos(2.48*x+6.13).zxy;\n")
   +string("    div_free += 0.12*cos(4.12*x+11.49).yzx;\n")
   +string("    div_free += 0.06*sin(7.83*x+11.82).zxy;\n")
   +string("    return outwards + div_free;\n")
   +string("}\n")
   +string("vec3 direction = vec3(0.0,0.0,0.0);\n")
   +string("vec3 drift(vec3 x, float speed ) {\n")
   +string("    // drift backwards in time to sample the original nebula\n")
   +string("    // keep the last velocity to help sample velocity aligned noise\n")
   +string("    direction = drift_field(x);\n")
   +string("    x -= direction * speed;\n")
   +string("    for(int i=0; i<drift_count-1; i++) {\n")
   +string("    	x -= drift_field(x) * speed;\n")
   +string("    }\n")
   +string("    return x;\n")
   +string("}\n")
   +string("vec4 nebula(vec3 x) {\n")
   +string("    // opacity via fbm\n")
   +string("    float drift_speed = 0.2 * noise3d(x * 0.5 + 1.24);\n")
   +string("    vec3 x_drifted = drift(x, drift_speed) * 0.7;\n")
   +string("    float density = 0.01 + 0.2 * smoothstep(0.50, 0.90, fbm3d(x_drifted + 23.6));\n")
   +string("    float radial_fade = smoothstep(nebula_radius, nebula_radius * 0.7, length(x));\n")
   +string("    // color via mix of global noise and drift aligned noise\n")
   +string("    float color_noise = noise3d(x_drifted);\n")
   +string("    float aligned_noise = noise3d(10.0 * (x - dot(x, normalize(direction))));\n")
   +string("    float noise = mix(color_noise, aligned_noise, 0.1);\n")
   +string("    float brightness = 0.1 * 0.9 + smoothstep(0.0, 1.0, noise);\n")
   +string("    vec3 color = mix(1.0, brightness, 0.7) * nebula_color(1.0 - noise);\n")
   +string("    \n")
   +string("    return vec4(color, radial_fade * density);\n")
   +string("}\n")
   +string("vec4 ray_trace(vec3 origin, vec3 ray) {\n")
   +string("    const float loop_max = 1.5 * nebula_radius + camera_radius;\n")
   +string("    const float fudge_factor = 2.1;\n")
   +string("    vec4 acc = vec4(0.0);\n")
   +string("    for(float i=0.0; i<loop_max; i+=step_size) {\n")
   +string("        vec3 pos = origin + i * ray;\n")
   +string("        vec4 sample = nebula(pos);\n")
   +string("        // TODO: accumulator is not step_size independent... why?\n")
   +string("        // TODO: remove the fudge factor exp(w * step_size)\n")
   +string("        acc = acc + (1.0 - acc.w) * vec4(sample.xyz * sample.w, sample.w);\n")
   +string("    }\n")
   +string("    acc.xyz *= fudge_factor;\n")
   +string("    return acc;\n")
   +string("}\n")
   +string("void main() {\n")
   +string("    vec2 pixel = -1.0 + 2.0 * v_vTexcoord.xy / resolution.xy;\n")
   +string("    pixel.x *= resolution.x / resolution.y;\n")
   +string("    vec2 mouse = pi * ((position.xy + 0.001) / resolution.xy * vec2(-2.0, 1.0));\n")
   +string("    // camera\n")
   +string("    const float motion_speed = 1.0;\n")
   +string("    float motion = (0.3 * cos(motion_speed * time * 0.2) + 0.7);\n")
   +string("    mouse += mod(motion_speed * time * 0.1, 2.0 * pi);\n")
   +string("    vec3 origin = camera_radius * motion * normalize(vec3(cos(mouse.x) * sin(mouse.y), cos(mouse.y), sin(mouse.x) * sin(mouse.y)));\n")
   +string("    vec3 target = vec3(0.0);\n")
   +string("    // orthonormal basis\n")
   +string("    vec3 vz = normalize(target - origin);\n")
   +string("    vec3 vx = normalize(cross(vec3(0.0, 1.0, 0.0), vz));\n")
   +string("    vec3 vy = normalize(cross(vz, vx));\n")
   +string("    // ray \n")
   +string("    vec3 ray = normalize(vx*pixel.x + vy*pixel.y + vz*field_of_view);\n")
   +string("    vec4 trace = ray_trace(origin, ray);\n")
   +string("    // stars\n")
   +string("    gl_FragColor = vec4(haze(ray, star_field(ray, 18.0)), 1.0);\n")
   +string("    gl_FragColor += vec4(star_field(ray, 4.0), 1.0);\n")
   +string("    star_brightness = star_brightness*1.6; // lens flares\n")
   +string("    gl_FragColor += vec4( lens_flare(pixel, screen_space(ray, vx, vy, vz), star_brightness), 1.0);\n")
   +string("    // nebula\n")
   +string("    gl_FragColor = vec4(mix(gl_FragColor.xyz, trace.xyz, trace.w), 1.0);\n")
   +string("}\n")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uposition=glGetUniformLocation(program, "position");
  Uresolution=glGetUniformLocation(program, "resolution");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  UblueColor=glGetUniformLocation(program, "blueColor");
  UyellowColor=glGetUniformLocation(program, "yellowColor");
  UredColor=glGetUniformLocation(program, "redColor");
  UorangeColor=glGetUniformLocation(program, "orangeColor");
  UbrownColor=glGetUniformLocation(program, "brownColor");
  UblackColor=glGetUniformLocation(program, "blackColor");
  UcoolestColor=glGetUniformLocation(program, "coolestColor");
  UmiddleColor=glGetUniformLocation(program, "middleColor");
  UhottestColor=glGetUniformLocation(program, "hottestColor");
  Ustructure_size=glGetUniformLocation(program, "structure_size");
  Ubackground_radiation1=glGetUniformLocation(program, "background_radiation1");
  Utime=glGetUniformLocation(program, "time");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uposition,(GLfloat)position.x,(GLfloat)position.y);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform3f(UblueColor,(GLfloat)blueColor.rf,(GLfloat)blueColor.gf,(GLfloat)blueColor.bf);
  glUniform3f(UyellowColor,(GLfloat)yellowColor.rf,(GLfloat)yellowColor.gf,(GLfloat)yellowColor.bf);
  glUniform3f(UredColor,(GLfloat)redColor.rf,(GLfloat)redColor.gf,(GLfloat)redColor.bf);
  glUniform3f(UorangeColor,(GLfloat)orangeColor.rf,(GLfloat)orangeColor.gf,(GLfloat)orangeColor.bf);
  glUniform3f(UbrownColor,(GLfloat)brownColor.rf,(GLfloat)brownColor.gf,(GLfloat)brownColor.bf);
  glUniform3f(UblackColor,(GLfloat)blackColor.rf,(GLfloat)blackColor.gf,(GLfloat)blackColor.bf);
  glUniform3f(UcoolestColor,(GLfloat)coolestColor.rf,(GLfloat)coolestColor.gf,(GLfloat)coolestColor.bf);
  glUniform3f(UmiddleColor,(GLfloat)middleColor.rf,(GLfloat)middleColor.gf,(GLfloat)middleColor.bf);
  glUniform3f(UhottestColor,(GLfloat)hottestColor.rf,(GLfloat)hottestColor.gf,(GLfloat)hottestColor.bf);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
  glUniform1f(Ustructure_size,(GLfloat)structure_size);
  glUniform1f(Ubackground_radiation1,(GLfloat)background_radiation1);
  glUniform1f(Utime,(GLfloat)time);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( NebulazProfile *in ) {
  position.Set(&in->position);
  resolution.Set(&in->resolution);
  noiseTex=&in->noiseTex->texture;
  blueColor.fromCrayon(in->blueColor);
  yellowColor.fromCrayon(in->yellowColor);
  redColor.fromCrayon(in->redColor);
  orangeColor.fromCrayon(in->orangeColor);
  brownColor.fromCrayon(in->brownColor);
  blackColor.fromCrayon(in->blackColor);
  coolestColor.fromCrayon(in->coolestColor);
  middleColor.fromCrayon(in->middleColor);
  hottestColor.fromCrayon(in->hottestColor);
  structure_size=in->structure_size;
  background_radiation1=in->background_radiation1;
  time=in->time;
 }
};

extern NebulazShader nebulazShader;

void ShadedNebulaz( FBOColor *out, NebulazProfile *in );



// ----------------------------------------------------
ONE(ToonTroidProfile,{
 noiseTex=library.Load("data/images/textures/noise4.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
 sway=0.0;      // = 2.12
 L.Set(10.25,0.33,-0.7);
 backlight=0.15;
 diffuseColor.Double( 1.0, 0.9, 0.7, 1.0 );
 ambientColor.Double(0.1, 0.11, 0.13, 1.0 );
 lightComponents.Set( 0.1, 0.2, 0.5 ); lightComponents.w=0.2;
 emmissive=0.1;
 roughness = 0.001;
 dimensions.Set( 0.54030230586, 0.8414709848, -0.8414709848 ); dimensions.w= 0.54030230586;
 craterDims.Set( 0.25, 0.05, 0.5 ); craterDims.w=0.15;
 craterParams.Set( 0.15, 0.6, 5.0 ); craterParams.w= 0.0;
 surfaceParams.Set( 0.005, 0.1, 2.0 ); surfaceParams.w= 20.0;
 kaleidoscope.Set( 0.9, 0.6, 0.2 ); kaleidoscope.w=0.15; //   ... make z a huge number for odd effects
 scale.Set(1.0,1.0,1.0);
 cam.Set(-2.0,0.0,0.0);
 tintColor.Double(1.0,1.0,1.0,1.0);
})
 Zp<GLImage> noiseTex;
 Cartesiand resolution,scale,L,lightComponents,rotation,dimensions,craterDims,craterParams,surfaceParams,kaleidoscope,cam;
 Zdouble time,sway,backlight,emmissive,roughness;
 Crayon diffuseColor,ambientColor,tintColor;
 BINARY_IN({
  B_IN_TILE(noiseTex)
  B_IN_SSUB(resolution)
  B_IN_SSUB(scale)
  B_IN_SSUB(L)
  B_IN_SSUB(lightComponents)
  B_IN_SSUB(rotation)
  B_IN_SSUB(dimensions)
  B_IN_SSUB(craterDims)
  B_IN_SSUB(craterParams)
  B_IN_SSUB(surfaceParams)
  B_IN_SSUB(kaleidoscope)
  B_IN_SSUB(cam)
  B_IN_NUM(time)
  B_IN_NUM(sway)
  B_IN_NUM(backlight)
  B_IN_NUM(emmissive)
  B_IN_NUM(roughness)
  B_IN_TINT(diffuseColor)
  B_IN_TINT(ambientColor)
  B_IN_TINT(tintColor)
 })
 BINARY_OUT({
  B_OUT_TILE(noiseTex)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(scale)
  B_OUT_SSUB(L)
  B_OUT_SSUB(lightComponents)
  B_OUT_SSUB(rotation)
  B_OUT_SSUB(dimensions)
  B_OUT_SSUB(craterDims)
  B_OUT_SSUB(craterParams)
  B_OUT_SSUB(surfaceParams)
  B_OUT_SSUB(kaleidoscope)
  B_OUT_SSUB(cam)
  B_OUT_NUM(time)
  B_OUT_NUM(sway)
  B_OUT_NUM(backlight)
  B_OUT_NUM(emmissive)
  B_OUT_NUM(roughness)
  B_OUT_TINT(diffuseColor)
  B_OUT_TINT(ambientColor)
  B_OUT_TINT(tintColor)
 })
 KEYWORDS({
  SUBWORD("resolution",resolution)
  else SUBWORD("scale",scale)
  else SUBWORD("L",L)
  else SUBWORD("lightComponents",lightComponents)
  else SUBWORD("rotation",rotation)
  else SUBWORD("dimensions",dimensions)
  else SUBWORD("cam",cam)
  else SUBWORD("craterDims",craterDims)
  else SUBWORD("craterParams",craterParams)
  else SUBWORD("surfaceParams",surfaceParams)
  else SUBWORD("kaleidoscope",kaleidoscope)
  else KEYWORD("time",{time=w;})
  else KEYWORD("sway",{sway=w;})
  else KEYWORD("backlight",{backlight=w;})
  else KEYWORD("emmissive",{emmissive=w;})
  else KEYWORD("roughness",{roughness=w;})
  else TINTWORD("diffuseColor",diffuseColor)
  else TINTWORD("ambientColor",ambientColor)
  else TINTWORD("tintColor",tintColor)
  else TILEWORD("noiseTex",noiseTex)
 })
 WORDKEYS({
  SSUBKEY("resolution",resolution)
  SSUBKEY("scale",scale)
  SSUBKEY("L",L)
  SSUBKEY("lightComponents",lightComponents)
  SSUBKEY("rotation",rotation)
  SSUBKEY("dimensions",dimensions)
  SSUBKEY("cam",cam)
  SSUBKEY("craterDims",craterDims)
  SSUBKEY("craterParams",craterParams)
  SSUBKEY("surfaceParams",surfaceParams)
  SSUBKEY("kaleidoscope",kaleidoscope)
  NUMBKEY("time",time)
  NUMBKEY("sway",sway)
  NUMBKEY("backlight",backlight)
  NUMBKEY("emmissive",emmissive)
  NUMBKEY("roughness",roughness)
  TINTKEY("diffuseColor",diffuseColor)
  TINTKEY("ambientColor",ambientColor)
  TINTKEY("tintColor",tintColor)
  IMGKEY("noisetex",noiseTex)
 })
MANY(ToonTroidProfile,ToonTroidProfileHandle,ToonTroidProfileHandles,"ToonTroid",ToonTroidProfiles,{})
 KEYWORDSGroup("ToonTroid",ToonTroidProfile);
 WORDKEYSGroup("ToonTroid",ToonTroidProfile);
 BINARY_INOUT(ToonTroidProfile);
DONE(ToonTroidProfile);

class ToonTroidShader : public GLSLShader {
public:
  Zp<GLuint> noiseTex;
  Cartesiand resolution,scale,L,lightComponents,rotation,dimensions,craterDims,craterParams,surfaceParams,kaleidoscope,cam;
  Zdouble time,sway,backlight,emmissive,roughness;
  Crayon diffuseColor,ambientColor,tintColor;
 GLuint Ucam,Uresolution,Uscale,UL,UlightComponents,Urotation,Udimensions,UcraterDims,UcraterParams,UsurfaceParams,Ukaleidoscope,Utime,Usway,Ubacklight,Uemmissive,Uroughness,UdiffuseColor,UambientColor,UnoiseTex,UtintColor;
 void Init() {
  OUTPUT("ToonTroidShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec2 scale;\n")
   +string("uniform float time;\n")
   +string("uniform float sway;         // = 2.12\n")
   +string("uniform vec3 L;             // = normalize(vec3(10.25,.33,-.7));\n")
   +string("uniform float backlight;    // .15\n")
   +string("uniform vec3 cam;           // = vec3(-2.0,0,0) \n")
   +string("uniform vec3 diffuseColor;  // 1.0, 0.9, 0.7\n")
   +string("uniform vec3 ambientColor;  // 0.1, 0.11, 0.13\n")
   +string("uniform vec3 tintColor;  // 1.0 1.0 1.0 \n")
   +string("uniform vec4 lightComponents; // 0.1, 0.2, 0.5, 0.2\n")
   +string("uniform float emmissive;    // 0.1\n")
   +string("uniform vec2 rotation;      // radians\n")
   +string("uniform float roughness;    // = 0.001\n")
   +string("uniform vec2 dimensions;    // = 0.54030230586, 0.8414709848, -0.8414709848, 0.54030230586\n")
   +string("uniform vec4 craterDims;    // .25, .05, 0.5, .15\n")
   +string("uniform vec4 craterParams;  // .15, .6, 5.0, 0.0*f\n")
   +string("uniform vec4 surfaceParams; // 0.005, 0.1, 2.0, 20.0\n")
   +string("uniform vec4 kaleidoscope;  // .9, .6, .2, .15   ... make z a huge number for odd effects\n")
   +string("uniform sampler2D noiseTex;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("const int NB_ITER=128;\n")
   +string("const float PI=3.1415926535897932384626433832795;\n")
   +string("const float TAO=6.28318531;\n")
   +string("const float MAX_DIST=4000.0;\n")
   +string("const float PRECISION=-0.0001;\n")
   +string("const float PLANET=200.0;\n")
   +string("const vec2 V01 = vec2(0,1);\n")
   +string("vec2 Ve = V01.yx*roughness;\n")
   +string("mat2 Rot1 = mat2(dimensions.x,dimensions.y,-dimensions.x,dimensions.y);\n")
   +string("float C1,S1, C2, S2, t; \n")
   +string("bool withPlanet = true;\n")
   +string("float Noise(in vec3 x) {\n")
   +string("    vec3 p = floor(x);\n")
   +string("    vec3 f = fract(x);\n")
   +string("	f = f*f*(3.0-2.0*f);\n")
   +string("	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;\n")
   +string("	vec2 rg = texture2D( noiseTex, (uv+ 0.5)/256.0, -100.0 ).yx;\n")
   +string("	return mix( rg.x, rg.y, f.z );\n")
   +string("}\n")
   +string("float Noise( in vec2 x ) {\n")
   +string("    vec2 p = floor(x);\n")
   +string("    vec2 f = fract(x);\n")
   +string("	vec2 uv = p.xy + f.xy*f.xy*(3.0-2.0*f.xy);\n")
   +string("	return texture2D( noiseTex, (uv+118.4)/256.0, -100.0 ).x;\n")
   +string("}\n")
   +string("float smin( float a, float b, float k ) {\n")
   +string("    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );\n")
   +string("    return mix(b, a, h ) - k*h*(1.0-h);\n")
   +string("}\n")
   +string("float Kaleido(inout vec2 v, in float nb){\n")
   +string("	float id=floor(.5+atan(v.x,-v.y)*nb/TAO);\n")
   +string("	float a = id*TAO/nb;\n")
   +string("	float ca = cos(a), sa = sin(a);\n")
   +string("	v*=mat2(ca,sa,-sa,ca);\n")
   +string("	return id;\n")
   +string("}\n")
   +string("vec2 Kaleido2(inout vec3 p, float nb1, float nb2, float d) {\n")
   +string("	float id1 = Kaleido(p.yx, nb1);\n")
   +string("	float id2 = Kaleido(p.xz, nb2*2.);\n")
   +string("	p.z+=d;	\n")
   +string("	return vec2(id1,id2);\n")
   +string("}\n")
   +string("vec2 minObj(vec2 o1, vec2 o2) {\n")
   +string("	return o1.x<o2.x?o1:o2;\n")
   +string("}\n")
   +string("vec2 sminObj(vec2 o1, vec2 o2, float k) {\n")
   +string("	float d = smin(o1.x, o2.x, k);\n")
   +string("	return vec2(d, o1.x<o2.x?o1.y:o2.y);\n")
   +string("}\n")
   +string("vec2 maxObj(vec2 o1, vec2 o2) {\n")
   +string("	return o1.x>o2.x?o1:o2;\n")
   +string("}\n")
   +string("float sdCapsule( vec3 p, vec3 a, vec3 b, float r1, float r2) {\n")
   +string("    vec3 pa = p - a, ba = b - a;\n")
   +string("    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );\n")
   +string("    return length( pa - ba*h ) - mix(r1,r2,h);\n")
   +string("}\n")
   +string("float DECrater(vec3 p) {\n")
   +string("	float d = MAX_DIST;\n")
   +string("	vec2 id = Kaleido2(p, kaleidoscope.x, kaleidoscope.y, kaleidoscope.z);\n")
   +string("	float noise = Noise(id*10.);\n")
   +string("	if (noise<craterParams.y && abs(id.y)>craterParams.w && abs(id.y)<craterParams.z) {  \n")
   +string("		d = sdCapsule(p, vec3(0,0,-craterParams.x), vec3(0,0,.1),.1+noise*.2,.1+noise*.5);\n")
   +string("		d = max(-(length(p-vec3(0,0,-craterDims.x))-(.1+noise*craterDims.z)),d);\n")
   +string("		d = max(-(length(p-vec3(0,0,-craterDims.y))-(.1+noise*craterDims.w)),d);\n")
   +string("		d*=.8;\n")
   +string("	}\n")
   +string("	return d;\n")
   +string("}\n")
   +string("bool intersectSphere(vec3 ro, vec3 rd, vec3 c, float r) {\n")
   +string("    ro -= c;\n")
   +string("	float b = dot(rd,ro), d = b*b - dot(ro,ro) + r*r;\n")
   +string("	return (d>0. && -sqrt(d)-b > 0.);\n")
   +string("}\n")
   +string("// vec4 : distance / id (object) / uv (texture) \n")
   +string("vec4 DE(vec3 p0) {\n")
   +string("	float scalePlanet = 10.0;\n")
   +string("	vec4 res = vec4(1000);	\n")
   +string("	vec3 p = p0;\n")
   +string("    float d,d1,dx;\n")
   +string("	p = p0;\n")
   +string("	p.x+=2.;\n")
   +string("	p*=scalePlanet;\n")
   +string("	p.yz *= Rot1;\n")
   +string("	p.xz *= mat2(C2,S2,-S2,C2);\n")
   +string("	d1 = DECrater(p);\n")
   +string("	d = smin(length(p)-2.,d1,kaleidoscope.w); //smin(d2, d1,.2),.15);\n")
   +string("\n")
   +string("	d += surfaceParams.y*Noise((p)*surfaceParams.z);\n")
   +string("	d += surfaceParams.x*Noise((p)*surfaceParams.w);\n")
   +string("	res = vec4(d/=scalePlanet,PLANET, length(p), p.z);\n")
   +string("	return res;\n")
   +string("}\n")
   +string("vec3 N(vec3 p) {\n")
   +string("    vec2 e = vec2(Ve.x, -Ve.x); \n")
   +string("    return normalize(e.xyy * DE(p + e.xyy).x + e.yyx * DE(p + e.yyx).x + e.yxy * DE(p + e.yxy).x + e.xxx * DE(p + e.xxx).x);;\n")
   +string("}\n")
   +string("float softshadow(in vec3 ro, in vec3 rd, in float mint, in float maxt, in float k) {\n")
   +string("	float res = 1.0, h, t = mint;\n")
   +string("    for( int i=0; i<20; i++ ) {\n")
   +string("		h = DE( ro + rd*t ).x;\n")
   +string("		res = min( res, k*h/t );\n")
   +string("                if( res<0.0001 ) break;\n")
   +string("		t += 0.02;\n")
   +string("    }\n")
   +string("    return clamp(res, 0., 1.);\n")
   +string("}\n")
   +string("float one_pi = 0.31830988618;\n")
   +string("float lightIntensity = 1.0;\n")
   +string("vec4 findColor(float obj, vec2 uv, vec3 n) {\n")
   +string("    if (obj == PLANET) {\n")
   +string("		return vec4(mix(vec3(.7,.7,.7),vec3(1,1,1), clamp(1.1-5.*(uv.x-1.8),0.1,.9)),1.0);\n")
   +string("	} else {\n")
   +string("		return vec4(0.0,0.0,0.0,0.0);\n")
   +string("	}\n")
   +string("}\n")
   +string("vec4 Render(vec3 p, vec3 rd, float t, float obj, vec2 uv) {\n")
   +string("	//return V01.xxy*(dot(N(p),L));	\n")
   +string("\n")
   +string("	vec3 nor = N(p);\n")
   +string("	vec4 foundColor = findColor(obj, uv, reflect(rd,nor));	\n")
   +string("    vec3 col = foundColor.xyz;\n")
   +string("	vec3 sunLight = normalize(L);\n")
   +string("	float	amb = clamp(.5+.5*nor.y, .0, 1.),\n")
   +string("            dif = clamp(dot( nor, sunLight ), 0., 1.),\n")
   +string("            bac = clamp(dot( nor, normalize(vec3(-sunLight.x,0.,-sunLight.z))), 0., 1.)*clamp( 1.0-p.y,0.0,1.0);\n")
   +string("	float sh = softshadow( p, sunLight, .02, 100., 7.); \n")
   +string("	if (obj != PLANET ) {\n")
   +string("        return vec4(0.0,0.0,0.0,0.0);\n")
   +string("	} else {\n")
   +string("	    dif *= sh; \n")
   +string("		vec3 brdf = lightComponents.w*(amb*ambientColor + bac*backlight) +	1.2*dif*diffuseColor;\n")
   +string("		float fre = pow( clamp(1.+dot(nor,rd),0.,1.), 2.);\n")
   +string("		col = col*(lightComponents.x+brdf + emmissive) + lightComponents.y*fre*(lightComponents.z+.5*col)*exp(-.01*t*t);\n")
   +string("		return vec4(sqrt(clamp(col,0.,1.)),foundColor.w);\n")
   +string("	}\n")
   +string("}\n")
   +string("mat3 lookat(vec3 ro, vec3 up){\n")
   +string("	vec3 fw=normalize(ro);\n")
   +string("	vec3 rt=normalize(cross(fw,up));\n")
   +string("	return mat3(rt, cross(rt,fw),fw);\n")
   +string("}\n")
   +string("vec3 RD(vec3 ro, vec3 cp, vec2 fCoord) {\n")
   +string("	return lookat(cp-ro, V01.yxx)*normalize(vec3((2.*fCoord-resolution.xy)/resolution.y, 12.0));\n")
   +string("}\n")
   +string("void main() {\n")
   +string("	t = 3.0*TAO+time*0.75;	\n")
   +string("	C1 = cos(rotation.y);\n")
   +string("	S1 = sin(rotation.y);\n")
   +string("	S2 = 2.0*S1*C1;\n")
   +string("	C2 = 1.0-2.0*S1*S1;	\n")
   +string("	t /= TAO;\n")
   +string("	vec3 cp = vec3(-2.0,0,0);\n")
   +string("	float rCam = 5.0;\n")
   +string("	vec3 rd, ro = rCam*vec3(-0.5+4.0*scale.y,\n")
   +string("						-abs(sin(t*sway+scale.x)),\n")
   +string("						-abs(cos(t*sway))\n")
   +string("					    );	\n")
   +string("	vec4 ctot = vec4(0);\n")
   +string("		vec2 fCoord = v_vTexcoord.xy;\n")
   +string("		rd = RD(ro, cp, fCoord);	\n")
   +string("        withPlanet = intersectSphere(ro, rd, cam,0.21);        \n")
   +string("		// Ray marching\n")
   +string("		float t=0.0,d=1.0,od=1.0;\n")
   +string("		vec4 res=vec4(0.0);\n")
   +string("		for(int i=0;i<NB_ITER;i++){\n")
   +string("			if(d<PRECISION|| t>MAX_DIST)break;\n")
   +string("			t += res.x;\n")
   +string("			res=DE(ro+rd*t); // *0.95;\n")
   +string("		}	\n")
   +string("		// Render colors\n")
   +string("		if(t<MAX_DIST){// if we hit a surface color it\n")
   +string("			ctot += Render(ro + rd*t, rd,t, res.y, res.zw);\n")
   +string("		}	\n")
   +string("	gl_FragColor = ctot*vec4(tintColor,1.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uresolution=glGetUniformLocation(program, "resolution");
  Uscale=glGetUniformLocation(program, "scale");
  UL=glGetUniformLocation(program, "L");
  UlightComponents=glGetUniformLocation(program, "lightComponents");
  Urotation=glGetUniformLocation(program, "rotation");
  Udimensions=glGetUniformLocation(program, "dimensions");
  UcraterDims=glGetUniformLocation(program, "craterDims");
  UcraterParams=glGetUniformLocation(program, "craterParams");
  UsurfaceParams=glGetUniformLocation(program, "surfaceParams");
  Ukaleidoscope=glGetUniformLocation(program, "kaleidoscope");
  Utime=glGetUniformLocation(program, "time");
  Usway=glGetUniformLocation(program, "sway");
  Ubacklight=glGetUniformLocation(program, "backlight");
  Uemmissive=glGetUniformLocation(program, "emmissive");
  Uroughness=glGetUniformLocation(program, "roughness");
  UdiffuseColor=glGetUniformLocation(program, "diffuseColor");
  UambientColor=glGetUniformLocation(program, "ambientColor");
  Ucam=glGetUniformLocation(program,"cam");
  UtintColor=glGetUniformLocation(program,"tintColor");
  UnoiseTex=glGetUniformLocation(program,"noiseTex");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Uscale,(GLfloat)scale.x,(GLfloat)scale.y);
  glUniform3f(UL,(GLfloat)L.x,(GLfloat)L.y,(GLfloat)L.z);
  glUniform4f(UlightComponents,(GLfloat)lightComponents.x,(GLfloat)lightComponents.y,(GLfloat)lightComponents.z,(GLfloat)lightComponents.w);
  glUniform2f(Urotation,(GLfloat)rotation.x,(GLfloat)rotation.y);
  glUniform2f(Udimensions,(GLfloat)dimensions.x,(GLfloat)dimensions.y);
  glUniform4f(UcraterDims,(GLfloat)craterDims.x,(GLfloat)craterDims.y,(GLfloat)craterDims.z,(GLfloat)craterDims.w);
  glUniform4f(UcraterParams,(GLfloat)craterParams.x,(GLfloat)craterParams.y,(GLfloat)craterParams.z,(GLfloat)craterParams.w);
  glUniform4f(UsurfaceParams,(GLfloat)surfaceParams.x,(GLfloat)surfaceParams.y,(GLfloat)surfaceParams.z,(GLfloat)surfaceParams.w);
  glUniform4f(Ukaleidoscope,(GLfloat)kaleidoscope.x,(GLfloat)kaleidoscope.y,(GLfloat)kaleidoscope.z,(GLfloat)kaleidoscope.w);
  glUniform3f(UdiffuseColor,(GLfloat)diffuseColor.rf,(GLfloat)diffuseColor.gf,(GLfloat)diffuseColor.bf);
  glUniform3f(UambientColor,(GLfloat)ambientColor.rf,(GLfloat)ambientColor.gf,(GLfloat)ambientColor.bf);
  glUniform3f(UtintColor,(GLfloat)tintColor.rf,(GLfloat)tintColor.gf,(GLfloat)tintColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Usway,(GLfloat)sway);
  glUniform1f(Ubacklight,(GLfloat)backlight);
  glUniform1f(Uemmissive,(GLfloat)emmissive);
  glUniform1f(Uroughness,(GLfloat)roughness);
  glUniform3f(Ucam,(GLfloat) cam.x, (GLfloat) cam.y, (GLfloat) cam.z );
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( ToonTroidProfile *in ) {
  resolution.Set(&in->resolution);
  scale.Set(&in->scale);
  L.Set(&in->L);
  lightComponents.Set(&in->lightComponents);
  rotation.Set(&in->rotation);
  dimensions.Set(&in->dimensions);
  craterDims.Set(&in->craterDims);
  craterParams.Set(&in->craterParams);
  surfaceParams.Set(&in->surfaceParams);
  kaleidoscope.Set(&in->kaleidoscope);
  time=in->time;
  sway=in->sway;
  backlight=in->backlight;
  emmissive=in->emmissive;
  roughness=in->roughness;
  diffuseColor.fromCrayon(in->diffuseColor);
  ambientColor.fromCrayon(in->ambientColor);
  tintColor.fromCrayon(in->tintColor);
  noiseTex=&in->noiseTex->texture;
  cam.Set(&cam);
 }
};

extern ToonTroidShader toontroidShader;

void ShadedToonTroid( FBOColor *out, ToonTroidProfile *in );


// ----------------------------------------------------
// Planetary Shaders

// ----------------------------------------------------

ONE(TexturedPlanetProfile,{
 LIGHT_COLOR.Double(252.0/255.0,212.0/255.0,128.0/255.0,1.0);
 LIGHT_AMB_COLOR.Double(0.3,0.3,0.3,1.0);
 LIGHT_DIFF_COLOR.Double(1.0,1.0,1.0,1.0);
 LIGHT_SPEC_COLOR.Double(0.25,0.25,0.25,1.0);
 LIGHT_DIR.Set(0.45,0.3,0.45);
 tilt=0.75;
 noiseTex=library.Load("data/images/textures/noise9.png",tllto_Tiling);
 surfaceTex=library.Load("data/images/textures/pitted.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
})
 Cartesiand position,resolution,LIGHT_DIR;
 Crayon LIGHT_COLOR,LIGHT_AMB_COLOR,LIGHT_DIFF_COLOR,LIGHT_SPEC_COLOR;
 Zdouble tilt;
  Zp<GLImage> noiseTex,surfaceTex;
 BINARY_IN({
  B_IN_SSUB(position)
  B_IN_SSUB(resolution)
  B_IN_SSUB(LIGHT_DIR)
  B_IN_TINT(LIGHT_COLOR)
  B_IN_TINT(LIGHT_AMB_COLOR)
  B_IN_TINT(LIGHT_DIFF_COLOR)
  B_IN_TINT(LIGHT_SPEC_COLOR)
  B_IN_NUM(tilt)
  B_IN_TILE(noiseTex)
  B_IN_TILE(surfaceTex)
 })
 BINARY_OUT({
  B_OUT_SSUB(position)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(LIGHT_DIR)
  B_OUT_TINT(LIGHT_COLOR)
  B_OUT_TINT(LIGHT_AMB_COLOR)
  B_OUT_TINT(LIGHT_DIFF_COLOR)
  B_OUT_TINT(LIGHT_SPEC_COLOR)
  B_OUT_NUM(tilt)
  B_IN_TILE(noiseTex)
  B_IN_TILE(surfaceTex)
 })
 KEYWORDS({
  SUBWORD("position",position)
  else SUBWORD("resolution",resolution)
  else SUBWORD("LIGHT_DIR",LIGHT_DIR)
  else TINTWORD("LIGHT_COLOR",LIGHT_COLOR)
  else TINTWORD("LIGHT_AMB_COLOR",LIGHT_AMB_COLOR)
  else TINTWORD("LIGHT_DIFF_COLOR",LIGHT_DIFF_COLOR)
  else TINTWORD("LIGHT_SPEC_COLOR",LIGHT_SPEC_COLOR)
  else KEYWORD("tilt",{tilt=w;})
  else TILEWORD("noiseTex",noiseTex)
  else TILEWORD("surfaceTex",surfaceTex)
 })
 WORDKEYS({
  SSUBKEY("position",position)
  SSUBKEY("resolution",resolution)
  SSUBKEY("LIGHT_DIR",LIGHT_DIR)
  TINTKEY("LIGHT_COLOR",LIGHT_COLOR)
  TINTKEY("LIGHT_AMB_COLOR",LIGHT_AMB_COLOR)
  TINTKEY("LIGHT_DIFF_COLOR",LIGHT_DIFF_COLOR)
  TINTKEY("LIGHT_SPEC_COLOR",LIGHT_SPEC_COLOR)
  NUMBKEY("tilt",tilt)
  IMGKEY("noiseTex",noiseTex)
  IMGKEY("surfaceTex",surfaceTex)
 })
MANY(TexturedPlanetProfile,TexturedPlanetProfileHandle,TexturedPlanetProfileHandles,"TexturedPlanet",TexturedPlanetProfiles,{})
 KEYWORDSGroup("TexturedPlanet",TexturedPlanetProfile);
 WORDKEYSGroup("TexturedPlanet",TexturedPlanetProfile);
 BINARY_INOUT(TexturedPlanetProfile)
DONE(TexturedPlanetProfile);

class TexturedPlanetShader : public GLSLShader {
public:
  Cartesiand position,resolution,LIGHT_DIR;
  Crayon LIGHT_COLOR,LIGHT_AMB_COLOR,LIGHT_DIFF_COLOR,LIGHT_SPEC_COLOR;
  Zdouble tilt;
  Zp<GLuint> noiseTex,surfaceTex;
 GLuint Uposition,Uresolution,ULIGHT_DIR,ULIGHT_COLOR,ULIGHT_AMB_COLOR,ULIGHT_DIFF_COLOR,ULIGHT_SPEC_COLOR,Utilt,UnoiseTex,UsurfaceTex;
 void Init() {
  OUTPUT("TexturedPlanetShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec2 position;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec3 LIGHT_COLOR;  // vec3(252.,212.,128.)/255.\n")
   +string("uniform vec3 LIGHT_AMB_COLOR;  // vec3(.3)\n")
   +string("uniform vec3 LIGHT_DIFF_COLOR; // vec3(1.)\n")
   +string("uniform vec3 LIGHT_SPEC_COLOR; // vec3(0.25)\n")
   +string("uniform vec3 LIGHT_DIR;  // normalize(vec3(45.,30.,45.))\n")
   +string("uniform float tilt;      //=0.75;\n")
   +string("uniform sampler2D noiseTex;\n")
   +string("uniform sampler2D surfaceTex;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("const int MAX_ITERATIONS =16;\n")
   +string("const float MAX_DISTANCE =20000.0;\n")
   +string("const float SHADOW_BIAS  =0.01;\n")
   +string("const float EPSILON = 0.001;\n")
   +string("const float PI = 3.14159265358979323846264338327950288419;\n")
   +string("vec2 rot2D(vec2 p, float angle) {\n")
   +string("	angle = radians(angle);\n")
   +string("    float s = sin(angle), c = cos(angle);\n")
   +string("    return mat2(c,s,-s,c)*p;\n")
   +string("}\n")
   +string("float noise( in vec3 x )\n")
   +string("{\n")
   +string("    vec3 p = floor(x);\n")
   +string("    vec3 f = fract(x);\n")
   +string("	f = f*f*(3.0-2.0*f);\n")
   +string("	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;\n")
   +string("	vec2 rg = texture2D( noiseTex, (uv+ 0.5)/256.0, -100.0 ).yx;\n")
   +string("	return -1.0+2.0*mix( rg.x, rg.y, f.z );\n")
   +string("}\n")
   +string("float dstScene(vec3 p) {\n")
   +string("    vec3  n = normalize(p);\n")
   +string("    vec2 uv = asin(n.xy)/PI+0.5;\n")
   +string("    float h = texture2D(surfaceTex,uv).x*0.1;\n")
   +string("	return length(p)-(6300.0+(1250.0*h));\n")
   +string("}\n")
   +string("float raymarch(vec3 ori, vec3 dir) {\n")
   +string("    float t = 0.;\n")
   +string("    for(int i = 0; i < MAX_ITERATIONS; i++) {\n")
   +string("    	float scn = dstScene(ori+dir*t);\n")
   +string("        if(scn < EPSILON*t || t > MAX_DISTANCE)\n")
   +string("            break;\n")
   +string("        t += scn * 0.75;\n")
   +string("    }\n")
   +string("    return t;\n")
   +string("}\n")
   +string("vec3 calcNormal(vec3 p, float t) {\n")
   +string("	vec2 e = vec2(EPSILON*t,0.0);\n")
   +string("    vec3 n = vec3(dstScene(p+e.xyy)-dstScene(p-e.xyy),dstScene(p+e.yxy)-dstScene(p-e.yxy),dstScene(p+e.yyx)-dstScene(p-e.yyx));\n")
   +string("    return normalize(n);\n")
   +string("}\n")
   +string("float softshadow( vec3 ro, vec3 rd, float mint, float tmax, float k )\n")
   +string("{\n")
   +string("	float res = 1.0;\n")
   +string("    float t = mint;\n")
   +string("    for( int i=0; i<64; i++ )\n")
   +string("    {\n")
   +string("		float h = dstScene( ro + rd*t );\n")
   +string("        res = min( res, k*h/t );\n")
   +string("        t += clamp( h, 0.07, 0.5 );\n")
   +string("        if( h<0.001 || t>tmax ) break;\n")
   +string("    }\n")
   +string("    return clamp( res, 0.0, 1.0 );\n")
   +string("}\n")
   +string("vec3 calcLighting(vec3 col, vec3 p, vec3 n, vec3 r, float sh, vec3 ori) {\n")
   +string("	vec3 ds = vec3(0.0);\n")
   +string("    vec3 ss = vec3(0.0);\n")
   +string("    float d = max(dot(LIGHT_DIR,n),0.0);\n")
   +string("    float s = 0.0;\n")
   +string("    d *= softshadow(p,LIGHT_DIR,SHADOW_BIAS,MAX_DISTANCE,64.0);\n")
   +string("    if(d > 0.0 && sh > 0.0)\n")
   +string("        s = pow(max(dot(LIGHT_DIR,r),0.0),sh);\n")
   +string("    ds += LIGHT_COLOR * d;\n")
   +string("    ss += LIGHT_COLOR * s;\n")
   +string("    vec3 plv = ori-p;\n")
   +string("    vec3 pld = normalize(plv);\n")
   +string("    d = max(dot(pld,n),0.0);\n")
   +string("    s = 0.;\n")
   +string("    d *= softshadow(p,pld,SHADOW_BIAS,length(plv),64.0);\n")
   +string("    if(d > 0.0 && sh > 0.0)\n")
   +string("        s = pow(max(dot(pld,r),0.0),sh);\n")
   +string("    float a = 1.0-clamp(length(plv)/3500.0,0.0,1.0);\n")
   +string("    ds += vec3(d*a);\n")
   +string("    ss += vec3(s*a);\n")
   +string("    return (col*(LIGHT_AMB_COLOR+ds*LIGHT_DIFF_COLOR))+ss*LIGHT_SPEC_COLOR;\n")
   +string("}\n")
   +string("vec4 shade(vec3 ori, vec3 dir) {\n")
   +string("    float  t = raymarch(ori,dir);\n")
   +string("    vec4 col = vec4(0.0);    \n")
   +string("    if(t < MAX_DISTANCE) {\n")
   +string("    	vec3 p = ori+dir*t;\n")
   +string("        vec3 n = calcNormal(p,t);\n")
   +string("        vec3 r = normalize(reflect(dir, n));        \n")
   +string("        vec2 uv = asin(n.xy)/PI+0.5;        \n")
   +string("        col = vec4(texture2D(noiseTex,uv*2.0).xyz,1.0);\n")
   +string("        vec3 lighting=calcLighting(col.xyz, p, n, r, 30.0, ori);\n")
   +string("        col = vec4(lighting,1.0);\n")
   +string("    float f = pow(max(dot(LIGHT_DIR,dir),0.0),60.0);\n")
   +string("  	if(raymarch(ori,LIGHT_DIR) >= MAX_DISTANCE)\n")
   +string("         col += vec4(LIGHT_COLOR * f,1.0);\n")
   +string("    } else {\n")
   +string("    	col = vec4(0.0,0.0,0.0,0.0); //col = getSky(dir);\n")
   +string("    }    \n")
   +string("    return col;    \n")
   +string("}\n")
   +string("vec3 getCameraOrigin(float t) {\n")
   +string("	float gt = 1.0; //mod(t, 15.0);\n")
   +string("    float  r = 1.0-(smoothstep(3.0,6.0,gt)*(1.0-smoothstep(12.0,15.0,gt)));\n")
   +string("    float  a = radians(35.0*(2.0+t*0.1));\n")
   +string("    return vec3(cos(a),tilt,sin(a))*mix(1050.0,15000.0,r);\n")
   +string("}\n")
   +string("void main()\n")
   +string("{\n")
   +string("	vec2 uv = (v_vTexcoord.xy - resolution.xy * 0.5) / resolution.y;\n")
   +string("    vec3 ori = getCameraOrigin(position.x);\n")
   +string("    vec3 dir = vec3(uv,1.0);\n")
   +string("    vec3 f = normalize(-ori);\n")
   +string("    vec3 u = normalize(cross(f,vec3(0.0,1.0,0.0)));\n")
   +string("    vec3 v = normalize(cross(u,f));\n")
   +string("    dir = normalize(mat3(u,v,f)*dir);\n")
   +string("	gl_FragColor = shade(ori,dir);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uposition=glGetUniformLocation(program, "position");
  Uresolution=glGetUniformLocation(program, "resolution");
  ULIGHT_DIR=glGetUniformLocation(program, "LIGHT_DIR");
  ULIGHT_COLOR=glGetUniformLocation(program, "LIGHT_COLOR");
  ULIGHT_AMB_COLOR=glGetUniformLocation(program, "LIGHT_AMB_COLOR");
  ULIGHT_DIFF_COLOR=glGetUniformLocation(program, "LIGHT_DIFF_COLOR");
  ULIGHT_SPEC_COLOR=glGetUniformLocation(program, "LIGHT_SPEC_COLOR");
  Utilt=glGetUniformLocation(program, "tilt");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  UsurfaceTex=glGetUniformLocation(program, "surfaceTex");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uposition,(GLfloat)position.x,(GLfloat)position.y);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform3f(ULIGHT_DIR,(GLfloat)LIGHT_DIR.x,(GLfloat)LIGHT_DIR.y,(GLfloat)LIGHT_DIR.z);
  glUniform3f(ULIGHT_COLOR,(GLfloat)LIGHT_COLOR.rf,(GLfloat)LIGHT_COLOR.gf,(GLfloat)LIGHT_COLOR.bf);
  glUniform3f(ULIGHT_AMB_COLOR,(GLfloat)LIGHT_AMB_COLOR.rf,(GLfloat)LIGHT_AMB_COLOR.gf,(GLfloat)LIGHT_AMB_COLOR.bf);
  glUniform3f(ULIGHT_DIFF_COLOR,(GLfloat)LIGHT_DIFF_COLOR.rf,(GLfloat)LIGHT_DIFF_COLOR.gf,(GLfloat)LIGHT_DIFF_COLOR.bf);
  glUniform3f(ULIGHT_SPEC_COLOR,(GLfloat)LIGHT_SPEC_COLOR.rf,(GLfloat)LIGHT_SPEC_COLOR.gf,(GLfloat)LIGHT_SPEC_COLOR.bf);
  glUniform1f(Utilt,(GLfloat)tilt);
  glEnable(GL_TEXTURE_2D);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
  glUniform1i(UnoiseTex, 1);  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *surfaceTex);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);
 }
 void Set( TexturedPlanetProfile *in ) {
  position.Set(&in->position);
  resolution.Set(&in->resolution);
  LIGHT_DIR.Set(&in->LIGHT_DIR);
  LIGHT_COLOR.fromCrayon(in->LIGHT_COLOR);
  LIGHT_AMB_COLOR.fromCrayon(in->LIGHT_AMB_COLOR);
  LIGHT_DIFF_COLOR.fromCrayon(in->LIGHT_DIFF_COLOR);
  LIGHT_SPEC_COLOR.fromCrayon(in->LIGHT_SPEC_COLOR);
  tilt=in->tilt;
  noiseTex=&in->noiseTex->texture;
  surfaceTex=&in->surfaceTex->texture;
 }
};

extern TexturedPlanetShader texturedplanetShader;

void ShadedTexturedPlanet( FBOColor *out, TexturedPlanetProfile *in );


//---------------------------

ONE(EarthlikeProfile,{
 noiseOffset.Set(0.0, 0.0);
 noiseScale.Set(16.0, 16.0);
 noiseScale2.Set(200.0, 200.0);
 noiseScale3.Set(50.0, 50.0);
 cloudNoise.Set(10.0, 30.0);
 oceanColor.Double(13.0 / 255.0, 55.0 / 255.0, 79.0 / 255.0, 1.0);
 iceColor.Double(250.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0, 1.0);
 coldColor.Double(53.0 / 255.0, 102.0 / 255.0, 100.0 / 255.0, 1.0);
 temperateColor.Double(79.0 / 255.0, 109.0 / 255.0, 68.0 / 255.0, 1.0);
 warmColor.Double(119.0 / 255.0, 141.0 / 255.0, 82.0 / 255.0, 1.0);
 hotColor.Double(223.0 / 255.0, 193.0 / 255.0, 148.0 / 255.0, 1.0);
 cloudiness=0.5;
 resolution.Set(1.0,1.0);
 timescale=0.01;
})
 Cartesiand pos2d,resolution,noiseOffset,noiseScale,noiseScale2,noiseScale3,cloudNoise;
 Zdouble time,timescale,cloudiness;
 Crayon oceanColor,iceColor,coldColor,temperateColor,warmColor,hotColor;
 BINARY_IN({
  B_IN_SSUB(pos2d)
  B_IN_SSUB(resolution)
  B_IN_SSUB(noiseOffset)
  B_IN_SSUB(noiseScale)
  B_IN_SSUB(noiseScale2)
  B_IN_SSUB(noiseScale3)
  B_IN_SSUB(cloudNoise)
  B_IN_NUM(time)
  B_IN_NUM(timescale)
  B_IN_NUM(cloudiness)
  B_IN_TINT(oceanColor)
  B_IN_TINT(iceColor)
  B_IN_TINT(coldColor)
  B_IN_TINT(temperateColor)
  B_IN_TINT(warmColor)
  B_IN_TINT(hotColor)
 })
 BINARY_OUT({
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(noiseOffset)
  B_OUT_SSUB(noiseScale)
  B_OUT_SSUB(noiseScale2)
  B_OUT_SSUB(noiseScale3)
  B_OUT_SSUB(cloudNoise)
  B_OUT_NUM(time)
  B_OUT_NUM(timescale)
  B_OUT_NUM(cloudiness)
  B_OUT_TINT(oceanColor)
  B_OUT_TINT(iceColor)
  B_OUT_TINT(coldColor)
  B_OUT_TINT(temperateColor)
  B_OUT_TINT(warmColor)
  B_OUT_TINT(hotColor)
 })
 KEYWORDS({
  SUBWORD("pos2d",pos2d)
  else SUBWORD("resolution",resolution)
  else SUBWORD("noiseOffset",noiseOffset)
  else SUBWORD("noiseScale",noiseScale)
  else SUBWORD("noiseScale2",noiseScale2)
  else SUBWORD("noiseScale3",noiseScale3)
  else SUBWORD("cloudNoise",cloudNoise)
  else KEYWORD("time",{time=w;})
  else KEYWORD("time",{timescale=w;})
  else KEYWORD("cloudiness",{cloudiness=w;})
  else TINTWORD("oceanColor",oceanColor)
  else TINTWORD("iceColor",iceColor)
  else TINTWORD("coldColor",coldColor)
  else TINTWORD("temperateColor",temperateColor)
  else TINTWORD("warmColor",warmColor)
  else TINTWORD("hotColor",hotColor)
 })
 WORDKEYS({
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("resolution",resolution)
  SSUBKEY("noiseOffset",noiseOffset)
  SSUBKEY("noiseScale",noiseScale)
  SSUBKEY("noiseScale2",noiseScale2)
  SSUBKEY("noiseScale3",noiseScale3)
  SSUBKEY("cloudNoise",cloudNoise)
  NUMBKEY("time",time)
  NUMBKEY("time",timescale)
  NUMBKEY("cloudiness",cloudiness)
  TINTKEY("oceanColor",oceanColor)
  TINTKEY("iceColor",iceColor)
  TINTKEY("coldColor",coldColor)
  TINTKEY("temperateColor",temperateColor)
  TINTKEY("warmColor",warmColor)
  TINTKEY("hotColor",hotColor)
 })
MANY(EarthlikeProfile,EarthlikeProfileHandle,EarthlikeProfileHandles,"Earthlike",EarthlikeProfiles,{})
 KEYWORDSGroup("Earthlike",EarthlikeProfile);
 WORDKEYSGroup("Earthlike",EarthlikeProfile);
 BINARY_INOUT(EarthlikeProfile)
DONE(EarthlikeProfile);

class EarthlikeShader : public GLSLShader {
public:
 Cartesiand pos2d,resolution,noiseOffset,noiseScale,noiseScale2,noiseScale3,cloudNoise;
 Zdouble time,timescale,cloudiness;
 Crayon oceanColor,iceColor,coldColor,temperateColor,warmColor,hotColor;
 GLuint Upos2d,Uresolution,UnoiseOffset,UnoiseScale,UnoiseScale2,UnoiseScale3,UcloudNoise,Utime,Utimescale,Ucloudiness,UoceanColor,UiceColor,UcoldColor,UtemperateColor,UwarmColor,UhotColor;
 void Init() {
  OUTPUT("EarthlikeShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("// Planet\n")
   +string("// https://www.shadertoy.com/view/MdX3RM \n")
   +string("#ifdef GL_ES\n")
   +string("precision highp float;\n")
   +string("#endif\n")
   +string("\n")
   +string("uniform vec2 pos2d;\n")
   +string("uniform float time;\n")
   +string("uniform float timescale;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec2 noiseOffset; //= vec2(0.0, 0.0);\n")
   +string("uniform vec2 noiseScale;  //= vec2(16.0, 16.0);\n")
   +string("uniform vec2 noiseScale2; //= vec2(200.0, 200.0);\n")
   +string("uniform vec2 noiseScale3; //= vec2(50.0, 50.0);\n")
   +string("uniform vec2 cloudNoise;  //= vec2(10.0, 30.0);\n")
   +string("uniform vec3 oceanColor;     //= vec3(13.0 / 255.0, 55.0 / 255.0, 79.0 / 255.0);\n")
   +string("uniform vec3 iceColor;       //= vec3(250.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0);\n")
   +string("uniform vec3 coldColor;      //= vec3(53.0 / 255.0, 102.0 / 255.0, 100.0 / 255.0);\n")
   +string("uniform vec3 temperateColor; //= vec3(79.0 / 255.0, 109.0 / 255.0, 68.0 / 255.0);\n")
   +string("uniform vec3 warmColor;      //= vec3(119.0 / 255.0, 141.0 / 255.0, 82.0 / 255.0);\n")
   +string("uniform vec3 hotColor;       //= vec3(223.0 / 255.0, 193.0 / 255.0, 148.0 / 255.0);\n")
   +string("uniform float cloudiness;  //= 0.5\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("float t10 = time * timescale;\n")
   +string("float left = 0.0;\n")
   +string("float top = 0.0;\n")
   +string("vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec4 permute(vec4 x) { return mod289(((x*34.0)+1.0)*x); }\n")
   +string("vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }\n")
   +string("vec2 fade(vec2 t) { return t*t*t*(t*(t*6.0-15.0)+10.0); }\n")
   +string("float spow(float x, float y) { float s = sign(x); return s * pow(s * x, y); }\n")
   +string("float pnoise(vec2 P, vec2 rep)\n")
   +string("{\n")
   +string("  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);\n")
   +string("  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);\n")
   +string("  Pi = mod(Pi, rep.xyxy); // To create noise with explicit period\n")
   +string("  Pi = mod289(Pi);        // To avoid truncation effects in permutation\n")
   +string("  vec4 ix = Pi.xzxz;\n")
   +string("  vec4 iy = Pi.yyww;\n")
   +string("  vec4 fx = Pf.xzxz;\n")
   +string("  vec4 fy = Pf.yyww;\n")
   +string("  vec4 i = permute(permute(ix) + iy);\n")
   +string("  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;\n")
   +string("  vec4 gy = abs(gx) - 0.5 ;\n")
   +string("  vec4 tx = floor(gx + 0.5);\n")
   +string("  gx = gx - tx;\n")
   +string("  vec2 g00 = vec2(gx.x,gy.x);\n")
   +string("  vec2 g10 = vec2(gx.y,gy.y);\n")
   +string("  vec2 g01 = vec2(gx.z,gy.z);\n")
   +string("  vec2 g11 = vec2(gx.w,gy.w);\n")
   +string("  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));\n")
   +string("  g00 *= norm.x;  \n")
   +string("  g01 *= norm.y;  \n")
   +string("  g10 *= norm.z;  \n")
   +string("  g11 *= norm.w;\n")
   +string("  float n00 = dot(g00, vec2(fx.x, fy.x));\n")
   +string("  float n10 = dot(g10, vec2(fx.y, fy.y));\n")
   +string("  float n01 = dot(g01, vec2(fx.z, fy.z));\n")
   +string("  float n11 = dot(g11, vec2(fx.w, fy.w));\n")
   +string("  vec2 fade_xy = fade(Pf.xy);\n")
   +string("  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);\n")
   +string("  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);\n")
   +string("  return 2.3 * n_xy;\n")
   +string("}\n")
   +string("vec4 planet(vec2 pix, float rotspeed, float rot, float light, float zLight) {\n")
   +string(" vec2 p = -1.0 + 2.0 * pix;\n")
   +string(" p.x *= resolution.x / resolution.y;\n")
   +string(" p = mat2(cos(rot), sin(rot), -sin(rot), cos(rot)) * p;	\n")
   +string(" vec3 ro = vec3( 0.0, 0.0, 2.25 );\n")
   +string(" vec3 rd = normalize( vec3( p, -2.0 ) );\n")
   +string(" vec3 col = vec3(0.0);\n")
   +string(" // intersect sphere\n")
   +string(" float b = dot(ro,rd);\n")
   +string(" float c = dot(ro,ro) - 1.0;\n")
   +string(" float h = b*b - c;\n")
   +string("	float t = -b - sqrt(h);\n")
   +string("	vec3 pos = ro + t*rd;\n")
   +string("	vec3 nor = pos;\n")
   +string("	// texture mapping\n")
   +string("	vec2 uv;\n")
   +string("	uv.x = atan(nor.x,nor.z)/6.2831 + rotspeed*t10;\n")
   +string("	uv.y = acos(nor.y)/3.1416;\n")
   +string("	uv.y = 0.5 + spow(uv.y - 0.5, 1.2);\n")
   +string("	uv += noiseOffset;	\n")
   +string("	float n2 = pnoise(uv * noiseScale2, noiseScale2) * 0.05;\n")
   +string("	float n = pnoise(uv * noiseScale, noiseScale) + n2;	\n")
   +string("	float temp = cos(nor.y * 4.0) + pnoise(uv * noiseScale3, noiseScale3) * 0.8 + n * 0.5;	\n")
   +string("	float oceanity = min(1.0, 1.0 - smoothstep(0.19, 0.2, n) + 1.0 - smoothstep(0.05, 0.08, mod(temp - uv.x * 35.0 + 0.3, 1.0) + n * n * 0.35));\n")
   +string("	float iceity = max(0.0, 1.0 - oceanity - smoothstep(-0.8, -0.6, temp));\n")
   +string("	float coldity = max(0.0, 1.0 - iceity - oceanity - smoothstep(-0.4, 0.0, temp));\n")
   +string("	float temperateity = max(0.0, 1.0 - iceity - coldity - oceanity - smoothstep(0.3, 0.8, temp));\n")
   +string("	float warmity = max(0.0, 1.0 - iceity - coldity - temperateity - oceanity - smoothstep(1.05, 1.3, temp));\n")
   +string("	float hottity = max(0.0, 1.0 - oceanity - iceity - coldity - temperateity - warmity);	\n")
   +string("	col = oceanColor * oceanity + iceColor * iceity + coldColor * coldity + temperateColor * temperateity + warmColor * warmity + hotColor * hottity;\n")
   +string("	col *= (0.7 + abs(temp + n * 0.2) * 0.3);\n")
   +string("	col *= 0.92 + step(0.1, mod(n2, 0.4)) * 0.08;\n")
   +string("	col *= 1.0 + step(0.39, mod(n + uv.x, 0.4)) * 0.1;\n")
   +string("	float cloudN = max(0.0, pnoise((uv + vec2(rotspeed * t10, 0)) * cloudNoise, cloudNoise) + cloudiness + n2);\n")
   +string("	col *= 0.7;\n")
   +string("	col += vec3(cloudN, cloudN, cloudN) * 0.5;\n")
   +string(" float lighting = max(sin(light) * nor.y * 2.0 + cos(light) * nor.x * 2.0 + nor.z * zLight,0.0);\n")
   +string("	col *= 0.2 + lighting * 0.7;\n")
   +string(" float den=step(0.0, h);")
   +string(" return vec4(mix(vec3(0.0), col, den), den);\n")
   +string("}\n")
   +string("void main() {\n")
   +string("	vec3 coord = vec3(v_vTexcoord,0.0);\n")
   +string("	coord.x += left + pos2d.x;\n")
   +string("	coord.y += top + pos2d.y;\n")
   +string("	vec4 col = planet(coord.xy / resolution.xy, 0.05, float(int(t10 * 0.08)), t10 * 0.1, sin(t10 * 0.05) * 2.0);\n")
   +string("	gl_FragColor=col; //vec4(col.xyz,min(1.0,0.95+col.x+col.y+col.z));\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Upos2d=glGetUniformLocation(program, "pos2d");
  Uresolution=glGetUniformLocation(program, "resolution");
  UnoiseOffset=glGetUniformLocation(program, "noiseOffset");
  UnoiseScale=glGetUniformLocation(program, "noiseScale");
  UnoiseScale2=glGetUniformLocation(program, "noiseScale2");
  UnoiseScale3=glGetUniformLocation(program, "noiseScale3");
  UcloudNoise=glGetUniformLocation(program, "cloudNoise");
  Utime=glGetUniformLocation(program, "time");
  Utimescale=glGetUniformLocation(program, "timescale");
  Ucloudiness=glGetUniformLocation(program, "cloudiness");
  UoceanColor=glGetUniformLocation(program, "oceanColor");
  UiceColor=glGetUniformLocation(program, "iceColor");
  UcoldColor=glGetUniformLocation(program, "coldColor");
  UtemperateColor=glGetUniformLocation(program, "temperateColor");
  UwarmColor=glGetUniformLocation(program, "warmColor");
  UhotColor=glGetUniformLocation(program, "hotColor");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(UnoiseOffset,(GLfloat)noiseOffset.x,(GLfloat)noiseOffset.y);
  glUniform2f(UnoiseScale,(GLfloat)noiseScale.x,(GLfloat)noiseScale.y);
  glUniform2f(UnoiseScale2,(GLfloat)noiseScale2.x,(GLfloat)noiseScale2.y);
  glUniform2f(UnoiseScale3,(GLfloat)noiseScale3.x,(GLfloat)noiseScale3.y);
  glUniform2f(UcloudNoise,(GLfloat)cloudNoise.x,(GLfloat)cloudNoise.y);
  glUniform3f(UoceanColor,(GLfloat)oceanColor.rf,(GLfloat)oceanColor.gf,(GLfloat)oceanColor.bf);
  glUniform3f(UiceColor,(GLfloat)iceColor.rf,(GLfloat)iceColor.gf,(GLfloat)iceColor.bf);
  glUniform3f(UcoldColor,(GLfloat)coldColor.rf,(GLfloat)coldColor.gf,(GLfloat)coldColor.bf);
  glUniform3f(UtemperateColor,(GLfloat)temperateColor.rf,(GLfloat)temperateColor.gf,(GLfloat)temperateColor.bf);
  glUniform3f(UwarmColor,(GLfloat)warmColor.rf,(GLfloat)warmColor.gf,(GLfloat)warmColor.bf);
  glUniform3f(UhotColor,(GLfloat)hotColor.rf,(GLfloat)hotColor.gf,(GLfloat)hotColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Ucloudiness,(GLfloat)cloudiness);
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( EarthlikeProfile *in ) {
  pos2d.Set(&in->pos2d);
  resolution.Set(&in->resolution);
  noiseOffset.Set(&in->noiseOffset);
  noiseScale.Set(&in->noiseScale);
  noiseScale2.Set(&in->noiseScale2);
  noiseScale3.Set(&in->noiseScale3);
  cloudNoise.Set(&in->cloudNoise);
  time=in->time;
  cloudiness=in->cloudiness;
  oceanColor.fromCrayon(in->oceanColor);
  iceColor.fromCrayon(in->iceColor);
  coldColor.fromCrayon(in->coldColor);
  temperateColor.fromCrayon(in->temperateColor);
  warmColor.fromCrayon(in->warmColor);
  hotColor.fromCrayon(in->hotColor);
  timescale=in->timescale;
 }
};

extern EarthlikeShader earthlikeShader;

void ShadedEarthlike( FBOColor *out, EarthlikeProfile *in );

// ----------------------------------------------------



// ----------------------------------------------------

ONE(AccretionProfile,{
 tex1=library.Load("data/images/textures/moldy.png",tllto_Tiling);
 tex2=library.Load("data/images/textures/noise3.png",tllto_Tiling);
 tex3=library.Load("data/images/textures/noise6.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
})
 Cartesiand resolution,pos2d,scale;
  Zp<GLImage> tex1,tex2,tex3;
 Zdouble time;
 BINARY_IN({
  B_IN_SSUB(resolution)
  B_IN_SSUB(pos2d)
  B_IN_SSUB(scale)
  B_IN_TILE(tex1)
  B_IN_TILE(tex2)
  B_IN_TILE(tex3)
  B_IN_NUM(time)
 })
 BINARY_OUT({
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(scale)
  B_OUT_TILE(tex1)
  B_OUT_TILE(tex2)
  B_OUT_TILE(tex3)
  B_OUT_NUM(time)
 })
 KEYWORDS({
  SUBWORD("resolution",resolution)
  else SUBWORD("pos2d",pos2d)
  else SUBWORD("scale",scale)
  else TILEWORD("tex1",tex1)
  else TILEWORD("tex2",tex2)
  else TILEWORD("tex3",tex3)
  else KEYWORD("time",{time=w;})
 })
 WORDKEYS({
  SSUBKEY("resolution",resolution)
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("scale",scale)
  IMGKEY("tex1",tex1)
  IMGKEY("tex2",tex2)
  IMGKEY("tex3",tex3)
  NUMBKEY("time",time)
 })
MANY(AccretionProfile,AccretionProfileHandle,AccretionProfileHandles,"Accretion",AccretionProfiles,{})
 KEYWORDSGroup("Accretion",AccretionProfile);
 WORDKEYSGroup("Accretion",AccretionProfile);
 BINARY_INOUT(AccretionProfile);
DONE(AccretionProfile);

class AccretionShader : public GLSLShader {
public:
  Cartesiand resolution,pos2d,scale;
  Zp<GLuint> tex1,tex2,tex3;
  Zdouble time;
 GLuint Uresolution,Upos2d,Uscale,Utex1,Utex2,Utex3,Utime;
 void Init() {
  OUTPUT("AccretionShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec2 resolution;  \n")
   +string("uniform vec2 pos2d;       \n")
   +string("uniform vec3 scale;       //=vec2(1.1,1.8,0.9)\n")
   +string("uniform sampler2D tex1;\n")
   +string("uniform sampler2D tex2;\n")
   +string("uniform sampler2D tex3;\n")
   +string("uniform float time;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("vec2 uniformCoord(vec2 uv) { return vec2(uv - resolution.xy / 2.0)  / (resolution.yy / 2.0); }\n")
   +string("vec2 rotate(vec2 uv, float angle)\n")
   +string("{\n")
   +string("    float s = sin(angle);\n")
   +string("    float c = cos(angle);\n")
   +string("    return uv * mat2(c, s, -s, c);\n")
   +string("}\n")
   +string("vec2 pp(vec2 uv)\n")
   +string("{\n")
   +string("    float y = uv.y * 0.5 + 0.5;\n")
   +string("    float ny = scale.y * y * scale.z;\n")
   +string("    return vec2(uv.x / scale.x * (1.0 - ny / 3.0), ny * 2.0 - 1.5);\n")
   +string("}\n")
   +string("float spiral(vec2 uv)\n")
   +string("{\n")
   +string("    float dist = 1.0 - length(uv);\n")
   +string("    float d = dist * 0.6 * texture2D(tex1, rotate(uv * 2.5, time * 0.1 + dist * 3.0)).x\n")
   +string("            + dist * 0.5 * texture2D(tex1, rotate(uv * 2.5, time * 0.125 + dist * 3.0)).x;\n")
   +string("    for(int i=0; i < 10; ++i)\n")
   +string("    {\n")
   +string("        vec2 coord = uv * (1.0 - fract(time * -0.02 + float(i) * 0.089));\n")
   +string("        coord = rotate(coord, time * 0.01 + dist * float(i) * 0.2);\n")
   +string("        vec2 tx = texture2D(tex1, coord).xy;\n")
   +string("        d += tx.x / 30.0;\n")
   +string("        d -= tx.y / 70.0;\n")
   +string("    }\n")
   +string("    return d;\n")
   +string("}\n")
   +string("vec3 star(vec2 uv)\n")
   +string("{\n")
   +string("    const float aspect = 5.5;\n")
   +string("    const float radius = 1.0/aspect;\n")
   +string("    vec3 c = vec3(0.0);\n")
   +string("    float dist = distance(uv, vec2(0,0));\n")
   +string("    uv = uv * aspect;\n")
   +string("    float r = dot(uv,uv);\n")
   +string("    float f = (1.0-sqrt(abs(1.0-r)))/(r);\n")
   +string("    if( dist < radius ){\n")
   +string("  	   vec2 newUv      = vec2(uv.x * f, (uv.y - 0.8) * f);		\n")
   +string("	   float wobble    = texture2D( tex2, newUv ).r * 0.3;\n")
   +string("	   float uOff      = ( wobble - time * 0.2);\n")
   +string("	   vec2 starUV	   = newUv + vec2( uOff, 0.0 );\n")
   +string("	   vec3 starSphere = texture2D( tex2, starUV ).rgb;\n")
   +string("       c = starSphere;\n")
   +string("       c = vec3(c.r + 1., c.g + 1., c.b + 1.1);\n")
   +string("       c *= (1.0 - dist * aspect);\n")
   +string("    }\n")
   +string("    c = c * (uv.y * 2.0 + 0.3);\n")
   +string("    return clamp(c, -0.03, 1.0);\n")
   +string("}\n")
   +string("vec3 gas(vec2 uv, float distort)\n")
   +string("{\n")
   +string("    const vec3 grading1	= vec3( 0.15, 0.7, 1.8 );\n")
   +string("    const vec3 grading2	= vec3( 1.0, 0.55, 0.0 );\n")
   +string("    const vec3 grading3	= vec3( 2.5, 1.0, 0.5 );\n")
   +string("    float dist = (1.0 - length(uv * vec2(0.6, 1.4))) * 0.04;\n")
   +string("    vec2 wobble = texture2D(tex2, rotate(uv, time * 0.1)).rg * dist;\n")
   +string("    vec3 c = vec3(spiral(pp(uv * 0.9 + wobble + distort * 0.5)));\n")
   +string("    vec3 cColor = vec3(c + ((c - 0.5) * grading1) + (c * grading2) + ((atan(c) * 0.3 + 1.1) * grading3 *0.1));\n")
   +string("    return cColor;\n")
   +string("}\n")
   +string("vec3 parts(vec2 uv)\n")
   +string("{\n")
   +string("    vec3 fi = texture2D(tex2, rotate(pp(uv), time * 0.06)).rgb;\n")
   +string("    vec2 uv1 = pp((uv + fi.rg * 4.1) * 0.8) * 0.5;\n")
   +string("    vec2 uv2 = pp((uv ) * 1.8) * 0.9;\n")
   +string("	float dist = 1.0 - length(uv * vec2(0.1, 0.2));\n")
   +string("    float d = dist * 0.63 * texture2D(tex3, rotate(uv1.yy, time * 0.01 + dist * 1.0)).x\n")
   +string("            + dist * 0.5 * texture2D(tex3, rotate(uv1, time * 0.12 + dist * 1.2)).x\n")
   +string("            + dist * 0.33 * texture2D(tex3, rotate(uv2.xx, time * 0.014 + dist * 1.4)).x\n")
   +string("            + dist * 0.7 * texture2D(tex3, rotate(uv2, time * 0.16 + dist * 1.6)).x;    \n")
   +string("    vec3 c = vec3(pow(d, 8.0) *  0.2) * fi;\n")
   +string("    return c;\n")
   +string("}\n")
   +string("void main()\n")
   +string("{\n")
   +string("	vec2 uv = uniformCoord(-pos2d.xy+v_vTexcoord*1.5);\n")
   +string("    vec3 c1 = parts(uv);\n")
   +string("    vec3 c2 = gas(uv, c1.r);\n")
   +string("    vec3 c3 = star(uv);\n")
   +string("    gl_FragColor = vec4((c1 + c2 + c3),(c1+c2+c3)/3.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uresolution=glGetUniformLocation(program, "resolution");
  Upos2d=glGetUniformLocation(program, "pos2d");
  Uscale=glGetUniformLocation(program, "scale");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utime=glGetUniformLocation(program, "time");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform3f(Uscale,(GLfloat)scale.x,(GLfloat)scale.y,(GLfloat)scale.z);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1i(Utex1, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex1);
  glUniform1i(Utex2, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex2);
  glUniform1i(Utex3, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex3);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( AccretionProfile *in ) {
  resolution.Set(&in->resolution);
  pos2d.Set(&in->pos2d);
  scale.Set(&in->scale);
  tex1=&in->tex1->texture;
  tex2=&in->tex2->texture;
  tex3=&in->tex3->texture;
  time=in->time;
 }
};

extern AccretionShader accretionShader;

void ShadedAccretion( FBOColor *out, AccretionProfile *in );

// ----------------------------------------------------

ONE(BlackHoleProfile,{
 color1.Double(0.5,0.35,0.1,1.0);
 color2.Double(1.0,0.8,0.6,1.0);
 color3.Double(1.0,0.9,0.7,1.0);
 blackholeSize.Set(0.3,5.0);
 attenuate.Set(0.1,0.75,0.1);
 tex2=library.Load("data/images/textures/noise1.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
})
 Zdouble time;
 Crayon color1,color2,color3;
 Cartesiand resolution,orientation,blackholeSize,attenuate;
 Zp<GLImage> tex2;
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_TINT(color1)
  B_IN_TINT(color2)
  B_IN_TINT(color3)
  B_IN_SSUB(resolution)
  B_IN_SSUB(orientation)
  B_IN_SSUB(blackholeSize)
  B_IN_SSUB(attenuate)
  B_IN_TILE(tex2)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_TINT(color1)
  B_OUT_TINT(color2)
  B_OUT_TINT(color3)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(orientation)
  B_OUT_SSUB(blackholeSize)
  B_OUT_SSUB(attenuate)
  B_OUT_TILE(tex2)
 })
 KEYWORDS({
  NUMWORD("time",time)
  else TINTWORD("color1",color1)
  else TINTWORD("color2",color2)
  else TINTWORD("color3",color3)
  else SUBWORD("resolution",resolution)
  else SUBWORD("orientation",orientation)
  else SUBWORD("blackholeSize",blackholeSize)
  else SUBWORD("attenuate",attenuate)
  else TILEWORD("tex2",tex2)
 })
 WORDKEYS({
  TINTKEY("color1",color1)
  TINTKEY("color2",color2)
  TINTKEY("color3",color3)
  SSUBKEY("resolution",resolution)
  SSUBKEY("orientation",orientation)
  SSUBKEY("blackholeSize",blackholeSize)
  SSUBKEY("attenuate",attenuate)
  IMGKEY("tex2",tex2)
 })
MANY(BlackHoleProfile,BlackHoleProfileHandle,BlackHoleProfileHandles,"BlackHole",BlackHoleProfiles,{})
 KEYWORDSGroup("BlackHole",BlackHoleProfile);
 WORDKEYSGroup("BlackHole",BlackHoleProfile);
 BINARY_INOUT(BlackHoleProfile);
DONE(BlackHoleProfile);

class BlackHoleShader : public GLSLShader {
public:
  Zdouble time;
  Crayon color1,color2,color3;
  Cartesiand resolution,orientation,blackholeSize,attenuate;
  Zp<GLuint> tex2;
 GLuint Ucolor1,Ucolor2,Ucolor3,Uresolution,Uorientation,UblackholeSize,Uattenuate,Utex2,Utime;
 void Init() {
  OUTPUT("BlackHoleShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec3 color1; //= vec3(0.5,0.35,0.1)\n")
   +string("uniform vec3 color2; //= vec3(1.0,0.8,0.6)\n")
   +string("uniform vec3 color3; //= vec3(1.0,0.9,0.7)\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec2 orientation;\n")
   +string("uniform vec2 blackholeSize; // vec2(0.3,5.0)\n")
   +string("uniform vec3 attenuate;  // = vec2(0.1,0.75,0.1);\n")
   +string("uniform float time;\n")
   +string("uniform sampler2D tex2;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("const float pi = 3.141592659;\n")
   +string("float sdSphere( vec3 p, float s ) { return length(p)-s; }\n")
   +string("float sdCappedCylinder( vec3 p, vec2 h ) { \n")
   +string(" vec2 d = abs(vec2(length(p.xz),p.y)) - h;\n")
   +string(" return min(max(d.x,d.y),0.0) + length(max(d,0.0));\n")
   +string("}\n")
   +string("float sdTorus( vec3 p, vec2 t )\n")
   +string("{\n")
   +string(" vec2 q = vec2(length(p.xz)-t.x,p.y);\n")
   +string(" return length(q)-t.y;\n")
   +string("}\n")
   +string("void main()\n")
   +string("{\n")
   +string("	vec2 pp = v_vTexcoord.xy/resolution.xy * 2.0 - 0.5;\n")
   +string("	pp = -1.0 + 2.0*pp;\n")
   +string("	pp.x *= resolution.x/resolution.y;\n")
   +string("	vec3 lookAt = vec3(0.0, -0.1, 0.0);\n")
   +string("    float eyer = 2.0;\n")
   +string("    float eyea = (orientation.x / resolution.x) * pi * 2.0;\n")
   +string("    float eyea2 = ((orientation.y / resolution.y)-0.24) * pi * 2.0;\n")
   +string("	vec3 ro = vec3( eyer * cos(eyea) * sin(eyea2), eyer * cos(eyea2), eyer * sin(eyea) * sin(eyea2)); //camera position\n")
   +string("	vec3 front = normalize(lookAt - ro);\n")
   +string("	vec3 left = normalize(cross(normalize(vec3(0.0,1.0,-0.1)), front));\n")
   +string("	vec3 up = normalize(cross(front, left));\n")
   +string("	vec3 rd = normalize(front*1.5 + left*pp.x + up*pp.y); // rect vector\n")
   +string("    vec3 bh = vec3(0.0,0.0,0.0);\n")
   +string("    float bhr = blackholeSize.x;\n")
   +string("    float bhmass = blackholeSize.y;\n")
   +string("   	bhmass *= 0.001; // premul G\n")
   +string("    vec3 p = ro;\n")
   +string("    vec3 pv = rd;\n")
   +string("    float dt = 0.02;\n")
   +string("    vec3 col = vec3(0.0);\n")
   +string("    float noncaptured = 1.0;\n")
   +string("    for(float t=0.0;t<1.0;t+=0.005)\n")
   +string("    {\n")
   +string("        p += pv * dt * noncaptured;        \n")
   +string("        vec3 bhv = bh - p;\n")
   +string("        float r = dot(bhv,bhv);\n")
   +string("        pv += normalize(bhv) * ((bhmass) / r);\n")
   +string("        noncaptured = smoothstep(0.0,0.01,sdSphere(p-bh,bhr));\n")
   +string("        float dr = length(bhv.xz);\n")
   +string("        float da = atan(bhv.x,bhv.z);\n")
   +string("        vec2 ra = vec2(dr,da * (0.01 + (dr - bhr)*0.002) + 2.0 * pi + time*0.02 );\n")
   +string("        ra *= vec2(10.0,20.0);\n")
   +string("        vec3 dcol = mix(color2,color1,pow(length(bhv)-bhr,2.0)) * max(0.0,texture2D(tex2,ra*vec2(0.1,0.5)).r+0.05) * (4.0 / ((0.001+(length(bhv) - bhr)*50.0) ));\n")
   +string("        col += max(vec3(0.0),dcol * step(0.0,-sdTorus( (p * vec3(1.0,50.0,1.0)) - bh, vec2(0.8,0.99))) * noncaptured);\n")
   +string("        // glow\n")
   +string("        col += color3 * attenuate.z * (1.0/vec3(dot(bhv,bhv))) * 0.003 * noncaptured;\n")
   +string("    }\n")
   +string("	float atten = attenuate.x+distance(v_vTexcoord.xy/resolution.xy,vec2(0.5,0.5)) * attenuate.y;\n")
   +string("	col = col*atten;\n")
   +string(" gl_FragColor = vec4(col,(col.x+col.y+col.z)/3.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ucolor1=glGetUniformLocation(program, "color1");
  Ucolor2=glGetUniformLocation(program, "color2");
  Ucolor3=glGetUniformLocation(program, "color3");
  Uresolution=glGetUniformLocation(program, "resolution");
  Uorientation=glGetUniformLocation(program, "orientation");
  UblackholeSize=glGetUniformLocation(program, "blackholeSize");
  Uattenuate=glGetUniformLocation(program, "attenuate");
  Utex2=glGetUniformLocation(program, "tex2");
  Utime=glGetUniformLocation(program,"time");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Uorientation,(GLfloat)orientation.x,(GLfloat)orientation.y);
  glUniform2f(UblackholeSize,(GLfloat)blackholeSize.x,(GLfloat)blackholeSize.y);
  glUniform3f(Uattenuate,(GLfloat)attenuate.x,(GLfloat)attenuate.y,(GLfloat)attenuate.z);
  glUniform3f(Ucolor1,(GLfloat)color1.rf,(GLfloat)color1.gf,(GLfloat)color1.bf);
  glUniform3f(Ucolor2,(GLfloat)color2.rf,(GLfloat)color2.gf,(GLfloat)color2.bf);
  glUniform3f(Ucolor3,(GLfloat)color3.rf,(GLfloat)color3.gf,(GLfloat)color3.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1i(Utex2, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex2);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( BlackHoleProfile *in ) {
  color1.fromCrayon(in->color1);
  color2.fromCrayon(in->color2);
  color3.fromCrayon(in->color3);
  resolution.Set(&in->resolution);
  orientation.Set(&in->orientation);
  blackholeSize.Set(&in->blackholeSize);
  attenuate.Set(&in->attenuate);
  tex2=&in->tex2->texture;
  time=in->time;
 }
};

extern BlackHoleShader blackholeShader;

void ShadedBlackHole( FBOColor *out, BlackHoleProfile *in );





// ----------------------------------------------------
ONE(RingedJovianProfile,{
 ringColor.Double(0.95,0.8,0.7,1.0);
 planetColor.Double(0.9,0.75,0.6,1.0);
 surfaceParams.Set(1.2,2.0,0.05); surfaceParams.w=0.6;
 ringParams.Set(0.6,2.0,8.1);
 sunpos.Set(50,10,20);
 resolution.Set(1.0,1.0);
})
 Crayon ringColor,planetColor;
 Cartesiand surfaceParams,ringParams,sunpos,resolution;
 Zdouble time;
 BINARY_IN({
  B_IN_TINT(ringColor)
  B_IN_TINT(planetColor)
  B_IN_SSUB(surfaceParams)
  B_IN_SSUB(ringParams)
  B_IN_SSUB(sunpos)
  B_IN_SSUB(resolution)
  B_IN_NUM(time)
 })
 BINARY_OUT({
  B_OUT_TINT(ringColor)
  B_OUT_TINT(planetColor)
  B_OUT_SSUB(surfaceParams)
  B_OUT_SSUB(ringParams)
  B_OUT_SSUB(sunpos)
  B_OUT_SSUB(resolution)
  B_OUT_NUM(time)
 })
 KEYWORDS({
  TINTWORD("ringColor",ringColor)
  else TINTWORD("planetColor",planetColor)
  else SUBWORD("surfaceParams",surfaceParams)
  else SUBWORD("ringParams",ringParams)
  else SUBWORD("sunpos",sunpos)
  else SUBWORD("resolution",resolution)
  else KEYWORD("time",{time=w;})
 })
 WORDKEYS({
  TINTKEY("ringColor",ringColor)
  TINTKEY("planetColor",planetColor)
  SSUBKEY("surfaceParams",surfaceParams)
  SSUBKEY("ringParams",ringParams)
  SSUBKEY("sunpos",sunpos)
  SSUBKEY("resolution",resolution)
  NUMBKEY("time",time)
 })
MANY(RingedJovianProfile,RingedJovianProfileHandle,RingedJovianProfileHandles,"RingedJovian",RingedJovianProfiles,{})
 KEYWORDSGroup("RingedJovian",RingedJovianProfile);
 WORDKEYSGroup("RingedJovian",RingedJovianProfile);
 BINARY_INOUT(RingedJovianProfile)
DONE(RingedJovianProfile);

class RingedJovianShader : public GLSLShader {
public:
  Crayon ringColor,planetColor;
  Cartesiand surfaceParams,ringParams,sunpos,resolution;
  Zdouble time;
 GLuint UringColor,UplanetColor,UsurfaceParams,UringParams,Usunpos,Uresolution,Utime;
 void Init() {
  OUTPUT("RingedJovianShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec3 ringColor;     //= vec3(0.95,0.8,0.7)\n")
   +string("uniform vec3 planetColor;   //= vec3(0.9,0.75,0.6)\n")
   +string("uniform vec4 surfaceParams; //= vec4(1.2,2.0,0.05,0.6)\n")
   +string("uniform vec3 ringParams;    //= vec3(8.1,126.0,0.6)\n")
   +string("uniform vec3 sunpos;        //= vec3(50,10,20);\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform float time;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("const int steps = 25;\n")
   +string("vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec4 permute(vec4 x) { return mod289(((x*34.0)+1.0)*x); }\n")
   +string("vec4 taylorInvSqrt(vec4 r){ return 1.79284291400159 - 0.85373472095314 * r; }\n")
   +string("float snoise(vec3 v) {\n")
   +string("  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;\n")
   +string("  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);\n")
   +string("  vec3 i  = floor(v + dot(v, C.yyy) );\n")
   +string("  vec3 x0 =   v - i + dot(i, C.xxx) ;\n")
   +string("  vec3 g = step(x0.yzx, x0.xyz);\n")
   +string("  vec3 l = 1.0 - g;\n")
   +string("  vec3 i1 = min( g.xyz, l.zxy );\n")
   +string("  vec3 i2 = max( g.xyz, l.zxy );\n")
   +string("  vec3 x1 = x0 - i1 + C.xxx;\n")
   +string("  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y\n")
   +string("  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y\n")
   +string("  i = mod289(i);\n")
   +string("  vec4 p = permute( permute( permute(\n")
   +string("             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))\n")
   +string("           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))\n")
   +string("           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));\n")
   +string("  float n_ = 0.142857142857; // 1.0/7.0\n")
   +string("  vec3  ns = n_ * D.wyz - D.xzx;\n")
   +string("  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)\n")
   +string("  vec4 x_ = floor(j * ns.z);\n")
   +string("  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)\n")
   +string("  vec4 x = x_ *ns.x + ns.yyyy;\n")
   +string("  vec4 y = y_ *ns.x + ns.yyyy;\n")
   +string("  vec4 h = 1.0 - abs(x) - abs(y);\n")
   +string("  vec4 b0 = vec4( x.xy, y.xy );\n")
   +string("  vec4 b1 = vec4( x.zw, y.zw );\n")
   +string("  vec4 s0 = floor(b0)*2.0 + 1.0;\n")
   +string("  vec4 s1 = floor(b1)*2.0 + 1.0;\n")
   +string("  vec4 sh = -step(h, vec4(0.0));\n")
   +string("  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;\n")
   +string("  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;\n")
   +string("  vec3 p0 = vec3(a0.xy,h.x);\n")
   +string("  vec3 p1 = vec3(a0.zw,h.y);\n")
   +string("  vec3 p2 = vec3(a1.xy,h.z);\n")
   +string("  vec3 p3 = vec3(a1.zw,h.w);\n")
   +string("  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));\n")
   +string("  p0 *= norm.x;\n")
   +string("  p1 *= norm.y;\n")
   +string("  p2 *= norm.z;\n")
   +string("  p3 *= norm.w;\n")
   +string("  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);\n")
   +string("  m = m * m;\n")
   +string("  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),\n")
   +string("                                dot(p2,x2), dot(p3,x3) ) );\n")
   +string("}\n")
   +string("float torus (vec3 p, vec2 t) {\n")
   +string("  vec2 q = vec2(length(p.xz)-t.x,p.y);\n")
   +string("  return length(q)-t.y;\n")
   +string("}\n")
   +string("float sphere(vec3 p, float r) {\n")
   +string("  return length(p)-r;\n")
   +string("}\n")
   +string("mat4 scale (float x, float y, float z) {\n")
   +string("  return mat4(x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1);\n")
   +string("}\n")
   +string("mat4 invert(mat4 m) {\n")
   +string("  float\n")
   +string("      a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],\n")
   +string("      a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],\n")
   +string("      a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],\n")
   +string("      a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3],\n")
   +string("      b00 = a00 * a11 - a01 * a10,\n")
   +string("      b01 = a00 * a12 - a02 * a10,\n")
   +string("      b02 = a00 * a13 - a03 * a10,\n")
   +string("      b03 = a01 * a12 - a02 * a11,\n")
   +string("      b04 = a01 * a13 - a03 * a11,\n")
   +string("      b05 = a02 * a13 - a03 * a12,\n")
   +string("      b06 = a20 * a31 - a21 * a30,\n")
   +string("      b07 = a20 * a32 - a22 * a30,\n")
   +string("      b08 = a20 * a33 - a23 * a30,\n")
   +string("      b09 = a21 * a32 - a22 * a31,\n")
   +string("      b10 = a21 * a33 - a23 * a31,\n")
   +string("      b11 = a22 * a33 - a23 * a32,\n")
   +string("      det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;\n")
   +string("  return mat4(\n")
   +string("      a11 * b11 - a12 * b10 + a13 * b09,\n")
   +string("      a02 * b10 - a01 * b11 - a03 * b09,\n")
   +string("      a31 * b05 - a32 * b04 + a33 * b03,\n")
   +string("      a22 * b04 - a21 * b05 - a23 * b03,\n")
   +string("      a12 * b08 - a10 * b11 - a13 * b07,\n")
   +string("      a00 * b11 - a02 * b08 + a03 * b07,\n")
   +string("      a32 * b02 - a30 * b05 - a33 * b01,\n")
   +string("      a20 * b05 - a22 * b02 + a23 * b01,\n")
   +string("      a10 * b10 - a11 * b08 + a13 * b06,\n")
   +string("      a01 * b08 - a00 * b10 - a03 * b06,\n")
   +string("      a30 * b04 - a31 * b02 + a33 * b00,\n")
   +string("      a21 * b02 - a20 * b04 - a23 * b00,\n")
   +string("      a11 * b07 - a10 * b09 - a12 * b06,\n")
   +string("      a00 * b09 - a01 * b07 + a02 * b06,\n")
   +string("      a31 * b01 - a30 * b03 - a32 * b00,\n")
   +string("      a20 * b03 - a21 * b01 + a22 * b00) / det;\n")
   +string("}\n")
   +string("vec2 model(vec3 p) {\n")
   +string("  mat4 m = invert(scale(1.8,0.3,1.8));\n")
   +string("  float ring = torus(vec3(m*vec4(p,1)),vec2(4.5,0.5))\n")
   +string("    + snoise(p*8.0)*0.05;\n")
   +string("  float planet = sphere(p,4.0);\n")
   +string("  float sun = sphere(p-sunpos,2.0);\n")
   +string("  return vec2(min(ring,planet));//min(planet,sun)));\n")
   +string("}\n")
   +string("vec3 calcNormal(vec3 pos) {\n")
   +string("  const float eps = 0.002;\n")
   +string("  const vec3 v1 = vec3( 1.0,-1.0,-1.0);\n")
   +string("  const vec3 v2 = vec3(-1.0,-1.0, 1.0);\n")
   +string("  const vec3 v3 = vec3(-1.0, 1.0,-1.0);\n")
   +string("  const vec3 v4 = vec3( 1.0, 1.0, 1.0);\n")
   +string("  return normalize(v1*model(pos + v1*eps).x\n")
   +string("    + v2*model(pos + v2*eps).x\n")
   +string("    + v3*model(pos + v3*eps).x\n")
   +string("    + v4*model(pos + v4*eps).x);\n")
   +string("}\n")
   +string("vec2 calcRayIntersection(vec3 rayOrigin, vec3 rayDir, float maxd, float precis) {\n")
   +string("  float latest = precis * 2.0;\n")
   +string("  float dist   = +0.0;\n")
   +string("  float type   = -1.0;\n")
   +string("  vec2  res    = vec2(-1.0, -1.0);\n")
   +string("  for (int i = 0; i < steps; i++) {\n")
   +string("    if (latest < precis || dist > maxd) break;\n")
   +string("    vec2 result = model(rayOrigin + rayDir * dist);\n")
   +string("    latest = result.x;\n")
   +string("    type = result.y;\n")
   +string("    dist += latest;\n")
   +string("  }\n")
   +string("  if (dist < maxd) res = vec2(dist, type);\n")
   +string("  return res;\n")
   +string("}\n")
   +string("vec2 raytrace(vec3 rayOrigin, vec3 rayDir) {\n")
   +string("  return calcRayIntersection(rayOrigin, rayDir, 50.0, 0.001);\n")
   +string("}\n")
   +string("vec2 square(vec2 screenSize) {\n")
   +string("  vec2 position = 2.0 * (v_vTexcoord.xy / screenSize.xy) - 1.0;\n")
   +string("  position.x *= screenSize.x / screenSize.y;\n")
   +string("  return position;\n")
   +string("}\n")
   +string("vec3 camera(mat3 camMat, vec2 screenPos, float lensLength) {\n")
   +string("  return normalize(camMat * vec3(screenPos, lensLength));\n")
   +string("}\n")
   +string("mat3 lookAt(vec3 origin, vec3 target, float roll) {\n")
   +string("  vec3 rr = vec3(sin(roll), cos(roll), 0.0);\n")
   +string("  vec3 ww = normalize(target - origin);\n")
   +string("  vec3 uu = normalize(cross(ww, rr));\n")
   +string("  vec3 vv = normalize(cross(uu, ww));\n")
   +string("  return mat3(uu, vv, ww);\n")
   +string("}\n")
   +string("vec3 camera(vec3 origin, vec3 target, vec2 screenPos, float lensLength) {\n")
   +string("  mat3 camMat = lookAt(origin, target, 0.0);\n")
   +string("  return camera(camMat, screenPos, lensLength);\n")
   +string("}\n")
   +string("vec3 lighting (vec3 pos, vec3 nor, vec3 rd, float dis, vec3 mal) {\n")
   +string("  vec3 lin = vec3(0.0);\n")
   +string("  vec3  lig = normalize(vec3(1.0,0.7,0.9));\n")
   +string("  float dif = max(dot(nor,lig),0.0);\n")
   +string("  lin += dif*vec3(2);\n")
   +string("  lin += vec3(0.05);\n")
   +string("  return mal*lin;\n")
   +string("}\n")
   +string("void main() {\n")
   +string("  float t = time;\n")
   +string("  vec2 uv = v_vTexcoord.xy / resolution.xy;\n")
   +string("  float cameraAngle = 0.01 * time;\n")
   +string("  vec3 rayOrigin = vec3(24.0 * sin(cameraAngle), 2.2, 24.0 * cos(cameraAngle));\n")
   +string("  vec3 rayTarget = vec3(0, 0, 0);\n")
   +string("  vec2 screenPos = square(resolution.xy);\n")
   +string("  float lensLength = 2.0;\n")
   +string("  vec3 rayDirection = camera(rayOrigin, rayTarget, screenPos, lensLength);\n")
   +string("  vec2 collision = raytrace(rayOrigin, rayDirection);\n")
   +string("  if (collision.x > -0.5) {\n")
   +string("    vec3 pos = rayOrigin + rayDirection * collision.x;\n")
   +string("    if (length(pos) <= 6.0) { // planet\n")
   +string("      vec3 nor = calcNormal(pos);\n")
   +string("      vec3 mat = (vec3(planetColor.x,planetColor.y,planetColor.z+sin(pos.y*2.0+3.0)*0.05))*vec3(\n")
   +string("        sin(pos.y*surfaceParams.y*sin(pos.y*2.5)+sin(pos.y*2.0)+snoise(pos*3.0)*0.4)*surfaceParams.z+surfaceParams.w\n")
   +string("      );\n")
   +string("      vec3 col = lighting(pos, nor, rayDirection, collision.x, mat);\n")
   +string("      col = pow(clamp(col,0.0,1.0), vec3(surfaceParams.x));\n")
   +string("     gl_FragColor = vec4(col, 1.0);\n")
   +string("    } else { // ring\n")
   +string("      float edge = length(pos)+snoise(pos*8.0)*0.03;\n")
   +string("      vec3 col = ringColor*vec3(\n")
   +string("        sin(1.5+sqrt(length(pos)*4.0)*ringParams.y+sqrt(sin(length(pos)*ringParams.z))\n")
   +string("          +snoise(pos*12.0)*0.3)*0.2+ringParams.x)\n")
   +string("        * 1.0 //(abs(edge-8.6)<0.5?1.5:1.0)\n")
   +string("      ;\n")
   +string("      if (length(pos)>=10.0) col = vec3(0,0,0);\n")
   +string("      else if (abs(pos.y)>0.35) col = vec3(0,0,0);\n")
   +string("      else { // shadow\n")
   +string("        vec3 p = normalize(-sunpos)*7.5;\n")
   +string("        col *= min(1.0,(length(pos-p)-4.0)/4.0);\n")
   +string("      }\n")
   +string("     gl_FragColor = vec4(col,1.0);\n")
   +string("    }\n")
   +string("  } else {\n")
   +string("   gl_FragColor = vec4(0.0,0.0,0.0,0.0);\n")
   +string("  }\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  UringColor=glGetUniformLocation(program, "ringColor");
  UplanetColor=glGetUniformLocation(program, "planetColor");
  UsurfaceParams=glGetUniformLocation(program, "surfaceParams");
  UringParams=glGetUniformLocation(program, "ringParams");
  Usunpos=glGetUniformLocation(program, "sunpos");
  Uresolution=glGetUniformLocation(program, "resolution");
  Utime=glGetUniformLocation(program, "time");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform4f(UsurfaceParams,(GLfloat)surfaceParams.x,(GLfloat)surfaceParams.y,(GLfloat)surfaceParams.z,(GLfloat)surfaceParams.w);
  glUniform3f(UringParams,(GLfloat)ringParams.x,(GLfloat)ringParams.y,(GLfloat)ringParams.z);
  glUniform3f(Usunpos,(GLfloat)sunpos.x,(GLfloat)sunpos.y,(GLfloat)sunpos.z);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform3f(UringColor,(GLfloat)ringColor.rf,(GLfloat)ringColor.gf,(GLfloat)ringColor.bf);
  glUniform3f(UplanetColor,(GLfloat)planetColor.rf,(GLfloat)planetColor.gf,(GLfloat)planetColor.bf);
  glUniform1f(Utime,(GLfloat)time);
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( RingedJovianProfile *in ) {
  ringColor.fromCrayon(in->ringColor);
  planetColor.fromCrayon(in->planetColor);
  surfaceParams.Set(&in->surfaceParams);
  ringParams.Set(&in->ringParams);
  sunpos.Set(&in->sunpos);
  resolution.Set(&in->resolution);
  time=in->time;
 }
};

extern RingedJovianShader ringedjovianShader;

void ShadedRingedJovian( FBOColor *out, RingedJovianProfile *in );


// ----------------------------------------------------
ONE(ElectroPlanetProfile,{
 surfaceTex=library.Load("data/images/textures/moldy.png",tllto_Tiling);
 sampleTex=library.Load("data/images/textures/noise1.png",tllto_Tiling);
 position.Set(0.5,0.5);
 resolution.Set(1.0,1.0);
 oColor.Double( 0.2, 0.25, 0.3, 1.0 );
 oRColor.Double( 0.6, 0.75, 0.6, 1.0 );
 glowAmount=2.2;
})
 Crayon oColor,oRColor;
 Zp<GLImage> surfaceTex,sampleTex;
 Cartesiand resolution,position;
 Zdouble time,glowAmount;
 BINARY_IN({
  B_IN_TINT(oColor)
  B_IN_TINT(oRColor)
  B_IN_TILE(surfaceTex)
  B_IN_TILE(sampleTex)
  B_IN_SSUB(resolution)
  B_IN_SSUB(position)
  B_IN_NUM(time)
  B_IN_NUM(glowAmount)
 })
 BINARY_OUT({
  B_OUT_TINT(oColor)
  B_OUT_TINT(oRColor)
  B_OUT_TILE(surfaceTex)
  B_OUT_TILE(sampleTex)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(position)
  B_OUT_NUM(time)
  B_OUT_NUM(glowAmount)
 })
 KEYWORDS({
  TINTWORD("oColor",oColor)
  else TINTWORD("oRColor",oRColor)
  else TILEWORD("surfaceTex",surfaceTex)
  else TILEWORD("sampleTex",sampleTex)
  else SUBWORD("resolution",resolution)
  else SUBWORD("position",position)
  else KEYWORD("time",{time=w;})
  else KEYWORD("glowAmount",{glowAmount=w;})
 })
 WORDKEYS({
  TINTKEY("oColor",oColor)
  TINTKEY("oRColor",oRColor)
  IMGKEY("surfaceTex",surfaceTex)
  IMGKEY("sampleTex",sampleTex)
  SSUBKEY("resolution",resolution)
  SSUBKEY("position",position)
  NUMBKEY("time",time)
  NUMBKEY("glowAmount",glowAmount)
 })
MANY(ElectroPlanetProfile,ElectroPlanetProfileHandle,ElectroPlanetProfileHandles,"ElectroPlanet",ElectroPlanetProfiles,{})
 KEYWORDSGroup("ElectroPlanet",ElectroPlanetProfile);
 WORDKEYSGroup("ElectroPlanet",ElectroPlanetProfile);
DONE(ElectroPlanetProfile);

class ElectroPlanetShader : public GLSLShader {
public:
  Crayon oColor,oRColor;
  Zp<GLuint> surfaceTex,sampleTex;
  Cartesiand resolution,position;
  Zdouble time,glowAmount;
 GLuint UoColor,UoRColor,UsurfaceTex,UsampleTex,Uresolution,Uposition,Utime,UglowAmount;
 void Init() {
  OUTPUT("ElectroPlanetShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec3 oColor;			   // = vec3( 0.2, 0.25, 0.3 );\n")
   +string("uniform vec3 oRColor;		   // = vec3( 0.6, 0.75, 0.6 );\n")
   +string("uniform float glowAmount;   // = 2.20;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec2 position;\n")
   +string("uniform float time;\n")
   +string("uniform sampler2D surfaceTex;\n")
   +string("uniform sampler2D sampleTex;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("float t10 = time * 0.1;\n")
   +string("float freqs1,freqs2;\n")
   +string("float snoise(vec3 uv, float res) {\n")
   +string("	const vec3 s = vec3(1e0, 1e2, 1e4);	\n")
   +string("	uv *= res;	\n")
   +string("	vec3 uv0 = floor(mod(uv, res))*s;\n")
   +string("	vec3 uv1 = floor(mod(uv+vec3(1.), res))*s;	\n")
   +string("	vec3 f = fract(uv);\n")
   +string("	f = f*f*(3.0-2.0*f);	\n")
   +string("	vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,\n")
   +string("		      	  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);	\n")
   +string("	vec4 r = fract(sin(v*1e-3)*1e5);\n")
   +string("	float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);	\n")
   +string("	r = fract(sin((v + uv1.z - uv0.z)*1e-3)*1e5);\n")
   +string("	float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);	\n")
   +string("	return mix(r0, r1, f.z)*2.0-1.0;\n")
   +string("}\n")
   +string("void main() {\n")
   +string("//	freqs0 = texture2D( surfaceTex, vec2( 0.01, 0.25 ) ).x;\n")
   +string("	freqs1 = texture2D( surfaceTex, vec2( 0.07, 0.25 ) ).x;\n")
   +string("	freqs2 = texture2D( surfaceTex, vec2( 0.15, 0.25 ) ).x;\n")
   +string("//	freqs3 = texture2D( surfaceTex, vec2( 0.30, 0.25 ) ).x;\n")
   +string("	float brightness	= freqs1 * 0.25 + freqs2 * 0.25;\n")
   +string("	float radius		= 0.24 + brightness * 0.2;\n")
   +string("	float invRadius 	= 1.0/radius;	\n")
   +string("	float time		= time * 0.1;\n")
   +string("	float aspect	= resolution.x/resolution.y;\n")
   +string("	vec2 uv			= v_vTexcoord.xy / resolution.xy;\n")
   +string("	vec2 p 			= -0.5 + uv;\n")
   +string("    vec2 moved      = ( -position.xy/resolution.xy + 0.5 );\n")
   +string("    p = p + moved;\n")
   +string("	p.x *= aspect;\n")
   +string("	float fade		= pow( length( 2.0 * p ), 0.5 );\n")
   +string("	float fVal1		= 1.0 - fade;\n")
   +string("	float fVal2		= 1.0 - fade;	\n")
   +string("	float angle		= atan( p.x, p.y )/6.2832;\n")
   +string("	float dist		= length(p);\n")
   +string("	vec3 coord		= vec3( angle, dist, t10 * 0.1 );	\n")
   +string("	float newTime1	= abs( snoise( coord + vec3( 0.0, -t10 * ( 0.35 + brightness * 0.001 ), t10 * 0.015 ), 15.0 ) );\n")
   +string("	float newTime2	= abs( snoise( coord + vec3( 0.0, -t10 * ( 0.15 + brightness * 0.001 ), t10 * 0.015 ), 45.0 ) );	\n")
   +string("	for( int i=1; i<=7; i++ ){\n")
   +string("		float power = pow( 2.0, float(i + 1) );\n")
   +string("		fVal1 += ( 0.5 / power ) * snoise( coord + vec3( 0.0, -t10, t10 * 0.2 ), ( power * ( 10.0 ) * ( newTime1 + 1.0 ) ) );\n")
   +string("		fVal2 += ( 0.5 / power ) * snoise( coord + vec3( 0.0, -t10, t10 * 0.2 ), ( power * ( 25.0 ) * ( newTime2 + 1.0 ) ) );\n")
   +string("	}	\n")
   +string("	float corona		= pow( fVal1 * max( 1.1 - fade, 0.0 ), 2.0 ) * 50.0;\n")
   +string("	corona				+= pow( fVal2 * max( 1.1 - fade, 0.0 ), 2.0 ) * 50.0;\n")
   +string("	corona				*= 1.2 - newTime1;\n")
   +string("	vec3 sphereNormal 	= vec3( 0.0, 0.0, 1.0 );\n")
   +string("	vec3 dir 			= vec3( 0.0 );\n")
   +string("	vec3 center			= vec3( 0.5, 0.5, 1.0 );\n")
   +string("	vec3 starSphere		= vec3( 0.0 );    \n")
   +string("	vec2 sp = (-1.536 + 2.536 * (uv + moved + vec2(radius/2.0,radius/2.0))) ;\n")
   +string("	sp.x *= aspect;\n")
   +string("	sp *= ( 2.536 - brightness );\n")
   +string("  	float r = dot(sp,sp);\n")
   +string("	float f = (1.0-sqrt(abs(1.0-r)))/(r) + brightness * 0.5;\n")
   +string("	if( dist < radius ){\n")
   +string("		corona			*= pow( dist * invRadius, 24.0 );\n")
   +string("  		vec2 newUv;\n")
   +string(" 		newUv.x = sp.x*f;\n")
   +string("  		newUv.y = sp.y*f;\n")
   +string("		newUv += vec2( t10, 0.0 );		\n")
   +string("		vec3 texSample 	= texture2D( sampleTex, newUv ).rgb;\n")
   +string("		float uOff		= ( texSample.g * brightness * 4.5 + t10 );\n")
   +string("		vec2 starUV		= newUv + vec2( uOff, 0.0 );\n")
   +string("		starSphere		= texture2D( sampleTex, starUV ).rgb;\n")
   +string("	}\n")
   +string("	float starGlow	= min( max( 1.0 - dist * ( 1.0 - brightness ) * glowAmount, 0.0 ), 1.0 );\n")
   +string("	//fragColor.rgb	= vec3( r );\n")
   +string("	vec3 col = vec3( f * ( 0.75 + brightness * 0.3 ) * oColor ) + starSphere + corona * oColor + starGlow * oRColor;\n")
   +string(" gl_FragColor = vec4(col,(col.r+col.g+col.b)/3.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  UoColor=glGetUniformLocation(program, "oColor");
  UoRColor=glGetUniformLocation(program, "oRColor");
  UsurfaceTex=glGetUniformLocation(program, "surfaceTex");
  UsampleTex=glGetUniformLocation(program, "sampleTex");
  Uresolution=glGetUniformLocation(program, "resolution");
  Uposition=glGetUniformLocation(program, "position");
  Utime=glGetUniformLocation(program, "time");
  UglowAmount=glGetUniformLocation(program, "glowAmount");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Uposition,(GLfloat)position.x,(GLfloat)position.y);
  glUniform3f(UoColor,(GLfloat)oColor.rf,(GLfloat)oColor.gf,(GLfloat)oColor.bf);
  glUniform3f(UoRColor,(GLfloat)oRColor.rf,(GLfloat)oRColor.gf,(GLfloat)oRColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UglowAmount,(GLfloat)glowAmount);
  glUniform1i(UsurfaceTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *surfaceTex);
  glUniform1i(UsampleTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *sampleTex);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( ElectroPlanetProfile *in ) {
  oColor.fromCrayon(in->oColor);
  oRColor.fromCrayon(in->oRColor);
  surfaceTex=&in->surfaceTex->texture;
  sampleTex=&in->sampleTex->texture;
  resolution.Set(&in->resolution);
  position.Set(&in->position);
  time=in->time;
  glowAmount=in->glowAmount;
 }
};

extern ElectroPlanetShader electroplanetShader;

void ShadedElectroPlanet( FBOColor *out, ElectroPlanetProfile *in );

// ----------------------------------------------------

ONE(AtmosphericPlanetProfile,{
 oceanBlueColor.Double(0.0,0.031,0.11,1.0);
 snowColor.Double(0.85/1.85,0.85/1.85,0.85/1.85,1.0);
 desertColor.Double(1.5/13.0,1.3/13.0,1.0/13.0,1.0);
 grassColor.Double(1.0/12.0,1.3/12.0,0.5/12.0,1.0);
 landRatios.Set(1.85,13.0,12.0);
 cloudColor.Double(1.0,1.0,1.0,1.0);
 lightIntensity=4.0;
 landHeight= 4.0;
 cloudComplexity=2.01;
 opacityGain=1.4;
 brightnessGain=0.7;
 haze=2.0;
 baseAtmColor.Double(0.0*1.5,0.041*1.5,0.11*1.5,1.0);
 glowIntensity=5.5;
 glowCutoff=16.0;
 position.Set(0.5,0.5);
 resolution.Set(1.0,1.0);
})
 Cartesiand resolution,position,landRatios;
 Zdouble time,lightIntensity,landHeight,cloudComplexity,opacityGain,brightnessGain,haze,glowIntensity,glowCutoff;
 Crayon oceanBlueColor,snowColor,desertColor,grassColor,cloudColor,baseAtmColor;
 BINARY_IN({
  B_IN_SSUB(resolution)
  B_IN_SSUB(position)
  B_IN_SSUB(landRatios)
  B_IN_NUM(time)
  B_IN_NUM(lightIntensity)
  B_IN_NUM(landHeight)
  B_IN_NUM(cloudComplexity)
  B_IN_NUM(opacityGain)
  B_IN_NUM(brightnessGain)
  B_IN_NUM(haze)
  B_IN_NUM(glowIntensity)
  B_IN_NUM(glowCutoff)
  B_IN_TINT(oceanBlueColor)
  B_IN_TINT(snowColor)
  B_IN_TINT(desertColor)
  B_IN_TINT(grassColor)
  B_IN_TINT(cloudColor)
  B_IN_TINT(baseAtmColor)
 })
 BINARY_OUT({
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(position)
  B_OUT_SSUB(landRatios)
  B_OUT_NUM(time)
  B_OUT_NUM(lightIntensity)
  B_OUT_NUM(landHeight)
  B_OUT_NUM(cloudComplexity)
  B_OUT_NUM(opacityGain)
  B_OUT_NUM(brightnessGain)
  B_OUT_NUM(haze)
  B_OUT_NUM(glowIntensity)
  B_OUT_NUM(glowCutoff)
  B_OUT_TINT(oceanBlueColor)
  B_OUT_TINT(snowColor)
  B_OUT_TINT(desertColor)
  B_OUT_TINT(grassColor)
  B_OUT_TINT(cloudColor)
  B_OUT_TINT(baseAtmColor)
 })
 KEYWORDS({
  SUBWORD("resolution",resolution)
  else SUBWORD("position",position)
  else SUBWORD("landRatios",landRatios)
  else KEYWORD("time",{time=w;})
  else KEYWORD("lightIntensity",{lightIntensity=w;})
  else KEYWORD("landHeight",{landHeight=w;})
  else KEYWORD("cloudComplexity",{cloudComplexity=w;})
  else KEYWORD("opacityGain",{opacityGain=w;})
  else KEYWORD("brightnessGain",{brightnessGain=w;})
  else KEYWORD("haze",{haze=w;})
  else KEYWORD("glowIntensity",{glowIntensity=w;})
  else KEYWORD("glowCutoff",{glowCutoff=w;})
  else TINTWORD("oceanBlueColor",oceanBlueColor)
  else TINTWORD("snowColor",snowColor)
  else TINTWORD("desertColor",desertColor)
  else TINTWORD("grassColor",grassColor)
  else TINTWORD("cloudColor",cloudColor)
  else TINTWORD("baseAtmColor",baseAtmColor)
 })
 WORDKEYS({
  SSUBKEY("resolution",resolution)
  SSUBKEY("position",position)
  SSUBKEY("landRatios",landRatios)
  NUMBKEY("time",time)
  NUMBKEY("lightIntensity",lightIntensity)
  NUMBKEY("landHeight",landHeight)
  NUMBKEY("cloudComplexity",cloudComplexity)
  NUMBKEY("opacityGain",opacityGain)
  NUMBKEY("brightnessGain",brightnessGain)
  NUMBKEY("haze",haze)
  NUMBKEY("glowIntensity",glowIntensity)
  NUMBKEY("glowCutoff",glowCutoff)
  TINTKEY("oceanBlueColor",oceanBlueColor)
  TINTKEY("snowColor",snowColor)
  TINTKEY("desertColor",desertColor)
  TINTKEY("grassColor",grassColor)
  TINTKEY("cloudColor",cloudColor)
  TINTKEY("baseAtmColor",baseAtmColor)
 })
MANY(AtmosphericPlanetProfile,AtmosphericPlanetProfileHandle,AtmosphericPlanetProfileHandles,"AtmosphericPlanet",AtmosphericPlanetProfiles,{})
 KEYWORDSGroup("AtmosphericPlanet",AtmosphericPlanetProfile);
 WORDKEYSGroup("AtmosphericPlanet",AtmosphericPlanetProfile);
DONE(AtmosphericPlanetProfile);

class AtmosphericPlanetShader : public GLSLShader {
public:
  Cartesiand resolution,position,landRatios;
  Zdouble time,lightIntensity,landHeight,cloudComplexity,opacityGain,brightnessGain,haze,glowIntensity,glowCutoff;
  Crayon oceanBlueColor,snowColor,desertColor,grassColor,cloudColor,baseAtmColor;
 GLuint Uresolution,Uposition,UlandRatios,Utime,UlightIntensity,UlandHeight,UcloudComplexity,UopacityGain,UbrightnessGain,Uhaze,UglowIntensity,UglowCutoff,UoceanBlueColor,UsnowColor,UdesertColor,UgrassColor,UcloudColor,UbaseAtmColor;
 void Init() {
  OUTPUT("AtmosphericPlanetShader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec2 position;\n")
   +string("uniform float time;\n")
   +string("uniform vec3 oceanBlueColor;   // = vec3(0.0,0.031,0.11); \n")
   +string("uniform vec3 snowColor;        // = vec3(0.85,0.85,0.85)/1.85;\n")
   +string("uniform vec3 desertColor;      // = vec3(1.5,1.3,1.0)/13.0;\n")
   +string("uniform vec3 grassColor;       // = vec3(1.0,1.3,0.5)/12.0;\n")
   +string("uniform vec3 landRatios;       // = 1.85,13.0,12.0\n")
   +string("uniform vec3 cloudColor;       // = vec4(1.0,1.0,1.0)\n")
   +string("uniform float lightIntensity;  // = 4.0;\n")
   +string("uniform float landHeight;      // = 4.0;\n")
   +string("uniform float cloudComplexity; //= 2.01\n")
   +string("uniform float opacityGain;     // = 1.4;\n")
   +string("uniform float brightnessGain;  // = 0.7;\n")
   +string("uniform float haze;            // = 2.0;\n")
   +string("uniform vec3 baseAtmColor;     // = vec3(0.0,0.041,0.11)*1.5;\n")
   +string("uniform float glowIntensity;   // = 5.5\n")
   +string("uniform float glowCutoff;      // = 16.0\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("const int steps = 8;\n")
   +string("const int nsteps = 6;\n")
   +string("const mat3 m = mat3( 0.00,  0.80,  0.60,      -0.80,  0.36, -0.48,      -0.60, -0.48,  0.64 );\n")
   +string("float hash( float n ) { return fract(sin(n)*753.5453123); }\n")
   +string("float noise( in vec3 x ) {\n")
   +string(" vec3 p = floor(x);\n")
   +string(" vec3 f = fract(x);\n")
   +string(" f = f*f*(3.0-2.0*f);\n")
   +string(" float n = p.x + p.y*157.0 + 113.0*p.z;\n")
   +string(" return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),\n")
   +string("                mix( hash(n+157.0), hash(n+158.0),f.x),f.y),\n")
   +string("            mix(mix( hash(n+113.0), hash(n+114.0),f.x),\n")
   +string("                mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);\n")
   +string("}\n")
   +string("float cloudForm(vec3 q) {\n")
   +string(" q.x += sin(q.y*2.0);\n")
   +string(" q.z += cos(q.x*2.0)*0.3;\n")
   +string(" float f = -0.15 +\n")
   +string("  noise(q) +\n")
   +string("  noise(q*2.0) * 0.5 + \n")
   +string("  noise(q*4.0) * 0.25 +\n")
   +string("  noise(q*8.0) * 0.125 +\n")
   +string("  noise(q*16.0)* 0.05 + \n")
   +string("  noise(q*32.0)* 0.025 +\n")
   +string("  noise(q*64.0)* 0.0125;\n")
   +string(" f = min(pow(abs(f),10.0)*0.5,0.5);\n")
   +string(" return f;\n")
   +string("}\n")
   +string("float octaves6(vec3 q) {\n")
   +string(" float f = 0.0;\n")
   +string(" float ti = pow(2.0,0.0);\n")
   +string(" f += noise(q * ti ) / ti;\n")
   +string(" ti = pow(2.0,1.0);\n")
   +string(" f += noise(q * ti ) / ti;\n")
   +string(" ti = pow(2.0,2.0);\n")
   +string(" f += noise(q * ti ) / ti;\n")
   +string(" ti = pow(2.0,3.0);\n")
   +string(" f += noise(q * ti ) / ti;\n")
   +string(" ti = pow(2.0,4.0);\n")
   +string(" f += noise(q * ti ) / ti;\n")
   +string(" ti = pow(2.0,5.0);\n")
   +string(" f += noise(q * ti ) / ti;\n")
   +string(" return f;\n")
   +string("}\n")
   +string("void main() {\n")
   +string("	float tmin = 10000.0;\n")
   +string("	vec3  nor = vec3(0.0);\n")
   +string("	float occ = 1.0;\n")
   +string("	vec3  pos = vec3(0.0);\n")
   +string("	float h = 0.0;\n")
   +string("	vec3 col = vec3(0.9,0.9,0.9);    \n")
   +string("	float f = 0.0;		\n")
   +string(" float isBckg=1.0;\n")
   +string("	float an = 0.5+0.02*time;                         // camera movement	\n")
   +string(" vec3 off = vec3(0.1);\n")
   +string(" vec3 bckg = vec3(0.0,0.0,0.0); \n")
   +string("	vec2 p = ((-position.xy/resolution.xy)+0.5) + (-resolution.xy + 2.0*v_vTexcoord.xy) / resolution.y;\n")
   +string("	vec3 ro = vec3( 2.0*cos(an), 1.0, 2.0*sin(an) );\n")
   +string(" vec3 ta = vec3( 0.0, 1.0, 0.0 );\n")
   +string(" // camera matrix\n")
   +string(" vec3 ww = normalize( ta - ro );\n")
   +string(" vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );\n")
   +string(" vec3 vv = normalize( cross(uu,ww));\n")
   +string("	// create view ray\n")
   +string("	vec3 rd = normalize( p.x*uu + p.y*vv + 1.7*ww );\n")
   +string(" // sphere center	\n")
   +string("	vec3 sc = vec3(0.0,1.0,0.0);\n")
   +string(" // raytrace\n")
   +string("	// raytrace-sphere\n")
   +string("	vec3  ce = ro - sc;\n")
   +string("	float b = dot( rd, ce );\n")
   +string("	float c = dot( ce, ce ) - 1.0;\n")
   +string("	h = b*b - c;\n")
   +string("	if( h>0.0 ) {\n")
   +string("		h = -b - sqrt(h);\n")
   +string("		if( h<tmin ) { \n")
   +string("			tmin=h; \n")
   +string("			nor = normalize(ro+h*rd-sc); \n")
   +string("			occ = 0.5 + 0.5*nor.y;\n")
   +string("		}\n")
   +string("	}\n")
   +string(" // shading/lighting	\n")
   +string("	pos = ro + tmin*rd;\n")
   +string(" float sunNDL = max(0.0,0.05+0.95*(nor.z));       \n")
   +string(" vec3 q = pos;            \n")
   +string(" // Ocean\n")
   +string(" col = oceanBlueColor;\n")
   +string(" col = col * (0.5 + 0.5*noise(q*4.0));\n")
   +string("	col = col * (0.9 + 0.2*noise(q*8.0));\n")
   +string(" col = col * (0.9 + 0.2*noise(q*16.0));\n")
   +string(" col = col * (0.9 + 0.1*noise(q*32.0));\n")
   +string(" col = col * (0.9 + 0.1*noise(q*64.0));            \n")
   +string(" // Land\n")
   +string(" q = m * q * 2.01;            \n")
   +string(" float hm = octaves6(q+vec3(137.0)) / 1.5;\n")
   +string(" hm *= hm;            \n")
   +string(" f = hm; \n")
   +string(" float landBlend = min(landHeight*max(f-0.3,0.0),1.0);            \n")
   +string(" float nG = exp(-abs(f-0.4)*20.0);//max(f - 0.4,0.0); \n")
   +string(" float nD = exp(-abs(f-0.6)*20.0);//3.0*max(f - 0.75,0.0); \n")
   +string(" float snowLevel = 0.8+ noise(q*64.0)*0.1;\n")
   +string(" float nS = exp(-abs(f-snowLevel)*20.0);            \n")
   +string(" vec3 snow = snowColor/landRatios.x;\n")
   +string(" vec3 desert = desertColor/landRatios.y;\n")
   +string(" vec3 grass = grassColor/landRatios.z;            \n")
   +string("	float normn = nG + nD + nS;            \n")
   +string(" float grassD = 1.0 + (noise(q*128.0) + noise(q*64.0) + noise(q*32.0))*0.5;            \n")
   +string(" float snowD = 1.0 + (noise(q*vec3(5.0,1.0,1.0)*3.0)  + noise(q*16.0)*0.5 + noise(q*64.0)*0.5)*0.25;            \n")
   +string(" float desertD = 1.0 + noise(q*vec3(1.0,7.0,1.0)*3.0) + noise(q*32.0)*0.5 + noise(q*64.0)*0.5;            \n")
   +string(" vec3 land = (desertD*desert*nD + grassD*grass*nG + snowD*snow*nS)/normn;\n")
   +string(" col = mix(col,land,landBlend);            \n")
   +string(" float shore = clamp(1.42*pow(1.0-abs(landBlend-0.4),1.0)-1.2,0.0,1.0);\n")
   +string(" col = mix(col, col + vec3(0.0,0.1,0.05),shore);\n")
   +string(" // Cloud shadows\n")
   +string(" q = m * q * cloudComplexity + time*0.03;\n")
   +string("	f = cloudForm(q+off);\n")
   +string(" f = 0.2 + 0.8*pow(1.0 - f*0.75,2.0);\n")
   +string(" col = col * f;            \n")
   +string("	f = cloudForm(q);  // Clouds  \n")
   +string("	float cloudAmount=min(f*opacityGain,1.0);\n")
   +string(" vec3 cloudCol =	cloudColor * ( vec3(0.2)+0.7*noise(q*2.0)+0.4*noise(q*6.0)+0.2*noise(q*16.0)+0.05*noise(q*32.0)+0.04*noise(q*64.0) ); \n")
   +string(" cloudCol = (cloudCol + f*0.05)*0.55;\n")
   +string(" col = mix(col,cloudCol*brightnessGain,cloudAmount);            \n")
   +string(" float depth = tmin * 2.7 / 2.0;                // Depth\n")
   +string("	col *= (0.005 + sunNDL * lightIntensity);      // Surface Diffuse Lighting\n")
   +string(" vec3 ref = reflect(nor,-vec3(0.0,0.0,-1.0));   // Surface Specular\n")
   +string(" float sunRDE = max(dot(ref,rd),0.0);\n")
   +string(" float spec = 0.125*pow(sunRDE,512.0);\n")
   +string(" col += spec*mix(col,vec3(1.0),0.05)*sunNDL;        \n")
   +string(" float atm = -max(-1.0,1.1 - depth);           // Inner Atmosphere\n")
   +string(" col = mix(col,baseAtmColor*sunNDL*lightIntensity,pow(atm,haze));\n")
   +string(" float glow = 0.0;                             // Atmosphere glow\n")
   +string(" const float di = (128.0/50.0)/float(steps);\n")
   +string(" for(float i = 0; i < steps; ++i)\n")
   +string(" {\n")
   +string("  vec3 px = ro + rd * (i)*di;\n")
   +string("  vec3 tosc = sc - px;\n")
   +string("  float d = length(tosc);\n")
   +string("  float x = abs(d - 1.0);\n")
   +string("  float n = 0.01+max((-tosc/d).z,0.0);\n")
   +string("  glow += glowIntensity * exp(-x*glowCutoff) * n * di;\n")
   +string(" }\n")
   +string(" vec3 glowCol = glow * baseAtmColor;        \n")
   +string(" bckg = bckg + glowCol; // Background\n")
   +string(" isBckg = 1.0-exp( -0.00001*tmin*tmin );\n")
   +string("	col = mix( col,bckg, isBckg );\n")
   +string("	col = sqrt( col);	\n")
   +string("	//col *= smoothstep( 0.006, 0.008, abs(p.x) );	\n")
   +string("	gl_FragColor = vec4( col, 1.0-isBckg );\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uresolution=glGetUniformLocation(program, "resolution");
  Uposition=glGetUniformLocation(program, "position");
  UlandRatios=glGetUniformLocation(program, "landRatios");
  Utime=glGetUniformLocation(program, "time");
  UlightIntensity=glGetUniformLocation(program, "lightIntensity");
  UlandHeight=glGetUniformLocation(program, "landHeight");
  UcloudComplexity=glGetUniformLocation(program, "cloudComplexity");
  UopacityGain=glGetUniformLocation(program, "opacityGain");
  UbrightnessGain=glGetUniformLocation(program, "brightnessGain");
  Uhaze=glGetUniformLocation(program, "haze");
  UglowIntensity=glGetUniformLocation(program, "glowIntensity");
  UglowCutoff=glGetUniformLocation(program, "glowCutoff");
  UoceanBlueColor=glGetUniformLocation(program, "oceanBlueColor");
  UsnowColor=glGetUniformLocation(program, "snowColor");
  UdesertColor=glGetUniformLocation(program, "desertColor");
  UgrassColor=glGetUniformLocation(program, "grassColor");
  UcloudColor=glGetUniformLocation(program, "cloudColor");
  UbaseAtmColor=glGetUniformLocation(program, "baseAtmColor");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Uposition,(GLfloat)position.x,(GLfloat)position.y);
  glUniform3f(UlandRatios,(GLfloat)landRatios.x,(GLfloat)landRatios.y,(GLfloat)landRatios.z);
  glUniform3f(UoceanBlueColor,(GLfloat)oceanBlueColor.rf,(GLfloat)oceanBlueColor.gf,(GLfloat)oceanBlueColor.bf);
  glUniform3f(UsnowColor,(GLfloat)snowColor.rf,(GLfloat)snowColor.gf,(GLfloat)snowColor.bf);
  glUniform3f(UdesertColor,(GLfloat)desertColor.rf,(GLfloat)desertColor.gf,(GLfloat)desertColor.bf);
  glUniform3f(UgrassColor,(GLfloat)grassColor.rf,(GLfloat)grassColor.gf,(GLfloat)grassColor.bf);
  glUniform3f(UcloudColor,(GLfloat)cloudColor.rf,(GLfloat)cloudColor.gf,(GLfloat)cloudColor.bf);
  glUniform3f(UbaseAtmColor,(GLfloat)baseAtmColor.rf,(GLfloat)baseAtmColor.gf,(GLfloat)baseAtmColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UlightIntensity,(GLfloat)lightIntensity);
  glUniform1f(UlandHeight,(GLfloat)landHeight);
  glUniform1f(UcloudComplexity,(GLfloat)cloudComplexity);
  glUniform1f(UopacityGain,(GLfloat)opacityGain);
  glUniform1f(UbrightnessGain,(GLfloat)brightnessGain);
  glUniform1f(Uhaze,(GLfloat)haze);
  glUniform1f(UglowIntensity,(GLfloat)glowIntensity);
  glUniform1f(UglowCutoff,(GLfloat)glowCutoff);
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( AtmosphericPlanetProfile *in ) {
  resolution.Set(&in->resolution);
  position.Set(&in->position);
  landRatios.Set(&in->landRatios);
  time=in->time;
  lightIntensity=in->lightIntensity;
  landHeight=in->landHeight;
  cloudComplexity=in->cloudComplexity;
  opacityGain=in->opacityGain;
  brightnessGain=in->brightnessGain;
  haze=in->haze;
  glowIntensity=in->glowIntensity;
  glowCutoff=in->glowCutoff;
  oceanBlueColor.fromCrayon(in->oceanBlueColor);
  snowColor.fromCrayon(in->snowColor);
  desertColor.fromCrayon(in->desertColor);
  grassColor.fromCrayon(in->grassColor);
  cloudColor.fromCrayon(in->cloudColor);
  baseAtmColor.fromCrayon(in->baseAtmColor);
 }
};

extern AtmosphericPlanetShader atmosphericplanetShader;

void ShadedAtmosphericPlanet( FBOColor *out, AtmosphericPlanetProfile *in );

// ----------------------------------------------------------

ONE(Earthlike2Profile,{
 planetParams.Set(0.3,2.5,0.55); planetParams.w=0.45;
 planetParams2.Set(0.5,15.5,0.4); planetParams2.w=0.45;
 planetParams3.Set(0.5,0.75,0.55); planetParams3.w=0.8;
 surfaceThresholdColor.Double(0.2,0.9,0.5,1.0);
 secondaryMaterialColor.Double(0.3,0.3,0.3,1.0);
 specular=0.03;
 specularPos.Set(0.9,0.3,1.0);
 ambientColor.Double(0.1,0.1,0.1,1.0);
 atmosphereDiffuseColor.Double(0.4,0.0,1.0,1.0);
 atmosphereSpecularColor.Double(0.7,0.9,1.0,1.0);
 darkDiffuseColor.Double(0.2,0.1,0.05,1.0);
 darkSpecularColor.Double(0.2,0.1,0.05,1.0);
 oceanColor.Double(0.2,0.3,0.4,1.0);
 diffusion=0.75;
 zoom=2.5;
 cloudset=2.0;
 landset=0.5;
 tex=library.Load("data/images/textures/rubble.png",tllto_Tiling);
 tex2=library.Load("data/images/textures/noise8.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
})
 Zp<GLImage> tex,tex2;
 Zdouble time,specular,diffusion,zoom,cloudset,landset;
 Cartesiand resolution,rotation,planetParams,planetParams2,planetParams3,specularPos;
 Crayon surfaceThresholdColor,secondaryMaterialColor,ambientColor,atmosphereDiffuseColor,atmosphereSpecularColor,darkDiffuseColor,darkSpecularColor,oceanColor;
 BINARY_IN({
  B_IN_TILE(tex)
  B_IN_TILE(tex2)
  B_IN_NUM(time)
  B_IN_NUM(specular)
  B_IN_NUM(diffusion)
  B_IN_NUM(zoom)
  B_IN_NUM(cloudset)
  B_IN_NUM(landset)
  B_IN_SSUB(resolution)
  B_IN_SSUB(rotation)
  B_IN_SSUB(planetParams)
  B_IN_SSUB(planetParams2)
  B_IN_SSUB(planetParams3)
  B_IN_SSUB(specularPos)
  B_IN_TINT(surfaceThresholdColor)
  B_IN_TINT(secondaryMaterialColor)
  B_IN_TINT(ambientColor)
  B_IN_TINT(atmosphereDiffuseColor)
  B_IN_TINT(atmosphereSpecularColor)
  B_IN_TINT(darkDiffuseColor)
  B_IN_TINT(darkSpecularColor)
  B_IN_TINT(oceanColor)
 })
 BINARY_OUT({
  B_OUT_TILE(tex)
  B_OUT_TILE(tex2)
  B_OUT_NUM(time)
  B_OUT_NUM(specular)
  B_OUT_NUM(diffusion)
  B_OUT_NUM(zoom)
  B_OUT_NUM(cloudset)
  B_OUT_NUM(landset)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(rotation)
  B_OUT_SSUB(planetParams)
  B_OUT_SSUB(planetParams2)
  B_OUT_SSUB(planetParams3)
  B_OUT_SSUB(specularPos)
  B_OUT_TINT(surfaceThresholdColor)
  B_OUT_TINT(secondaryMaterialColor)
  B_OUT_TINT(ambientColor)
  B_OUT_TINT(atmosphereDiffuseColor)
  B_OUT_TINT(atmosphereSpecularColor)
  B_OUT_TINT(darkDiffuseColor)
  B_OUT_TINT(darkSpecularColor)
  B_OUT_TINT(oceanColor)
 })
 KEYWORDS({
  TILEWORD("tex",tex)
  else TILEWORD("tex2",tex2)
  else KEYWORD("time",{time=w;})
  else KEYWORD("specular",{specular=w;})
  else KEYWORD("diffusion",{diffusion=w;})
  else KEYWORD("zoom",{zoom=w;})
  else KEYWORD("cloudset",{cloudset=w;})
  else KEYWORD("landset",{landset=w;})
  else SUBWORD("resolution",resolution)
  else SUBWORD("rotation",rotation)
  else SUBWORD("planetParams",planetParams)
  else SUBWORD("planetParams2",planetParams2)
  else SUBWORD("planetParams3",planetParams3)
  else SUBWORD("specularPos",specularPos)
  else TINTWORD("surfaceThresholdColor",surfaceThresholdColor)
  else TINTWORD("secondaryMaterialColor",secondaryMaterialColor)
  else TINTWORD("ambientColor",ambientColor)
  else TINTWORD("atmosphereDiffuseColor",atmosphereDiffuseColor)
  else TINTWORD("atmosphereSpecularColor",atmosphereSpecularColor)
  else TINTWORD("darkDiffuseColor",darkDiffuseColor)
  else TINTWORD("darkSpecularColor",darkSpecularColor)
  else TINTWORD("oceanColor",oceanColor)
 })
 WORDKEYS({
  IMGKEY("tex",tex)
  IMGKEY("tex2",tex2)
  NUMBKEY("time",time)
  NUMBKEY("specular",specular)
  NUMBKEY("diffusion",diffusion)
  NUMBKEY("zoom",zoom)
  NUMBKEY("cloudset",cloudset)
  NUMBKEY("landset",landset)
  SSUBKEY("resolution",resolution)
  SSUBKEY("rotation",rotation)
  SSUBKEY("planetParams",planetParams)
  SSUBKEY("planetParams2",planetParams2)
  SSUBKEY("planetParams3",planetParams3)
  SSUBKEY("specularPos",specularPos)
  TINTKEY("surfaceThresholdColor",surfaceThresholdColor)
  TINTKEY("secondaryMaterialColor",secondaryMaterialColor)
  TINTKEY("ambientColor",ambientColor)
  TINTKEY("atmosphereDiffuseColor",atmosphereDiffuseColor)
  TINTKEY("atmosphereSpecularColor",atmosphereSpecularColor)
  TINTKEY("darkDiffuseColor",darkDiffuseColor)
  TINTKEY("darkSpecularColor",darkSpecularColor)
  TINTKEY("oceanColor",oceanColor)
 })
MANY(Earthlike2Profile,Earthlike2ProfileHandle,Earthlike2ProfileHandles,"Earthlike2",Earthlike2Profiles,{})
 KEYWORDSGroup("Earthlike2",Earthlike2Profile);
 WORDKEYSGroup("Earthlike2",Earthlike2Profile);
 BINARY_INOUT(Earthlike2Profile)
DONE(Earthlike2Profile);

class Earthlike2Shader : public GLSLShader {
public:
 Zp<GLuint> tex,tex2;
 Zdouble time,specular,diffusion,zoom,cloudset,landset;
 Cartesiand resolution,rotation,planetParams,planetParams2,planetParams3,specularPos;
 Crayon surfaceThresholdColor,secondaryMaterialColor,ambientColor,atmosphereDiffuseColor,atmosphereSpecularColor,darkDiffuseColor,darkSpecularColor,oceanColor;
 GLuint Utex,Utex2,Utime,Uspecular,Udiffusion,Uzoom,Ucloudset,Ulandset,Uresolution,Urotation,UplanetParams,UplanetParams2,UplanetParams3,UspecularPos,UsurfaceThresholdColor,UsecondaryMaterialColor,UambientColor,UatmosphereDiffuseColor,UatmosphereSpecularColor,UdarkDiffuseColor,UdarkSpecularColor,UoceanColor;
 void Init() {
  OUTPUT("Earthlike2Shader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform sampler2D tex;\n")
   +string("uniform sampler2D tex2;\n")
   +string("uniform float time;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec2 rotation;\n")
   +string("uniform vec4 planetParams;\n")
   +string("uniform vec4 planetParams2;\n")
   +string("uniform vec4 planetParams3;\n")
   +string("uniform vec3 surfaceThresholdColor;   // .2, .9, .5\n")
   +string("uniform vec3 secondaryMaterialColor;  // 0.3, 0.3, 0.3\n")
   +string("uniform float specular;               //=0.03\n")
   +string("uniform vec3 specularPos;             //=vec3(0.9,0.3,1.0)\n")
   +string("uniform vec3 ambientColor;            //=vec3(0.1,0.1,0.1);\n")
   +string("uniform vec3 atmosphereDiffuseColor;  //=vec3(0.4,0.0,1.0)\n")
   +string("uniform vec3 atmosphereSpecularColor; //=vec3(0.7,0.9,1.0)\n")
   +string("uniform vec3 darkDiffuseColor;        //=vec3(0.20,0.10,0.05)\n")
   +string("uniform vec3 darkSpecularColor;       //=vec3(0.20,0.10,0.05)\n")
   +string("uniform vec3 oceanColor;              //=vec3(0.2,0.3,0.4);\n")
   +string("uniform float diffusion;              //=0.75\n")
   +string("uniform float zoom;                   //=2.5\n")
   +string("uniform float cloudset;               //=2.0\n")
   +string("uniform float landset;                //=0.5\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("vec4 doit( vec2 pix ) {\n")
   +string(" vec2 p = -1.0 + 2.0*pix;\n")
   +string(" p.x *= resolution.x/resolution.y;\n")
   +string(" vec3 ro = vec3( 0.0, 0.0, zoom );\n")
   +string(" vec3 rd = normalize( vec3( p, -2.0 ) );\n")
   +string(" vec3 col = ambientColor;\n")
   +string(" // intersect sphere\n")
   +string(" float b = dot(ro,rd);\n")
   +string(" float c = dot(ro,ro) - 1.0;\n")
   +string(" float h = b*b - c;\n")
   +string(" if( h>0.0 ) {\n")
   +string("     float t = -b - sqrt(h);\n")
   +string("     vec3 pos = ro + t*rd;\n")
   +string("     vec3 nor = pos;\n")
   +string("     vec2 uv;\n")
   +string("     uv.x = atan(nor.x,nor.z)/6.2831 - 0.03*time - rotation.x/resolution.x;\n")
   +string("     uv.y = acos(nor.y)/3.1416;\n")
   +string(" 	   uv.y *= 0.5;\n")
   +string("     col = oceanColor;\n")
   +string("     vec3 te  = texture2D( tex2, landset*uv.yx ).xyz;\n")
   +string("          te += planetParams.x*texture2D( tex2, planetParams.y*uv.yx ).xyz;\n")
   +string(" 	   col = mix( col, (surfaceThresholdColor*planetParams.z + planetParams.w*te + planetParams2.x*texture2D( tex, planetParams2.y*uv.yx ).xyz)*planetParams2.z, smoothstep( planetParams2.w,planetParams3.x,te.x) );\n")
   +string("     vec3 cl = texture2D( tex, cloudset*uv ).xxx;\n")
   +string(" 	   col = mix( col, secondaryMaterialColor, planetParams3.z*smoothstep( planetParams3.y,planetParams3.w,cl.x) );	\n")
   +string("     float dif = max(nor.x*2.0+nor.z,0.0);\n")
   +string("     float fre = 1.0-clamp(nor.z,0.0,1.0);\n")
   +string("     float spe = clamp( dot( nor,normalize(specularPos) ), 0.0, 1.0 );\n")
   +string("     col *= specular + diffusion*dif;\n")
   +string("     col += pow(spe,64.0)*(1.0-te.x);\n")
   +string("     col += mix( darkDiffuseColor, atmosphereDiffuseColor, dif )*0.3*fre;\n")
   +string("     col += mix( darkSpecularColor, atmosphereSpecularColor, dif )*7.0*fre*fre*fre*fre;\n")
   +string(" } else return vec4(0.0,0.0,0.0,0.0);\n")
   +string(" col = vec3(0.5)*(col+sqrt(col));\n")
   +string(" return vec4(col,1.0);\n")
   +string("}\n")
   +string("void main() { \n")
   +string("    // render this with four sampels per pixel\n")
   +string("    vec4 col0 = doit( (v_vTexcoord.xy+vec2(0.0,0.0) )/resolution.xy );\n")
   +string("    vec4 col1 = doit( (v_vTexcoord.xy+vec2(0.0,0.0) )/resolution.xy );\n")
   +string("    vec4 col2 = doit( (v_vTexcoord.xy+vec2(0.0,0.0) )/resolution.xy );\n")
   +string("    vec4 col3 = doit( (v_vTexcoord.xy+vec2(0.0,0.0) )/resolution.xy );\n")
   +string("    vec4 col = vec4(0.25)*(col0 + col1 + col2 + col3);\n")
   +string("    gl_FragColor = col;\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex=glGetUniformLocation(program, "tex");
  Utex2=glGetUniformLocation(program, "tex2");
  Utime=glGetUniformLocation(program, "time");
  Uspecular=glGetUniformLocation(program, "specular");
  Udiffusion=glGetUniformLocation(program, "diffusion");
  Uzoom=glGetUniformLocation(program, "zoom");
  Ucloudset=glGetUniformLocation(program, "cloudset");
  Ulandset=glGetUniformLocation(program, "landset");
  Uresolution=glGetUniformLocation(program, "resolution");
  Urotation=glGetUniformLocation(program, "rotation");
  UplanetParams=glGetUniformLocation(program, "planetParams");
  UplanetParams2=glGetUniformLocation(program, "planetParams2");
  UplanetParams3=glGetUniformLocation(program, "planetParams3");
  UspecularPos=glGetUniformLocation(program, "specularPos");
  UsurfaceThresholdColor=glGetUniformLocation(program, "surfaceThresholdColor");
  UsecondaryMaterialColor=glGetUniformLocation(program, "secondaryMaterialColor");
  UambientColor=glGetUniformLocation(program, "ambientColor");
  UatmosphereDiffuseColor=glGetUniformLocation(program, "atmosphereDiffuseColor");
  UatmosphereSpecularColor=glGetUniformLocation(program, "atmosphereSpecularColor");
  UdarkDiffuseColor=glGetUniformLocation(program, "darkDiffuseColor");
  UdarkSpecularColor=glGetUniformLocation(program, "darkSpecularColor");
  UoceanColor=glGetUniformLocation(program, "oceanColor");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Urotation,(GLfloat)rotation.x,(GLfloat)rotation.y);
  glUniform4f(UplanetParams,(GLfloat)planetParams.x,(GLfloat)planetParams.y,(GLfloat)planetParams.z,(GLfloat)planetParams.w);
  glUniform4f(UplanetParams2,(GLfloat)planetParams2.x,(GLfloat)planetParams2.y,(GLfloat)planetParams2.z,(GLfloat)planetParams2.w);
  glUniform4f(UplanetParams3,(GLfloat)planetParams3.x,(GLfloat)planetParams3.y,(GLfloat)planetParams3.z,(GLfloat)planetParams3.w);
  glUniform3f(UspecularPos,(GLfloat)specularPos.x,(GLfloat)specularPos.y,(GLfloat)specularPos.z);
  glUniform3f(UsurfaceThresholdColor,(GLfloat)surfaceThresholdColor.rf,(GLfloat)surfaceThresholdColor.gf,(GLfloat)surfaceThresholdColor.bf);
  glUniform3f(UsecondaryMaterialColor,(GLfloat)secondaryMaterialColor.rf,(GLfloat)secondaryMaterialColor.gf,(GLfloat)secondaryMaterialColor.bf);
  glUniform3f(UambientColor,(GLfloat)ambientColor.rf,(GLfloat)ambientColor.gf,(GLfloat)ambientColor.bf);
  glUniform3f(UatmosphereDiffuseColor,(GLfloat)atmosphereDiffuseColor.rf,(GLfloat)atmosphereDiffuseColor.gf,(GLfloat)atmosphereDiffuseColor.bf);
  glUniform3f(UatmosphereSpecularColor,(GLfloat)atmosphereSpecularColor.rf,(GLfloat)atmosphereSpecularColor.gf,(GLfloat)atmosphereSpecularColor.bf);
  glUniform3f(UdarkDiffuseColor,(GLfloat)darkDiffuseColor.rf,(GLfloat)darkDiffuseColor.gf,(GLfloat)darkDiffuseColor.bf);
  glUniform3f(UdarkSpecularColor,(GLfloat)darkSpecularColor.rf,(GLfloat)darkSpecularColor.gf,(GLfloat)darkSpecularColor.bf);
  glUniform3f(UoceanColor,(GLfloat)oceanColor.rf,(GLfloat)oceanColor.gf,(GLfloat)oceanColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Uspecular,(GLfloat)specular);
  glUniform1f(Udiffusion,(GLfloat)diffusion);
  glUniform1f(Uzoom,(GLfloat)zoom);
  glUniform1f(Ucloudset,(GLfloat)cloudset);
  glUniform1f(Ulandset,(GLfloat)landset);
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex);
  glUniform1i(Utex2, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex2);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( Earthlike2Profile *in ) {
  tex=&in->tex->texture;
  tex2=&in->tex2->texture;
  time=in->time;
  specular=in->specular;
  diffusion=in->diffusion;
  zoom=in->zoom;
  cloudset=in->cloudset;
  landset=in->landset;
  resolution.Set(&in->resolution);
  rotation.Set(&in->rotation);
  planetParams.Set(&in->planetParams);
  planetParams2.Set(&in->planetParams2);
  planetParams3.Set(&in->planetParams3);
  specularPos.Set(&in->specularPos);
  surfaceThresholdColor.fromCrayon(in->surfaceThresholdColor);
  secondaryMaterialColor.fromCrayon(in->secondaryMaterialColor);
  ambientColor.fromCrayon(in->ambientColor);
  atmosphereDiffuseColor.fromCrayon(in->atmosphereDiffuseColor);
  atmosphereSpecularColor.fromCrayon(in->atmosphereSpecularColor);
  darkDiffuseColor.fromCrayon(in->darkDiffuseColor);
  darkSpecularColor.fromCrayon(in->darkSpecularColor);
  oceanColor.fromCrayon(in->oceanColor);
 }
};

extern Earthlike2Shader earthlike2Shader;

void ShadedEarthlike2( FBOColor *out, Earthlike2Profile *in );


// --------------------------------------------------------------

ONE(RingedJovian2Profile,{
 noiseTex=library.Load("data/images/textures/grain1.png",tllto_Tiling);
 resolution.Set(1.0,1.0);
 cameraPos.Set(32.0,1.5,1); //vec3( 16.0*cos(0.2+0.5*.1*time*1.5), 1.5, 16.0*sin(0.1+0.5*0.11*time*1.5) ); }\n")
 lightPos.Set(0.3,0.9,0.7);
 bumpiness=-21.0;
 color1.Double(0.8,0.6,0.0,1.0);
 color2.Double(0.7,0.7,1.0,1.0);
 albedoColor.Double(0.0,0.5,0.8,1.0);
 ringColor1.Double(1.0,0.3,0.0,1.0);
 ringColor2.Double(1.0,0.2,0.0,1.0);
 centerGlowColor.Double(1,0.8,0.5,1.0);
 amb=2.5;
 diffuseColor.Double(0.8,0.8,0.8,1.0);
 ringParams.Set(1.2,0.9,573.62); ringParams.w=0.4;
 ringRadRatio=2.1;
 bandFreq=8.0;
 roll=-200.0;
})
 Zdouble time,bumpiness,amb,ringRadRatio,bandFreq,rotation,roll;
 Zp<GLImage> noiseTex;
 Cartesiand resolution,cameraPos,lightPos,ringParams;
 Crayon color1,color2,albedoColor,ringColor1,ringColor2,centerGlowColor,diffuseColor;
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_NUM(bumpiness)
  B_IN_NUM(amb)
  B_IN_NUM(ringRadRatio)
  B_IN_NUM(bandFreq)
  B_IN_NUM(rotation)
  B_IN_NUM(roll)
  B_IN_TILE(noiseTex)
  B_IN_SSUB(resolution)
  B_IN_SSUB(cameraPos)
  B_IN_SSUB(lightPos)
  B_IN_SSUB(ringParams)
  B_IN_TINT(color1)
  B_IN_TINT(color2)
  B_IN_TINT(albedoColor)
  B_IN_TINT(ringColor1)
  B_IN_TINT(ringColor2)
  B_IN_TINT(centerGlowColor)
  B_IN_TINT(diffuseColor)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_NUM(bumpiness)
  B_OUT_NUM(amb)
  B_OUT_NUM(ringRadRatio)
  B_OUT_NUM(bandFreq)
  B_OUT_NUM(rotation)
  B_OUT_NUM(roll)
  B_OUT_TILE(noiseTex)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(cameraPos)
  B_OUT_SSUB(lightPos)
  B_OUT_SSUB(ringParams)
  B_OUT_TINT(color1)
  B_OUT_TINT(color2)
  B_OUT_TINT(albedoColor)
  B_OUT_TINT(ringColor1)
  B_OUT_TINT(ringColor2)
  B_OUT_TINT(centerGlowColor)
  B_OUT_TINT(diffuseColor)
 })
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("bumpiness",{bumpiness=w;})
  else KEYWORD("ringRadRatio",{ringRadRatio=w;})
  else KEYWORD("amb",{amb=w;})
  else KEYWORD("roll",{roll=w;})
  else KEYWORD("rotation",{rotation=w;})
  else KEYWORD("bandFreq",{bandFreq=w;})
  else TILEWORD("noiseTex",noiseTex)
  else SUBWORD("cameraPos",cameraPos)
  else SUBWORD("resolution",resolution)
  else SUBWORD("lightPos",lightPos)
  else SUBWORD("ringParams",ringParams)
  else TINTWORD("color1",color1)
  else TINTWORD("color2",color2)
  else TINTWORD("albedoColor",albedoColor)
  else TINTWORD("ringColor1",ringColor1)
  else TINTWORD("ringColor2",ringColor2)
  else TINTWORD("centerGlowColor",centerGlowColor)
  else TINTWORD("diffuseColor",diffuseColor)
 })
 WORDKEYS({
  NUMBKEY("time",time)
  NUMBKEY("bumpiness",bumpiness)
  NUMBKEY("amb",amb)
  NUMBKEY("rotation",rotation)
  NUMBKEY("roll",roll)
  NUMBKEY("ringRadRatio",ringRadRatio)
  NUMBKEY("bandFreq",bandFreq)
  IMGKEY("noiseTex",noiseTex)
  SSUBKEY("cameraPos",cameraPos)
  SSUBKEY("resolution",resolution)
  SSUBKEY("lightPos",lightPos)
  SSUBKEY("ringParams",ringParams)
  TINTKEY("color1",color1)
  TINTKEY("color2",color2)
  TINTKEY("albedoColor",albedoColor)
  TINTKEY("ringColor1",ringColor1)
  TINTKEY("ringColor2",ringColor2)
  TINTKEY("centerGlowColor",centerGlowColor)
  TINTKEY("diffuseColor",diffuseColor)
 })
MANY(RingedJovian2Profile,RingedJovian2ProfileHandle,RingedJovian2ProfileHandles,"RingedJovian2",RingedJovian2Profiles,{})
 KEYWORDSGroup("RingedJovian2",RingedJovian2Profile);
 WORDKEYSGroup("RingedJovian2",RingedJovian2Profile);
 BINARY_INOUT(RingedJovian2Profile)
DONE(RingedJovian2Profile);

class RingedJovian2Shader : public GLSLShader {
public:
  Zdouble time,bumpiness,amb,ringRadRatio,bandFreq,rotation,roll;
  Zp<GLuint> noiseTex;
  Cartesiand resolution,cameraPos,lightPos,ringParams;
  Crayon color1,color2,albedoColor,ringColor1,ringColor2,centerGlowColor,diffuseColor;
 GLuint Utime,Ubumpiness,Uamb,UringRadRatio,UnoiseTex,UcameraPos,UlightPos,UringParams,Ucolor1,Ucolor2,UalbedoColor,UringColor1,UringColor2,UcenterGlowColor,UdiffuseColor,Uresolution,UbandFreq,Urotation,Uroll;
 void Init() {
  OUTPUT("RingedJovian2Shader::Loading\n");
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
   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
   +string("#endif\n")
   +string("uniform float time;\n")
   +string("uniform sampler2D noiseTex;\n")
   +string("uniform vec3 cameraPos;         //=vec3(20,1.5,1); vec3( 16.0*cos(0.2+0.5*.1*time*1.5), 1.5, 16.0*sin(0.1+0.5*0.11*time*1.5) ); }\n")
   +string("uniform float rotation; \n")
   +string("uniform float roll; // = -200; // = 0.3*sin(1.0+0.001*t);\n")
   +string("uniform vec3 lightPos;          //=vec3 lig = normalize( vec3(0.3,0.9,0.7) );\n")
   +string("uniform float bumpiness;        //=1.0;\n")
   +string("uniform vec3 color1;            //=vec3(0.8,0.6,0.0)\n")
   +string("uniform vec3 color2;            //=vec3(0.7,0.7,1.0)\n")
   +string("uniform vec3 albedoColor;       //=vec3(0,0.5,0.8)\n")
   +string("uniform vec3 ringColor1;        //=vec3(1.0, 0.3, 0.0)\n")
   +string("uniform vec3 ringColor2;        //=vec3(1.0, 0.2, 0.0),\n")
   +string("uniform vec3 centerGlowColor;   //=vec3(1,0.8,0.5)\n")
   +string("uniform float amb;              //= 2.5;  // ambient gain\n")
   +string("uniform float ringRadRatio;     //= 2.1; multiple of satradius\n")
   +string("uniform float bandFreq;         //= 8.0; \n")
   +string("uniform vec3 diffuseColor;      //=vec3(0.8);\n")
   +string("uniform vec4 ringParams;        //=vec3(1.2,0.9,573.62,0.4)\n")
   +string("uniform vec2 resolution; \n")
   +string("varying vec2 v_vTexcoord; \n")
   +string("const int marches=12;           // was 160\n")
   +string("const int shadow_precision=12;  // was 48\n")
   +string("const float PI = 3.1415926535897932384626433832795;\n")
   +string("const float TPI = 2.0 * PI;\n")
   +string("const mat3 m = mat3( 0.00,  0.80,  0.60,   -0.80,  0.36, -0.48,   -0.60, -0.48,  0.64 );  // I think this is just a rotation matrix - haven't analyzed it really.\n")
   +string("const float satradius = 5.0;\n")
   +string("const float satRotSpeed = 3.14*0.01;\n")
   +string("const float lodbias=0.0;\n")
   +string("vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec4 permute(vec4 x) { return mod289(((x*34.0)+1.0)*x); }\n")
   +string("vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }\n")
   +string("float snoise2d(vec2 v) {\n")
   +string(" const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0\n")
   +string("                     0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)\n")
   +string("                    -0.577350269189626,  // -1.0 + 2.0 * C.x\n")
   +string("                     0.024390243902439); // 1.0 / 41.0\n")
   +string(" vec2 i  = floor(v + dot(v, C.yy) );\n")
   +string(" vec2 x0 = v -   i + dot(i, C.xx);\n")
   +string(" vec2 i1;\n")
   +string(" i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\n")
   +string(" vec4 x12 = x0.xyxy + C.xxzz;\n")
   +string(" x12.xy -= i1;\n")
   +string(" i = mod289(i); // Avoid truncation effects in permutation\n")
   +string(" vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))\n")
   +string(" 	+ i.x + vec3(0.0, i1.x, 1.0 ));\n")
   +string(" vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\n")
   +string(" m = m*m ;\n")
   +string(" m = m*m ;\n")
   +string(" vec3 x = 2.0 * fract(p * C.www) - 1.0;\n")
   +string(" vec3 h = abs(x) - 0.5;\n")
   +string(" vec3 ox = floor(x + 0.5);\n")
   +string(" vec3 a0 = x - ox;\n")
   +string(" m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\n")
   +string(" vec3 g;\n")
   +string(" g.x  = a0.x  * x0.x  + h.x  * x0.y;\n")
   +string(" g.yz = a0.yz * x12.xz + h.yz * x12.yw;\n")
   +string(" return 130.0 * dot(m, g);\n")
   +string("}\n")
   +string("float snoise1d(float x) { return snoise2d(vec2(0.0, x)); }\n")
   +string("float noise( vec3 x ) {\n")
   +string("    vec3 p = floor(x);\n")
   +string("    vec3 f = fract(x);\n")
   +string("	f = f*f*(3.0-2.0*f);	\n")
   +string("	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;\n")
   +string("	vec2 rg = texture2D( noiseTex, (uv+ 0.5)/256.0, -100.0 ).yx;\n")
   +string("	return mix( rg.x, rg.y, f.z );\n")
   +string("}\n")
   +string("float noise( vec2 x ) { vec2 p = floor(x); vec2 f = fract(x);	vec2 uv = p.xy + f.xy*f.xy*(3.0-2.0*f.xy);	return texture2D( noiseTex, (uv+118.4)/256.0, -100.0 ).x; } \n")
   +string("vec4 texcube( sampler2D sam, in vec3 p, in vec3 n ) { \n")
   +string("	vec4 x = texture2D( sam, p.yz, lodbias );\n")
   +string("	vec4 y = texture2D( sam, p.zx, lodbias );\n")
   +string("	vec4 z = texture2D( sam, p.xy, lodbias );\n")
   +string("	return x*abs(n.x) + y*abs(n.y) + z*abs(n.z);\n")
   +string("}\n")
   +string("float displacement( vec3 p ) { \n")
   +string("    float f = 0.5000*noise( p ); p = m*p*2.02; \n")
   +string("    f += 0.2500*noise( p ); p = m*p*2.03; \n")
   +string("    f += 0.1250*noise( p ); p = m*p*2.01; \n")
   +string("    f += 0.0625*noise( p ); \n")
   +string("	float n = noise( p*5.5 ); \n")
   +string("    f += 0.03*n*n; \n")
   +string("    return f; \n")
   +string("}\n")
   +string("vec3 yRot( vec3 p, float rads ) {\n")
   +string("    return vec3(\n")
   +string("        p.x*cos(rads) - p.z*sin(rads),\n")
   +string("        p.y,\n")
   +string("        p.x*sin(rads) + p.z*cos(rads) );\n")
   +string("}\n")
   +string("float planetSDF( vec3 p ) {\n")
   +string(" p = yRot(p, time*satRotSpeed);\n")
   +string("	vec3 spotDir = normalize(vec3(0.0,1.0,0.0));\n")
   +string("	float noisePart = bumpiness * 0.005 * noise(5.0*p) * sin(3.0*bandFreq*dot(normalize(p),spotDir));\n")
   +string("	return length(p) - satradius + noisePart;\n")
   +string("}\n")
   +string("float unionn( float a, float b ) { return min(a,b); }\n")
   +string("float subtract( float a, float b ) { return max(-a, b); }\n")
   +string("float intersect( float a, float b ) { return max(a, b); }\n")
   +string("float raymarchTerrain( vec3 ro, vec3 rd ) {\n")
   +string("	float maxd = 30.0;\n")
   +string("	float precis = 0.001;						\n")
   +string("    float distToSurf = 1.0;\n")
   +string("    float t = 0.1;\n")
   +string("    for( int i=0; i<marches; i++ )\n")
   +string("    {\n")
   +string("     if( abs(distToSurf)<precis||t>maxd ) continue;//break;\n")
   +string("	    distToSurf = planetSDF( ro+rd*t );\n")
   +string("     t += distToSurf;\n")
   +string("    }\n")
   +string("    if( t>maxd ) t=-1.0;\n")
   +string("    return t;\n")
   +string("}\n")
   +string("vec3 calcNormal( in vec3 pos )\n")
   +string("{\n")
   +string("    vec3 eps = vec3(0.02,0.0,0.0);\n")
   +string("	return normalize( vec3(\n")
   +string("           planetSDF(pos+eps.xyy) - planetSDF(pos-eps.xyy),\n")
   +string("           planetSDF(pos+eps.yxy) - planetSDF(pos-eps.yxy),\n")
   +string("           planetSDF(pos+eps.yyx) - planetSDF(pos-eps.yyx) ) );\n")
   +string("\n")
   +string("}\n")
   +string("float softshadow( vec3 ro, vec3 rd, float mint, float k )\n")
   +string("{\n")
   +string("    float res = 1.0;\n")
   +string("    float t = mint;\n")
   +string("    for( int i=0; i<shadow_precision; i++ )\n")
   +string("    {\n")
   +string("        float h = planetSDF(ro + rd*t);\n")
   +string("		h = max( h, 0.0 );\n")
   +string("        res = min( res, k*h/t );\n")
   +string("        t += clamp( h, 0.02, 0.5 );\n")
   +string("    }\n")
   +string("    return clamp(res,0.0,1.0);\n")
   +string("}\n")
   +string("float density=0.0; \n")
   +string("vec3 blendrings( vec3 col, float t, vec3 ro, vec3 rd ) {\n")
   +string(" float ringrad = satradius*ringRadRatio;    \n")
   +string(" float tring = -ro.y/rd.y;\n")
   +string(" vec3 pring = ro + tring*rd;\n")
   +string(" pring = yRot(pring, time*PI/20.);\n")
   +string(" float r = length(pring.xz);    \n")
   +string(" if( (tring < t || t <= 0.0) && r <= ringrad )\n")
   +string(" {\n")
   +string("  float noise = snoise1d(r*0.4 + ringParams.z);\n")
   +string("  float normnoise = noise * 0.5 + 0.5;\n")
   +string("  float edgesoft = 0.1;\n")
   +string("  float denstart = ringParams.w;\n")
   +string("  density = smoothstep(denstart, denstart+edgesoft, normnoise);\n")
   +string("  // more high freq radial noise on top\n")
   +string("  density += snoise1d(r*0.8)*0.3;\n")
   +string("  density = clamp(density, 0., 1.);\n")
   +string("  // soften outer edge\n")
   +string("  density *= 1.0 - smoothstep(ringrad*0.98, ringrad, r);\n")
   +string("  // soften inner edge, against planet\n")
   +string("  density *= smoothstep(satradius, satradius*1.05, r);\n")
   +string("  vec3 ringcol = mix( ringColor1, ringColor2, snoise1d(r)*0.5+0.5 );\n")
   +string("  ringcol = mix(vec3(0.0,0.0,0.0), ringcol, pow(r/ringrad,2.0));\n")
   +string("  ringcol *= mix(0.95, 1.0, snoise2d(pring.xz*5.));\n")
   +string("  // darken towards the middle\n")
   +string("  ringcol *= smoothstep(ringrad-satradius*ringParams.x, ringrad, r);   \n")
   +string("  ringcol *= ringParams.y;\n")
   +string("  //col += density*ringcol;\n")
   +string("  col = mix( col, ringcol.xyz, density);\n")
   +string(" }\n")
   +string(" return col;\n")
   +string("}\n")
   +string("vec2 curl2d(vec2 p) {\n")
   +string(" const float E = 0.9;\n")
   +string(" float x = snoise2d(p);\n")
   +string(" float xey = snoise2d(p+vec2(0., E));\n")
   +string(" float xex = snoise2d(p+vec2(E, 0.));\n")
   +string(" return vec2( xex-x, -(xey-x) ) / E;\n")
   +string("}\n")
   +string("vec3 saturn_gradient(vec3 pos, vec3 rd) {\n")
   +string(" vec3 albedo = mix( color1, color2, snoise2d( vec2(0.0, pos.y*1.2) ) );    \n")
   +string(" albedo += albedoColor * smoothstep(0.0, -2.0, pos.y);  \n")
   +string(" return albedo;\n")
   +string("}\n")
   +string("vec3 shade_saturn(vec3 pos, vec3 rd ) {\n")
   +string(" float t = time;\n")
   +string(" vec3 p = pos;    \n")
   +string(" p = yRot(p, time * PI/20.);     // rotate the flow map a bit\n")
   +string(" // to spherical\n")
   +string(" float r = length(p);\n")
   +string(" float phi = atan(p.x, p.z);\n")
   +string(" float theta = acos(p.y/r);\n")
   +string(" vec2 uv = vec2( phi/PI - 0.5, -(theta/PI - 0.5) );    // to UV\n")
   +string(" for( int i = 0; i < 20; i++ ) {    // backwards curl advection\n")
   +string("  vec2 duv = curl2d(uv*20.)*.0004*.5;\n")
   +string("  uv += duv;\n")
   +string(" }\n")
   +string(" // back to spherical\n")
   +string(" phi = (uv.x+0.5) * PI;\n")
   +string(" theta = (-uv.y+0.5) * PI;\n")
   +string(" theta += sin(TPI*phi/(0.1*PI) + t*(1.3*PI)) * PI*0.005;    // add some ripple in opposite direction so it can flow through the warp\n")
   +string(" // back to cartesian\n")
   +string(" p = vec3(\n")
   +string(" r * sin(theta) * cos(phi),\n")
   +string(" r * cos(theta),\n")
   +string(" r * sin(theta) * sin(phi));\n")
   +string(" vec3 alb = saturn_gradient( p, rd );    \n")
   +string(" // make the center glow a bit\n")
   +string("	vec3 nor = pos/length(pos);    \n")
   +string(" alb += centerGlowColor * smoothstep(1.5, 0.0, abs(pos.y)) * pow(dot(nor,-rd),1.0);\n")
   +string(" return alb;\n")
   +string("}\n")
   +string("vec2 hash( vec2 p ) { return texture2D( noiseTex, (p+0.5)/200.0, -100.0 ).xy; }\n")
   +string("void main() {\n")
   +string(" vec3 lig = normalize(lightPos);")
   +string(" vec2 q = v_vTexcoord.xy / resolution.xy;\n")
   +string("	vec2 p = -1.0 + 2.0*q;\n")
   +string("	p.x *= resolution.x / resolution.y;\n")
   +string(" // camera	\n")
   +string("	float t = 2.7+time;\n")
   +string(" t *= 0.1;    // TEMP freeze camera\n")
   +string("	vec3 ro = cameraPos;\n")
   +string("	vec3 camTarget = cameraPos;\n")
   +string("	camTarget = vec3(0,0,0);\n")
   +string("	ro = normalize(ro)*max( 10.0, length(ro)); // stop entering planet\n")
   +string("	vec3 cw = normalize(camTarget-ro);\n")
   +string("	vec3 cp = vec3(sin(roll), cos(roll),rotation);\n")
   +string("	vec3 cu = normalize(cross(cw,cp)); \n")
   +string("	vec3 cv = normalize(cross(cu,cw)); \n")
   +string("	float r2 = p.x*p.x*0.32 + p.y*p.y; \n")
   +string(" p *= (7.0-sqrt(37.5-11.5*r2))/(r2+1.0); \n")
   +string("	vec3 rd = normalize( p.x*cu + p.y*cv + 2.1*cw ); \n")
   +string("	vec3 col = vec3(0.0,0.0,0.0); \n")
   +string(" col *= 0.9;    \n")
   +string("	vec3 bcol = col;\n")
   +string("	float ter = raymarchTerrain(ro, rd)+0.0;    // terrain	\n")
   +string(" if( ter>0.0 )	{ // we've hit saturn        \n")
   +string("		vec3 pos = ro + ter*rd; \n")
   +string("		vec3 nor = calcNormal( pos ); \n")
   +string("		vec3 ref = reflect( rd, nor ); \n")
   +string("		vec3 bn = -1.0 + 2.0*texcube( noiseTex, 3.0*pos/4.0, nor ).xyz; \n")
   +string("		nor = normalize( nor + 0.2*bn );		\n")
   +string("		float hh = 1.0 - smoothstep( -2.0, 1.0, pos.y );\n")
   +string("        // lighting\n")
   +string("		float dif = sqrt(clamp( dot( nor, lig ), 0.0, 1.0 ));\n")
   +string("		float sha = 0.0;\n")
   +string("		if( dif>0.01)\n")
   +string("			sha=softshadow(pos,lig,0.01,32.0);\n")
   +string("		float bac = clamp( dot( nor, normalize(lig*vec3(-1.0,0.0,-1.0)) ), 0.0, 1.0 );\n")
   +string("		float sky = 0.5 + 0.5*nor.y;\n")
   +string("		float occ = pow( (1.0-displacement(pos*vec3(0.8,1.0,0.8)))*1.6-0.5, 2.0 );\n")
   +string("		col = diffuseColor;\n")
   +string("		float cd = length(pos);\n")
   +string("  vec3 albedo = shade_saturn(pos, rd);        // shade Saturn\n")
   +string("  vec3 brdf =	dif * albedo;\n")
   +string("  brdf += 0.1 * amb * albedo;\n")
   +string("  brdf -= pow(1.0-clamp(dot(nor,-rd),0.,1.),1.9);        // anti-rim lighting\n")
   +string("  col = brdf * col;                        // light/surface interaction		\n")
   +string("		col = mix( col, (1.0-0.7*hh)*bcol, 1.0-exp(-0.00006*t*t*t) );		// atmospheric\n")
   +string("  col = blendrings(col, ter, ro, rd);        // rings - ray trace directly on to the y=0 plane\n")
   +string("  col *= mix(0.95, 1.0, snoise2d(p*50.0)); // film grain over everything\n")
   +string("	 gl_FragColor = vec4( col, 1.0 );\n")
   +string("	} else {\n")
   +string("  col = vec3( 0.0,0.0,0.0 ); \n")
   +string("  col = blendrings(col, ter, ro, rd);        // rings - ray trace directly on to the y=0 plane\n")
   +string("	 gl_FragColor = vec4(col,density);\n")
   +string(" }\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Ubumpiness=glGetUniformLocation(program, "bumpiness");
  Uamb=glGetUniformLocation(program, "amb");
  UringRadRatio=glGetUniformLocation(program, "ringRadRatio");
  UbandFreq=glGetUniformLocation(program, "bandFreq");
  Urotation=glGetUniformLocation(program, "rotation");
  UcameraPos=glGetUniformLocation(program, "cameraPos");
  UlightPos=glGetUniformLocation(program, "lightPos");
  UringParams=glGetUniformLocation(program, "ringParams");
  Ucolor1=glGetUniformLocation(program, "color1");
  Ucolor2=glGetUniformLocation(program, "color2");
  UalbedoColor=glGetUniformLocation(program, "albedoColor");
  UringColor1=glGetUniformLocation(program, "ringColor1");
  UringColor2=glGetUniformLocation(program, "ringColor2");
  UcenterGlowColor=glGetUniformLocation(program, "centerGlowColor");
  UdiffuseColor=glGetUniformLocation(program, "diffuseColor");
  Uresolution=glGetUniformLocation(program,"resolution");
  Uroll=glGetUniformLocation(program,"roll");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(UcameraPos,(GLfloat)cameraPos.x,(GLfloat)cameraPos.y,(GLfloat)cameraPos.z);
  glUniform3f(UlightPos,(GLfloat)lightPos.x,(GLfloat)lightPos.y,(GLfloat)lightPos.z);
  glUniform4f(UringParams,(GLfloat)ringParams.x,(GLfloat)ringParams.y,(GLfloat)ringParams.z,(GLfloat)ringParams.w);
  glUniform3f(Ucolor1,(GLfloat)color1.rf,(GLfloat)color1.gf,(GLfloat)color1.bf);
  glUniform3f(Ucolor2,(GLfloat)color2.rf,(GLfloat)color2.gf,(GLfloat)color2.bf);
  glUniform3f(UalbedoColor,(GLfloat)albedoColor.rf,(GLfloat)albedoColor.gf,(GLfloat)albedoColor.bf);
  glUniform3f(UringColor1,(GLfloat)ringColor1.rf,(GLfloat)ringColor1.gf,(GLfloat)ringColor1.bf);
  glUniform3f(UringColor2,(GLfloat)ringColor2.rf,(GLfloat)ringColor2.gf,(GLfloat)ringColor2.bf);
  glUniform3f(UcenterGlowColor,(GLfloat)centerGlowColor.rf,(GLfloat)centerGlowColor.gf,(GLfloat)centerGlowColor.bf);
  glUniform3f(UdiffuseColor,(GLfloat)diffuseColor.rf,(GLfloat)diffuseColor.gf,(GLfloat)diffuseColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Ubumpiness,(GLfloat)bumpiness);
  glUniform1f(Uamb,(GLfloat)amb);
  glUniform1f(UringRadRatio,(GLfloat)ringRadRatio);
  glUniform1f(UbandFreq,(GLfloat)bandFreq);
  glUniform1f(Urotation,(GLfloat)rotation);
  glUniform1f(Uroll,(GLfloat)roll);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glEnable(GL_TEXTURE_2D);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D,0);
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);
 }
 void Set( RingedJovian2Profile *in ) {
  time=in->time;
  bumpiness=in->bumpiness;
  amb=in->amb;
  ringRadRatio=in->ringRadRatio;
  bandFreq=in->bandFreq;
  rotation=in->rotation;
  roll=in->roll;
  cameraPos.Set(&in->cameraPos);
  lightPos.Set(&in->lightPos);
  ringParams.Set(&in->ringParams);
  color1.fromCrayon(in->color1);
  color2.fromCrayon(in->color2);
  albedoColor.fromCrayon(in->albedoColor);
  ringColor1.fromCrayon(in->ringColor1);
  ringColor2.fromCrayon(in->ringColor2);
  centerGlowColor.fromCrayon(in->centerGlowColor);
  diffuseColor.fromCrayon(in->diffuseColor);
  resolution.Set(&in->resolution);
  noiseTex=&in->noiseTex->texture;
 }
};

extern RingedJovian2Shader ringedjovian2Shader;

void ShadedRingedJovian2( FBOColor *out, RingedJovian2Profile *in );

// ----------------------------------------------

ONE(LunarPlanetProfile,{
 tex=library.Load("data/images/textures/tile37.png",tllto_Tiling);
 offSet.Set(0.6,0.0);
 texScale.Set(1.0,1.0);
 origin.Set(0.0,0.0,-2.5);
 baseColor.Double(0.7,0.8,1.0,1.0);
 multiplyColor.Double(0.9,0.55,0.9,1.0);
 heightFactor=0.6;
 scaleFactor=1.0;
 rotationSpeed=-0.05;
 zScale=3.5;
 smoothness=0.01;
 ambientColor.Double(0.2, 0.0, 0.05, 1.0);
 lightPos.Set(1.0, 0.0, -1.0);
 resolution.Set(1.0,1.0);
})
 Zdouble time,heightFactor,scaleFactor,rotationSpeed,zScale,smoothness;
 Cartesiand resolution,position,offSet,texScale,origin,lightPos;
 Crayon baseColor,multiplyColor,ambientColor;
 Zp<GLImage> tex;
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_NUM(heightFactor)
  B_IN_NUM(scaleFactor)
  B_IN_NUM(rotationSpeed)
  B_IN_NUM(zScale)
  B_IN_NUM(smoothness)
  B_IN_SSUB(resolution)
  B_IN_SSUB(position)
  B_IN_SSUB(offSet)
  B_IN_SSUB(texScale)
  B_IN_SSUB(origin)
  B_IN_SSUB(lightPos)
  B_IN_TINT(baseColor)
  B_IN_TINT(multiplyColor)
  B_IN_TINT(ambientColor)
  B_IN_IMAGE(tex)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_NUM(heightFactor)
  B_OUT_NUM(scaleFactor)
  B_OUT_NUM(rotationSpeed)
  B_OUT_NUM(zScale)
  B_OUT_NUM(smoothness)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(position)
  B_OUT_SSUB(offSet)
  B_OUT_SSUB(texScale)
  B_OUT_SSUB(origin)
  B_OUT_SSUB(lightPos)
  B_OUT_TINT(baseColor)
  B_OUT_TINT(multiplyColor)
  B_OUT_TINT(ambientColor)
  B_OUT_IMAGE(tex)
 })
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("heightFactor",{heightFactor=w;})
  else KEYWORD("scaleFactor",{scaleFactor=w;})
  else KEYWORD("rotationSpeed",{rotationSpeed=w;})
  else KEYWORD("zScale",{zScale=w;})
  else KEYWORD("smoothness",{smoothness=w;})
  else SUBWORD("resolution",resolution)
  else SUBWORD("position",position)
  else SUBWORD("offSet",offSet)
  else SUBWORD("texScale",texScale)
  else SUBWORD("origin",origin)
  else SUBWORD("lightPos",lightPos)
  else TINTWORD("baseColor",baseColor)
  else TINTWORD("multiplyColor",multiplyColor)
  else TINTWORD("ambientColor",ambientColor)
  else TILEWORD("tex",tex)
 })
 WORDKEYS({
  NUMBKEY("time",time)
  NUMBKEY("heightFactor",heightFactor)
  NUMBKEY("scaleFactor",scaleFactor)
  NUMBKEY("rotationSpeed",rotationSpeed)
  NUMBKEY("zScale",zScale)
  NUMBKEY("smoothness",smoothness)
  SSUBKEY("resolution",resolution)
  SSUBKEY("position",position)
  SSUBKEY("offSet",offSet)
  SSUBKEY("texScale",texScale)
  SSUBKEY("origin",origin)
  SSUBKEY("lightPos",lightPos)
  TINTKEY("baseColor",baseColor)
  TINTKEY("multiplyColor",multiplyColor)
  TINTKEY("ambientColor",ambientColor)
  IMGKEY("tex",tex)
 })
MANY(LunarPlanetProfile,LunarPlanetProfileHandle,LunarPlanetProfileHandles,"LunarPlanet",LunarPlanetProfiles,{})
 KEYWORDSGroup("LunarPlanet",LunarPlanetProfile);
 WORDKEYSGroup("LunarPlanet",LunarPlanetProfile);
 BINARY_INOUT(LunarPlanetProfile)
DONE(LunarPlanetProfile);

class LunarPlanetShader : public GLSLShader {
public:
  Zdouble time,heightFactor,scaleFactor,rotationSpeed,zScale,smoothness;
  Cartesiand resolution,position,offSet,texScale,origin,lightPos;
  Crayon baseColor,multiplyColor,ambientColor;
  Zp<GLuint> tex;
 GLuint Utime,UheightFactor,UscaleFactor,UrotationSpeed,UzScale,Usmoothness,Uresolution,Uposition,UoffSet,UtexScale,Uorigin,UlightPos,UbaseColor,UmultiplyColor,UambientColor,Utex;
 void Init() {
  OUTPUT("LunarPlanetShader::Loading\n");
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
    string("uniform float time;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec2 position;       //=iMouse.xy/iResolution.xy;\n")
   +string("uniform vec2 offSet;         //=vec2(0.6, 0.0);\n")
   +string("uniform vec2 texScale;       //=vec2(1.0,1.0);\n")
   +string("uniform vec3 origin;         //=vec3(0.0,0.0,-2.5);\n")
   +string("uniform vec4 baseColor;      //=vec4(0.7,0.8,1.0, 1.0);\n")
   +string("uniform vec4 multiplyColor;  //=vec4(0.9,0.55,0.9, 1.0);\n")
   +string("uniform float heightFactor;  //=0.6;\n")
   +string("uniform float scaleFactor;   //=1.0;\n")
   +string("uniform float rotationSpeed; //=-0.05;\n")
   +string("uniform float zScale;        //=3.5;\n")
   +string("uniform float smoothness;    //=0.01;\n")
   +string("uniform vec4 ambientColor;   //=vec4(0.2, 0.0, 0.05, 1.0);\n")
   +string("uniform vec3 lightPos;       // = vec3(1.0, 0.0, -1.0);\n")
   +string("uniform sampler2D tex;\n")
   +string("const float PI = 3.14159265358979323846264338327950288419;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("vec2 sphere_map(vec3 n) { return texScale * (vec2((atan(n.z,n.x)/(2.0 * PI)) + time * rotationSpeed, acos(n.y) / (PI))) + offSet; }\n")
   +string("float scene(vec3 p) {\n")
   +string(" vec2 uv = sphere_map(normalize(p));\n")
   +string(" return length(p)-(scaleFactor*0.3 - pow(texture2D(tex, uv).x, 0.7) * (heightFactor*0.1));\n")
   +string("}\n")
   +string("vec3 getNormal(vec3 ray_hit_position, float smoothness) {	\n")
   +string(" vec3 n;\n")
   +string(" vec2 dn = vec2(smoothness, 0.0);\n")
   +string(" n.x	= scene(ray_hit_position + dn.xyy) - scene(ray_hit_position - dn.xyy);\n")
   +string(" n.y	= scene(ray_hit_position + dn.yxy) - scene(ray_hit_position - dn.yxy);\n")
   +string(" n.z	= scene(ray_hit_position + dn.yyx) - scene(ray_hit_position - dn.yyx);\n")
   +string(" return normalize(n);\n")
   +string("}\n")
   +string("float raymarch(vec3 position, vec3 direction) {\n")
   +string(" float total_distance = 0.0;\n")
   +string(" for(int i = 0 ; i < 32 ; ++i) {\n")
   +string("  float result = scene(position + direction * total_distance);\n")
   +string("  if(result < 0.005) { return total_distance; }\n")
   +string("  total_distance += result;\n")
   +string(" }\n")
   +string(" return -1.0;\n")
   +string("}\n")
   +string("void main() {\n")
   +string("vec2 uv = (position.xy + v_vTexcoord.xy / resolution.y) - vec2(0.5*resolution.x/resolution.y, 0.5);\n")
   +string("vec3 direction = normalize(vec3(uv, zScale));\n")
   +string(" float dist = raymarch(origin, direction);\n")
   +string(" if(dist < 0.0) {\n")
   +string("  gl_FragColor = vec4(0.0,0.0,0.0,0.0); \n")
   +string(" } else{\n")
   +string("  vec3 fragPosition = origin+direction*dist;\n")
   +string("  vec3 N = getNormal(fragPosition, smoothness);\n")
   +string("  float diffuse = dot(normalize(lightPos), N);\n")
   +string("  diffuse = max(0.0, diffuse); \n")
   +string("  vec2 uv = sphere_map(normalize(fragPosition));\n")
   +string("  vec4 cheeseColor = baseColor - texture2D(tex, uv).x * multiplyColor;\n")
   +string("  gl_FragColor = (cheeseColor*diffuse)+ambientColor+\n")
   +string("  pow(max(0.0,dot(normalize(-reflect(fragPosition-lightPos, N)),normalize(-fragPosition))), 80.0) ;\n")
   +string(" }\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  UheightFactor=glGetUniformLocation(program, "heightFactor");
  UscaleFactor=glGetUniformLocation(program, "scaleFactor");
  UrotationSpeed=glGetUniformLocation(program, "rotationSpeed");
  UzScale=glGetUniformLocation(program, "zScale");
  Usmoothness=glGetUniformLocation(program, "smoothness");
  Uresolution=glGetUniformLocation(program, "resolution");
  Uposition=glGetUniformLocation(program, "position");
  UoffSet=glGetUniformLocation(program, "offSet");
  UtexScale=glGetUniformLocation(program, "texScale");
  Uorigin=glGetUniformLocation(program, "origin");
  UlightPos=glGetUniformLocation(program, "lightPos");
  UbaseColor=glGetUniformLocation(program, "baseColor");
  UmultiplyColor=glGetUniformLocation(program, "multiplyColor");
  UambientColor=glGetUniformLocation(program, "ambientColor");
  Utex=glGetUniformLocation(program, "tex");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Uposition,(GLfloat)position.x,(GLfloat)position.y);
  glUniform2f(UoffSet,(GLfloat)offSet.x,(GLfloat)offSet.y);
  glUniform2f(UtexScale,(GLfloat)texScale.x,(GLfloat)texScale.y);
  glUniform3f(Uorigin,(GLfloat)origin.x,(GLfloat)origin.y,(GLfloat)origin.z);
  glUniform3f(UlightPos,(GLfloat)lightPos.x,(GLfloat)lightPos.y,(GLfloat)lightPos.z);
  glUniform4f(UbaseColor,(GLfloat)baseColor.rf,(GLfloat)baseColor.gf,(GLfloat)baseColor.bf,(GLfloat)baseColor.af);
  glUniform4f(UmultiplyColor,(GLfloat)multiplyColor.rf,(GLfloat)multiplyColor.gf,(GLfloat)multiplyColor.bf,(GLfloat)multiplyColor.af);
  glUniform4f(UambientColor,(GLfloat)ambientColor.rf,(GLfloat)ambientColor.gf,(GLfloat)ambientColor.bf,(GLfloat)ambientColor.af);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UheightFactor,(GLfloat)heightFactor);
  glUniform1f(UscaleFactor,(GLfloat)scaleFactor);
  glUniform1f(UrotationSpeed,(GLfloat)rotationSpeed);
  glUniform1f(UzScale,(GLfloat)zScale);
  glUniform1f(Usmoothness,(GLfloat)smoothness);
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex.pointer));
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( LunarPlanetProfile *in ) {
  time=in->time;
  heightFactor=in->heightFactor;
  scaleFactor=in->scaleFactor;
  rotationSpeed=in->rotationSpeed;
  zScale=in->zScale;
  smoothness=in->smoothness;
  resolution.Set(&in->resolution);
  position.Set(&in->position);
  offSet.Set(&in->offSet);
  texScale.Set(&in->texScale);
  origin.Set(&in->origin);
  lightPos.Set(&in->lightPos);
  baseColor.fromCrayon(in->baseColor);
  multiplyColor.fromCrayon(in->multiplyColor);
  ambientColor.fromCrayon(in->ambientColor);
  tex=&in->tex->texture;
 }
 void Render( FBOColor *out, LunarPlanetProfile *in );
};

extern LunarPlanetShader lunarplanetShader;

void ShadedLunarPlanet( FBOColor *out, LunarPlanetProfile *in );


// ----------------------------------------------

//void ShadedBurningPlanet( FBOColor *out );
//
//class BurningPlanetShader  : public GLSLShader {
//public:
// void Init() {
//  OUTPUT("BurningPlanetShader::Loading\n");
// // Simple passthrough vertex shader
//  string vert=
//    string("#version 120\n")
//   +string("#ifdef GL_ES\n")
//   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
//   +string("#endif\n")
//   +string("varying vec2 v_vTexcoord;\n")
//   +string("varying vec4 v_vColour;\n")
//   +string("uniform sampler2D tex;\n")
//   +string("")
//   +string("void main() {\n")
//   +string(" gl_Position = ftransform();\n")
//   +string(" gl_TexCoord[0]=gl_MultiTexCoord0;\n")
//   +string(" v_vColour = gl_Color;\n")
//   +string(" v_vTexcoord = gl_TexCoord[0].st;\n")
//   +string("}\n")
//  ;
//  string frag=string(
//  );
//  loadShaders(vert.c_str(),frag.c_str());
//  InitUniforms();
// }
// void InitUniforms() {
// }
// void UpdateUniforms() {
//  glUseProgram(program);
// }
// void Disable() {
//  glBindTexture(GL_TEXTURE_2D, 0);
//  glUseProgram(0);
// }
//};

// ----------------------------------------------------

ONE(FlaringStarProfile,{
 resolution.Set(1.0,1.0);
 scaleStar=1.25;
 scaleClamp=1.5;
 invCircleWidth=0.25;
 rays_innerR=0.96;
 rays_outerR=2.0;
 position.Set(0.5,0.5);
 rotation.Set(0.1,0.0);
 convectiveColor.Double(0.5,0.0,0.0,1.0);
 surfaceStaticColor.Double(0.0,1.0,1.0,1.0);
 surfaceStaticColor2.Double(1.0,1.0,1.0,1.0);
 surfaceColor1.Double(1.0,0.0,0.0,1.0);
 surfaceColor2.Double(1.0,1.0,0.0,1.0);
 tintColor.Double(1.0,1.0,1.0,1.0);
 coronaColor.Double(1.0,1.0,1.0,1.0);
 plasmaColor.Double(1.0,0.6,0.1,1.0);
 windColor1.Double(1.0,0.0,0.0,1.0);
 windColor2.Double(0.0,0.0,1.0,1.0);
 windColor3.Double(0.9,1.0,0.1,1.0);
})
 Zdouble time,scaleStar,scaleClamp,invCircleWidth,rays_innerR,rays_outerR;
 Cartesiand resolution,position,rotation;
 Crayon convectiveColor,surfaceStaticColor,surfaceStaticColor2,surfaceColor1,surfaceColor2,tintColor,coronaColor,plasmaColor,windColor1,windColor2,windColor3;
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_NUM(scaleStar)
  B_IN_NUM(scaleClamp)
  B_IN_NUM(invCircleWidth)
  B_IN_NUM(rays_innerR)
  B_IN_NUM(rays_outerR)
  B_IN_SSUB(resolution)
  B_IN_SSUB(position)
  B_IN_SSUB(rotation)
  B_IN_TINT(convectiveColor)
  B_IN_TINT(surfaceStaticColor)
  B_IN_TINT(surfaceStaticColor2)
  B_IN_TINT(surfaceColor1)
  B_IN_TINT(surfaceColor2)
  B_IN_TINT(tintColor)
  B_IN_TINT(coronaColor)
  B_IN_TINT(plasmaColor)
  B_IN_TINT(windColor1)
  B_IN_TINT(windColor2)
  B_IN_TINT(windColor3)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_NUM(scaleStar)
  B_OUT_NUM(scaleClamp)
  B_OUT_NUM(invCircleWidth)
  B_OUT_NUM(rays_innerR)
  B_OUT_NUM(rays_outerR)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(position)
  B_OUT_SSUB(rotation)
  B_OUT_TINT(convectiveColor)
  B_OUT_TINT(surfaceStaticColor)
  B_OUT_TINT(surfaceStaticColor2)
  B_OUT_TINT(surfaceColor1)
  B_OUT_TINT(surfaceColor2)
  B_OUT_TINT(tintColor)
  B_OUT_TINT(coronaColor)
  B_OUT_TINT(plasmaColor)
  B_OUT_TINT(windColor1)
  B_OUT_TINT(windColor2)
  B_OUT_TINT(windColor3)
 })
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("scaleStar",{scaleStar=w;})
  else KEYWORD("scaleClamp",{scaleClamp=w;})
  else KEYWORD("invCircleWidth",{invCircleWidth=w;})
  else KEYWORD("rays_innerR",{rays_innerR=w;})
  else KEYWORD("rays_outerR",{rays_outerR=w;})
  else SUBWORD("resolution",resolution)
  else SUBWORD("position",position)
  else SUBWORD("rotation",rotation)
  else TINTWORD("convectiveColor",convectiveColor)
  else TINTWORD("surfaceStaticColor",surfaceStaticColor)
  else TINTWORD("surfaceStaticColor2",surfaceStaticColor2)
  else TINTWORD("surfaceColor1",surfaceColor1)
  else TINTWORD("surfaceColor2",surfaceColor2)
  else TINTWORD("tintColor",tintColor)
  else TINTWORD("coronaColor",coronaColor)
  else TINTWORD("plasmaColor",plasmaColor)
  else TINTWORD("windColor1",windColor1)
  else TINTWORD("windColor2",windColor2)
  else TINTWORD("windColor3",windColor3)
 })
 WORDKEYS({
  NUMBKEY("time",time)
  NUMBKEY("scaleStar",scaleStar)
  NUMBKEY("scaleClamp",scaleClamp)
  NUMBKEY("invCircleWidth",invCircleWidth)
  NUMBKEY("rays_innerR",rays_innerR)
  NUMBKEY("rays_outerR",rays_outerR)
  SSUBKEY("resolution",resolution)
  SSUBKEY("position",position)
  SSUBKEY("rotation",rotation)
  TINTKEY("convectiveColor",convectiveColor)
  TINTKEY("surfaceStaticColor",surfaceStaticColor)
  TINTKEY("surfaceStaticColor2",surfaceStaticColor2)
  TINTKEY("surfaceColor1",surfaceColor1)
  TINTKEY("surfaceColor2",surfaceColor2)
  TINTKEY("tintColor",tintColor)
  TINTKEY("coronaColor",coronaColor)
  TINTKEY("plasmaColor",plasmaColor)
  TINTKEY("windColor1",windColor1)
  TINTKEY("windColor2",windColor2)
  TINTKEY("windColor3",windColor3)
 })
MANY(FlaringStarProfile,FlaringStarProfileHandle,FlaringStarProfileHandles,"FlaringStar",FlaringStarProfiles,{})
 KEYWORDSGroup("FlaringStar",FlaringStarProfile);
 WORDKEYSGroup("FlaringStar",FlaringStarProfile);
 BINARY_INOUT(FlaringStarProfile);
DONE(FlaringStarProfile);

class FlaringStarShader : public GLSLShader {
public:
  Zdouble time,scaleStar,scaleClamp,invCircleWidth,rays_innerR,rays_outerR;
  Cartesiand resolution,position,rotation;
  Crayon convectiveColor,surfaceStaticColor,surfaceStaticColor2,surfaceColor1,surfaceColor2,tintColor,coronaColor,plasmaColor,windColor1,windColor2,windColor3;
 GLuint Utime,UscaleStar,UscaleClamp,UinvCircleWidth,Urays_innerR,Urays_outerR,Uresolution,Uposition,Urotation,UconvectiveColor,UsurfaceStaticColor,UsurfaceStaticColor2,UsurfaceColor1,UsurfaceColor2,UtintColor,UcoronaColor,UplasmaColor,UwindColor1,UwindColor2,UwindColor3;
 void Init() {
  OUTPUT("FlaringStarShader::Loading\n");
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
   +string("uniform float time;\n")
   +string("uniform vec2 resolution;          // vec2(1.0,1.0);\n")
   +string("uniform float scaleStar;          // =1.25;\n")
   +string("uniform float scaleClamp;         // =1.5;\n")
   +string("uniform float invCircleWidth;     // =0.25;\n")
   +string("uniform float rays_innerR;        // =0.96;\n")
   +string("uniform float rays_outerR;        // =2.0;\n")
   +string("uniform vec2 position;            // = vec2(0.5,0.5);\n")
   +string("uniform vec2 rotation;            // = vec2(iGlobalTime*0.1,0);\n")
   +string("uniform vec3 convectiveColor;     // = vec3(0.5,0.0,0.0);\n")
   +string("uniform vec3 surfaceStaticColor;  // = vec3(0.0,1.0,1.0);\n")
   +string("uniform vec3 surfaceStaticColor2; // = vec3(1.0,1.0,1.0);\n")
   +string("uniform vec3 surfaceColor1;       // = vec3(1.0,0.0,0.0);\n")
   +string("uniform vec3 surfaceColor2;       // = vec3(1.0,1.0,0.0);\n")
   +string("uniform vec3 tintColor;           // = vec3(1.0,1.0,1.0);\n")
   +string("uniform vec3 coronaColor;         // = vec3(1.0,1.0,1.0);\n")
   +string("uniform vec3 plasmaColor;         // = vec3(1.0,0.6,0.1);\n")
   +string("uniform vec3 windColor1;          // = vec3(1.0,0.0,0.0);\n")
   +string("uniform vec3 windColor2;          // = vec3(0.0,0.0,1.0);\n")
   +string("uniform vec3 windColor3;          // = vec3(0.9,1.0,0.1);\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("vec4 hash4( vec4 n ) { return fract(sin(n)*1399763.5453123); }\n")
   +string("vec3 hash3( vec3 n ) { return fract(sin(n)*1399763.5453123); }\n")
   +string("vec3 hpos( vec3 n ) { return hash3(vec3(dot(n,vec3(157.0,113.0,271.0)),dot(n,vec3(271.0,157.0,113.0)),dot(n,vec3(113.0,271.0,157.0)))); }\n")
   +string("float noise4q(vec4 x)\n")
   +string("{\n")
   +string("	vec4 n3 = vec4(0,0.25,0.5,0.75);\n")
   +string("	vec4 p2 = floor(x.wwww+n3);\n")
   +string("	vec4 b = floor(x.xxxx+n3) + floor(x.yyyy+n3)*157.0 + floor(x.zzzz +n3)*113.0;\n")
   +string("	vec4 p1 = b + fract(p2*0.00390625)*vec4(164352.0, -164352.0, 163840.0, -163840.0);\n")
   +string("	p2 = b + fract((p2+1.0)*0.00390625)*vec4(164352.0, -164352.0, 163840.0, -163840.0);\n")
   +string("	vec4 f1 = fract(x.xxxx+n3);\n")
   +string("	vec4 f2 = fract(x.yyyy+n3);\n")
   +string("	f1=f1*f1*(3.0-2.0*f1);\n")
   +string("	f2=f2*f2*(3.0-2.0*f2);\n")
   +string("	vec4 n1 = vec4(0,1.0,157.0,158.0);\n")
   +string("	vec4 n2 = vec4(113.0,114.0,270.0,271.0);	\n")
   +string("	vec4 vs1 = mix(hash4(p1), hash4(n1.yyyy+p1), f1);\n")
   +string("	vec4 vs2 = mix(hash4(n1.zzzz+p1), hash4(n1.wwww+p1), f1);\n")
   +string("	vec4 vs3 = mix(hash4(p2), hash4(n1.yyyy+p2), f1);\n")
   +string("	vec4 vs4 = mix(hash4(n1.zzzz+p2), hash4(n1.wwww+p2), f1);	\n")
   +string("	vs1 = mix(vs1, vs2, f2);\n")
   +string("	vs3 = mix(vs3, vs4, f2);\n")
   +string("	vs2 = mix(hash4(n2.xxxx+p1), hash4(n2.yyyy+p1), f1);\n")
   +string("	vs4 = mix(hash4(n2.zzzz+p1), hash4(n2.wwww+p1), f1);\n")
   +string("	vs2 = mix(vs2, vs4, f2);\n")
   +string("	vs4 = mix(hash4(n2.xxxx+p2), hash4(n2.yyyy+p2), f1);\n")
   +string("	vec4 vs5 = mix(hash4(n2.zzzz+p2), hash4(n2.wwww+p2), f1);\n")
   +string("	vs4 = mix(vs4, vs5, f2);\n")
   +string("	f1 = fract(x.zzzz+n3);\n")
   +string("	f2 = fract(x.wwww+n3);\n")
   +string("	f1=f1*f1*(3.0-2.0*f1);\n")
   +string("	f2=f2*f2*(3.0-2.0*f2);\n")
   +string("	vs1 = mix(vs1, vs2, f1);\n")
   +string("	vs3 = mix(vs3, vs4, f1);\n")
   +string("	vs1 = mix(vs1, vs3, f2);\n")
   +string("	float r=dot(vs1,vec4(0.25));\n")
   +string("	return r*r*(3.0-2.0*r);\n")
   +string("}\n")
   +string("float starBody(vec3 ray,vec3 pos,float r,mat3 mr,float zoom,vec3 subnoise,float anim)\n")
   +string("{\n")
   +string("  	float b = dot(ray,pos);\n")
   +string("  	float c = dot(pos,pos) - b*b;    \n")
   +string("    vec3 r1=vec3(0.0);    \n")
   +string("    float s=0.0;\n")
   +string("    float d=0.03125;\n")
   +string("    float d2=zoom/(d*d); \n")
   +string("    float ar=5.0;   \n")
   +string("    for (int i=0;i<3;i++) {\n")
   +string("		float rq=r*r;\n")
   +string("        if(c <rq) {\n")
   +string("            float l1=sqrt(rq-c);\n")
   +string("            r1= ray*(b-l1)-pos;\n")
   +string("            r1=r1*mr;\n")
   +string("            s+=abs(noise4q(vec4(r1*d2+subnoise*ar,anim*ar))*d);\n")
   +string("        }\n")
   +string("        ar-=2.0;\n")
   +string("        d*=4.0;\n")
   +string("        d2*=0.0625;\n")
   +string("        r=r-r*0.02;\n")
   +string("    }\n")
   +string("    return s;\n")
   +string("}\n")
   +string("// glow ring\n")
   +string("float ring(vec3 ray,vec3 pos,float r,float size) {\n")
   +string("  	float b = dot(ray,pos);\n")
   +string("  	float c = dot(pos,pos) - b*b;   \n")
   +string("    float s=max(0.0,(1.0-size*abs(r-sqrt(c))));    \n")
   +string("    return s;\n")
   +string("}\n")
   +string("// rays of a star\n")
   +string("float ringRayNoise(vec3 ray,vec3 pos,float r,float size,mat3 mr,float anim) {\n")
   +string("  	float b = dot(ray,pos);\n")
   +string("    vec3 pr=ray*b-pos;       \n")
   +string("    float c=length(pr);\n")
   +string("    pr*=mr;    \n")
   +string("    pr=normalize(pr);    \n")
   +string("    float s=max(0.0,(1.0-size*abs(r-c)));    \n")
   +string("    float nd=noise4q(vec4(pr*1.0,-anim+c))*2.0;\n")
   +string("    nd=pow(nd,2.0);\n")
   +string("    float n=0.4;\n")
   +string("    float ns=1.0;\n")
   +string("    if (c>r) {\n")
   +string("        n=noise4q(vec4(pr*10.0,-anim+c));\n")
   +string("        ns=noise4q(vec4(pr*50.0,-anim*2.5+c*2.0))*2.0;\n")
   +string("    }\n")
   +string("    n=n*n*nd*ns;    \n")
   +string("    return pow(s,4.0)+s*s*n;\n")
   +string("}\n")
   +string("vec4 noiseSpace(vec3 ray,vec3 pos,float r,mat3 mr,float zoom,vec3 subnoise,float anim){\n")
   +string("  	float b = dot(ray,pos);\n")
   +string("  	float c = dot(pos,pos) - b*b;    \n")
   +string("    vec3 r1=vec3(0.0);    \n")
   +string("    float s=0.0;\n")
   +string("    float d=0.0625*1.5;\n")
   +string("    float d2=zoom/d;\n")
   +string("	float rq=r*r;\n")
   +string("    float l1=sqrt(abs(rq-c));\n")
   +string("    r1= (ray*(b-l1)-pos)*mr;\n")
   +string("    r1*=d2;\n")
   +string("    s+=abs(noise4q(vec4(r1+subnoise,anim))*d);\n")
   +string("    s+=abs(noise4q(vec4(r1*0.5+subnoise,anim))*d*2.0);\n")
   +string("    s+=abs(noise4q(vec4(r1*0.25+subnoise,anim))*d*4.0);\n")
   +string("    //return s;\n")
   +string("    return vec4(s*2.0,abs(noise4q(vec4(r1*0.1+subnoise,anim))),abs(noise4q(vec4(r1*0.1+subnoise*6.0,anim))),abs(noise4q(vec4(r1*0.1+subnoise*13.0,anim))));\n")
   +string("}\n")
   +string("float sphereZero(vec3 ray,vec3 pos,float r) {\n")
   +string("  	float b = dot(ray,pos);\n")
   +string("  	float c = dot(pos,pos) - b*b;\n")
   +string("    float s=1.0;\n")
   +string("    if (c<r*r) s=0.0;\n")
   +string("    return s;\n")
   +string("}\n")
   +string("void main() {\n")
   +string("	vec2 p = (-resolution.xy + 2.0*v_vTexcoord.xy) / resolution.y * scaleStar;\n")
   +string("	vec2 q = (-resolution.xy + 2.0*v_vTexcoord.xy) / resolution.y * scaleClamp;\n")
   +string("	float mx = position.x>0.0?position.x/resolution.x*10.0:0.5;\n")
   +string(" float my = position.y>0.0?position.y/resolution.y*4.0-2.0:0.0;\n")
   +string(" vec2 sins=sin(rotation);\n")
   +string(" vec2 coss=cos(rotation);\n")
   +string(" mat3 mr=mat3(vec3(coss.x,0.0,sins.x),vec3(0.0,1.0,0.0),vec3(-sins.x,0.0,coss.x));\n")
   +string(" mr=mat3(vec3(1.0,0.0,0.0),vec3(0.0,coss.y,sins.y),vec3(0.0,-sins.y,coss.y))*mr;    \n")
   +string(" mat3 imr=mat3(vec3(coss.x,0.0,-sins.x),vec3(0.0,1.0,0.0),vec3(sins.x,0.0,coss.x));\n")
   +string(" imr=imr*mat3(vec3(1.0,0.0,0.0),vec3(0.0,coss.y,-sins.y),vec3(0.0,sins.y,coss.y));    \n")
   +string(" float t=time*1.0;	\n")
   +string(" vec3 ray = normalize(vec3(p,2.0));\n")
   +string(" vec3 pos = vec3(position.x/resolution.x-0.5,position.y/resolution.y-0.5,3.0); //0.0,0.0,3.0);    \n")
   +string(" float s1=starBody(ray,pos,1.0,mr,0.5,vec3(0.0),t);\n")
   +string(" s1=pow(min(1.0,s1*2.4),2.0);\n")
   +string(" float s2=starBody(ray,pos,1.0,mr,4.0,vec3(83.23,34.34,67.453),t);\n")
   +string(" s2=min(1.0,s2*2.2);\n")
   +string(" float zero=sphereZero(ray,pos,0.9);\n")
   +string("	vec4 col=vec4(0.0,0.0,0.0,1.0);\n")
   +string(" if (zero > 0.0) {\n")
   +string("  col = vec4( mix(surfaceStaticColor,surfaceStaticColor2,pow(s1,60.0))*s1, 1.0 );\n")
   +string(" } else {\n")
   +string("  col = vec4( convectiveColor + mix(surfaceStaticColor,surfaceStaticColor2,pow(s1,60.0))*s1, 1.0 );\n")
   +string(" }\n")
   +string(" col += vec4( mix(surfaceStaticColor,surfaceStaticColor2,pow(s1,60.0))*s1, 1.0 );\n")
   +string(" col += vec4( mix(mix(surfaceColor1,surfaceColor2,pow(s2,2.0)),vec3(1.0),pow(s2,10.0))*s2, 1.0 );	\n")
   +string(" col.xyz -= vec3(ring(ray,pos,1.03,11.0))*2.0;\n")
   +string(" col = max( vec4(0.0), col );    \n")
   +string(" float s3=max(ringRayNoise(ray,pos,rays_innerR,rays_outerR,mr,t),0.0);\n")
   +string(" col.xyz += mix(plasmaColor,coronaColor,pow(s3,3.0))*s3;\n")
   +string(" if (zero > 0.0) {\n")
   +string("	 vec4 s4=noiseSpace(ray,pos,100.0,mr,0.05,vec3(1.0,2.0,4.0),0.0);\n")
   +string("  s4.x=pow(s4.x,3.0);\n")
   +string("  col.xyz += mix(mix(windColor1,windColor2,s4.y*1.9),windColor3,s4.w*0.75)*s4.x*pow(s4.z*2.5,3.0)*0.2*zero;\n")
   +string(" }  \n")
   +string(" col = max( vec4(0.0), col );\n")
   +string("	col = min( vec4(1.0), col );\n")
   +string("	float circle = ( q.x * q.x + q.y * q.y ) * invCircleWidth;\n")
   +string(" gl_FragColor = (1.0-circle)*vec4( tintColor*col.xyz, (col.x+col.y+col.z)/3.0 );\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  UscaleStar=glGetUniformLocation(program, "scaleStar");
  UscaleClamp=glGetUniformLocation(program, "scaleClamp");
  UinvCircleWidth=glGetUniformLocation(program, "invCircleWidth");
  Urays_innerR=glGetUniformLocation(program, "rays_innerR");
  Urays_outerR=glGetUniformLocation(program, "rays_outerR");
  Uresolution=glGetUniformLocation(program, "resolution");
  Uposition=glGetUniformLocation(program, "position");
  Urotation=glGetUniformLocation(program, "rotation");
  UconvectiveColor=glGetUniformLocation(program, "convectiveColor");
  UsurfaceStaticColor=glGetUniformLocation(program, "surfaceStaticColor");
  UsurfaceStaticColor2=glGetUniformLocation(program, "surfaceStaticColor2");
  UsurfaceColor1=glGetUniformLocation(program, "surfaceColor1");
  UsurfaceColor2=glGetUniformLocation(program, "surfaceColor2");
  UtintColor=glGetUniformLocation(program, "tintColor");
  UcoronaColor=glGetUniformLocation(program, "coronaColor");
  UplasmaColor=glGetUniformLocation(program, "plasmaColor");
  UwindColor1=glGetUniformLocation(program, "windColor1");
  UwindColor2=glGetUniformLocation(program, "windColor2");
  UwindColor3=glGetUniformLocation(program, "windColor3");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Uposition,(GLfloat)position.x,(GLfloat)position.y);
  glUniform2f(Urotation,(GLfloat)rotation.x,(GLfloat)rotation.y);
  glUniform3f(UconvectiveColor,(GLfloat)convectiveColor.rf,(GLfloat)convectiveColor.gf,(GLfloat)convectiveColor.bf);
  glUniform3f(UsurfaceStaticColor,(GLfloat)surfaceStaticColor.rf,(GLfloat)surfaceStaticColor.gf,(GLfloat)surfaceStaticColor.bf);
  glUniform3f(UsurfaceStaticColor2,(GLfloat)surfaceStaticColor2.rf,(GLfloat)surfaceStaticColor2.gf,(GLfloat)surfaceStaticColor2.bf);
  glUniform3f(UsurfaceColor1,(GLfloat)surfaceColor1.rf,(GLfloat)surfaceColor1.gf,(GLfloat)surfaceColor1.bf);
  glUniform3f(UsurfaceColor2,(GLfloat)surfaceColor2.rf,(GLfloat)surfaceColor2.gf,(GLfloat)surfaceColor2.bf);
  glUniform3f(UtintColor,(GLfloat)tintColor.rf,(GLfloat)tintColor.gf,(GLfloat)tintColor.bf);
  glUniform3f(UcoronaColor,(GLfloat)coronaColor.rf,(GLfloat)coronaColor.gf,(GLfloat)coronaColor.bf);
  glUniform3f(UplasmaColor,(GLfloat)plasmaColor.rf,(GLfloat)plasmaColor.gf,(GLfloat)plasmaColor.bf);
  glUniform3f(UwindColor1,(GLfloat)windColor1.rf,(GLfloat)windColor1.gf,(GLfloat)windColor1.bf);
  glUniform3f(UwindColor2,(GLfloat)windColor2.rf,(GLfloat)windColor2.gf,(GLfloat)windColor2.bf);
  glUniform3f(UwindColor3,(GLfloat)windColor3.rf,(GLfloat)windColor3.gf,(GLfloat)windColor3.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UscaleStar,(GLfloat)scaleStar);
  glUniform1f(UscaleClamp,(GLfloat)scaleClamp);
  glUniform1f(UinvCircleWidth,(GLfloat)invCircleWidth);
  glUniform1f(Urays_innerR,(GLfloat)rays_innerR);
  glUniform1f(Urays_outerR,(GLfloat)rays_outerR);
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( FlaringStarProfile *in ) {
  time=in->time;
  scaleStar=in->scaleStar;
  scaleClamp=in->scaleClamp;
  invCircleWidth=in->invCircleWidth;
  rays_innerR=in->rays_innerR;
  rays_outerR=in->rays_outerR;
  resolution.Set(&in->resolution);
  position.Set(&in->position);
  rotation.Set(&in->rotation);
  convectiveColor.fromCrayon(in->convectiveColor);
  surfaceStaticColor.fromCrayon(in->surfaceStaticColor);
  surfaceStaticColor2.fromCrayon(in->surfaceStaticColor2);
  surfaceColor1.fromCrayon(in->surfaceColor1);
  surfaceColor2.fromCrayon(in->surfaceColor2);
  tintColor.fromCrayon(in->tintColor);
  coronaColor.fromCrayon(in->coronaColor);
  plasmaColor.fromCrayon(in->plasmaColor);
  windColor1.fromCrayon(in->windColor1);
  windColor2.fromCrayon(in->windColor2);
  windColor3.fromCrayon(in->windColor3);
 }
 void Render( FBOColor *out, FlaringStarProfile *in );
};


extern FlaringStarShader flaringstarShader;

void ShadedFlaringStar( FBOColor *out, FlaringStarProfile *in );


// ----------------------------------------------------

//void ShadedExplosion( FBOColor *out );
//
//class ExplosionShader : public GLSLShader {
//public:
// void Init() {
//  OUTPUT("ExplosionShader::Loading\n");
// // Simple passthrough vertex shader
// // Simple passthrough vertex shader
//  string vert=
//    string("#version 120\n")
//   +string("#ifdef GL_ES\n")
//   +string("precision highp float;\n")  //  string("//precision mediump float;\n")
//   +string("#endif\n")
//   +string("varying vec2 v_vTexcoord;\n")
//   +string("varying vec4 v_vColour;\n")
//   +string("uniform sampler2D tex;\n")
//   +string("")
//   +string("void main() {\n")
//   +string(" gl_Position = ftransform();\n")
//   +string(" gl_TexCoord[0]=gl_MultiTexCoord0;\n")
//   +string(" v_vColour = gl_Color;\n")
//   +string(" v_vTexcoord = gl_TexCoord[0].st;\n")
//   +string("}\n")
//  ;
//  string frag=string(
//  );
//  loadShaders(vert.c_str(),frag.c_str());
//  InitUniforms();
// }
// void InitUniforms() {
// }
// void UpdateUniforms() {
//  glUseProgram(program);
// }
// void Disable() {
//  glBindTexture(GL_TEXTURE_2D, 0);
//  glUseProgram(0);
// }
//};



ONE(ElectronCloudsProfile,{
 iterations=4;
 formuparam2=0.89;
 volsteps=10;
 stepsize=0.190;
 zoom=3.9;
 tile=0.450;
 speed2=0.001;
 brightness=0.2;
 darkmatter=0.4;
 distfading=0.56;
 saturation=0.4;
 transverseSpeed=0.001;
 cloud=0.2;
 col1.Pick(black);
 col2.Pick(black);
 resolution.Set(1.0,1.0);
})
 Zint iterations,volsteps;
 Zdouble formuparam2,stepsize,zoom,tile,speed2,brightness,darkmatter,distfading,saturation,transverseSpeed,cloud,time;
 Crayon col1,col2;
 Cartesiand resolution,pos2d,move2d;
 BINARY_IN({
  B_IN_NUM(iterations)
  B_IN_NUM(volsteps)
  B_IN_NUM(formuparam2)
  B_IN_NUM(stepsize)
  B_IN_NUM(zoom)
  B_IN_NUM(tile)
  B_IN_NUM(speed2)
  B_IN_NUM(brightness)
  B_IN_NUM(darkmatter)
  B_IN_NUM(distfading)
  B_IN_NUM(saturation)
  B_IN_NUM(transverseSpeed)
  B_IN_NUM(cloud)
  B_IN_NUM(time)
  B_IN_TINT(col1)
  B_IN_TINT(col2)
  B_IN_SSUB(resolution)
  B_IN_SSUB(pos2d)
  B_IN_SSUB(move2d)
 })
 BINARY_OUT({
  B_OUT_NUM(iterations)
  B_OUT_NUM(volsteps)
  B_OUT_NUM(formuparam2)
  B_OUT_NUM(stepsize)
  B_OUT_NUM(zoom)
  B_OUT_NUM(tile)
  B_OUT_NUM(speed2)
  B_OUT_NUM(brightness)
  B_OUT_NUM(darkmatter)
  B_OUT_NUM(distfading)
  B_OUT_NUM(saturation)
  B_OUT_NUM(transverseSpeed)
  B_OUT_NUM(cloud)
  B_OUT_NUM(time)
  B_OUT_TINT(col1)
  B_OUT_TINT(col2)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(move2d)
 })
 void Set( ElectronCloudsProfile *in ) {
  iterations=in->iterations;
  formuparam2=in->formuparam2;
  volsteps=in->volsteps;
  stepsize=in->stepsize;
  zoom=in->zoom;
  tile=in->tile;
  speed2=in->speed2;
  brightness=in->brightness;
  darkmatter=in->darkmatter;
  distfading=in->distfading;
  saturation=in->saturation;
  transverseSpeed=in->transverseSpeed;
  cloud=in->cloud;
  time=in->time;
  col1.fromCrayon(in->col1);
  col2.fromCrayon(in->col2);
  resolution.Set(&in->resolution);
  pos2d.Set(&in->pos2d);
  move2d.Set(&in->move2d);
 }
 KEYWORDS({
  KEYWORD("iterations",{iterations=w;})
  else KEYWORD("formuparam2",{formuparam2=w;})
  else KEYWORD("volsteps",{volsteps=w;})
  else KEYWORD("stepsize",{stepsize=w;})
  else KEYWORD("zoom",{zoom=w;})
  else KEYWORD("tile",{tile=w;})
  else KEYWORD("speed2",{speed2=w;})
  else KEYWORD("brightness",{brightness=w;})
  else KEYWORD("darkmatter",{darkmatter=w;})
  else KEYWORD("distfading",{distfading=w;})
  else KEYWORD("saturation",{saturation=w;})
  else KEYWORD("transverseSpeed",{transverseSpeed=w;})
  else KEYWORD("cloud",{cloud=w;})
  else KEYWORD("time",{time=w;})
  else TINTWORD("col1",col1)
  else TINTWORD("col2",col2)
  else SUBWORD("resolution",resolution)
  else SUBWORD("pos2d",pos2d)
  else SUBWORD("move2d",move2d)
 })
 WORDKEYS({
  NUMBKEY("iterations",iterations)
  NUMBKEY("formuparam2",formuparam2)
  NUMBKEY("volsteps",volsteps)
  NUMBKEY("stepsize",stepsize)
  NUMBKEY("zoom",zoom)
  NUMBKEY("tile",tile)
  NUMBKEY("speed2",speed2)
  NUMBKEY("brightness",brightness)
  NUMBKEY("darkmatter",darkmatter)
  NUMBKEY("distfading",distfading)
  NUMBKEY("saturation",saturation)
  NUMBKEY("transverseSpeed",transverseSpeed)
  NUMBKEY("cloud",cloud)
  NUMBKEY("time",time)
  TINTKEY("col1",col1)
  TINTKEY("col2",col2)
  SSUBKEY("resolution",resolution)
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("move2d",move2d)
 })
MANY(ElectronCloudsProfile,ElectronCloudsProfileHandle,ElectronCloudsProfileHandles,"ElectronClouds",ElectronCloudsProfiles,{})
 KEYWORDSGroup("ElectronClouds",ElectronCloudsProfile);
 WORDKEYSGroup("ElectronClouds",ElectronCloudsProfile);
DONE(ElectronCloudsProfile);

class ElectronCloudsShader : public GLSLShader {
public:
  Zint iterations,volsteps;
  Zdouble formuparam2,stepsize,zoom,tile,speed2,brightness,darkmatter,distfading,saturation,transverseSpeed,cloud,time;
  Crayon col1,col2;
  Cartesiand resolution,move2d,pos2d;
 GLuint Umove2d,Uiterations,Uformuparam2,Uvolsteps,Ustepsize,Uzoom,Utile,Uspeed2,Ubrightness,Udarkmatter,Udistfading,Usaturation,UtransverseSpeed,Ucloud,Utime,Ucol1,Ucol2,Uresolution,Upos2d;
 void Init() {
  OUTPUT("ElectronCloudsShader::Loading\n");
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
   +string("uniform int iterations; // 4\n")
   +string("uniform float formuparam2; // 0.89\n")
   +string("uniform int volsteps; // 10\n")
   +string("uniform float stepsize; // 0.190\n")
   +string("uniform float zoom; // 3.900\n")
   +string("uniform float tile; //   0.450\n")
   +string("uniform float speed2; //  0.0010\n")
   +string("uniform float brightness; // 0.2\n")
   +string("uniform float darkmatter; // 0.400\n")
   +string("uniform float distfading; // 0.560\n")
   +string("uniform float saturation; // 0.400\n")
   +string("uniform float transverseSpeed; // 1.1\n")
   +string("uniform float cloud; //0.2\n")
   +string("uniform vec4 col1;\n")
   +string("uniform vec4 col2;\n")
   +string("uniform float time;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform vec2 pos2d;\n")
   +string("uniform vec2 move2d;\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("varying vec4 v_vColour;\n")
   +string("float triangle(float x, float a) {\n")
   +string(" float output2 = 2.0 * abs(3.0 * ((x / a) - floor((x / a) + 0.5))) - 1.0;\n")
   +string(" return output2;\n")
   +string("}\n")
   +string("float field(in vec3 p) {\n")
   +string(" float strength = 7.0 + 0.03 * log(1.e-6 + fract(sin(time) * 4373.11));\n")
   +string(" float accum = 0.;\n")
   +string(" float prev = 0.;\n")
   +string(" float tw = 0.; \n")
   +string(" for (int i = 0; i < 6; ++i) {\n")
   +string("  float mag = dot(p, p);\n")
   +string("  p = abs(p) / mag + vec3(-.5, -.8 + 0.1 * sin(time * 0.2 + 2.0), -1.1 + 0.3 * cos(time * 0.15));\n")
   +string("  float w = exp(-float(i) / 7.);\n")
   +string("  accum += w * exp(-strength * pow(abs(mag - prev), 2.3));\n")
   +string("  tw += w;\n")
   +string("  prev = mag;\n")
   +string(" }\n")
   +string(" return max(0., 5. * accum / tw - .7);\n")
   +string("}\n")
   +string("void main() {\n")
   +string(" vec2 uv2 = pos2d.xy + v_vTexcoord.xy / resolution.xy - 1.;\n")
   +string(" vec2 uvs = uv2 * resolution.xy;\n")
   +string(" float time1=time/100.0;\n")
   +string(" float time2 = time/10.0;\n")
   +string(" float speed = speed2;\n")
   +string(" float formuparam = formuparam2;\n")
   +string(" vec2 uv = uvs;\n")
   +string(" float a_xz = col2.x;// 0.9;\n")
   +string(" float a_yz = col2.y;//-.6;\n")
   +string(" float a_xy = col2.z /*0.9*/ + time*0.04;\n")
   +string(" mat2 rot_xz = mat2(cos(a_xz),sin(a_xz),-sin(a_xz),cos(a_xz));\n")
   +string(" mat2 rot_yz = mat2(cos(a_yz),sin(a_yz),-sin(a_yz),cos(a_yz));\n")
   +string(" mat2 rot_xy = mat2(cos(a_xy),sin(a_xy),-sin(a_xy),cos(a_xy));\n")
   +string(" float v2 =1.0;\n")
   +string(" vec3 dir=vec3((uv*zoom)/2.0,1.);\n")
   +string(" vec3 from=vec3(0.0, 0.0,0.0);\n")
   +string(" from.x -= 5.0* (0.5);\n")
   +string(" from.y -= 5.0* (0.5);\n")
   +string(" vec3 forward = vec3(0.,0.,1.);\n")
   +string(" from.x += transverseSpeed*(1.0)*cos(0.01*time1/10.0) + 0.001*time1/10.0;\n")
   +string(" from.y += transverseSpeed*(1.0)*sin(0.01*time1/10.0) +0.001*time1/10.0;\n")
   +string(" from.z += 0.003*time1;\n")
   +string(" dir.xy*=rot_xy;\n")
   +string(" forward.xy *= rot_xy;\n")
   +string(" dir.xz*=rot_xz;\n")
   +string(" forward.xz *= rot_xz;\n")
   +string(" dir.yz*= rot_yz;\n")
   +string(" forward.yz *= rot_yz;\n")
   +string(" from.xy*=-rot_xy/10.0;\n")
   +string(" from.xz*=rot_xz/10.0;\n")
   +string(" from.yz*= rot_yz/10.0;\n")
   +string(" //zoom\n")
   +string(" float zooom = (time2-3311.)*speed;\n")
   +string(" from += forward*zooom;\n")
   +string(" float sampleShift = mod( zooom, stepsize );\n")
   +string(" float zoffset = -sampleShift;\n")
   +string(" sampleShift /= stepsize; // make from 0 to 1\n")
   +string(" //volumetric rendering\n")
   +string(" float s=0.24;\n")
   +string(" float s3 = s + stepsize/2.0;\n")
   +string(" vec3 v=vec3(0.);\n")
   +string(" float t3 = 0.0;\n")
   +string(" vec3 backCol2 = vec3(0.);\n")
   +string(" for (int r=0; r<volsteps; r++) {\n")
   +string("  vec3 p2=from+(s+zoffset)*dir;// + vec3(0.,0.,zoffset);\n")
   +string("  vec3 p3=(from+(s3+zoffset)*dir )* (1.9/zoom);// + vec3(0.,0.,zoffset);\n")
   +string("  p2 = abs(vec3(tile)-mod(p2,vec3(tile*2.))); // tiling fold\n")
   +string("  p3 = abs(vec3(tile)-mod(p3,vec3(tile*2.))); // tiling fold\n")
   +string("  t3 = field(p3);\n")
   +string("  float pa,a=pa=0.;\n")
   +string("  for (int i=0; i<iterations; i++) {\n")
   +string("   p2=abs(p2)/dot(p2,p2)-formuparam; // the magic formula\n")
   +string("   //p=abs(p)/max(dot(p,p),0.005)-formuparam; // another interesting way to reduce noise\n")
   +string("   float D = abs(length(p2)-pa); // absolute sum of average change\n")
   +string("   if (i > 2) a += i > 7 ? min( 12., D) : D;\n")
   +string("   pa=length(p2);\n")
   +string("  }\n")
   +string("  a*=a*a; // add contrast\n")
   +string("  // brightens stuff up a bit\n")
   +string("  float s1 = s+zoffset;\n")
   +string("  float fade = pow(distfading,max(0.,float(r)-sampleShift));\n")
   +string("  v+=fade;\n")
   +string("  // fade out samples as they approach the camera\n")
   +string("  if( r == 0 ) fade *= (1. - (sampleShift));\n")
   +string("  // fade in samples as they approach from the distance\n")
   +string("  if( r == volsteps-1 )fade *= sampleShift;\n")
   +string("  v+=vec3(s1,s1*s1,s1*s1*s1*s1)*a*brightness*fade; // coloring based on distance\n")
   +string("  backCol2 += mix(.4, 1., v2) * vec3(0.20 * t3 * t3 * t3, 0.4 * t3 * t3, t3 * 0.7) * fade;\n")
   +string("  s+=stepsize;\n")
   +string("  s3 += stepsize;\n")
   +string(" }\n")
   +string(" v=mix(vec3(length(v)),v,saturation);\n")
   +string(" vec4 forCol2 = vec4(v*.01,1.);\n")
   +string(" backCol2 *= cloud;\n")
   +string(" gl_FragColor = col1 + forCol2 + vec4(backCol2, 1.0);\n")
   +string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uiterations=glGetUniformLocation(program, "iterations");
  Uformuparam2=glGetUniformLocation(program, "formuparam2");
  Uvolsteps=glGetUniformLocation(program, "volsteps");
  Ustepsize=glGetUniformLocation(program, "stepsize");
  Uzoom=glGetUniformLocation(program, "zoom");
  Utile=glGetUniformLocation(program, "tile");
  Uspeed2=glGetUniformLocation(program, "speed2");
  Ubrightness=glGetUniformLocation(program, "brightness");
  Udarkmatter=glGetUniformLocation(program, "darkmatter");
  Udistfading=glGetUniformLocation(program, "distfading");
  Usaturation=glGetUniformLocation(program, "saturation");
  UtransverseSpeed=glGetUniformLocation(program, "transverseSpeed");
  Ucloud=glGetUniformLocation(program, "cloud");
  Utime=glGetUniformLocation(program, "time");
  Ucol1=glGetUniformLocation(program, "col1");
  Ucol2=glGetUniformLocation(program, "col2");
  Uresolution=glGetUniformLocation(program, "resolution");
  Upos2d=glGetUniformLocation(program, "pos2d");
  Umove2d=glGetUniformLocation(program, "move2d");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1i(Uiterations,(GLint)(int)iterations);
  glUniform1i(Uvolsteps,(GLint)(int)volsteps);
  glUniform4f(Ucol1,(GLfloat)col1.r,(GLfloat)col1.g,(GLfloat)col1.b,(GLfloat)col1.a);
  glUniform4f(Ucol2,(GLfloat)col2.r,(GLfloat)col2.g,(GLfloat)col2.b,(GLfloat)col2.a);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform2f(Umove2d,(GLfloat)move2d.x,(GLfloat)move2d.y);
  glUniform1f(Uformuparam2,(GLfloat)formuparam2);
  glUniform1f(Ustepsize,(GLfloat)stepsize);
  glUniform1f(Uzoom,(GLfloat)zoom);
  glUniform1f(Utile,(GLfloat)tile);
  glUniform1f(Uspeed2,(GLfloat)speed2);
  glUniform1f(Ubrightness,(GLfloat)brightness);
  glUniform1f(Udarkmatter,(GLfloat)darkmatter);
  glUniform1f(Udistfading,(GLfloat)distfading);
  glUniform1f(Usaturation,(GLfloat)saturation);
  glUniform1f(UtransverseSpeed,(GLfloat)transverseSpeed);
  glUniform1f(Ucloud,(GLfloat)cloud);
  glUniform1f(Utime,(GLfloat)time);
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( ElectronCloudsProfile *in ) {
  iterations=in->iterations;
  volsteps=in->volsteps;
  formuparam2=in->formuparam2;
  stepsize=in->stepsize;
  zoom=in->zoom;
  tile=in->tile;
  speed2=in->speed2;
  brightness=in->brightness;
  darkmatter=in->darkmatter;
  distfading=in->distfading;
  saturation=in->saturation;
  transverseSpeed=in->transverseSpeed;
  cloud=in->cloud;
  time=in->time;
  col1.fromCrayon(in->col1);
  col2.fromCrayon(in->col2);
  resolution.Set(&in->resolution);
  pos2d.Set(&in->pos2d);
 }
 void Render( FBOColor *out, ElectronCloudsProfile *in );
};


extern ElectronCloudsShader electroncloudsShader;

void ShadedElectronClouds( FBOColor *out, ElectronCloudsProfile *in );



ONE(ClassicPlanetProfile,{
  resolution.Set(1.0,1.0);
  tex1=library.Load("data/images/textures/tile12.png",tllto_Tiling);
  tex2=library.Load("data/images/textures/tile13.png",tllto_Tiling);
  tex3=library.Load("data/images/textures/tile14.png",tllto_Tiling);
  tex4=library.Load("data/images/textures/tile15.png",tllto_Tiling);
  sF.Set(1.0,100.0,90.0); sF.w=45.0;
  nT.Set(2.0,3.0,4.0); nT.w=5.0;
  striations.Set(0.0,0.8);
  noiseform.Set(3.0,2.0,3.0); noiseform.w=2.0;
  nf2.Set(4.0,8.0,0.66); nf2.w=0.34;
  nf3.Set(0.0,41.0,289.0); nf3.w=330.0;
  nf5.Set(10.0,0.5);
  edgeGlow=0.001;
  noisescale=0.005;
  colMixer.Set(0.5,0.5,0.5); colMixer.w=0.5;
  colorA.Double(0.15,0.05,1.0,1.0);
  colorB.Double(1.0,0.1,0.02,1.0);
  e.Set(0.1001,0.0);
  nf4.Set(12.0,48.0);
})
 Cartesiand pos2d,resolution,sF,nT,striations,noiseform,nf2,nf3,nf5,colMixer,e,nf4;
 Zp<GLImage> tex1,tex2,tex3,tex4;
 Zdouble seed,edgeGlow,noisescale;
 Crayon colorA,colorB;
 BINARY_IN({
  B_IN_SSUB(pos2d)
  B_IN_SSUB(resolution)
  B_IN_SSUB(sF)
  B_IN_SSUB(nT)
  B_IN_SSUB(striations)
  B_IN_SSUB(noiseform)
  B_IN_SSUB(nf2)
  B_IN_SSUB(nf3)
  B_IN_SSUB(nf5)
  B_IN_SSUB(colMixer)
  B_IN_SSUB(e)
  B_IN_SSUB(nf4)
  B_IN_IMAGE(tex1)
  B_IN_IMAGE(tex2)
  B_IN_IMAGE(tex3)
  B_IN_IMAGE(tex4)
  B_IN_NUM(seed)
  B_IN_NUM(edgeGlow)
  B_IN_NUM(noisescale)
  B_IN_TINT(colorA)
  B_IN_TINT(colorB)
 })
 BINARY_OUT({
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(resolution)
  B_OUT_SSUB(sF)
  B_OUT_SSUB(nT)
  B_OUT_SSUB(striations)
  B_OUT_SSUB(noiseform)
  B_OUT_SSUB(nf2)
  B_OUT_SSUB(nf3)
  B_OUT_SSUB(nf5)
  B_OUT_SSUB(colMixer)
  B_OUT_SSUB(e)
  B_OUT_SSUB(nf4)
  B_OUT_IMAGE(tex1)
  B_OUT_IMAGE(tex2)
  B_OUT_IMAGE(tex3)
  B_OUT_IMAGE(tex4)
  B_OUT_NUM(seed)
  B_OUT_NUM(edgeGlow)
  B_OUT_NUM(noisescale)
  B_OUT_TINT(colorA)
  B_OUT_TINT(colorB)
 })
 KEYWORDS({
  SUBWORD("pos2d",pos2d)
  else SUBWORD("resolution",resolution)
  else SUBWORD("sF",sF)
  else SUBWORD("nT",nT)
  else SUBWORD("striations",striations)
  else SUBWORD("noiseform",noiseform)
  else SUBWORD("nf2",nf2)
  else SUBWORD("nf3",nf3)
  else SUBWORD("nf5",nf5)
  else SUBWORD("colMixer",colMixer)
  else SUBWORD("e",e)
  else SUBWORD("nf4",nf4)
  else TILEWORD("tex1",tex1)
  else TILEWORD("tex2",tex2)
  else TILEWORD("tex3",tex3)
  else TILEWORD("tex4",tex4)
  else KEYWORD("seed",{seed=w;})
  else KEYWORD("edgeGlow",{edgeGlow=w;})
  else KEYWORD("noisescale",{noisescale=w;})
  else TINTWORD("colorA",colorA)
  else TINTWORD("colorB",colorB)
 })
 WORDKEYS({
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("resolution",resolution)
  SSUBKEY("sF",sF)
  SSUBKEY("nT",nT)
  SSUBKEY("striations",striations)
  SSUBKEY("noiseform",noiseform)
  SSUBKEY("nf2",nf2)
  SSUBKEY("nf3",nf3)
  SSUBKEY("nf5",nf5)
  SSUBKEY("colMixer",colMixer)
  SSUBKEY("e",e)
  SSUBKEY("nf4",nf4)
  IMGKEY("tex1",tex1)
  IMGKEY("tex2",tex2)
  IMGKEY("tex3",tex3)
  IMGKEY("tex4",tex4)
  NUMBKEY("seed",seed)
  NUMBKEY("edgeGlow",edgeGlow)
  NUMBKEY("noisescale",noisescale)
  TINTKEY("colorA",colorA)
  TINTKEY("colorB",colorB)
 })
MANY(ClassicPlanetProfile,ClassicPlanetProfileHandle,ClassicPlanetProfileHandles,"ClassicPlanet",ClassicPlanetProfiles,{})
 KEYWORDSGroup("ClassicPlanet",ClassicPlanetProfile);
 WORDKEYSGroup("ClassicPlanet",ClassicPlanetProfile);
 BINARY_INOUT(ClassicPlanetProfile)
DONE(ClassicPlanetProfile);

class ClassicPlanetShader : public GLSLShader {
public:
  Cartesiand pos2d,resolution,sF,nT,striations,noiseform,nf2,nf3,nf5,colMixer,e,nf4;
  Zp<GLuint> tex1,tex2,tex3,tex4;
  Zdouble seed,edgeGlow,noisescale;
  Crayon colorA,colorB;
 GLuint Upos2d,Uresolution,UsF,UnT,Ustriations,Unoiseform,Unf2,Unf3,Unf5,UcolMixer,Ue,Unf4,Utex1,Utex2,Utex3,Utex4,Useed,UedgeGlow,Unoisescale,UcolorA,UcolorB;
 void Init() {
  OUTPUT("ClassicPlanetShader::Loading\n");
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
    string("uniform vec2 pos2d;\n")
   +string("uniform vec2 resolution;\n")
   +string("uniform sampler2D tex1;\n")
   +string("uniform sampler2D tex2;\n")
   +string("uniform sampler2D tex3;\n")
   +string("uniform sampler2D tex4;\n")
   +string("uniform float seed; //=0.0;\n")
   +string("uniform vec4 sF; //=vec4(1.0,100.0,90.0,45.0);\n")
   +string("uniform vec4 nT; //=vec4(2.0,3.0,4.0,5.0);\n")
   +string("uniform vec2 striations; //=vec2(0.0,0.8);\n")
   +string("uniform vec4 noiseform; //=vec4(3.0,2.0,3.0,2.0);\n")
   +string("uniform vec4 nf2; //=vec4(4.0,8.0,0.66,0.34);\n")
   +string("uniform vec4 nf3; //=vec4(0,41,289,330);\n")
   +string("uniform vec2 nf5; //=vec2(10.0,0.5);\n")
   +string("uniform float edgeGlow; //=0.001;\n")
   +string("uniform float noisescale; //=0.005;\n")
   +string("uniform vec4 colMixer; //=vec4(0.5,0.5,0.5,0.5);\n")
   +string("uniform vec3 colorA; //=vec3(0.15, 0.05, 1);\n")
   +string("uniform vec3 colorB; //=vec3(1., 0.1, 0.02);\n")
   +string("uniform vec2 e; // = vec2(0.1001, 0); \n")
   +string("uniform vec2 nf4; //=vec2(12., 48.);\n")
   +string("varying vec2 v_vTexcoord;\n")
   +string("varying vec4 v_vColour;\n")
   +string("float rand(float n){return fract(sin(n) * 43758.5453123);}\n")
   +string("float rand(vec2 c){return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);}\n")
   +string("float valueNoise2D(vec2 p, float t) {\n")
   +string(" vec2 f = fract(p); // Fractional cell position.\n")
   +string(" f *= f*(noiseform.z-noiseform.w*f);// Smooth step\n")
   +string(" vec4 h = fract(sin(nf3 + dot(floor(p), nf3.yz))*43758.5453);\n")
   +string(" h = cos(h*6.283 + t)*0.5 + 0.5; // Animation.\n")
   +string(" return dot(vec2(1.0-f.y, f.y), vec2(1.0-f.x, f.x)*mat2(h));\n")
   +string("}\n")
   +string("float func2D(vec2 p, float t){ return valueNoise2D(p*nf2.x,t)*nf2.z + valueNoise2D(p*nf2.y,t)*nf2.w; }\n")
   +string("float smoothFract(float x, float sf){\n")
   +string("    x = fract(x);\n")
   +string("    return min(x, x*(1.-x)*sf);\n")
   +string("}\n")
   +string("vec4 noise2d( in vec2 coord, float t ) {\n")
   +string(" vec2 uv = coord.xy * noisescale;\n")
   +string(" float f = func2D(uv,t); // Range [0, 1]\n")
   +string(" float g = length( vec2(f - func2D(uv-e.xy,t), f - func2D(uv-e.yx,t)) )/(e.x);\n")
   +string(" g = f/max(g, edgeGlow);\n")
   +string(" float c = smoothFract(f*noiseform.x, g*resolution.y/noiseform.y); // Range [0, 1]\n")
   +string(" vec3 col = vec3(c);\n")
   +string(" f = f*noiseform.x;\n")
   +string(" float tx = smoothstep(striations.x,striations.y, (func2D((uv + (1. - c)*.01)*nf4,t))); // Range: [0, 1]\n")
   +string(" if(f>nT.x && f<nT.y) col *= colorB*(tx); // Red.\n")
   +string(" else if(f>nT.z && f<nT.w) col *= colorA*(tx); // Blue.\n")
   +string(" else col *= abs(tx - .5)*.4 + .8; // White.\n")
   +string(" col += min(g*g*g*g*.1, 1.)*(1.-col);\n")
   +string(" return vec4(sqrt(clamp(col, 0., 1.)), 1.0 );\n")
   +string("}\n")
   +string("float noise(vec2 n) {\n")
   +string(" const vec2 d = vec2(0.0, 1.0);\n")
   +string(" vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));\n")
   +string(" return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);\n")
   +string("}\n")
   +string("void main()\n")
   +string("{\n")
   +string(" vec2 uv = pos2d.xy + v_vTexcoord.xy / resolution.xy;\n")
   +string(" vec4 col1,col2,col3,col4,col5,col6;\n")
   +string(" col5=noise2d(uv*nf5.x,seed);\n")
   +string(" col6=vec4(noise(uv*nf5.y)); col6.a=1.0;\n")
   +string(" col1=texture2D(tex1,uv/sF.x);\n")
   +string(" col2=texture2D(tex2,uv/sF.y)*(col5+col6)/2.0;\n")
   +string(" col3=texture2D(tex3,uv/sF.z)+col5;\n")
   +string(" col4=texture2D(tex4,uv/sF.w)*colMixer.w;\n")
   +string(" if ( col3.r < 15.0/255.0 && col3.r > 0.0 ) col2/=col5.r;\n")
   +string(" vec4 col=vec4(0,0,0,1.);\n")
   +string(" if ( col3.r > colMixer.x ) col=col1;\n")
   +string(" else col=col1;\n")
   +string(" if ( col4.r > colMixer.y ) col=col2;\n")
   +string(" else col+=col4;\n")
   +string(" vec4 fcol=mix(col,col5,colMixer.z);\n")
   +string(" fcol.a=1.0;\n")
   +string(" gl_FragColor=fcol;\n")
   +string("}\n")
   +string("\n")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Upos2d=glGetUniformLocation(program, "pos2d");
  Uresolution=glGetUniformLocation(program, "resolution");
  UsF=glGetUniformLocation(program, "sF");
  UnT=glGetUniformLocation(program, "nT");
  Ustriations=glGetUniformLocation(program, "striations");
  Unoiseform=glGetUniformLocation(program, "noiseform");
  Unf2=glGetUniformLocation(program, "nf2");
  Unf3=glGetUniformLocation(program, "nf3");
  Unf5=glGetUniformLocation(program, "nf5");
  UcolMixer=glGetUniformLocation(program, "colMixer");
  Ue=glGetUniformLocation(program, "e");
  Unf4=glGetUniformLocation(program, "nf4");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utex4=glGetUniformLocation(program, "tex4");
  Useed=glGetUniformLocation(program, "seed");
  UedgeGlow=glGetUniformLocation(program, "edgeGlow");
  Unoisescale=glGetUniformLocation(program, "noisescale");
  UcolorA=glGetUniformLocation(program, "colorA");
  UcolorB=glGetUniformLocation(program, "colorB");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform4f(UsF,(GLfloat)sF.x,(GLfloat)sF.y,(GLfloat)sF.z,(GLfloat)sF.w);
  glUniform4f(UnT,(GLfloat)nT.x,(GLfloat)nT.y,(GLfloat)nT.z,(GLfloat)nT.w);
  glUniform2f(Ustriations,(GLfloat)striations.x,(GLfloat)striations.y);
  glUniform4f(Unoiseform,(GLfloat)noiseform.x,(GLfloat)noiseform.y,(GLfloat)noiseform.z,(GLfloat)noiseform.w);
  glUniform4f(Unf2,(GLfloat)nf2.x,(GLfloat)nf2.y,(GLfloat)nf2.z,(GLfloat)nf2.w);
  glUniform4f(Unf3,(GLfloat)nf3.x,(GLfloat)nf3.y,(GLfloat)nf3.z,(GLfloat)nf3.w);
  glUniform2f(Unf5,(GLfloat)nf5.x,(GLfloat)nf5.y);
  glUniform4f(UcolMixer,(GLfloat)colMixer.x,(GLfloat)colMixer.y,(GLfloat)colMixer.z,(GLfloat)colMixer.w);
  glUniform2f(Ue,(GLfloat)e.x,(GLfloat)e.y);
  glUniform2f(Unf4,(GLfloat)nf4.x,(GLfloat)nf4.y);
  glUniform3f(UcolorA,(GLfloat)colorA.rf,(GLfloat)colorA.gf,(GLfloat)colorA.bf);
  glUniform3f(UcolorB,(GLfloat)colorB.rf,(GLfloat)colorB.gf,(GLfloat)colorB.bf);
  glUniform1f(Useed,(GLfloat)seed);
  glUniform1f(UedgeGlow,(GLfloat)edgeGlow);
  glUniform1f(Unoisescale,(GLfloat)noisescale);
  glUniform1i(Utex1, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex1.pointer));
  glUniform1i(Utex2, 1);  glActiveTexture(GL_TEXTURE1);  glBindTexture(GL_TEXTURE_2D, (*tex2.pointer));
  glUniform1i(Utex3, 2);  glActiveTexture(GL_TEXTURE2);  glBindTexture(GL_TEXTURE_2D, (*tex3.pointer));
  glUniform1i(Utex4, 3);  glActiveTexture(GL_TEXTURE3);  glBindTexture(GL_TEXTURE_2D, (*tex4.pointer));
  glUniform1i(Utex1, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex1.pointer));
  glUniform1i(Utex2, 1);  glActiveTexture(GL_TEXTURE1);  glBindTexture(GL_TEXTURE_2D, (*tex2.pointer));
  glUniform1i(Utex3, 2);  glActiveTexture(GL_TEXTURE2);  glBindTexture(GL_TEXTURE_2D, (*tex3.pointer));
  glUniform1i(Utex4, 3);  glActiveTexture(GL_TEXTURE3);  glBindTexture(GL_TEXTURE_2D, (*tex4.pointer));
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( ClassicPlanetProfile *in ) {
  pos2d.Set(&in->pos2d);
  resolution.Set(&in->resolution);
  sF.Set(&in->sF);
  nT.Set(&in->nT);
  striations.Set(&in->striations);
  noiseform.Set(&in->noiseform);
  nf2.Set(&in->nf2);
  nf3.Set(&in->nf3);
  nf5.Set(&in->nf5);
  colMixer.Set(&in->colMixer);
  e.Set(&in->e);
  nf4.Set(&in->nf4);
  tex1=&in->tex1->texture;
  tex2=&in->tex2->texture;
  tex3=&in->tex3->texture;
  tex4=&in->tex4->texture;
  seed=in->seed;
  edgeGlow=in->edgeGlow;
  noisescale=in->noisescale;
  colorA.fromCrayon(in->colorA);
  colorB.fromCrayon(in->colorB);
 }
 void Render( FBOColor *out, ClassicPlanetProfile *in );
};

extern ClassicPlanetShader classicplanetShader;

void ShadedClassicPlanet( FBOColor *out, ClassicPlanetProfile *in );