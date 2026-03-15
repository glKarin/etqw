// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "DeclSurfaceType.h"

/*
===============================================================================

sdDeclSurfaceType

===============================================================================
*/

const char* sdDeclSurfaceType::DefaultDefinition( void ) const {
	return "{  }";
}

bool sdDeclSurfaceType::Parse( const char *text, const int textLength ) {
	type.Clear();
	properties.Clear();
	return true;
}

void sdDeclSurfaceType::FreeData() {
}
