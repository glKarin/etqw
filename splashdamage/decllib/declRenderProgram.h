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
		void ParsePost(void);
		shaderType_t GetType(void) const {
			return type;
		}
		shaderLang_t GetLang(void) const {
			return lang;
		}
		const char * GetSource(void) const {
			return source.c_str();
		}
		const idStrList & GetPlaceholders(void) const {
			return placeholders;
		}
		const idList<const sdDeclRenderBinding *> & GetBindings(void) const {
			return bindings;
		}
		int NumBindings(void) const {
			return bindings.Num();
		}
		const sdDeclRenderBinding * GetBinding(int i) const {
			return i >= 0 && i < bindings.Num() ? bindings[i] : NULL;
		}
		const sdDeclRenderBinding * GetBinding(const char *name) const;
		const char * GetPlaceholder(int i) const {
			return i >= 0 && i < placeholders.Num() ? placeholders[i].c_str() : NULL;
		}

	private:
		shaderType_t type;
		shaderLang_t lang;
		idStr sourceRaw;
		idStrList placeholders;
		idStr source;
		idList<const sdDeclRenderBinding *> bindings;

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
	const sdRenderProgramShader	*	GetVertexShader(void) const {
		return vertex.IsValid() ? &vertex : NULL;
	}
	const sdRenderProgramShader	*	GetFragmentShader(void) const {
		return fragment.IsValid() ? &fragment : NULL;
	}
	bool							IsCompleted(void) const {
		return vertex.IsValid() && fragment.IsValid();
	}

private:
	void							Init(void);
	bool							ParseShader(idParser &src);

private:
	sdRenderProgramShader			vertex;
	sdRenderProgramShader			fragment;
};

#endif /* !__DECLRENDERPROGRAM_H__ */
