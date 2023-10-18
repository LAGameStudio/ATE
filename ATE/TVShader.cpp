#include "TVShader.h"

TVShader tvShader;

OldTVShader oldtvShader;

void OldTVShader::Render( FBOColor *out, OldTVProfile *in ) {
 out->Bind();
 oldtvShader.Set(in);
 oldtvShader.UpdateUniforms();
 out->QuadYInverted();
 oldtvShader.Disable();
 out->Unbind();
}

void ShadedOldTV( FBOColor *out, OldTVProfile *in ) {
 out->Bind();
 oldtvShader.Set(in);
 oldtvShader.UpdateUniforms();
 out->QuadYInverted();
 oldtvShader.Disable();
 out->Unbind();
}