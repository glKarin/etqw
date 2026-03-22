// Copyright (C) 2007 Id Software, Inc.
//

#include "SDNetTask_local.h"

sdNetTask_Local::sdNetTask_Local()
    : taskStatus(TS_DONE),
    errorCode(SDNET_NO_ERROR)
{
}

sdNetTask_Local::~sdNetTask_Local() {

}

void sdNetTask_Local::Cancel( bool blocking ) {

}

sdNetTask::taskStatus_e sdNetTask_Local::GetState() const {
    return taskStatus;
}

sdNetErrorCode_e sdNetTask_Local::GetErrorCode() const {
    return errorCode;
}

void sdNetTask_Local::AcquireLock() {

}

void sdNetTask_Local::ReleaseLock() {

}
