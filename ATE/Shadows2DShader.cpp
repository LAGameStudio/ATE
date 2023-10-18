#include "Shadows2DShader.h"

Light2DShader light2dShader;
Lights2DShader lights2dShader;
LightsNoRadius2DShader lightsnoradius2dShader;
LightsNoColor2DShader lightsnocolor2dShader;
LightsPulsing2DShader lightspulsing2dShader;
Shadows2DShader shadows2dShader;
Shadow2DShader shadow2dShader;
MultiShadow2DShader multishadow2dShader;

void Shadow2DShader::Render( FBOColor *out, Shadow2DProfile *in ) {
 out->Bind();
 shadow2dShader.Set(in);
 shadow2dShader.UpdateUniforms();
 out->QuadYInverted();
 shadow2dShader.Disable();
 out->Unbind();
}

void ShadedShadow2D( FBOColor *out, Shadow2DProfile *in ) {
 out->Bind();
 shadow2dShader.Set(in);
 shadow2dShader.UpdateUniforms();
 out->QuadYInverted();
 shadow2dShader.Disable();
 out->Unbind();
}

void ShadedShadow2D( FBOColor *out, Shadow2DProfile *in, GLuint *color, GLuint *addlight, GLuint *light, GLuint *emap, GLuint *back, GLuint *height ) {
 out->Bind();
 shadow2dShader.pos.Set(&in->pos);
 shadow2dShader.ambience.fromCrayon(in->ambience);
 shadow2dShader.colormap.pointer=color;
 shadow2dShader.heightmap.pointer=height;
 shadow2dShader.lightmap.pointer=light;
 shadow2dShader.emap.pointer=emap;
 shadow2dShader.addlightmap.pointer=addlight;
 shadow2dShader.backlight.pointer=back;
 shadow2dShader.UpdateUniforms();
 out->QuadYInverted();
 shadow2dShader.Disable();
 out->Unbind();
}

//----

void MultiShadow2DShader::Init() {
 OUTPUT("MultiShadow2DShader::Loading\n");
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
   string("varying vec4 v_vColour;\n")
  +string("varying vec2 v_vTexcoord;\n")
  +string("const int num = 256;\n")
  +string("const float recip = 1.0/float(num);\n")
  +string("\n")
  +string("uniform vec3 pos;\n")
  +string("uniform vec3 pos1;\n")
  +string("uniform vec3 pos2;\n")
  +string("uniform vec3 pos3;\n")
  +string("uniform vec3 pos4;\n")
  +string("uniform vec3 pos5;\n")
  +string("uniform vec3 pos6;\n")
  +string("uniform vec3 pos7;\n")
  +string("uniform vec3 ambience;\n")
  +string("uniform int count;\n")
  +string("uniform sampler2D colormap;\n")
  +string("uniform sampler2D lightmap;\n")
  +string("uniform sampler2D emap;\n")
  +string("uniform sampler2D addlightmap;\n")
  +string("uniform sampler2D backlight;\n")
  +string("uniform sampler2D heightmap;\n")
  +string("float shade( vec3 p, float height )\n")
  +string("{\n")
  +string("    float shadow=0.0;\n")
  +string("    for(int Z=0;Z<num;Z+=2)\n")
  +string("        {\n")
  +string("            float Zr=float(Z)*recip;\n")
  +string("            shadow += clamp(texture2D(heightmap,v_vTexcoord * (1.0-Zr)\n")
  +string("            + (Zr*p.xy*0.5)).r - Zr - height,0.0,1.0);\n")
  +string("        }\n")
  +string("    return shadow;\n") // clamp(shadow,0.0,1.0);\n")
  +string("}\n")
  +string("vec4 image() {\n")
  +string("    float height = texture2D(heightmap,v_vTexcoord).r-(1.0-pos.z);\n")
  +string("    vec4 BS=texture2D(backlight,v_vTexcoord);")
  +string("    float BL=(BS.r+BS.g+BS.b)/3.0;\n")
  +string("    vec4 CM=texture2D(colormap,v_vTexcoord);\n")
  +string("    vec4 LM=texture2D(lightmap,v_vTexcoord);\n")
  +string("    float LI=(LM.r+LM.g+LM.b)/3.0;\n")
  +string("    vec4 ALM=texture2D(addlightmap,v_vTexcoord);\n")
  +string("    float ALI=1.0-(ALM.r+ALM.g+ALM.b)/3.0;\n")
  +string("    vec4 EM=texture2D(emap,v_vTexcoord);\n")
  +string("    float M=(BL-LI)+ALI;\n")
  +string(" float shadow=0.0;\n")
  +string(" if ( count > 0 ) shadow+=shade(pos, height); \n")
  +string(" if ( count > 1 ) shadow+=shade(pos1,height);\n")
  +string(" if ( count > 2 ) shadow+=shade(pos2,height);\n")
  +string(" if ( count > 3 ) shadow+=shade(pos3,height);\n")
  +string(" if ( count > 4 ) shadow+=shade(pos4,height);\n")
  +string(" if ( count > 5 ) shadow+=shade(pos5,height);\n")
  +string(" if ( count > 6 ) shadow+=shade(pos6,height);\n")
  +string(" if ( count > 7 ) shadow+=shade(pos7,height);\n")
//  +string(" M=clamp(shadow/(float(count)*2.0)+M,0.0,1.0);\n")
  +string("    vec4 AM=vec4((ambience)*texture2D(colormap,v_vTexcoord).rgb,1.0);\n")
  +string(" return clamp(EM + CM * LM * vec4((1.0-ambience),1.0) + AM - clamp(vec4(vec3(shadow/(float(count)*4.0)),1.0),0.0,1.0)*.2,0.0,1.0);\n")
  +string("}\n")
  +string("void main() {\n")
  +string(" if ( v_vTexcoord.x < 0.5 && v_vTexcoord.y > 0.5 ) gl_FragColor=image();\n")
  +string(" else gl_FragColor=vec4(0);\n")
  +string("}\n")
 ;
 loadShaders(vert.c_str(),frag.c_str());
 InitUniforms();
}



void MultiShadow2DShader::Render( FBOColor *out, MultiShadow2DProfile *in ) {
 out->Bind();
 multishadow2dShader.Set(in);
 multishadow2dShader.UpdateUniforms();
 out->QuadYInverted();
 multishadow2dShader.Disable();
 out->Unbind();
}

void MultiShadedShadow2D( FBOColor *out, MultiShadow2DProfile *in ) {
 out->Bind();
 multishadow2dShader.Set(in);
 multishadow2dShader.UpdateUniforms();
 out->QuadYInverted();
 multishadow2dShader.Disable();
 out->Unbind();
}

void MultiShadedShadow2D( FBOColor *out, MultiShadow2DProfile *in, GLuint *color, GLuint *addlight, GLuint *light, GLuint *emap, GLuint *back, GLuint *height ) {
 out->Bind();
 multishadow2dShader.pos.Set(&in->pos);
 multishadow2dShader.pos1.Set(&in->pos1);
 multishadow2dShader.pos2.Set(&in->pos2);
 multishadow2dShader.pos3.Set(&in->pos3);
 multishadow2dShader.pos4.Set(&in->pos4);
 multishadow2dShader.pos5.Set(&in->pos5);
 multishadow2dShader.pos6.Set(&in->pos6);
 multishadow2dShader.pos7.Set(&in->pos7);
 multishadow2dShader.ambience.fromCrayon(in->ambience);
 multishadow2dShader.activeLights=in->activeLights;
 multishadow2dShader.colormap.pointer=color;
 multishadow2dShader.heightmap.pointer=height;
 multishadow2dShader.lightmap.pointer=light;
 multishadow2dShader.emap.pointer=emap;
 multishadow2dShader.addlightmap.pointer=addlight;
 multishadow2dShader.backlight.pointer=back;
 multishadow2dShader.UpdateUniforms();
 out->QuadYInverted();
 multishadow2dShader.Disable();
 out->Unbind();
}
