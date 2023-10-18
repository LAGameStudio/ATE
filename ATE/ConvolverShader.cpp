#include "ConvolverShader.h"
#include "FBO.h"

EmbossShader embossShader;
EmbossConvolveAShader embossconvolveAShader;
EmbossPreserveAShader embosspreserveAShader;
GaussianBlurShader gaussianblurShader;
SharpnessShader sharpnessShader;
EdgeDetectShader edgedetectShader;
SigmaV9GaussianShader sigmaV9gaussianShader;
SigmaH9GaussianShader sigmaH9gaussianShader;
SigmaV7GaussianShader sigmaV7gaussianShader;
SigmaH7GaussianShader sigmaH7gaussianShader;
SigmaV5GaussianShader sigmaV5gaussianShader;
SigmaH5GaussianShader sigmaH5gaussianShader;
SigmaVGaussianShader sigmaVgaussianShader;
SigmaHGaussianShader sigmaHgaussianShader;
NormalMapShader normalmapShader;
NormalMapHeightShader normalmapheightShader;

void ShadedEmboss( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 embossShader.pTex0=in;
 embossShader.UpdateUniforms();
 out->QuadYInverted();
 embossShader.Disable();
 out->Unbind();
}

void ShadedEmbossConvolveA( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 embossconvolveAShader.pTex0=in;
 embossconvolveAShader.UpdateUniforms();
 out->QuadYInverted();
 embossconvolveAShader.Disable();
 out->Unbind();
}

void ShadedEmbossPreserveA( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 embosspreserveAShader.pTex0=in;
 embosspreserveAShader.UpdateUniforms();
 out->QuadYInverted();
 embosspreserveAShader.Disable();
 out->Unbind();
}

void ShadedGaussian( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 gaussianblurShader.pTex0=in;
 gaussianblurShader.UpdateUniforms();
 out->QuadYInverted();
 gaussianblurShader.Disable();
 out->Unbind();
}

void ShadedSharpen( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 sharpnessShader.pTex0=in;
 sharpnessShader.UpdateUniforms();
 out->QuadYInverted();
 sharpnessShader.Disable();
 out->Unbind();
}

void ShadedEdge( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 edgedetectShader.pTex0=in;
 edgedetectShader.UpdateUniforms();
 out->QuadYInverted();
 edgedetectShader.Disable();
 out->Unbind();
}

void ShadedSigma9Blur( GLuint *in, FBOColor *work, FBOColor *out ) {
 work->background.Pick(clear);
 work->Bind();
 sigmaV9gaussianShader.pTex0=in;
 sigmaV9gaussianShader.UpdateUniforms();
 work->QuadYInverted();
 sigmaV9gaussianShader.Disable();
 work->Unbind();
 out->background.Pick(clear);
 out->Bind();
 sigmaH9gaussianShader.pTex0=&work->texture;
 sigmaH9gaussianShader.UpdateUniforms();
 out->QuadYInverted();
 sigmaH9gaussianShader.Disable();
 out->Unbind();
}

void ShadedSigma7Blur( GLuint *in, FBOColor *work, FBOColor *out ) {
 work->background.Pick(clear);
 work->Bind();
 sigmaV7gaussianShader.pTex0=in;
 sigmaV7gaussianShader.UpdateUniforms();
 work->QuadYInverted();
 sigmaV9gaussianShader.Disable();
 work->Unbind();
 out->background.Pick(clear);
 out->Bind();
 sigmaH7gaussianShader.pTex0=&work->texture;
 sigmaH7gaussianShader.UpdateUniforms();
 out->QuadYInverted();
 sigmaH7gaussianShader.Disable();
 out->Unbind();
}

void ShadedSigma5Blur( GLuint *in, FBOColor *work, FBOColor *out ) {
 work->background.Pick(clear);
 work->Bind();
 sigmaV5gaussianShader.pTex0=in;
 sigmaV5gaussianShader.UpdateUniforms();
 work->QuadYInverted();
 sigmaV5gaussianShader.Disable();
 work->Unbind();
 out->background.Pick(clear);
 out->Bind();
 sigmaH5gaussianShader.pTex0=&work->texture;
 sigmaH5gaussianShader.UpdateUniforms();
 out->QuadYInverted();
 sigmaH5gaussianShader.Disable();
 out->Unbind();
}

void ShadedSigmaBlur( GLuint *in, FBOColor *work, FBOColor *out ) {
 work->background.Pick(clear);
 work->Bind();
 sigmaVgaussianShader.pTex0=in;
 sigmaVgaussianShader.UpdateUniforms();
 work->QuadYInverted();
 sigmaV5gaussianShader.Disable();
 work->Unbind();
 out->background.Pick(clear);
 out->Bind();
 sigmaHgaussianShader.pTex0=&work->texture;
 sigmaHgaussianShader.UpdateUniforms();
 out->QuadYInverted();
 sigmaHgaussianShader.Disable();
 out->Unbind();
}