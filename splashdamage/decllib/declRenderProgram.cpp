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
			src.SkipUntilString("{");
			src.SkipBracedSection(false);
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

		if( !token.Icmp( "alphaToCoverageVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "notlitVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "depthVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "earlyCullVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "coverageVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "amblitVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "ambientVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "machineSpec" )) {
			src.ParseInt();
			continue;
		}

		if( !token.Icmp( "imposterBrightness" )) {
			src.ParseFloat();
			continue;
		}

		if( !token.Icmp( "fallBack" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "lodVersion" )) {
			src.ReadToken(&token);
			continue;
		}

		if( !token.Icmp( "interaction" )) {
			continue;
		}

		if( !token.Icmp( "lowrangeuv" )) {
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
	sourceRaw.Clear();
	placeholders.Clear();
	source.Clear();
	bindings.Clear();
}

bool sdRenderProgramShader::IsValid(void) const
{
	return type != ST_INVALID && lang != SL_UNKNOWN && !sourceRaw.IsEmpty();
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
	text.ReplaceChar('"', ' ');
	text.StripTrailingWhitespace();
	if (!sdDeclTemplate::ExpandTemplate(sourceRaw, text.c_str(), text.Length()))
		sourceRaw = text;

	return IsValid();
}

const sdDeclRenderBinding * sdRenderProgramShader::GetBinding(const char *name) const {
	for(int i = 0; i < bindings.Num(); i++)
	{
		const sdDeclRenderBinding *binding = bindings[i];
		if(!binding)
			continue;
		if(!idStr::Icmp(binding->GetName(), name))
			return binding;
	}
	return NULL;
}

void sdRenderProgramShader::ParsePost(void) {
	if(sourceRaw.IsEmpty())
		return;

	idParser src;
	src.LoadMemory(sourceRaw.c_str(), sourceRaw.Length(), "shader");
	src.SetFlags(LEXFL_NOFATALERRORS);
	idToken token;
	const idDecl *decl;

	sdStringBuilder_Heap buf;
	while (1) {
		if(!src.ReadToken(&token))
			break;

		if(token.linesCrossed && buf.Length() > 0)
			buf.Append("\n");

		if(token == "$")
		{
			if(!src.ReadToken(&token))
			{
				src.Warning("sdRenderProgramShader::ParsePost: missing placeholder name");
				break;
			}
			placeholders.Append(token);
		}

		if(buf.Length() > 0)
			buf.Append(' ');
		buf.Append(token);
	}

	source = buf.c_str();
	for(int i = 0; i < placeholders.Num(); i++) {
		decl = declManager->FindType(DECL_RENDERBINDING, placeholders[i], false);
		if( !decl ) {
			common->Warning( "sdRenderProgramShader::ParsePost: could't find binding '%s'.", placeholders[i].c_str() );
			bindings.Append(NULL);
		}
		else
			bindings.Append(static_cast<const sdDeclRenderBinding *>(decl));
	}

	bindings.Resize(bindings.Num());
	bindings.SetGranularity(1);
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
	bool isRef = false;
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
		if(type == sdRenderProgramShader::ST_VERTEX)
			*shader = program->vertex;
		else
			*shader = program->fragment;
		shader->type = type;
		isRef = true;
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

//#define RENDERPROGRAM_OUTPUT_TO_FILE 1
#if RENDERPROGRAM_OUTPUT_TO_FILE
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
	fileSystem->WriteFile(va("progs/%s.%s.%s", GetName(), typeName, langName), shader->sourceRaw.c_str(), shader->sourceRaw.Length());
#endif

	if (!isRef)
		shader->ParsePost();

#if RENDERPROGRAM_OUTPUT_TO_FILE
	idStr str;
	str.Append(shader->source.c_str());
	str.Append("\n\n");
	for(int i = 0; i < shader->placeholders.Num(); i++)
	{
		str.Append("$");
		str.Append(shader->placeholders[i]);
		str.Append(" ");
		const sdDeclRenderBinding *binding = shader->GetBinding(i);
		str.Append(va("%d", binding ? (int)binding->GetBindingType() : -1));
		str.Append("\n");
	}
	fileSystem->WriteFile(va("progs/%s.%s.post.%s", GetName(), typeName, langName), str.c_str(), str.Length());
#endif

	return true;
}
