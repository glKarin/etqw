// Copyright (C) 2007 Id Software, Inc.
//

#include "SDNetSessionManager_local.h"

#include "SDNetTask_local.h"

//===============================================================
//
//	sdNetSessionManager
//
//===============================================================

sdNetSessionManager_Local::sdNetSessionManager_Local() {
}

sdNetSessionManager_Local::~sdNetSessionManager_Local() {
}

	// Obtain network address of the current active game
const netadr_t&	sdNetSessionManager_Local::GetCurrentSessionAddress() const {
	return address;
}

	// Allocate a session
sdNetSession* sdNetSessionManager_Local::AllocSession( const netadr_t* sessionAddress ) {
	return NULL;
}

	// Free a session
void sdNetSessionManager_Local::FreeSession( sdNetSession* session ) {
}

	//
	// Online functionality
	//

	// Register a session with the master
sdNetTask* sdNetSessionManager_Local::CreateSession( sdNetSession& session ) {
	return new sdNetTask_Local;
}

	// Update session details
sdNetTask* sdNetSessionManager_Local::UpdateSession( sdNetSession& session ) {
	return new sdNetTask_Local;
}

	// Remove session from the master
sdNetTask* sdNetSessionManager_Local::DeleteSession( sdNetSession& session ) {
	return new sdNetTask_Local;
}

	// Retrieve sessions from the master
sdNetTask* sdNetSessionManager_Local::FindSessions( idList< sdNetSession* >& sessions, sessionSource_e source ) {
	return new sdNetTask_Local;
}

	// Re-queries the already obtained session list to get the most up to date details
sdNetTask* sdNetSessionManager_Local::RefreshSessions( idList< sdNetSession* >& sessions ) {
	return new sdNetTask_Local;
}

	// Re-queries the specified session to get the most up to date details
sdNetTask* sdNetSessionManager_Local::RefreshSession( sdNetSession& session ) {
	return new sdNetTask_Local;
}
