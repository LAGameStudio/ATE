#include "StarNestShader.h"


StarNestShader starNestShader;
StarNest2Shader starNest2Shader;
StarNestSimpleShader starNestSimpleShader;
StarNestSimple2Shader starNestSimple2Shader;


void ShadedStarNest( GLuint *sampler, float time, float pos_x, float pos_y, float res_x, float res_y ) {
 starNestShader.pTex=sampler;
 starNestShader.time=time;
 starNestShader.position.x=pos_x;
 starNestShader.position.y=pos_y;
 starNestShader.resolution.x=res_x;
 starNestShader.resolution.y=res_y;
 starNestShader.UpdateUniforms();
}

void ShadedStarNest2( GLuint *sampler, float time, float pos_x, float pos_y, float res_x, float res_y ) {
 starNest2Shader.pTex=sampler;
 starNest2Shader.time=time;
 starNest2Shader.position.x=pos_x;
 starNest2Shader.position.y=pos_y;
 starNest2Shader.resolution.x=res_x;
 starNest2Shader.resolution.y=res_y;
 starNest2Shader.UpdateUniforms();
}

void ShadedStarNestSimple( GLuint *sampler, float time, float pos_x, float pos_y, float res_x, float res_y ) {
 starNestSimpleShader.pTex=sampler;
 starNestSimpleShader.time=time;
 starNestSimpleShader.position.x=pos_x;
 starNestSimpleShader.position.y=pos_y;
 starNestSimpleShader.resolution.x=res_x;
 starNestSimpleShader.resolution.y=res_y;
 starNestSimpleShader.UpdateUniforms();
}

void ShadedStarNestSimple2( GLuint *sampler, float time, float pos_x, float pos_y, float res_x, float res_y ) {
 starNestSimple2Shader.pTex=sampler;
 starNestSimple2Shader.time=time;
 starNestSimple2Shader.position.x=pos_x;
 starNestSimple2Shader.position.y=pos_y;
 starNestSimple2Shader.resolution.x=res_x;
 starNestSimple2Shader.resolution.y=res_y;
 starNestSimple2Shader.UpdateUniforms();
}


void ShadedStarNest( GLuint *sampler, float time, float pos_x, float pos_y, float res_x, float res_y, FBOColor *out ) {
 out->Bind();
 starNestShader.pTex=sampler;
 starNestShader.time=time;
 starNestShader.position.x=pos_x;
 starNestShader.position.y=pos_y;
 starNestShader.resolution.x=res_x;
 starNestShader.resolution.y=res_y;
 starNestShader.UpdateUniforms();
 out->QuadYInverted();
 starNestShader.Disable();
 out->Unbind();
}

void ShadedStarNest2( GLuint *sampler, float time, float pos_x, float pos_y, float res_x, float res_y, FBOColor *out ) {
 out->Bind();
 starNest2Shader.pTex=sampler;
 starNest2Shader.time=time;
 starNest2Shader.position.x=pos_x;
 starNest2Shader.position.y=pos_y;
 starNest2Shader.resolution.x=res_x;
 starNest2Shader.resolution.y=res_y;
 starNest2Shader.UpdateUniforms();
 out->QuadYInverted();
 starNest2Shader.Disable();
 out->Unbind();
}

void ShadedStarNestSimple( GLuint *sampler, float time, float pos_x, float pos_y, float res_x, float res_y, FBOColor *out ) {
 out->Bind();
 starNestSimpleShader.pTex=sampler;
 starNestSimpleShader.time=time;
 starNestSimpleShader.position.x=pos_x;
 starNestSimpleShader.position.y=pos_y;
 starNestSimpleShader.resolution.x=res_x;
 starNestSimpleShader.resolution.y=res_y;
 starNestSimpleShader.UpdateUniforms();
 out->QuadYInverted();
 starNestSimpleShader.Disable();
 out->Unbind();
}

void ShadedStarNestSimple2( GLuint *sampler, float time, float pos_x, float pos_y, float res_x, float res_y, FBOColor *out ) {
 out->Bind();
 starNestSimple2Shader.pTex=sampler;
 starNestSimple2Shader.time=time;
 starNestSimple2Shader.position.x=pos_x;
 starNestSimple2Shader.position.y=pos_y;
 starNestSimple2Shader.resolution.x=res_x;
 starNestSimple2Shader.resolution.y=res_y;
 starNestSimple2Shader.UpdateUniforms();
 out->QuadYInverted();
 starNestSimple2Shader.Disable();
 out->Unbind();
}
