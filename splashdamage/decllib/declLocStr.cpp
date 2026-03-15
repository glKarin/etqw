// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declLocStr.h"

/*
===============================================================================

	sdDeclLocStr

===============================================================================
*/

size_t sdDeclLocStr::Size( void ) const {
	return sizeof(sdDeclLocStr);
}

const char * sdDeclLocStr::DefaultDefinition() const {
	return "{  }";
}

bool sdDeclLocStr::Parse( const char *text, const int textLength ) {
	locText.Clear();
	numArgs = 0;
	return true;
}

void sdDeclLocStr::FreeData( void ) {
}

void sdDeclLocStr::Print( void ) const {
}

bool sdDeclLocStr::Format( idWStr& result, const idWStrList& inputs ) const {
	result = locText;
	return true;
}
