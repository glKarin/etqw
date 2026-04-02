// Copyright (C) 2007 Id Software, Inc.
//

#if !defined( __SDNETUSER_LOCAL_H__ )
#define __SDNETUSER_LOCAL_H__

//===============================================================
//
//	sdNetUser
//
//===============================================================

#include "idlib/precompiled.h"

#include "SDNetProfile_local.h"
#include "SDNetAccount_local.h"

#include "SDNetUser.h"

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
	sdNetProfile_Local profile;
	sdNetAccount_Local account;
	friend class sdNetService_Local;
};

#endif /* !__SDNETUSER_LOCAL_H__ */
