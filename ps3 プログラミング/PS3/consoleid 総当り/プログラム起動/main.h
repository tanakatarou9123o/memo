/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef __LAUNCHER_MAIN_H__
#define __LAUNCHER_MAIN_H__

#include <cell/cell_fs.h>

#include "FWGLApplication.h"
#include "FWDebugFont.h"

class LauncherApp : public FWGLApplication
{
public:
					LauncherApp();
	virtual bool	onInit(int argc, char **ppArgv);
	virtual void	onRender();
	virtual bool	onUpdate();
	virtual	void	onShutdown();

protected:
	enum LauncherState {
		STATE_NOT_MOUNTED,
		STATE_MOUNTED_NO_APP_BIN,
		STATE_COUNT_EXITSPAWN,
	};

	void parseArgumentsFile(const char *pArgumentsFile);
	void updateStatus();
	int displayNetworkStatus();
	int displayHostName();
	void displayAppHome();

	LauncherState	mState;
	uint64_t		mUpdateTime;
	uint64_t		mExitSpawnCounter;

	int				mArgc;		// Number of command line arguments
	char		    **mppArgv;	// Array of command line arguments
	char			*mpArgFile;	// Arguments file pointer

	char			mAppBinFileName[CELL_FS_MAX_FS_FILE_NAME_LENGTH];
	char			mAppBinPath[CELL_FS_MAX_FS_FILE_NAME_LENGTH];
};

#endif //__LAUNCHER_MAIN_H__

