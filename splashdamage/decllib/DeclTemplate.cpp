// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "DeclTemplate.h"
#include "framework/DeclParseHelper.h"

/*
===============================================================================

	sdDeclTemplate

===============================================================================
*/

size_t sdDeclTemplate::Size( void ) const {
	return sizeof(sdDeclTemplate);
}

const char * sdDeclTemplate::DefaultDefinition() const {
	return "{  }";
}

bool sdDeclTemplate::Parse( const char *text, const int textLength ) {
	idParser src;
	idToken	token;

	src.SetFlags(DECL_LEXER_FLAGS);
	//src.LoadMemory( text, textLength, GetFileName(), GetLineNum() );
	sdDeclParseHelper declHelper( this, text, textLength, src );
	src.SkipUntilString("{");

	while (1) {
		if( !src.ReadToken( &token )) {
			src.Error( "sdDeclTemplate::Parse: unexpected end of file." );
			break;
		}

		if (!token.Icmp("}")) {
			break;
		}

		if( !token.Icmp( "text" )) {
			src.ParseBracedSection(this->text, -1, true);
			continue;
		}

		if( !token.Icmp( "parameters" )) {
			if(!ParseParameters(src))
			{
				src.SkipBracedSection(false);
				break;
			}
			continue;
		}

		src.Warning( "sdDeclTemplate::Parse: unexpected token '%s'.", token.c_str() );
		src.SkipBracedSection(false);
		break;
	}

	return true;
}

void sdDeclTemplate::FreeData( void ) {
	parameters.Clear();
	text.Clear();
}

void sdDeclTemplate::Print( void ) const {
}

bool sdDeclTemplate::ParseParameters( idParser &src ) {
	idToken token;
	if( !src.ExpectTokenString( "<" )) {
		src.Error( "sdDeclTemplate::ParseParameters: expected <." );
		return false;
	}

	while (1) {
		if( !src.ReadToken( &token )) {
			src.Error( "sdDeclTemplate::ParseParameters: unexpected end of file." );
			break;
		}

		if (!token.Icmp(">")) {
			break;
		}

		parameters.Append(token);
	}

	return true;
}

