// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declAtmosphere.h"

void sdPrecipitationParameters::Default() {
	preType = PT_NONE;
	maxParticles = 0;
	heightMin = 0.0f;
	heightMax = 0.0f;
	weightMin = 0.0f;
	weightMax = 0.0f;
	windScale = 0.0f;
	gustWindScale = 0.0f;
	fallMin = 0.0f;
	fallMax = 0.0f;
	timeMin = 0.0f;
	timeMax = 0.0f;
	tumbleStrength = 0.0f;
	precipitationDistance = 0.0f;
	material = NULL;
	model = NULL;
	effect = NULL;
}

bool sdPrecipitationParameters::Parse( idParser& src ) {
	return true;
}
void sdPrecipitationParameters::Save( idFile_Memory& f ) const {
}

static void postProcessParms_t_Init(sdDeclAtmosphere::postProcessParms_t &parms) {
	parms.tint.Set(1.0f, 1.0f, 1.0f);
	parms.saturation = 0.0f;
	parms.contrast = 0.0f;
	parms.glareParms.Set(1.0f, 1.0f, 1.0f, 1.0f);
	parms.glareBases.Set(1.0f, 1.0f, 1.0f, 1.0f);
}

sdDeclAtmosphere::sdDeclAtmosphere()
	: modified(false),
	  sunMaterial(NULL),
	  sunAzimuth(0.0f),
	  sunZenith(0.0f),
	  sunHaloScale(1.0f),
	  sunHaloBias(0.0f),
	  sunSpriteMaterial(NULL),
	  sunSpriteSize(0.0f),
	  sunFlareMaterial(NULL),
	  sunFlareSize(0.0f),
	  sunFlareTime(0.0f),
	  enableSunFlareAziZen(false),
	  sunFlareAzi(0.0f),
	  sunFlareZen(0.0f),
	  fogDistHalf(0.0f),
	  fogHeightHalf(0.0f),
	  fogHeightOffset(0.0f),
	  fogStart(0.0f),
	  fogEnd(0.0f),
	  atmosphereMaterial(NULL),
	  ambientCubeMap(NULL),
	  skyGradientImage(NULL),
	  farClip(0.0f),
	  isNight(false),
	  drawAtmosphereLast(false),
	  windAngle(0.0f),
	  windAngleDev(0.0f),
	  windStrength(0.0f),
	  windStrengthDev(0.0f),
	  numPrecipLayers(0)
{
	sunDir.Set(0.0f, 0.0f, 1.0f);
	sunColor.Set(1.0f, 1.0f, 1.0f);

	postProcessParms_t_Init(defaultPostProcessParms);
	postProcessParms = defaultPostProcessParms;

	fogColor.Set(1.0f, 1.0f, 1.0f);

	minSpecShadowColor.Zero();

	cloudLayers.Clear();

	for ( int i = 0; i < NUM_PRECIP_LAYERS; i++ ) {
		precipitation[i].Default();
	}
}

const char* sdDeclAtmosphere::DefaultDefinition( void ) const {
	return "{  }";
}

bool sdDeclAtmosphere::Parse( const char* text, const int textLength ) {
	modified = false;
	sunMaterial = NULL;
	sunDir.Set(0.0f, 0.0f, 1.0f);
	sunAzimuth = 0.0f;
	sunZenith = 0.0f;
	sunColor.Set(1.0f, 1.0f, 1.0f);
	sunHaloScale = 1.0f;
	sunHaloBias = 0.0f;

	sunSpriteMaterial = NULL;
	sunSpriteSize = 0.0f;

	sunFlareMaterial = NULL;
	sunFlareSize = 0.0f;
	sunFlareTime = 0.0f;
	enableSunFlareAziZen = false;
	sunFlareAzi = 0.0f;
	sunFlareZen = 0.0f;

	postProcessParms_t_Init(defaultPostProcessParms);
	postProcessParms = defaultPostProcessParms;

	fogDistHalf = 0.0f;
	fogHeightHalf = 0.0f;
	fogHeightOffset = 0.0f;
	fogColor.Set(1.0f, 1.0f, 1.0f);
	fogStart = 0.0f;
	fogEnd = 0.0f;

	atmosphereMaterial = NULL;
	ambientCubeMap = NULL;
	skyGradientImage = NULL;

	farClip = 0.0f;
	isNight = false;
	drawAtmosphereLast = false;

	minSpecShadowColor.Zero();

	windAngle = 0.0f;
	windAngleDev = 0.0f;
	windStrength = 0.0f;
	windStrengthDev = 0.0f;

	cloudLayers.Clear();

	numPrecipLayers = 0;

	for ( int i = 0; i < NUM_PRECIP_LAYERS; i++ ) {
		precipitation[i].Default();
	}

	return true;
}

void sdDeclAtmosphere::FreeData() {
}

void sdDeclAtmosphere::CacheFromDict( const idDict& dict ) {
}

void sdDeclAtmosphere::Save() {
}

void sdDeclAtmosphere::Save( idFile_Memory& f ) const {
}

bool sdDeclAtmosphere::SetSkyGradientImage( const char* imageName ) {
	skyGradientImage = globalImages->GetImage(imageName);
	return NULL != skyGradientImage;
}

bool sdDeclAtmosphere::ParsePostProcessParms( idParser& src ) {
	return true;
}

bool sdDeclAtmosphere::ParseCloudLayer( idParser& src ) {
	return true;
}

bool sdDeclAtmosphere::ParsePrecipitationLayer( idParser& src ) {
	return true;
}

void sdDeclAtmosphere::RebuildTextSource( idFile_Memory& f ) const {
}

void sdDeclAtmosphere::UpdateSunDirFromAziZen() {
}

