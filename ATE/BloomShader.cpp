#include "BloomShader.h"

BloomShader bloomShader;

void ShadedBloom( GLuint *in, FBOColor *out ) { 
 bloomShader.pTex0=in;
 out->background.Pick(clear);
 out->Bind();
 bloomShader.UpdateUniforms();
 out->QuadYInverted();
 bloomShader.Disable();
 out->Unbind();
}