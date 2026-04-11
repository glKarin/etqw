// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declRenderProgram.h"
#include "framework/DeclParseHelper.h"

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
	Init();
	idParser src;
	idToken	token;

	src.SetFlags(DECL_LEXER_FLAGS);
	//src.LoadMemory( text, textLength, GetFileName(), GetLineNum() );
	sdDeclParseHelper declHelper( this, text, textLength, src );
	src.SkipUntilString("{");

	while (1) {
		if( !src.ReadToken( &token )) {
			src.Error( "sdDeclRenderProgram::Parse: unexpected end of file." );
			break;
		}

		if (!token.Icmp("}")) {
			break;
		}

		if( !token.Icmp( "program" )) {
			if(!ParseShader(src)) {
				src.SkipUntilString("{");
				src.SkipBracedSection(false);
			}
			continue;
		}

		if( !token.Icmp( "state" )) {
			src.ReadToken(&token);
			src.SkipBracedSection(true);
			continue;
		}

		if( !token.Icmp( "hwSkinningVersion" )) {
			src.ReadToken(&token);
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "instanceVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		src.Warning( "sdDeclRenderProgram::Parse: unexpected token '%s'.", token.c_str() );
		src.SkipBracedSection(false);
		break;
	}

	return true;
}

void sdDeclRenderProgram::FreeData() {
	Init();
}

sdRenderProgramShader::sdRenderProgramShader(void)
	: type(ST_INVALID),
	lang(SL_UNKNOWN)
{
}

void sdRenderProgramShader::Init(void)
{
	type = ST_INVALID;
	lang = SL_UNKNOWN;
	source.Clear();
}

bool sdRenderProgramShader::IsValid(void) const
{
	return type != ST_INVALID && lang != SL_UNKNOWN && !source.IsEmpty();
}

bool sdRenderProgramShader::Parse(idParser &src)
{
	idToken	token;

	if(!src.ReadToken(&token))
	{
		src.Warning( "sdDeclRenderProgram::ParseShader: unable parse shader language." );
		return false;
	}

	if(!token.Icmp("cg")) {
		lang = SL_CG;
	}
	else if(!token.Icmp("glsl")) {
		lang = SL_GLSL;
	}
	else if(!token.Icmp("arb")) {
		lang = SL_ARB;
	}
	else if(!token.Icmp("hlsl")) {
		lang = SL_HLSL;
	}
	else {
		src.Warning( "sdDeclRenderProgram::ParseShader: unknown shader language '%s'.", token.c_str() );
		return false;
	}

	while( 1 ) {
		if( !src.ReadToken( &token )) {
			src.Warning( "sdRenderProgramShader::Parse: unexpected end of file." );
			return false;
		}
		if (!token.Icmp("{")) {
			src.UnreadToken(&token);
			break;
		}

		if(!token.Icmp("userDecompress"));
		else
			src.Warning( "sdRenderProgramShader::Parse: unknown shader flag '%s'.", token.c_str() );
	}

	idStr text;
	src.ParseBracedSection(text, -1, true);
	text.StripTrailingWhitespace();
	text.StripLeadingOnce("{");
	text.StripTrailingOnce("}");
	text.StripTrailingWhitespace();
	text.StripQuotes();
	text.StripTrailingWhitespace();
	if (!sdDeclTemplate::ExpandTemplate(source, text.c_str(), text.Length()))
		source = text;

	return IsValid();
}

void sdDeclRenderProgram::Init(void)
{
	vertex.Init();
	fragment.Init();
}

bool sdDeclRenderProgram::ParseShader(idParser &src)
{
	idToken	token;

	if(!src.ReadToken(&token))
	{
		src.Warning( "sdDeclRenderProgram::ParseShader: unable parse shader type." );
		return false;
	}

	sdRenderProgramShader::shaderType_t type;
	if(!token.Icmp("vertex")) {
		type = sdRenderProgramShader::ST_VERTEX;
	}
	else if(!token.Icmp("fragment")) {
		type = sdRenderProgramShader::ST_FRAGMENT;
	}
	else {
		src.Warning( "sdDeclRenderProgram::ParseShader: unknown shader type '%s'.", token.c_str() );
		return false;
	}

	sdRenderProgramShader *shader;
	if(type == sdRenderProgramShader::ST_VERTEX) {
		shader = &vertex;
	}
	else {
		shader = &fragment;
	}

	src.ReadToken(&token);
	if(!token.Icmp("reference")) {
		if( !src.ReadToken( &token )) {
			src.Warning( "sdDeclRenderProgram::ParseShader: expect reference shader program name." );
			return false;
		}
		const idDecl *decl = declManager->FindType(DECL_RENDERPROGRAM, token, false);
		if( !decl ) {
			src.Warning( "sdDeclRenderProgram::ParseShader: could't find reference shader program '%s'.", token.c_str() );
			return false;
		}
		const sdDeclRenderProgram *program = static_cast<const sdDeclRenderProgram *>(decl);
		if(shader->type == sdRenderProgramShader::ST_VERTEX)
			*shader = program->vertex;
		else
			*shader = program->fragment;
		shader->type = type;
	}
	else
	{
		src.UnreadToken(&token);

		shader->type = type;
		if(!shader->Parse(src)) {
			shader->Init();
			return false;
		}
	}

	const char *typeName;
	if(shader->type == sdRenderProgramShader::ST_VERTEX)
		typeName = "vert";
	else
		typeName = "frag";
	const char *langName;
	switch(shader->lang)
	{
		case sdRenderProgramShader::SL_CG:
			langName = "cg";
			break;
		case sdRenderProgramShader::SL_GLSL:
			langName = "glsl";
			break;
		case sdRenderProgramShader::SL_HLSL:
			langName = "hlsl";
			break;
		case sdRenderProgramShader::SL_ARB:
		default:
			langName = "arb";
			break;
	}

	fileSystem->WriteFile(va("progs/%s.%s.%s", GetName(), typeName, langName), shader->source.c_str(), shader->source.Length());
	return true;
}
