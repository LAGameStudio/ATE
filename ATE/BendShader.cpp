#include "BendShader.h"
#include "FBO.h"

BendShader bendShader;
BendRShader bendRShader;
BendGShader bendGShader;
BendBShader bendBShader;
BendRGShader bendRGShader;
BendGBShader bendGBShader;
BendBRShader bendBRShader;

void ShadedBend( GLuint *t1, GLuint *t2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 bendShader.pTex0=t1;
 bendShader.pTex1=t2;
 bendShader.UpdateUniforms();
 out->QuadYInverted();
 bendShader.Disable();
 out->Unbind();
}

void ShadedBendR( GLuint *t1, GLuint *t2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 bendRShader.pTex0=t1;
 bendRShader.pTex1=t2;
 bendRShader.UpdateUniforms();
 out->QuadYInverted();
 bendRShader.Disable();
 out->Unbind();
}

void ShadedBendG( GLuint *t1, GLuint *t2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 bendGShader.pTex0=t1;
 bendGShader.pTex1=t2;
 bendGShader.UpdateUniforms();
 out->QuadYInverted();
 bendGShader.Disable();
 out->Unbind();
}

void ShadedBendB( GLuint *t1, GLuint *t2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 bendBShader.pTex0=t1;
 bendBShader.pTex1=t2;
 bendBShader.UpdateUniforms();
 out->QuadYInverted();
 bendBShader.Disable();
 out->Unbind();
}

void ShadedBendRG( GLuint *t1, GLuint *t2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 bendRGShader.pTex0=t1;
 bendRGShader.pTex1=t2;
 bendRGShader.UpdateUniforms();
 out->QuadYInverted();
 bendRGShader.Disable();
 out->Unbind();
}

void ShadedBendGB( GLuint *t1, GLuint *t2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 bendGBShader.pTex0=t1;
 bendGBShader.pTex1=t2;
 bendGBShader.UpdateUniforms();
 out->QuadYInverted();
 bendGBShader.Disable();
 out->Unbind();
}

void ShadedBendBR( GLuint *t1, GLuint *t2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 bendBRShader.pTex0=t1;
 bendBRShader.pTex1=t2;
 bendBRShader.UpdateUniforms();
 out->QuadYInverted();
 bendBRShader.Disable();
 out->Unbind();
}