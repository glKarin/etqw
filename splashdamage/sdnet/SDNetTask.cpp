// Copyright (C) 2007 Id Software, Inc.
//

#include "SDNetTask_local.h"
#include "SDNet_local.h"

#define SD_NET_TASK_FAKE_UPDATE_INTERVAL 50

sdNetTask_Local::sdNetTask_Local(const char *_name)
    : taskStatus(TS_INITIAL),
    errorCode(SDNET_NO_ERROR)
{
    name = _name;
    startTime = Sys_Milliseconds();
    updateTime = startTime;
    common->Printf("Add task: %s", name.c_str());
    ((sdNetService_Local *)networkService)->AddTask(this);
}

sdNetTask_Local::~sdNetTask_Local() {
    common->Printf("[SDNet]: %s task deleted!\n", name.c_str());
}

void sdNetTask_Local::Cancel( bool blocking ) {
    if (taskStatus == TS_INITIAL || taskStatus == TS_PENDING) {
        taskStatus = TS_CANCELLING;
        updateTime = Sys_Milliseconds();
        common->Printf("[SDNet]: %s task cancel!\n", name.c_str());
    }
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

void sdNetTask_Local::RunFrame() {
    if (taskStatus == TS_DONE) {
        return;
    }
    int ts = Sys_Milliseconds();
    if (ts - updateTime < 1000) {
        return;
    }
    updateTime = ts;
    if (taskStatus == TS_INITIAL) {
        taskStatus = TS_PENDING;
        common->Printf("[SDNet]: %s task pending...\n", name.c_str());
    }
    else if (taskStatus == TS_PENDING) {
        taskStatus = TS_COMPLETING;
        common->Printf("[SDNet]: %s task completing...\n", name.c_str());
    }
    else if (taskStatus == TS_COMPLETING || taskStatus == TS_CANCELLING) {
        taskStatus = TS_DONE;
        common->Printf("[SDNet]: %s task done.\n", name.c_str());
    }
}
