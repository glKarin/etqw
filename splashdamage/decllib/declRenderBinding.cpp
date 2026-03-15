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

	struct textureData_t {
		idImage*		image;
		textureDepth_t	defaultDepth;
		cubeFiles_t		defaultCubeMap;
	};

	union bindingData_t {
		float			vector[4];
		textureData_t	texture;
		int				attrib;
	};

	type = BT_VECTOR;

	defaults.vector[0] = defaults.vector[1] = defaults.vector[2] = defaults.vector[3] = 0.0f;
	defaults.texture.image = NULL;
	defaults.texture.defaultDepth = TD_DEFAULT;
	defaults.texture.defaultCubeMap = CF_2D;

	infrequent = 0;

	data = defaults;

	return true;
}

void sdDeclRenderBinding::FreeData() {
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
