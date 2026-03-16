// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declLocStr.h"
#include "framework/DeclParseHelper.h"

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
	idParser src;
	idToken	token, token2;

	src.SetFlags(DECL_LEXER_FLAGS);
	//src.LoadMemory( text, textLength, GetFileName(), GetLineNum() );
	sdDeclParseHelper declHelper( this, text, textLength, src );
	src.SkipUntilString("{");

	while (1) {
		if( !src.ReadToken( &token )) {
			src.Error( "sdDeclLocStr::Parse: unexpected end of file." );
			break;
		}

		if (!token.Icmp("}")) {
			break;
		}

		if( !token.Icmp( "text" )) {
			if( !src.ReadToken(&token2)) {
				src.Error( "sdDeclLocStr::Parse: failed to parse text" );
				break;
			}
			locText = common->GetLanguageDict()->GetString(token);
			continue;
		}

		if( !token.Icmp( "arguments" )) {
			numArgs = src.ParseInt();
			continue;
		}

		if( !src.ReadToken( &token )) {
			src.Error( "sdDeclLocStr::Parse: unexpected token '%s'.", token.c_str() );
			src.SkipBracedSection(false);
			break;
		}
	}

	return true;
}

void sdDeclLocStr::FreeData( void ) {
	locText.Clear();
	numArgs = 0;
}

void sdDeclLocStr::Print( void ) const {
}

bool sdDeclLocStr::Format( idWStr& result, const idWStrList& inputs ) const {
	result = common->LocalizeText(this, inputs);
	return true;
}
