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

		if( !token.Icmp( "parameters" )) {
			if(!ParseParameters(src))
			{
				src.SkipBracedSection(false);
				break;
			}
			continue;
		}

		if( !token.Icmp( "text" )) {
			ParseText(src);
			continue;
		}

		if( !token.Icmp( "commands" )) {
			if(!ParseCommands(src))
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
	decls.Clear();
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

bool sdDeclTemplate::ParseCondition( idParser &src, condition_t &cond ) {
	idToken token;
	if( !src.ExpectTokenString( "(" )) {
		src.Error( "sdDeclTemplate::ParseCondition: expected (." );
		return false;
	}

	if( !src.ReadToken( &token )) {
		src.Error( "sdDeclTemplate::ParseCondition: unable to read variable name." );
		return false;
	}
	cond.name = token.c_str();

	if( !src.ReadToken( &token )) {
		src.Error( "sdDeclTemplate::ParseCondition: unable to read operator." );
		return false;
	}
	cond.op = token.c_str();

	if( !src.ReadToken( &token )) {
		src.Error( "sdDeclTemplate::ParseCondition: unable to read value." );
		return false;
	}
	cond.value = token.c_str();

	src.ExpectTokenString( ")" );

	return true;
}

bool sdDeclTemplate::ParseCommand( idParser &src, command_t &cmd ) {
	idToken token;
	if( !src.ExpectTokenString( "if" )) {
		src.Error( "sdDeclTemplate::ParseCommand: expected if." );
		return false;
	}

	if( !ParseCondition(src, cmd.cond)) {
		return false;
	}

	if( !src.ExpectTokenString( "{" )) {
		src.Error( "sdDeclTemplate::ParseCommand: expected {." );
		return false;
	}

	while (1) {
		if( !src.ReadToken( &token )) {
			src.Error( "sdDeclTemplate::ParseCommand: unexpected end of file." );
			break;
		}

		if (!token.Cmp("}")) {
			break;
		}

		if (!token.Icmp("append")) {
			src.ParseBracedSection(cmd.append, -1, true);
			continue;
		}

		src.Warning( "sdDeclTemplate::ParseCommand: unexpected token '%s'.", token.c_str() );
		src.SkipBracedSection(false);
	}

	return true;
}

bool sdDeclTemplate::ParseCommands( idParser &src ) {
	idToken token;
	if( !src.ExpectTokenString( "{" )) {
		src.Error( "sdDeclTemplate::ParseCommands: expected {." );
		return false;
	}

	idList<command_t> commands;
	while (1) {
		if( !src.ReadToken( &token )) {
			src.Error( "sdDeclTemplate::ParseCommands: unexpected end of file." );
			break;
		}

		if (!token.Cmp("}")) {
			break;
		}

		if (!token.Cmp("if")) {
			src.UnreadToken(&token);
			command_t cmd;
			if (ParseCommand(src, cmd)) {
				commands.Append(cmd);
			}
			continue;
		}

		src.Warning( "sdDeclTemplate::ParseCommands: unexpected token '%s'.", token.c_str() );
		src.SkipBracedSection(false);
	}

	if (commands.Num() > 0) {
		decl_t &decl = decls.Alloc();
		decl.type = SEC_COMMANDS;
		decl.commands.Swap(commands);
	}

	return true;
}

bool sdDeclTemplate::ParseText( idParser &src ) {
	idToken token;
	if( !src.ExpectTokenString( "{" )) {
		src.Error( "sdDeclTemplate::ParseText: expected {." );
		return false;
	}

	idStr text;
	src.ParseBracedSection(text, -1, false);
	text.StripTrailingWhitespace();
	text.StripLeadingOnce("{");
	text.StripTrailingOnce("}");

	if (!text.IsEmpty()) {
		decl_t &decl = decls.Alloc();
		decl.type = SEC_TEXT;
		decl.text = text;
	}

	return true;
}

void sdDeclTemplate::ExpandParameters(idLexer &src, idDict &newDecl) const {
	idToken token;

	src.ExpectTokenString("<");
	for (int i = 0; i < parameters.Num(); i++ )
	{
		const parameter_t &parm = parameters[i];
		src.ReadToken(&token);
		if(token == ">")
		{
			src.UnreadToken(&token);
			newDecl.Set(parm.name.c_str(), parm.defaultValue.c_str());
			continue;
		}

		newDecl.Set(parm.name.c_str(), token);

		src.ReadToken(&token);
		if(token != ",") //karin: maybe has ,
		{
			src.UnreadToken(&token);
		}
	}
	src.ExpectTokenString(">");
}

void sdDeclTemplate::ExpandText(const idStr &text, idStr &ret, const idDict &parms) const {
	idStr newDecl;
	newDecl.Append(text);

	for (int i = 0; i < parameters.Num(); i++ )
	{
		const parameter_t &parm = parameters[i];
		const char *token = parms.GetString(parm.name);
		newDecl.Replace(parm.name.c_str(), token);
	}

	ret.Append("\n");
	ret.Append(newDecl);
}

bool sdDeclTemplate::CheckCondition(const condition_t &cond, const idDict &parms) const {
	const char *value = parms.GetString(cond.name, NULL);
	if (!value)
		return false;

	if (!cond.op.Cmp("==")) {
		return cond.value.Icmp(value);
	}
	if (!cond.op.Cmp("!=")) {
		return cond.value.Icmp(value);
	}
	common->Warning("Unknown template command operator '%s' in %s:%s", cond.op.c_str(), GetFileName(), GetName());
	return false;
}

void sdDeclTemplate::ExpandCommand(const command_t &cmd, idStr &ret, const idDict &parms) const {
	if (!CheckCondition(cmd.cond, parms))
		return;

	idStr newDecl;
	newDecl.Append(cmd.append);

	for (int i = 0; i < parameters.Num(); i++ )
	{
		const parameter_t &parm = parameters[i];
		const char *token = parms.GetString(parm.name);
		newDecl.Replace(parm.name.c_str(), token);
	}

	ret.Append("\n");
	ret.Append(newDecl);
}

void sdDeclTemplate::ExpandCommands(const idList<command_t> &cmds, idStr &ret, const idDict &parms) const {
	for (int i = 0; i < cmds.Num(); i++ )
	{
		const command_t &cmd = cmds[i];
		ExpandCommand(cmd, ret, parms);
	}
}

void sdDeclTemplate::ExpandDecl(const decl_t &src, idStr &ret, const idDict &parms) const {
	if (src.type == SEC_COMMANDS)
		ExpandCommands(src.commands, ret, parms);
	else
		ExpandText(src.text, ret, parms);
}


void sdDeclTemplate::Expand(idLexer &src, idStr &ret) const {
	idDict parms;
	ExpandParameters(src, parms);

	for (int i = 0; i < decls.Num(); i++ )
	{
		const decl_t &decl = decls[i];
		ExpandDecl(decl, ret, parms);
	}
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

    src.LoadMemory(_text, textLength, "useTemplate", 0);
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

		idStr newDecl;
		if (!decl)
		{
			common->Warning("Failed to find template '%s'", name.c_str());
			// skip this template
			src.SkipUntilString("<");
			src.SkipUntilString(">");
		}
		else
		{
			const sdDeclTemplate *declTemplate = static_cast<const sdDeclTemplate *>(decl);
			declTemplate->Expand(src, newDecl);
		}

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

