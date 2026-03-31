// Copyright (C) 2007 Id Software, Inc.
//

#include "SDNet_local.h"

#include "SDNetTask_local.h"

//===============================================================
//
//	sdNetService
//
//===============================================================

sdNetService_Local::sdNetService_Local()
	: isInitialized(false),
	serviceState(SS_DISABLED),
	disconnectReason(DR_NONE),
	dedicatedState(DS_OFFLINE),
	lastError(SDNET_NO_ERROR)
{
}

sdNetService_Local::~sdNetService_Local() {
}

bool sdNetService_Local::Init() {
	serviceState = SS_INITIALIZED;
	dedicatedState = DS_ONLINE;
	isInitialized = true;
	return isInitialized;
}

void sdNetService_Local::Shutdown() {
}

void sdNetService_Local::RunFrame() {
	for (int i = 0; i < taskPools.Num(); ++i) {
		((sdNetTask_Local *)taskPools[i])->RunFrame();
	}
}

sdNetService::serviceState_e sdNetService_Local::GetState() const {
	return serviceState;
}

sdNetService::disconnectReason_e sdNetService_Local::GetDisconnectReason() const {
	return disconnectReason;
}

sdNetService::dedicatedState_e sdNetService_Local::GetDedicatedServerState() const {
	return dedicatedState;
}

const sdNetService::motdList_t& sdNetService_Local::GetMotD() const {
	return motdList;
}

	//
	// Key Code
	//
bool sdNetService_Local::CheckKey( const char* key, bool noChecksum ) const {
	return true;
}

const char* sdNetService_Local::GetStoredLicenseCode() const {
	return "";
}

bool sdNetService_Local::IsSteamActive() const {
	return false;
}

	//
	// User management
	//
sdNetErrorCode_e sdNetService_Local::CreateUser( sdNetUser** user, const char* username ) {
	return SDNET_NO_ERROR;
}

void sdNetService_Local::DeleteUser( sdNetUser* user ) {
}

int sdNetService_Local::NumUsers() const {
	return userList.Num();
}

sdNetUser* sdNetService_Local::GetUser( const int index ) {
	return &userList[index];
}

sdNetUser* sdNetService_Local::GetActiveUser() {
	return userList.Num() > 0 ? &userList[0] : NULL;
}

	//
	// Session management - deferred to Session Manager
	//
sdNetSessionManager& sdNetService_Local::GetSessionManager() {
	return sessionManager;
}

#if !defined( SD_DEMO_BUILD )
	//
	// Stats management - deferred to Stats Manager
	//
sdNetStatsManager& sdNetService_Local::GetStatsManager() {
	return statsManager;
}

	//
	// Friends management - deferred to Friends Manager
	//
sdNetFriendsManager& sdNetService_Local::GetFriendsManager() {
	return friendsManager;
}

	//
	// Friends management - deferred to Team Manager
	//
sdNetTeamManager& sdNetService_Local::GetTeamManager() {
	return teamManager;
}
#endif /* !SD_DEMO_BUILD */

	//
	// Task management
	//
void sdNetService_Local::FreeTask( sdNetTask* task ) {
	taskPools.Remove(task);
	delete task;
}

	//
	// Online Services
	//

sdNetErrorCode_e sdNetService_Local::GetLastError() const {
	return lastError;
}

	// Start online service and connect to auth system
sdNetTask* sdNetService_Local::Connect() {
	serviceState = SS_ONLINE;
	return new sdNetTask_Connect;
}

	// Authorize a dedicated server
sdNetTask* sdNetService_Local::SignInDedicated() {
	return new sdNetTask_SignInDedicated;
}

	// De-authorize a dedicated server
sdNetTask* sdNetService_Local::SignOutDedicated() {
	return new sdNetTask_SignOutDedicated;
}

#if !defined( SD_DEMO_BUILD )
	// Get a list of account names for a license code
sdNetTask* sdNetService_Local::GetAccountsForLicense( idStrList& accountNames, const char* licenseCode ) {
	return new sdNetTask_GetAccountsForLicense;
}

	// Get a user's profile
const idDict* sdNetService_Local::GetProfileProperties( sdNetClientId userID ) const {
	for (idList<sdNetUser_Local>::ConstIterator itor = userList.Begin(); itor != userList.End(); ++itor) {
		sdNetClientId id;
		sdNetUser &user = (sdNetUser &)*itor;
		user.GetAccount().GetNetClientId(id);
		if (id == userID) {
			return &user.GetProfile().GetProperties();
		}
	}
	return NULL;
}
#endif /* !SD_DEMO_BUILD */

idList<sdNetTask *> sdNetService_Local::taskPools;

void sdNetService_Local::AddTask(sdNetTask *task) {
	taskPools.Append( task );
}

sdNetService_Local networkServiceLocal;
sdNetService* networkService = &networkServiceLocal;
