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
			this->text.StripTrailingWhitespace();
			this->text.StripLeadingOnce("{");
			this->text.StripTrailingOnce("}");
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

		if (!token.Cmp(">")) {
			break;
		}

		if (!token.Cmp(",")) {
			continue;
		}

		parameter_t parm;
		parm.name = token.c_str();
		
		src.ReadToken(&token);
		if(!token.Cmp("="))
		{
			src.ReadToken(&token);
			parm.defaultValue = token.c_str();
		}
		else
			src.UnreadToken(&token);

		parameters.Append(parm);
	}

	return true;
}

void sdDeclTemplate::Expand(idLexer &src, idStr &newDecl) const {
	idToken token;
	newDecl.Append(text);

	src.ExpectTokenString("<");
	for (int i = 0; i < parameters.Num(); i++ )
	{
		const parameter_t &parm = parameters[i];
		src.ReadToken(&token);
		if(token == ">")
		{
			src.UnreadToken(&token);
			newDecl.Replace(parm.name.c_str(), parm.defaultValue.c_str());
		}
		else
		{
#if 0
		if (token.type == TT_STRING) {
			idStr str("\"");
			str.Append(token);
			str.Append("\"");
			newDecl.Replace(parameters[i].c_str(), idStr("\"") + token + idStr("\""));
		}
		else
#endif
			newDecl.Replace(parm.name.c_str(), token);
		}
	}
	src.ExpectTokenString(">");
}

int sdDeclTemplate::ReplacePlaceholder( int start, // include
		int end, // exclude
		idStr replaceStr, idStr &toStr)
{
	int length = end - start;
	int newLength = replaceStr.Length();
	idStr front = toStr.Left(start);
	idStr back = toStr.Right(toStr.Length() - end);
	toStr = front + replaceStr + back;
	return newLength - length;
}

bool sdDeclTemplate::ExpandTemplate(idStr &finalBuffer, const char *text, int textLength) {
	idStr _text(text, 0, textLength);
	if (_text.Find("useTemplate") == -1)
		return false;

	bool ret = false;
    idLexer src;
    idToken	token, token2;

    src.LoadMemory(_text, textLength, "", 0);
    src.SetFlags(DECL_LEXER_FLAGS);

    while (1)
    {
        if (!src.ReadToken(&token))
            break;

        if (idStr::Icmp(token, "useTemplate"))
			continue;

		int range_start = src.GetFileOffset() - idStr::Length("useTemplate"); //karin: record range start before next `ReadToken`
		idToken name;

		src.ReadToken(&name);
		const idDecl *decl = declManager->FindType(DECL_TEMPLATE, name, false);

		if (!decl)
		{
			common->Warning("Failed to find template '%s'", name.c_str());
			// skip this template
			src.SkipUntilString("<");
			src.SkipUntilString(">");
			continue;
		}

		const sdDeclTemplate *declTemplate = static_cast<const sdDeclTemplate *>(decl);

		idStr newDecl;
		declTemplate->Expand(src, newDecl);

		finalBuffer = _text;

		int range_end = src.GetFileOffset(); //karin: record range end after last `ReadToken`
		ReplacePlaceholder(range_start, range_end, newDecl, finalBuffer);
		ret = true;

		newDecl = "";
		if(ExpandTemplate(newDecl, finalBuffer.c_str(), finalBuffer.Length()))
		{
			finalBuffer = newDecl;
		}
		break;
	}

	return ret;
}

