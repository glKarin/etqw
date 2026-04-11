// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __DECLRENDERPROGRAM_H__
#define __DECLRENDERPROGRAM_H__

/*
===============================================================================

sdDeclRenderProgram

===============================================================================
*/

class sdRenderProgramShader
{
	public:
		typedef enum shaderType_e {
			ST_INVALID = 0,
			ST_VERTEX,
			ST_FRAGMENT,
		} shaderType_t;
		typedef enum shaderLang_e {
			SL_UNKNOWN = 0,
			SL_ARB,
			SL_GLSL,
			SL_CG,
			SL_HLSL,
		} shaderLang_t;

		sdRenderProgramShader(void);
		void Init(void);
		bool IsValid(void) const;
		bool Parse(idParser &src);

	private:
		shaderType_t type;
		shaderLang_t lang;
		idStr source;

		friend class sdDeclRenderProgram;
};

// NOT use, only for parse renderprogs decl
class sdDeclRenderProgram : public idDecl {
public:
									sdDeclRenderProgram();

	virtual							~sdDeclRenderProgram() {}

	// Override from idDecl
	virtual const char*				DefaultDefinition( void ) const;
	virtual bool					Parse( const char* text, const int textLength );
	virtual size_t					Size( void ) const { return sizeof( sdDeclRenderProgram ); }
	virtual void					FreeData();

private:
	void							Init(void);
	bool							ParseShader(idParser &src);

private:
	sdRenderProgramShader			vertex;
	sdRenderProgramShader			fragment;
};

#endif /* !__DECLRENDERPROGRAM_H__ */
