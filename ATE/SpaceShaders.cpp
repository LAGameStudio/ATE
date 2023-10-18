#include "SpaceShaders.h"

WaterAndGroundShader waterandgroundShader;

void ShadedWaterAndGround( FBOColor *out, WaterAndGroundProfile *in ) {
 out->Bind();
 waterandgroundShader.Set(in);
 waterandgroundShader.UpdateUniforms();
 out->QuadYInverted();
 waterandgroundShader.Disable();
 out->Unbind();
}

EmissionCloudsShader emissioncloudsShader;

void ShadedEmissionClouds( FBOColor *out, EmissionCloudsProfile *in ) {
 out->Bind();
 emissioncloudsShader.Set(in);
 emissioncloudsShader.UpdateUniforms();
 out->QuadYInverted();
 emissioncloudsShader.Disable();
 out->Unbind();
}

ElectroCloudsShader electrocloudsShader;

void ShadedElectroClouds( FBOColor *out, ElectroCloudsProfile *in ) {
 out->Bind();
 electrocloudsShader.Set(in);
 electrocloudsShader.UpdateUniforms();
 out->QuadYInverted();
 electrocloudsShader.Disable();
 out->Unbind();
}

NebulazShader nebulazShader;

void ShadedNebulaz( FBOColor *out, NebulazProfile *in ) {
 out->Bind();
 nebulazShader.Set(in);
 nebulazShader.UpdateUniforms();
 out->QuadYInverted();
 nebulazShader.Disable();
 out->Unbind();
}

ToonTroidShader toontroidShader;

void ShadedToonTroid( FBOColor *out, ToonTroidProfile *in ) {
 out->Bind();
 toontroidShader.Set(in);
 toontroidShader.UpdateUniforms();
 out->QuadYInverted();
 toontroidShader.Disable();
 out->Unbind();
}

TexturedPlanetShader texturedplanetShader;

void ShadedTexturedPlanet( FBOColor *out, TexturedPlanetProfile *in ) {
 out->Bind();
 texturedplanetShader.Set(in);
 texturedplanetShader.UpdateUniforms();
 out->QuadYInverted();
 texturedplanetShader.Disable();
 out->Unbind();
}

EarthlikeShader earthlikeShader;

void ShadedEarthlike( FBOColor *out, EarthlikeProfile *in ) {
 out->Bind();
 earthlikeShader.Set(in);
 earthlikeShader.UpdateUniforms();
 out->QuadYInverted();
 earthlikeShader.Disable();
 out->Unbind();
}

AccretionShader accretionShader;

void ShadedAccretion( FBOColor *out, AccretionProfile *in ) {
 out->Bind();
 accretionShader.Set(in);
 accretionShader.UpdateUniforms();
 out->QuadYInverted();
 accretionShader.Disable();
 out->Unbind();
}

BlackHoleShader blackholeShader;

void ShadedBlackHole( FBOColor *out, BlackHoleProfile *in ) {
 out->Bind();
 blackholeShader.Set(in);
 blackholeShader.UpdateUniforms();
 out->QuadYInverted();
 blackholeShader.Disable();
 out->Unbind();
}

RingedJovianShader ringedjovianShader;

void ShadedRingedJovian( FBOColor *out, RingedJovianProfile *in ) {
 out->Bind();
 ringedjovianShader.Set(in);
 ringedjovianShader.UpdateUniforms();
 out->QuadYInverted();
 ringedjovianShader.Disable();
 out->Unbind();
}

ElectroPlanetShader electroplanetShader;

void ShadedElectroPlanet( FBOColor *out, ElectroPlanetProfile *in ) {
 out->Bind();
 electroplanetShader.Set(in);
 electroplanetShader.UpdateUniforms();
 out->QuadYInverted();
 electroplanetShader.Disable();
 out->Unbind();
}

AtmosphericPlanetShader atmosphericplanetShader;

void ShadedAtmosphericPlanet( FBOColor *out, AtmosphericPlanetProfile *in ) {
 out->Bind();
 atmosphericplanetShader.Set(in);
 atmosphericplanetShader.UpdateUniforms();
 out->QuadYInverted();
 atmosphericplanetShader.Disable();
 out->Unbind();
}

Earthlike2Shader earthlike2Shader;

void ShadedEarthlike2( FBOColor *out, Earthlike2Profile *in ) {
 out->Bind();
 earthlike2Shader.Set(in);
 earthlike2Shader.UpdateUniforms();
 out->QuadYInverted();
 earthlike2Shader.Disable();
 out->Unbind();
}

RingedJovian2Shader ringedjovian2Shader;

void ShadedRingedJovian2( FBOColor *out, RingedJovian2Profile *in ) {
 out->Bind();
 ringedjovian2Shader.Set(in);
 ringedjovian2Shader.UpdateUniforms();
 out->QuadYInverted();
 ringedjovian2Shader.Disable();
 out->Unbind();
}

LunarPlanetShader lunarplanetShader;

void LunarPlanetShader::Render( FBOColor *out, LunarPlanetProfile *in ) {
 out->Bind();
 lunarplanetShader.Set(in);
 lunarplanetShader.UpdateUniforms();
 out->QuadYInverted();
 lunarplanetShader.Disable();
 out->Unbind();
}

void ShadedLunarPlanet( FBOColor *out, LunarPlanetProfile *in ) {
 out->Bind();
 lunarplanetShader.Set(in);
 lunarplanetShader.UpdateUniforms();
 out->QuadYInverted();
 lunarplanetShader.Disable();
 out->Unbind();
}



FlaringStarShader flaringstarShader;

void FlaringStarShader::Render( FBOColor *out, FlaringStarProfile *in ) {
 out->Bind();
 flaringstarShader.Set(in);
 flaringstarShader.UpdateUniforms();
 out->QuadYInverted();
 flaringstarShader.Disable();
 out->Unbind();
}

void ShadedFlaringStar( FBOColor *out, FlaringStarProfile *in ) {
 out->Bind();
 flaringstarShader.Set(in);
 flaringstarShader.UpdateUniforms();
 out->QuadYInverted();
 flaringstarShader.Disable();
 out->Unbind();
}


ElectronCloudsShader electroncloudsShader;

void ElectronCloudsShader::Render( FBOColor *out, ElectronCloudsProfile *in ) {
 out->Bind();
 electroncloudsShader.Set(in);
 electroncloudsShader.UpdateUniforms();
 out->QuadYInverted();
 electroncloudsShader.Disable();
 out->Unbind();
}

void ShadedElectronClouds( FBOColor *out, ElectronCloudsProfile *in ) {
 out->Bind();
 electroncloudsShader.Set(in);
 electroncloudsShader.UpdateUniforms();
 out->QuadYInverted();
 electroncloudsShader.Disable();
 out->Unbind();
}

ClassicPlanetShader classicplanetShader;

void ClassicPlanetShader::Render( FBOColor *out, ClassicPlanetProfile *in ) {
 out->Bind();
 classicplanetShader.Set(in);
 classicplanetShader.UpdateUniforms();
 out->QuadYInverted();
 classicplanetShader.Disable();
 out->Unbind();
}

void ShadedClassicPlanet( FBOColor *out, ClassicPlanetProfile *in ) {
 out->Bind();
 classicplanetShader.Set(in);
 classicplanetShader.UpdateUniforms();
 out->QuadYInverted();
 classicplanetShader.Disable();
 out->Unbind();
}
