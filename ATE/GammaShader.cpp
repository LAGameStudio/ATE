#include "GammaShader.h"

GammaShader gammaShader;


void ShadedGamma( GLuint *source, float gamma, FBOColor *out ) {
 out->Bind();
 gammaShader.pTex0=source;
 gammaShader.gamma=gamma;
 gammaShader.UpdateUniforms();
 out->QuadYInverted();
 gammaShader.Disable();
 out->Unbind();
}