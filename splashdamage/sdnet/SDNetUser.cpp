// Copyright (C) 2007 Id Software, Inc.
//

#include "SDNetUser_local.h"

sdNetUser_Local::sdNetUser_Local()
    : userState(US_INACTIVE)
{
}

sdNetUser_Local::~sdNetUser_Local() {
}

sdNetUser::userState_e sdNetUser_Local::GetState() const {
    return userState;
}

const char* sdNetUser_Local::GetUsername() const {
    return username.c_str();
}

const char* sdNetUser_Local::GetRawUsername() const {
    return username.c_str();
}

sdNetProfile& sdNetUser_Local::GetProfile() {
    return profile;
}

const sdNetProfile& sdNetUser_Local::GetProfile() const {
    return profile;
}

// Make this user the currently active one
void sdNetUser_Local::Activate() {
}

// Deactivate the user, effectively logging them out
void sdNetUser_Local::Deactivate() {
}

// Write user to permanent storage
bool sdNetUser_Local::Save( int saveItems ) const {
    return true;
}

// Get online account
sdNetAccount& sdNetUser_Local::GetAccount() {
    return account;
}
