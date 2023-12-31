#include "BlendShader.h"

BlendTransparentShader blendtransparentShader;
BlendTransparentFactorShader blendtransparentfactorShader;
FBOBlendTransparentShader fboblendtransparentShader;
FBOBlendTransparentFactorShader fboblendtransparentfactorShader;
FBOBlendTransparentFactorWhiteClearShader fboblendtransparentfactorwhiteclearShader;
FBOBlendTransparentFactorBlackEtchShader fboblendtransparentfactorblacketchShader;
ColorAboveShader coloraboveShader;
ColorAboveBShader coloraboveBShader;
ColorBelowShader colorbelowShader;
ColorBelowBShader colorbelowBShader;
ColorBelowABShader colorbelowABShader;
ColorAboveBelowShader colorabovebelowShader;
Blend3AddShader blend3addShader;
Blend3AvgShader blend3avgShader;
AlphaAvgShader alphaavgShader;
AlphaAvgThresholdShader alphaavgthresholdShader;
BlendMultiplyAlphaShader blendmultiplyalphaShader;
BlendAlphaShader blendalphaShader;
BlendAlphaMultShader blendalphamultShader;
BlendAlphaInvShader blendalphainvShader;
BlendAlpha0Shader blendalpha0Shader;
Blend2WayShader blend2wayShader;
Blend3WayShader blend3wayShader;
Blend4WayShader blend4wayShader;
Blend5WayShader blend5wayShader;
Blend6WayShader blend6wayShader;
Blend7WayShader blend7wayShader;
Blend8WayShader blend8wayShader;



void ShadedFBOBlendTransparentFactorBlackEtch( double lerpFactor, FBOColor *applying, FBOColor *onto, FBOColor *out ) {
 ShadedFBOBlendTransparentFactorBlackEtch(lerpFactor,&applying->texture,&onto->texture,out);
}

void ShadedFBOBlendTransparentFactorBlackEtch( double lerpFactor, GLuint *applying, FBOColor *onto, FBOColor *out ) {
 ShadedFBOBlendTransparentFactorBlackEtch(lerpFactor,applying,&onto->texture,out);
}

void ShadedFBOBlendTransparentFactorBlackEtch( double lerpFactor, GLuint *applying, GLuint *onto, FBOColor *out ) {
 fboblendtransparentfactorblacketchShader.factor=lerpFactor;
 fboblendtransparentfactorblacketchShader.pTex0=onto;
 fboblendtransparentfactorblacketchShader.pTex1=applying;
 out->background.Pick(clear);
 out->Bind();
 fboblendtransparentfactorblacketchShader.UpdateUniforms();
 out->QuadYInverted();
 fboblendtransparentfactorblacketchShader.Disable();
 out->Unbind();
}

void ShadedFBOBlendTransparentFactorWhiteClear( double lerpFactor, FBOColor *applying, FBOColor *onto, FBOColor *out ) {
 ShadedFBOBlendTransparentFactorWhiteClear(lerpFactor,&applying->texture,&onto->texture,out);
}

void ShadedFBOBlendTransparentFactorWhiteClear( double lerpFactor, GLuint *applying, FBOColor *onto, FBOColor *out ) {
 ShadedFBOBlendTransparentFactorWhiteClear(lerpFactor,applying,&onto->texture,out);
}

void ShadedFBOBlendTransparentFactorWhiteClear( double lerpFactor, GLuint *applying, GLuint *onto, FBOColor *out ) {
 fboblendtransparentfactorwhiteclearShader.factor=lerpFactor;
 fboblendtransparentfactorwhiteclearShader.pTex0=onto;
 fboblendtransparentfactorwhiteclearShader.pTex1=applying;
 out->background.Pick(clear);
 out->Bind();
 fboblendtransparentfactorwhiteclearShader.UpdateUniforms();
 out->QuadYInverted();
 fboblendtransparentfactorwhiteclearShader.Disable();
 out->Unbind();
}

void ShadedFBOBlendTransparentFactor( double lerpFactor, FBOColor *applying, FBOColor *onto, FBOColor *out ) {
 ShadedFBOBlendTransparentFactor(lerpFactor,&applying->texture,&onto->texture,out);
}

void ShadedFBOBlendTransparentFactor( double lerpFactor, GLuint *applying, FBOColor *onto, FBOColor *out ) {
 ShadedFBOBlendTransparentFactor(lerpFactor,applying,&onto->texture,out);
}

void ShadedFBOBlendTransparentFactor( double lerpFactor, GLuint *applying, GLuint *onto, FBOColor *out ) {
 fboblendtransparentfactorShader.factor=lerpFactor;
 fboblendtransparentfactorShader.pTex0=onto;
 fboblendtransparentfactorShader.pTex1=applying;
 out->background.Pick(clear);
 out->Bind();
 fboblendtransparentfactorShader.UpdateUniforms();
 out->QuadYInverted();
 fboblendtransparentfactorShader.Disable();
 out->Unbind();
}

void ShadedFBOBlendTransparent(  FBOColor *applying, FBOColor *onto, FBOColor *out ) {
 ShadedFBOBlendTransparent( &applying->texture, &onto->texture, out );
}

void ShadedFBOBlendTransparent(  GLuint *applying, FBOColor *onto, FBOColor *out ) {
 ShadedFBOBlendTransparent( applying, &onto->texture, out );
}

void ShadedFBOBlendTransparent(  GLuint *applying, GLuint *onto, FBOColor *out ) {
 fboblendtransparentShader.pTex0=onto;
 fboblendtransparentShader.pTex1=applying;
 out->Bind();
 fboblendtransparentShader.UpdateUniforms();
 out->QuadYInverted();
 fboblendtransparentShader.Disable();
 out->Unbind();
}



void ShadedBlendTransparentFactor( double lerpFactor, FBOColor *applying, FBOColor *onto, FBOColor *out ) {
 ShadedBlendTransparentFactor(lerpFactor,&applying->texture,&onto->texture,out);
}

void ShadedBlendTransparentFactor( double lerpFactor, GLuint *applying, FBOColor *onto, FBOColor *out ) {
 ShadedBlendTransparentFactor(lerpFactor,applying,&onto->texture,out);
}

void ShadedBlendTransparentFactor( double lerpFactor, GLuint *applying, GLuint *onto, FBOColor *out ) {
 blendtransparentfactorShader.factor=lerpFactor;
 blendtransparentfactorShader.pTex0=onto;
 blendtransparentfactorShader.pTex1=applying;
 out->background.Pick(clear);
 out->Bind();
 blendtransparentfactorShader.UpdateUniforms();
 out->QuadYInverted();
 blendtransparentfactorShader.Disable();
 out->Unbind();
}

void ShadedBlendTransparent(  FBOColor *applying, FBOColor *onto, FBOColor *out ) {
 ShadedBlendTransparent( &applying->texture, &onto->texture, out );
}

void ShadedBlendTransparent(  GLuint *applying, FBOColor *onto, FBOColor *out ) {
 ShadedBlendTransparent( applying, &onto->texture, out );
}

void ShadedBlendTransparent(  GLuint *applying, GLuint *onto, FBOColor *out ) {
 blendtransparentShader.pTex0=onto;
 blendtransparentShader.pTex1=applying;
 out->Bind();
 blendtransparentShader.UpdateUniforms();
 out->QuadYInverted();
 blendtransparentShader.Disable();
 out->Unbind();
}

void ShadedColorAbove( Crayon threshold, Crayon replace, GLuint *in, FBOColor *out ) {
 coloraboveShader.colorThreshold.fromCrayon(threshold);
 coloraboveShader.colorReplace.fromCrayon(replace);
 coloraboveShader.pTex0=in;
 out->background.Pick(clear);
 out->Bind();
 coloraboveShader.UpdateUniforms();
 out->QuadYInverted();
 coloraboveShader.Disable();
 out->Unbind();
}

void ShadedColorAboveB( float threshold, GLuint *in, FBOColor *out ) {
 coloraboveBShader.threshold=threshold;
 coloraboveBShader.pTex0=in;
 out->background.Pick(clear);
 out->Bind();
 coloraboveBShader.UpdateUniforms();
 out->QuadYInverted();
 coloraboveBShader.Disable();
 out->Unbind();
}

void ShadedColorBelow( Crayon threshold, Crayon replace, GLuint *in, FBOColor *out ) {
 colorbelowShader.colorThreshold.fromCrayon(threshold);
 colorbelowShader.colorReplace.fromCrayon(replace);
 colorbelowShader.pTex0=in;
 out->background.Pick(clear);
 out->Bind();
 colorbelowShader.UpdateUniforms();
 out->QuadYInverted();
 colorbelowShader.Disable();
 out->Unbind();
}

void ShadedColorBelowB( float threshold, GLuint *in, FBOColor *out ) {
 colorbelowBShader.threshold=threshold;
 colorbelowBShader.pTex0=in;
 out->background.Pick(clear);
 out->Bind();
 colorbelowBShader.UpdateUniforms();
 out->QuadYInverted();
 colorbelowBShader.Disable();
 out->Unbind();
}

void ShadedColorBelowAB( float a, float b, GLuint *in, FBOColor *out ) {
 colorbelowABShader.thresholdA=a;
 colorbelowABShader.thresholdA=b;
 colorbelowABShader.pTex0=in;
 out->background.Pick(clear);
 out->Bind();
 colorbelowABShader.UpdateUniforms();
 out->QuadYInverted();
 colorbelowBShader.Disable();
 out->Unbind();
}

void ShadedColorAboveBelow( Crayon threshold, Crayon above, Crayon below, GLuint *in, FBOColor *out ) {
 colorabovebelowShader.colorThreshold.fromCrayon(threshold);
 colorabovebelowShader.above.fromCrayon(above);
 colorabovebelowShader.below.fromCrayon(below);
 colorabovebelowShader.pTex0=in;
 out->background.Pick(clear);
 out->Bind();
 colorabovebelowShader.UpdateUniforms();
 out->QuadYInverted();
 colorabovebelowShader.Disable();
 out->Unbind();
}

void ShadedBlend3Avg( GLuint *mix, GLuint *r, GLuint *g, GLuint *b, FBOColor *out ) {
 blend3avgShader.pTex0=mix;
 blend3avgShader.pTex1=r;
 blend3avgShader.pTex2=g;
 blend3avgShader.pTex3=b;
 out->Bind();
 blend3avgShader.UpdateUniforms();
 out->QuadYInverted();
 blend3avgShader.Disable();
 out->Unbind();
}

void ShadedBlend3Add( GLuint *mix, GLuint *r, GLuint *g, GLuint *b, FBOColor *out ) {
 blend3addShader.pTex0=mix;
 blend3addShader.pTex1=r;
 blend3addShader.pTex2=g;
 blend3addShader.pTex3=b;
 out->Bind();
 blend3addShader.UpdateUniforms();
 out->QuadYInverted();
 blend3addShader.Disable();
 out->Unbind();
}

void ShadedAlphaAvg( GLuint *in, FBOColor *out ) {
 alphaavgShader.pTex0=in;
 out->Bind();
 alphaavgShader.UpdateUniforms();
 FullscreenQuadYInverted();
 alphaavgShader.Disable();
 out->Unbind();
}

void ShadedAlphaAvgThreshold( Crayon threshold, GLuint *in, FBOColor *out ) {
 alphaavgthresholdShader.colorThreshold.fromCrayon(threshold);
 alphaavgShader.pTex0=in;
 out->Bind();
 alphaavgthresholdShader.UpdateUniforms();
 out->QuadYInverted();
 alphaavgthresholdShader.Disable();
 out->Unbind();
}

void ShadedBlendMultiplyAlpha( GLuint *mixer, GLuint *sampler, FBOColor *out ) {
 blendmultiplyalphaShader.pTex0=mixer;
 blendmultiplyalphaShader.pTex1=sampler;
 out->Bind();
 blendmultiplyalphaShader.UpdateUniforms();
 out->QuadYInverted();
 blendmultiplyalphaShader.Disable();
 out->Unbind();
}

void ShadedBlendAlpha( GLuint *mixer, GLuint *sampler, FBOColor *out ) {
 blendalphaShader.pTex0=mixer;
 blendalphaShader.pTex1=sampler;
 out->Bind();
 blendalphaShader.UpdateUniforms();
 out->QuadYInverted();
 blendalphaShader.Disable();
 out->Unbind();
}

void ShadedBlendAlphaMult( GLuint *mixer, GLuint *sampler, FBOColor *out ) {
 blendalphamultShader.pTex0=mixer;
 blendalphamultShader.pTex1=sampler;
 out->Bind();
 blendalphamultShader.UpdateUniforms();
 out->QuadYInverted();
 blendalphamultShader.Disable();
 out->Unbind();
}

void ShadedBlendAlphaInv( GLuint *mixer, GLuint *sampler, FBOColor *out ) {
 blendalphainvShader.pTex0=mixer;
 blendalphainvShader.pTex1=sampler;
 out->Bind();
 blendalphainvShader.UpdateUniforms();
 out->QuadYInverted();
 blendalphainvShader.Disable();
 out->Unbind();
}

void ShadedBlendAlpha0( GLuint *mixer, GLuint *sampler, FBOColor *out ) {
 blendalpha0Shader.pTex0=mixer;
 blendalpha0Shader.pTex1=sampler;
 out->Bind();
 blendalpha0Shader.UpdateUniforms();
 out->QuadYInverted();
 blendalpha0Shader.Disable();
 out->Unbind();
}

void ShadedBlend2Way( GLuint *mixer, GLuint *tR, GLuint *fallback, FBOColor *out ) {
 blend2wayShader.pTex0=mixer;
 blend2wayShader.pTex1=tR;
 blend2wayShader.pTex2=fallback;
 out->Bind();
 blend2wayShader.UpdateUniforms();
 out->QuadYInverted();
 blend2wayShader.Disable();
 out->Unbind();
}

void ShadedBlend3Way( GLuint *mixer, GLuint *tR, GLuint *tG,
                      GLuint *fallback, FBOColor *out ) {
 blend3wayShader.pTex0=mixer;
 blend3wayShader.pTex1=tR;
 blend3wayShader.pTex2=tG;
 blend3wayShader.pTex3=fallback;
 out->Bind();
 blend3wayShader.UpdateUniforms();
 out->QuadYInverted();
 blend3wayShader.Disable();
 out->Unbind();
}

void ShadedBlend4Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB,
                      GLuint *fallback, FBOColor *out ) {
 blend4wayShader.pTex0=mixer;
 blend4wayShader.pTex1=tR;
 blend4wayShader.pTex2=tG;
 blend4wayShader.pTex3=tB;
 blend4wayShader.pTex4=fallback;
 out->Bind();
 blend4wayShader.UpdateUniforms();
 out->QuadYInverted();
 blend4wayShader.Disable();
 out->Unbind();
}

void ShadedBlend5Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB,
                      GLuint *tBlack,
                      GLuint *fallback, FBOColor *out ) {
 blend5wayShader.pTex0=mixer;
 blend5wayShader.pTex1=tR;
 blend5wayShader.pTex2=tG;
 blend5wayShader.pTex3=tB;
 blend5wayShader.pTex4=tBlack;
 blend5wayShader.pTex5=fallback;
 out->Bind();
 blend5wayShader.UpdateUniforms();
 out->QuadYInverted();
 blend5wayShader.Disable();
 out->Unbind();
}

void ShadedBlend6Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB,
                      GLuint *tBlack, GLuint *tGB,
                      GLuint *fallback, FBOColor *out ) {
 blend6wayShader.pTex0=mixer;
 blend6wayShader.pTex1=tR;
 blend6wayShader.pTex2=tG;
 blend6wayShader.pTex3=tB;
 blend6wayShader.pTex4=tBlack;
 blend6wayShader.pTex5=tGB;
 blend6wayShader.pTex6=fallback;
 out->Bind();
 blend6wayShader.UpdateUniforms();
 out->QuadYInverted();
 blend6wayShader.Disable();
 out->Unbind();
}

void ShadedBlend7Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB,
                      GLuint *tBlack, GLuint *tGB, GLuint *tRG,
                      GLuint *fallback, FBOColor *out ) {
 blend7wayShader.pTex0=mixer;
 blend7wayShader.pTex1=tR;
 blend7wayShader.pTex2=tG;
 blend7wayShader.pTex3=tB;
 blend7wayShader.pTex4=tBlack;
 blend7wayShader.pTex5=tGB;
 blend7wayShader.pTex6=tRG;
 blend7wayShader.pTex7=fallback;
 out->Bind();
 blend7wayShader.UpdateUniforms();
 out->QuadYInverted();
 blend7wayShader.Disable();
 out->Unbind();
}

void ShadedBlend8Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB,
                      GLuint *tBlack, GLuint *tGB, GLuint *tRG, GLuint *tRB,
                      GLuint *fallback, FBOColor *out ) {
 blend8wayShader.pTex0=mixer;
 blend8wayShader.pTex1=tR;
 blend8wayShader.pTex2=tG;
 blend8wayShader.pTex3=tB;
 blend8wayShader.pTex4=tBlack;
 blend8wayShader.pTex5=tGB;
 blend8wayShader.pTex6=tRG;
 blend8wayShader.pTex7=tRB;
 blend8wayShader.pTex8=fallback;
 out->Bind();
 blend8wayShader.UpdateUniforms();
 out->QuadYInverted();
 blend8wayShader.Disable();
 out->Unbind();
}