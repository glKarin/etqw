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

		if (!token.Icmp(">")) {
			break;
		}

		parameters.Append(token);
	}

	return true;
}

void sdDeclTemplate::Expand(idLexer &src, idStr &newDecl) const {
	idToken token;
	newDecl.Append(text);

	src.ExpectTokenString("<");
	for (int i = 0; i < parameters.Num(); i++ )
	{
		src.ReadToken(&token);
#if 0
		if (token.type == TT_STRING) {
			idStr str("\"");
			str.Append(token);
			str.Append("\"");
			newDecl.Replace(parameters[i].c_str(), idStr("\"") + token + idStr("\""));
		}
		else
#endif
			newDecl.Replace(parameters[i].c_str(), token);
	}
	src.ExpectTokenString(">");
}

//karin: helper struct
struct rvGuidePlaceholder
{
	// [start, end)
	int start; // include
	int end; // exclude
	idStr replaceStr;

	rvGuidePlaceholder(int start = 0, int end = 0, const idStr &str = idStr())
		: start(start),
		end(end),
		replaceStr(str)
	{ }
	void ReplaceSpace(idStr &str)
	{
		for(int m = start; m < end; m++)
		{
			if(!isspace(str[m])) //karin: keep raw format for debug
				str[m] = ' ';
		}
	}
	int Replace(int offset, idStr &toStr)
	{
		int length = end - start;
		int newLength = replaceStr.Length();
		idStr front = toStr.Left(start + offset);
		idStr back = toStr.Right(toStr.Length() - end - offset);
		toStr = front + replaceStr + back;
		return newLength - length;
	}
};

struct rvGuidePlaceholderList : public idList<rvGuidePlaceholder>
{
	void ReplaceSpace(idStr &str)
	{
		for(int i = 0; i < Num(); i++)
			this->operator[](i).ReplaceSpace(str);
	}
	void Replace(idStr &str)
	{
		int offset = 0;
		for(int i = 0; i < Num(); i++)
		{
			offset += this->operator[](i).Replace(offset, str);
		}
	}
};

bool sdDeclTemplate::Expand(idStr &finalBuffer, const char *text, int textLength) {
	idStr _text(text, 0, textLength);
	if (_text.Find("useTemplate") == -1)
		return false;

	bool ret = false;
    idLexer src;
    idToken	token, token2;

    finalBuffer = "";

    src.LoadMemory(_text, textLength, "", 0);
    src.SetFlags(DECL_LEXER_FLAGS);
	rvGuidePlaceholderList guideRanges; //karin: record a pair of read guide characters offset: start, end, characters in range will be replaced ' '

    while (1)
    {
        if (!src.ReadToken(&token))
        {
            break;
        }

        if (token == "useTemplate")
        {
			int range_start = src.GetFileOffset() - idStr::Length("useTemplate"); //karin: record range start before next `ReadToken`
            idToken name;
            idStr newDecl;

            src.ReadToken(&name);
        	const idDecl *decl = declManager->FindType(DECL_TEMPLATE, name, false);

        	if (decl == NULL)
        	{
        		common->Warning("Failed to find template '%s'\n", token.c_str());
        		// skip this template
        		src.SkipUntilString("<");
        		src.SkipUntilString(">");
        		continue;
        	}

        	const sdDeclTemplate *declTemplate = static_cast<const sdDeclTemplate *>(decl);

        	declTemplate->Expand(src, newDecl);

            newDecl += "\n";

            finalBuffer += newDecl;
			int range_end = src.GetFileOffset(); //karin: record range end after last `ReadToken`
			guideRanges.Append(rvGuidePlaceholder(range_start, range_end));
        	ret = true;
        }
    }

	//karin: replace all old guide source to space
	idStr oldText(_text);
	guideRanges.ReplaceSpace(oldText);
    finalBuffer += oldText;

	Sys_Printf("OOO|%s|\n----------------\n|%s|\n", _text.c_str(), finalBuffer.c_str());

	return ret;
}

