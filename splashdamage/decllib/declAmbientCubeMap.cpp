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

const idMat3 & CubeMap_GetAxis(int a1)
{
	static bool isInit = false;
	static idMat3 axis[6];
	if (!isInit)
	{
		// forward = east (positive x-axis in DR)
		axis[0][0][0] = 1;
		axis[0][1][1] = 1;
		axis[0][2][2] = 1;

		// left = north
		axis[1][0][1] = 1;
		axis[1][1][0] = -1;
		axis[1][2][2] = 1;

		// right = south
		axis[2][0][1] = -1;
		axis[2][1][0] = 1;
		axis[2][2][2] = 1;

		// back = west
		axis[3][0][0] = -1;
		axis[3][1][1] = -1;
		axis[3][2][2] = 1;

		// down, while facing forward
		axis[4][0][2] = -1;
		axis[4][1][1] = 1;
		axis[4][2][0] = 1;

		// up, while facing forward
		axis[5][0][2] = 1;
		axis[5][1][1] = 1;
		axis[5][2][0] = -1;
		isInit = true;
	}
	return axis[a1];
}

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

	ambientCubeMapImageFunctor.Init(this, &sdDeclAmbientCubeMap::AmbientCubeMapImage);
	lightCubeMapImageFunctor.Init(this, &sdDeclAmbientCubeMap::LightCubeMapImage);
	specularCubeMapImageFunctor.Init(this, &sdDeclAmbientCubeMap::SpecularCubeMapImage);
	gradientMapImageFunctor.Init(this, &sdDeclAmbientCubeMap::GradientMapImage);
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

		if (!token.Icmp("ambientColor")) {
			avgAmbientColor[0] = src.ParseFloat();
			avgAmbientColor[1] = src.ParseFloat();
			avgAmbientColor[2] = src.ParseFloat();
			continue;
		}

		if (!token.Icmp("highLightColor")) {
			highLightColor[0] = src.ParseFloat();
			highLightColor[1] = src.ParseFloat();
			highLightColor[2] = src.ParseFloat();
			continue;
		}

		if (!token.Icmp("brightness")) {
			brightness = src.ParseFloat();
			continue;
		}

		if( !token.Icmp( "envMap" )) {
			if( !src.ReadToken(&token)) {
				src.Error( "sdDeclAmbientCubeMap::Parse: failed to parse envMap" );
				break;
			}
			envMap = token.c_str();
			continue;
		}

		if (!token.Icmp("minSpecAmbientColor")) {
			minSpecAmbientColor[0] = src.ParseFloat();
			minSpecAmbientColor[1] = src.ParseFloat();
			minSpecAmbientColor[2] = src.ParseFloat();
			continue;
		}

		if (!token.Icmp("minSpecShadowColor")) {
			minSpecShadowColor[0] = src.ParseFloat();
			minSpecShadowColor[1] = src.ParseFloat();
			minSpecShadowColor[2] = src.ParseFloat();
			continue;
		}

		if (!token.Icmp("indoors")) {
			indoors = true;
			continue;
		}

		src.Warning( "sdDeclAmbientCubeMap::Parse: unexpected token '%s'.", token.c_str() );
		src.SkipBracedSection(false);
		break;
	}

	GenerateImages();

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
	const idKeyValue* kv = NULL;

	while( kv = dict.MatchPrefix( "ambientCubeMap", kv ) ) {
		if ( kv->GetValue().Length() ) {
			declAmbientCubeMapType[ kv->GetValue() ];
		}
	}
}
	
bool sdDeclAmbientCubeMap::Save() {
	return true;
}

void sdDeclAmbientCubeMap::GenerateImages() {
	idStr path = GetFileName();
	path.StripFilename();
	if (!ambientCubeMap)
		ambientCubeMap = globalImages->AllocImage(va("%s/%s_ambientCubeMap", path.c_str(), GetName()));

	AmbientCubeMapImage(ambientCubeMap);
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

		if (!token.Icmp("name")) {
			if( !src->ReadToken(&token)) {
				src->Error( "sdDeclAmbientCubeMap::ParseAmbientLight: failed to parse name" );
				break;
			}
			item.name = token.c_str();
			continue;
		}

		if (!token.Icmp("specular")) {
			item.specular = src->ParseBool();
			continue;
		}

		if (!token.Icmp("ambient")) {
			item.ambient = src->ParseBool();
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
	byte ** v3; // esi
	int v4; // edx
	int v5; // ebp
	int i; // ecx
	float v7; // [esp+14h] [ebp-10h]
	float ** fptr = cubeMapFloat;

	v3 = cubeMapByte;
	v4 = 4 * faceSize * faceSize;
	v5 = 6;
	do
	{
		for (i = 0; i < v4; ++i)
		{
			v7 = (*fptr)[4 * i] * 255.0f;
			(*v3)[i] = (int)fminf(fmaxf(v7, 0.0f), 255.0f);
		}
		++v3;
		fptr++;
		--v5;
	}
	while (v5);
}

void sdDeclAmbientCubeMap::BakeLight( float* cubeMap[6], const int faceSize, const idVec3& lightDir, const idVec3& lightColor) {
	int i; // edi
  int v5; // ecx
  int v6; // edx
  double v7; // st7
  double v8; // st6
  int v9; // esi
  double v10; // st2
  float *v11; // edx
  double v12; // st7
  double v13; // st6
  double v14; // st7
  int v15; // [esp+10h] [ebp-78h]
  float v16; // [esp+14h] [ebp-74h]
  float v17; // [esp+14h] [ebp-74h]
  float v18; // [esp+14h] [ebp-74h]
  float v19; // [esp+14h] [ebp-74h]
  float v20; // [esp+14h] [ebp-74h]
  float v21; // [esp+14h] [ebp-74h]
  float v22; // [esp+14h] [ebp-74h]
  float v23; // [esp+14h] [ebp-74h]
  float v24; // [esp+14h] [ebp-74h]
  float v25; // [esp+14h] [ebp-74h]
  int v26; // [esp+18h] [ebp-70h]
  double v27; // [esp+28h] [ebp-60h]
  idVec3 v28; // [esp+34h] [ebp-54h]
  idVec3 v29; // [esp+40h] [ebp-48h]
  idVec3 v30; // v30 v31 v32
  idVec3 v33; // [esp+58h] [ebp-30h]
  idVec3 v34; // v34 v35 v36
  idVec3 v37; // v37 v38 v39
  idVec3 v40; // v40 v41 v42

  for ( i = 0; i < 6; ++i )
  {
    v29 = CubeMap_GetAxis(i)[0];
    v28 = CubeMap_GetAxis(i)[1];
    v33 = CubeMap_GetAxis(i)[2];
    v5 = faceSize;
    v15 = 0;
    if ( faceSize > 0 )
    {
      v6 = 16 * faceSize;
      v7 = (double)faceSize - 1.0;
      v27 = v7;
      v8 = 2.0;
      do
      {
        v26 = 0;
        v9 = 16 * v15;
        v16 = -((double)v15 * v8 / v7 - 1.0);
        v30 = v28 * v16;
        do
        {
          v17 = -((double)v26 * v8 / v7 - 1.0);
          v37 = v33 * v17;
          v34 = v29 + v30;
          v40 = v34 + v37;
          v18 = lightDir * v40;
          v10 = v18;
          if ( v18 >= 0.0 )
          {
            v19 = v40.LengthSqr();
            v20 = sqrt(v19);
            v11 = cubeMap[i];
            v21 = v10 / v20;
            v12 = v21;
            v22 = lightColor.x * v21 + *(float *)((char *)v11 + v9);
            if ( v22 >= 1.0 )
              v22 = 1.0;
            *(float *)((char *)v11 + v9) = v22;
            v23 = lightColor.y * v12 + *(float *)((char *)v11 + v9 + 4);
            if ( v23 >= 1.0 )
              v23 = 1.0;
            *(float *)((char *)v11 + v9 + 4) = v23;
            v13 = v12 * lightColor.z + *(float *)((char *)v11 + v9 + 8);
            v14 = 1.0;
            v24 = v13;
            if ( v24 < 1.0 )
              v14 = v24;
            v5 = faceSize;
            v25 = v14;
            *(float *)((char *)v11 + v9 + 8) = v25;
            v6 = 16 * faceSize;
            v8 = 2.0;
            v7 = v27;
          }
          v9 += v6;
          ++v26;
        }
        while ( v26 < v5 );
        ++v15;
      }
      while ( v15 < v5 );
    }
  }
}

void sdDeclAmbientCubeMap::BakeLight( float* cubeMap[6], const int faceSize, const idVec3& lightDir, const idVec3& lightColor, const float power ) {
}

void sdDeclAmbientCubeMap::BakeGradientMap( byte* pic, const int size, const idVec3& ambientColor, const idVec3& highLightColor ) {
}

void sdDeclAmbientCubeMap::UploadCubeMap( idImage* image, const byte* cubeMap[6], const int faceSize ) {
	int i; // esi

	if ( !image->IsLoaded()) // a1->vtbl + 4
		image->Reload(false, true); // a1->vtbl + 11
	image->Bind(); // a1->vtbl + 2
	for ( i = 0; i < 6; ++i )
		qglTexImage2D(i + GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, faceSize, faceSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubeMap[i]);
}

void sdDeclAmbientCubeMap::AmbientCubeMapImage( idImage* image ) {
  int v4; // ebx
  float **v5; // edi
  int v6; // edi
  sdDeclAmbientCubeMap::ambientLight_t *list; // ecx
  float **v8; // ecx
  int v9; // edx
  int v10; // eax
  int v11; // edi
  double v12; // st6
  float *v13; // edi
  float *v14; // ebx
  double v15; // st6
  float v16; // [esp+10h] [ebp-Ch]
  float *v17; // [esp+14h] [ebp-8h]
  float *v18; // [esp+18h] [ebp-4h]
  float v19; // [esp+18h] [ebp-4h]
  float v20; // [esp+18h] [ebp-4h]
  float v21; // [esp+18h] [ebp-4h]
  float v22; // [esp+18h] [ebp-4h]
  float v23; // [esp+18h] [ebp-4h]
  float v24; // [esp+18h] [ebp-4h]
  float v25; // [esp+18h] [ebp-4h]
  float v26; // [esp+18h] [ebp-4h]
  float v27; // [esp+18h] [ebp-4h]

	for(int i = 0; i < 6; i++)
	{
		cubeMapFloat[i] = &cubeMapDataFloat[BAKEDLIGHT_SIZE * BAKEDLIGHT_SIZE * 4 * i];
		cubeMapByte[i] = &cubeMapDataByte[BAKEDLIGHT_SIZE * BAKEDLIGHT_SIZE * 4 * i];
	}

	float **off_81DA44 = &cubeMapFloat[0];
	byte **off_81DA5C = &cubeMapByte[0];

  v4 = 0;
  image->generatorFunctor = NULL;
  image->Reload(false, true); // (_DWORD *)image->vtbl + 11
  for (int i = 0; i < 6; i++ )
  {
  	memset(*off_81DA44++, 0, 0x10000u);
  	memset(*off_81DA5C++, 0, 0x4000u);
  }
  if ( this->ambientLights.Num() > 0 )
  {
    v6 = 0;
    do
    {
      list = this->ambientLights.Ptr();
      if ( list[v6].ambient )
        sdDeclAmbientCubeMap::BakeLight(cubeMapFloat, BAKEDLIGHT_SIZE, list[v6].dir, list[v6].color);
      ++v4;
      ++v6;
    }
    while ( v4 < this->ambientLights.Num() );
  }
  v8 = off_81DA44;
  this->avgAmbientColor.w = 0.0;
  this->avgAmbientColor.z = 0.0;
  this->avgAmbientColor.y = 0.0;
  this->avgAmbientColor.x = 0.0;
  v16 = 0.0;
  do
  {
    v9 = -2;
    v10 = 3;
    do
    {
      v11 = (v9 - 2) & 3;
      v12 = (*v8)[v10 - 3] + *(&this->avgAmbientColor.x + v11);
      v13 = &this->avgAmbientColor.x + v11;
      *v13 = v12;
      v14 = &this->avgAmbientColor.x + ((v9 - 1) & 3);
      *v14 = (*v8)[v10 - 2] + *v14;
      v17 = &this->avgAmbientColor.x + (v9 & 3);
      v15 = (*v8)[v10 - 1] + *v17;
      v10 += 8;
      *v17 = v15;
      v18 = &this->avgAmbientColor.x + ((v9 + 1) & 3);
      v9 += 8;
      *v18 = (*v8)[v10 - 8] + *v18;
      *v13 = (*v8)[v10 - 7] + *v13;
      *v14 = (*v8)[v10 - 6] + *v14;
      *v17 = (*v8)[v10 - 5] + *v17;
      *v18 = (*v8)[v10 - 4] + *v18;
      v19 = v16 + 1.0f;
      v20 = v19 + 1.0f;
      v21 = v20 + 1.0f;
      v22 = v21 + 1.0f;
      v23 = v22 + 1.0f;
      v24 = v23 + 1.0f;
      v25 = v24 + 1.0f;
      v16 = v25 + 1.0f;
    }
    while ( v9 + 2 < 0x4000 );
    ++v8;
  }
  while ( v8 < off_81DA44 );
  v26 = v16 * 0.25f;
  v27 = 1.0f / v26;
  this->avgAmbientColor.x = this->avgAmbientColor.x * v27;
  this->avgAmbientColor.y = this->avgAmbientColor.y * v27;
  this->avgAmbientColor.z = this->avgAmbientColor.z * v27;
  this->avgAmbientColor.w = v27 * this->avgAmbientColor.w;
  sdDeclAmbientCubeMap::CubeMapFtob(cubeMapFloat/*off_81DA44*/, cubeMapByte/*off_81DA5C*/, BAKEDLIGHT_SIZE);
  sdDeclAmbientCubeMap::UploadCubeMap(image, (const byte **)cubeMapByte/*off_81DA5C*/, BAKEDLIGHT_SIZE);
  image->generatorFunctor = &this->ambientCubeMapImageFunctor;
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

void sdDeclAmbientCubeMap::SetSunParameters( const idVec3& sunDirection, const idVec3& sunColor ) {
	this->sunDirection = sunDirection;
	this->sunColor = sunColor;
}

