// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declImposter.h"

void sdImposterSubImage::Write( idFile_Memory &f ) {
}

bool sdImposterSubImage::Read( idParser &src ) {
	return true;
}

static void imposterInfo_t_Init(sdDeclImposter::imposterInfo_t &info) {
	info.images.Clear();
	info.material = NULL;
	info.origin.Zero();
	info.scalex = 1.0f;
	info.scaley = 1.0f;
	info.screenScale = 1.0f;
	info.tileSize = 0;
	info.numAngles = 0;
}

sdDeclImposter::sdDeclImposter( void ) {
	imposterInfo_t_Init(info);
}

	// Override from idDecl
const char* sdDeclImposter::DefaultDefinition( void ) const {
	return "{  }";
}

bool sdDeclImposter::Parse( const char *text, const int textLength ) {
	imposterInfo_t_Init(info);
	return true;
}

void sdDeclImposter::FreeData( void ) {

}

void sdDeclImposter::CacheFromDict( const idDict& dict ) {

}

bool sdDeclImposter::Save( void ) {
	return true;
}

void sdDeclImposter::RebuildTextSource( void ) {

}



sdDeclImposterGenerator::sdDeclImposterGenerator( void )
	: vertexColor(false),
	  numAngles(0),
	  noBump(false),
	  startAngle(0.0f),
	  screenScale(1.0f)
{
	tileSize[0] = tileSize[1] = 0;
}

const char* sdDeclImposterGenerator::DefaultDefinition( void ) const {
	return "{  }";
}

bool sdDeclImposterGenerator::Parse( const char *text, const int textLength ) {
	return true;
}

void sdDeclImposterGenerator::FreeData( void ) {

}
