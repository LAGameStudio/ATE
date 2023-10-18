#include "Nebula4LightShader.h"

Nebula4LightShader nebula4Shader;

void ShadedNebula4( FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 nebula4Shader.UpdateUniforms();
 out->QuadYInverted();
 nebula4Shader.Disable();
 out->Unbind();
}