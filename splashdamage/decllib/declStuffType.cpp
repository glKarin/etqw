// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declStuffType.h"

sdDeclStuffType::sdDeclStuffType( void )
	: randomizeAngles(false),
	lodType(NULL)
{

}

const char* sdDeclStuffType::DefaultDefinition( void ) const {
	return "{  }";
}

bool sdDeclStuffType::Parse( const char *text, const int textLength ) {
	models.Clear();
	randomizeAngles = false;
	lodType = NULL;
	return true;
}

void sdDeclStuffType::FreeData( void ) {
}

bool sdDeclStuffType::RebuildTextSource( void ) {
	return true;
}
