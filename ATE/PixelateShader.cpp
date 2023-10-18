#include "PixelateShader.h"

PixelateShader pixelateShader;

void ShadedPixelate( FBOColor *out, GLuint *source, float sourceW, float sourceH ) {
 out->Bind();
 pixelateShader.pTex0=source;
 pixelateShader.resolution[0]=sourceW;
 pixelateShader.resolution[1]=sourceH;
// pixelateShader.pixel[0]=scalex;
// pixelateShader.pixel[1]=scaley;
 pixelateShader.UpdateUniforms();
 out->QuadYInverted();
 pixelateShader.Disable();
 out->Unbind();
}

CoverageShader coverageShader;

void CoverageShader::Render( FBOColor *out, CoverageProfile *in ) {
 out->Bind();
 coverageShader.Set(in);
 coverageShader.UpdateUniforms();
 out->QuadYInverted();
 coverageShader.Disable();
 out->Unbind();
}

void ShadedCoverage( FBOColor *out, CoverageProfile *in ) {
 out->Bind();
 coverageShader.Set(in);
 coverageShader.UpdateUniforms();
 out->QuadYInverted();
 coverageShader.Disable();
 out->Unbind();
}

void ShadedCoverage( FBOColor *out, CoverageProfile *in, FBOColor *in2 ) {
 out->Bind();
 coverageShader.Set(in2,in);
 coverageShader.UpdateUniforms();
 out->QuadYInverted();
 coverageShader.Disable();
 out->Unbind();
}

HQ4XShader hq4xShader;

void HQ4XShader::Render( FBOColor *out, HQ4XProfile *in ) {
 out->Bind();
 hq4xShader.Set(in);
 hq4xShader.UpdateUniforms();
 out->QuadYInverted();
 hq4xShader.Disable();
 out->Unbind();
}

void ShadedHQ4X( FBOColor *out, HQ4XProfile *in ) {
 out->Bind();
 hq4xShader.Set(in);
 hq4xShader.UpdateUniforms();
 out->QuadYInverted();
 hq4xShader.Disable();
 out->Unbind();
}

void ShadedHQ4X( FBOColor *in, FBOColor *out ) {
 out->Bind();
 hq4xShader.tex=&in->texture;
 hq4xShader.resolution.Set(in->w,in->h);
 hq4xShader.UpdateUniforms();
 out->QuadYInverted();
 hq4xShader.Disable();
 out->Unbind();
}

void ShadedHQ4X( GLImage *in, FBOColor *out ) {
 out->Bind();
 hq4xShader.tex=&in->texture;
 hq4xShader.resolution.Set(in->width,in->height);
 hq4xShader.UpdateUniforms();
 out->QuadYInverted();
 hq4xShader.Disable();
 out->Unbind();
}