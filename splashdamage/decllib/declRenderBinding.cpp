// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "renderer/Image.h"

#include "declRenderBinding.h"

/*
===============================================================================

sdDeclRenderBinding

===============================================================================
*/

const char* sdDeclRenderBinding::DefaultDefinition( void ) const {
	return "{  }";
}

bool sdDeclRenderBinding::Parse( const char* text, const int textLength ) {
	return true;
}

void sdDeclRenderBinding::FreeData() {
	type = BT_VECTOR;

	defaults.vector[0] = defaults.vector[1] = defaults.vector[2] = defaults.vector[3] = 0.0f;
	defaults.texture.image = NULL;
	defaults.texture.defaultDepth = TD_DEFAULT;
	defaults.texture.defaultCubeMap = CF_2D;

	infrequent = 0;

	data = defaults;
}

void sdDeclRenderBinding::List( void ) const {
}

bool sdDeclRenderBinding::ParseVector( idParser& src ) {
	return true;
}

bool sdDeclRenderBinding::ParseTexture( idParser& src ) {
	return true;
}

bool sdDeclRenderBinding::ParseAttrib( idParser& src ) {
	return true;
}
