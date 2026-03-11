// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __SDNETTASK_LOCAL_H__
#define __SDNETTASK_LOCAL_H__

#include "SDNetTask.h"

//===============================================================
//
//	sdNetTask
//
//===============================================================

class sdNetTask_Local : public sdNetTask {
public:
	sdNetTask_Local();
	virtual						~sdNetTask_Local();

	virtual void				Cancel( bool blocking = false );

	virtual taskStatus_e		GetState() const;
	virtual sdNetErrorCode_e	GetErrorCode() const;

	virtual void				AcquireLock();
	virtual void				ReleaseLock();

private:
	taskStatus_e taskStatus;
	sdNetErrorCode_e errorCode;
};

#endif /* !__SDNETTASK_LOCAL_H__ */
