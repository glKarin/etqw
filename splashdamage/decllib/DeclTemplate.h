// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __DECL_TEMPLATE_H__
#define __DECL_TEMPLATE_H__

#include "../framework/DeclManager.h"

/*
===============================================================================

	sdDeclTemplate

===============================================================================
*/

class sdDeclTemplate : public idDecl {
public:
	virtual					~sdDeclTemplate() {}

	virtual size_t			Size( void ) const;
	virtual const char *	DefaultDefinition() const;
	virtual bool			Parse( const char *text, const int textLength );
	virtual void			FreeData( void );
	virtual void			Print( void ) const;

private:
	bool					ParseParameters(idParser &src);

private:
    idStrList				parameters;
    idStr					text;
};

#endif /* !__DECL_TEMPLATE_H__ */

