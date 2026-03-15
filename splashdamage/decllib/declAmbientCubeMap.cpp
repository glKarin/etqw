// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declAmbientCubeMap.h"

//===============================================================
//
//	sdDeclAmbientCubeMap
//
//===============================================================


sdDeclAmbientCubeMap::sdDeclAmbientCubeMap()
	: indoors(false),
	  brightness(1.0f),
	  ambientCubeMap(NULL),
	  lightCubeMap(NULL),
	  specularCubeMap(NULL),
	  environmentCubeMap(NULL),
	  gradientMap(NULL) {
	ambientColor.Set(1.0f, 1.0, 1.0f);
	highLightColor.Set(1.0f, 1.0, 1.0f);
	sunDirection.Set(0.0f, 0.0, -1.0f);
	sunColor.Set(1.0f, 1.0, 1.0f);
	avgAmbientColor.Set(1.0f, 1.0, 1.0f, 1.0f);
	minSpecAmbientColor.Zero();
	minSpecShadowColor.Zero();
}

const char* sdDeclAmbientCubeMap::DefaultDefinition( void ) const {
	return "{  }";
}

bool sdDeclAmbientCubeMap::Parse( const char* text, const int textLength ) {
	indoors = false;
	brightness = 1.0f;
	ambientCubeMap = NULL;
	lightCubeMap = NULL;
	specularCubeMap = NULL;
	environmentCubeMap = NULL;
	gradientMap = NULL;
	ambientColor.Set(1.0f, 1.0, 1.0f);
	highLightColor.Set(1.0f, 1.0, 1.0f);
	sunDirection.Set(0.0f, 0.0, -1.0f);
	sunColor.Set(1.0f, 1.0, 1.0f);
	avgAmbientColor.Set(1.0f, 1.0, 1.0f, 1.0f);
	minSpecAmbientColor.Zero();
	minSpecShadowColor.Zero();
	return true;
}

void sdDeclAmbientCubeMap::FreeData() {

}

void sdDeclAmbientCubeMap::CacheFromDict( const idDict& dict ) {

}
	
bool sdDeclAmbientCubeMap::Save() {
	return true;
}

void sdDeclAmbientCubeMap::GenerateImages() {
}

bool sdDeclAmbientCubeMap::ParseAmbientLight( idParser *src ) {
	return true;
}

bool sdDeclAmbientCubeMap::RebuildTextSource() {
	return true;
}

float sdDeclAmbientCubeMap::cubeMapDataFloat[ 6 * BAKEDLIGHT_SIZE * BAKEDLIGHT_SIZE * 4 ];
byte sdDeclAmbientCubeMap::cubeMapDataByte[ 6 * BAKEDLIGHT_SIZE * BAKEDLIGHT_SIZE * 4 ];
byte sdDeclAmbientCubeMap::gradientMapData[ GRADIENT_SIZE * 4 ];

float* sdDeclAmbientCubeMap::cubeMapFloat[ 6 ];
byte* sdDeclAmbientCubeMap::cubeMapByte[ 6 ];

void sdDeclAmbientCubeMap::ClearCubeMap( float* cubeMap[6], const int faceSize ) {
}

void sdDeclAmbientCubeMap::ScaleCubeMapColor( float* cubeMap[6], const int faceSize, const float scale ) {
}

void sdDeclAmbientCubeMap::CubeMapFtob( float* cubeMapFloat[6], byte* cubeMapByte[6], const int faceSize ) {
}

void sdDeclAmbientCubeMap::BakeLight( float* cubeMap[6], const int faceSize, const idVec3& lightDir, const idVec3& lightColor) {
}

void sdDeclAmbientCubeMap::BakeLight( float* cubeMap[6], const int faceSize, const idVec3& lightDir, const idVec3& lightColor, const float power ) {
}

void sdDeclAmbientCubeMap::BakeGradientMap( byte* pic, const int size, const idVec3& ambientColor, const idVec3& highLightColor ) {
}

void sdDeclAmbientCubeMap::UploadCubeMap( idImage* image, const byte* cubeMap[6], const int faceSize ) {
}

void sdDeclAmbientCubeMap::AmbientCubeMapImage( idImage* image ) {
	this->ambientCubeMap = image;
}

void sdDeclAmbientCubeMap::LightCubeMapImage( idImage* image ) {
	this->lightCubeMap = image;
}

void sdDeclAmbientCubeMap::SpecularCubeMapImage( idImage* image ) {
	this->specularCubeMap = image;
}

void sdDeclAmbientCubeMap::GradientMapImage( idImage* image ) {
	this->gradientMap = image;
}

