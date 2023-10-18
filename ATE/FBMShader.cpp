#include "FBMShader.h"
#include "FBO.h"

FBMShader fbmShader;
FBMCheapShadowShader fbmcheapshadowShader;

void ShadedFBM( FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 fbmShader.UpdateUniforms();
 out->QuadYInverted();
 fbmShader.Disable();
 out->Unbind();
}