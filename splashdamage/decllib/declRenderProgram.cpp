// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declRenderProgram.h"

/*
===============================================================================

sdDeclRenderProgram

===============================================================================
*/

sdDeclRenderProgram::sdDeclRenderProgram() {

}

const char* sdDeclRenderProgram::DefaultDefinition( void ) const {
    return "{  }";
}

bool sdDeclRenderProgram::Parse( const char* text, const int textLength ) {
    return true;
}

void sdDeclRenderProgram::FreeData() {

}

