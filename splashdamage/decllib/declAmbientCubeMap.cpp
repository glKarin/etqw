// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declAmbientCubeMap.h"
#include "framework/DeclParseHelper.h"

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
	idParser src;
	idToken	token;

	src.SetFlags(DECL_LEXER_FLAGS);
	//src.LoadMemory( text, textLength, GetFileName(), GetLineNum() );
	sdDeclParseHelper declHelper( this, text, textLength, src );
	src.SkipUntilString("{");

	while (1) {
		if( !src.ReadToken( &token )) {
			src.Error( "sdDeclAmbientCubeMap::Parse: unexpected end of file." );
			break;
		}

		if (!token.Icmp("}")) {
			break;
		}

		if( !token.Icmp( "AmbientLight" )) {
			if(!ParseAmbientLight(&src))
			{
				src.SkipBracedSection(false);
				break;
			}
			continue;
		}

		src.Warning( "sdDeclAmbientCubeMap::Parse: unexpected token '%s'.", token.c_str() );
		src.SkipBracedSection(false);
		break;
	}

	return true;
}

void sdDeclAmbientCubeMap::FreeData() {
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
}

void sdDeclAmbientCubeMap::CacheFromDict( const idDict& dict ) {

}
	
bool sdDeclAmbientCubeMap::Save() {
	return true;
}

void sdDeclAmbientCubeMap::GenerateImages() {
}

bool sdDeclAmbientCubeMap::ParseAmbientLight( idParser *src ) {
	idToken token;
	if( !src->ExpectTokenString( "{" )) {
		src->Error( "sdDeclAmbientCubeMap::ParseAmbientLight: expected {." );
		return false;
	}

	ambientLight_t item;
	while (1) {
		if( !src->ReadToken( &token )) {
			src->Error( "sdDeclAmbientCubeMap::ParseAmbientLight: unexpected end of file." );
			break;
		}

		if (!token.Icmp("}")) {
			break;
		}

		if (!token.Icmp("Color")) {
			item.color[0] = src->ParseFloat();
			item.color[1] = src->ParseFloat();
			item.color[2] = src->ParseFloat();
			continue;
		}

		if (!token.Icmp("Direction")) {
			item.dir[0] = src->ParseFloat();
			item.dir[1] = src->ParseFloat();
			item.dir[2] = src->ParseFloat();
			continue;
		}

		src->Warning( "sdDeclAmbientCubeMap::ParseAmbientLight: unexpected token '%s'.", token.c_str() );
		src->SkipBracedSection(false);
		break;
	}
	ambientLights.Append(item);

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

