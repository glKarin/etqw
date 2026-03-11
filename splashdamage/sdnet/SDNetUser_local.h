// Copyright (C) 2007 Id Software, Inc.
//

#if !defined( __SDNETUSER_LOCAL_H__ )
#define __SDNETUSER_LOCAL_H__

//===============================================================
//
//	sdNetUser
//
//===============================================================

#include "SDNetUser.h"

#include "idlib/precompiled.h"

#include "SDNetProfile.h"
#include "SDNetAccount_local.h"

class sdNetUser_Local : public sdNetUser {
public:
	sdNetUser_Local();
	virtual						~sdNetUser_Local();

	virtual userState_e			GetState() const;
	virtual const char*			GetUsername() const;
	virtual const char*			GetRawUsername() const;

	virtual sdNetProfile&		GetProfile();
	virtual const sdNetProfile&	GetProfile() const;

	// Make this user the currently active one
	virtual void				Activate();

	// Deactivate the user, effectively logging them out
	virtual void				Deactivate();

	// Write user to permanent storage
	virtual bool				Save( int saveItems = ( SI_PROFILE | SI_CVARS | SI_BINDINGS ) ) const;

	// Get online account
	virtual sdNetAccount&		GetAccount();

private:
	userState_e userState;
	idStr username;
	idStr rawUsername;
	sdNetProfile profile;
	sdNetAccount_Local account;
};

#endif /* !__SDNETUSER_LOCAL_H__ */
