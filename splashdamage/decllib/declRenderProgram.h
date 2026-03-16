// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __DECLRENDERPROGRAM_H__
#define __DECLRENDERPROGRAM_H__

/*
===============================================================================

sdDeclRenderProgram

===============================================================================
*/

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
};

#endif /* !__DECLRENDERPROGRAM_H__ */
