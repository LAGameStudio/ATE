#include "VortexShader.h"

VortexShader vortexShader;

void ShadedVortex( GLuint *in, GLuint *in2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 vortexShader.pTex0=in;
 vortexShader.pTex1=in2;
 vortexShader.UpdateUniforms();
 out->QuadYInverted();// &vortexShader);
 vortexShader.Disable();
 out->Unbind();
}