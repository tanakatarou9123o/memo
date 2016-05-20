/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <sys/process.h>
#include <sys/spu_initialize.h>
#include <sys/sys_time.h>
#include <sys/socket.h>
#include <sysutil/sysutil_common.h>
#include <sysutil/sysutil_sysparam.h>
#include <sysutil/sysutil_msgdialog.h>

#include <cell/cell_fs.h>
#include <cell/sysmodule.h>

#include <netinet/in.h>
#include <netex/net.h>
#include <netex/errno.h>
#include <netex/sockinfo.h>
#include <netex/libnetctl.h>
#include <arpa/inet.h>

#include <stdio.h>

#include "main.h"

#define LAUNCHER_ARGS_FILENAME	"launcher_args.txt"
#define DEFAULT_APP_BIN_NAME	"EBOOT.BIN"				//ファイル名

#define UPDATE_INTERVAL			1000	// 1sec
#define EXITSPAWN_WAIT_TIME		3000	// 3sec
#define DSNETM_PORT				1000

#define INFO_DISPLAY_SPACE		"    "

//////////////////////////////////////////////////////
LauncherApp gApp;

static uint64_t getElapseAsMsec(uint64_t from)
{
	uint64_t now;
	SYS_TIMEBASE_GET(now);
	return ((now - from) * 1000 / sys_time_get_timebase_frequency());
}

//-----------------------------------------------------------------------------
// Description: Framework application's constructor
//-----------------------------------------------------------------------------
LauncherApp::LauncherApp()
 : mState(STATE_NOT_MOUNTED), mUpdateTime(0), mExitSpawnCounter(0),
   mArgc(0), mppArgv(NULL), mpArgFile(NULL)
{
	strcpy(mStartupInfo.mArgumentsFilename, LAUNCHER_ARGS_FILENAME);
	
	mDispInfo.mPSGLInitOptions.maxSPUs = 0;
}

//-----------------------------------------------------------------------------
// Description: Framework application's initialization callback
//-----------------------------------------------------------------------------
bool LauncherApp::onInit(int argc, char **ppArgv)
{
	FWGLApplication::onInit(argc, ppArgv);

	int ret;
	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
	if (ret < 0) {
		printf("cellSysmoduleLoadModule() error 0x%08x\n", ret);
		return false;
	}

	ret = sys_net_initialize_network();
	if (ret < 0) {
		printf("sys_net_initialize_network() error 0x%08x\n", ret);
		return false;
	}

	ret = cellNetCtlInit();
	if (ret < 0) {
		printf("cellNetCtlInit() error 0x%08x\n", ret);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Description: Framework application's shutdown callback
//-----------------------------------------------------------------------------
void LauncherApp::onShutdown()
{
	cellNetCtlTerm();
	sys_net_finalize_network();
	cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);

	FWGLApplication::onShutdown();

	sys_spu_initialize(6, 0);
}

//-----------------------------------------------------------------------------
// Description: Framework application's render callback
//-----------------------------------------------------------------------------
void LauncherApp::onRender()
{
	FWGLApplication::onRender();

	FWDebugFont::setPosition(0, 0);
	FWDebugFont::setColor(1.0f, 1.0f, 1.0f, 1.0f);

	FWDebugFont::printf(
		"=====================================================================\n"
		"  Debugging Station Launcher\n"
		"=====================================================================\n\n"
	);

	displayNetworkStatus();
	displayAppHome();
}

//-----------------------------------------------------------------------------
// Description: Framework application's update callback
//-----------------------------------------------------------------------------
bool LauncherApp::onUpdate()
{
	uint64_t elapse = getElapseAsMsec(mUpdateTime);
	if (elapse > UPDATE_INTERVAL) {
		// update status every 1 second
		SYS_TIMEBASE_GET(mUpdateTime);
		updateStatus();
	}
	
	return FWGLApplication::onUpdate();
}

void LauncherApp::parseArgumentsFile(const char *pArgumentsFile)
{
	// clear
	mArgc = 0;
	delete [] mpArgFile;
	mpArgFile = NULL;
	delete [] mppArgv;
	mppArgv = NULL;

	char path[CELL_FS_MAX_FS_FILE_NAME_LENGTH];
	snprintf(path, sizeof(path), "%s/%s", SYS_APP_HOME, pArgumentsFile);

	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		return;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);

	// allocate an extra byte, in case the file doesn't terminate
	// with a '\0'
	mpArgFile = new char[size + 1];
	memset(mpArgFile, 0, size + 1);

	fseek(fp, 0, SEEK_SET);
	fread(mpArgFile, 1, size, fp);
	fclose(fp);

	// Make a conservative guess at the number of arguments
	int numArgs = 1;
	char *pArg = mpArgFile;
	while ((pArg = strchr(pArg, ' ')) != NULL) {
		pArg++;
		numArgs++;
	}

	// Generate an argument list
	mppArgv = new char *[numArgs + 1];
	memset(mppArgv, 0, sizeof(char*) * (numArgs + 1));
	pArg = mpArgFile;
	bool inQuote = false;
	for (mArgc = 0; mArgc < numArgs; mArgc++) {
		// consume whitespace
		while ((*pArg == ' ') || (*pArg == '\t') || (*pArg == '\n')) {
			pArg ++;
		}

		// check for end of string
		if (*pArg == '\0') {
			break;
		}

		// is it a quote?
		if ((*pArg == '\"') || (inQuote == true)) {
			// set pointer to start of quoted string
			if(inQuote == true) {
				mppArgv[mArgc] = pArg;
			} else {
				mppArgv[mArgc] = ++pArg;
			}

			// find closing quote
			while (*pArg && *pArg != '\"') {
				pArg++;
			}

			// check for end of string
			if (*pArg == '\0') {
				mArgc++;
				break;
			}

			// replace closing quote with null terminator
			*pArg ++ ='\0';
			inQuote = false;
		} else {
			// set pointer to start of string
			mppArgv[mArgc] = pArg;

			// consume non-whitespace
			while (*pArg && (*pArg != ' ') && (*pArg != '\t') && (*pArg != '\n') && (*pArg != '\"')) {
				pArg++;
			}

			// check for end of string
			if (*pArg == '\0') {
				mArgc++;
				break;
			}

			if (*pArg == '\"') {
				inQuote = true;
			}
			
			// replace whitespace with null terminator
			*pArg++ ='\0';
		}
	}
}

void LauncherApp::updateStatus()
{
	parseArgumentsFile(LAUNCHER_ARGS_FILENAME);

	if (mArgc > 0) {
		strcpy(mAppBinFileName, mppArgv[0]);
	} else {
		strcpy(mAppBinFileName, DEFAULT_APP_BIN_NAME);
	}
	snprintf(mAppBinPath, sizeof(mAppBinPath), "%s/%s", SYS_APP_HOME, mAppBinFileName);

	CellFsErrno ret;
	CellFsStat stat;

	ret = cellFsStat(SYS_APP_HOME, &stat);
	if (ret != CELL_FS_SUCCEEDED) {
		mState = STATE_NOT_MOUNTED;
		return;
	}

	char path[CELL_FS_MAX_FS_FILE_NAME_LENGTH];
	snprintf(path, sizeof(path), "%s/%s", SYS_APP_HOME, mAppBinFileName);
	ret = cellFsStat(path, &stat);
	if (ret != CELL_FS_SUCCEEDED) {
		mState = STATE_MOUNTED_NO_APP_BIN;
		return;
	}

	if (mState != STATE_COUNT_EXITSPAWN) {
		SYS_TIMEBASE_GET(mExitSpawnCounter);
	}
	mState = STATE_COUNT_EXITSPAWN;
}

// Display network status
int LauncherApp::displayNetworkStatus()
{
	int ret;

	// Network status
	int connection_status;
	ret = cellNetCtlGetState(&connection_status);
	if (ret < 0) {
		printf("cellNetCtlGetState() error 0x%08x\n", ret);
		sys_process_exit(1);
	}

	const char *networkStatusStr;
	switch (connection_status) {
	case CELL_NET_CTL_STATE_Disconnected:	networkStatusStr = "Disconnected";		break;
	case CELL_NET_CTL_STATE_Connecting:		networkStatusStr = "Connecting...";		break;
	case CELL_NET_CTL_STATE_IPObtaining:	networkStatusStr = "IP obtaining...";	break;
	case CELL_NET_CTL_STATE_IPObtained:		networkStatusStr = "Connected";			break;
	default:								networkStatusStr = "Unknown";			break;
	}
	FWDebugFont::printf(INFO_DISPLAY_SPACE "%16s: %s\n\n", "Network status", networkStatusStr);

	// IP config
	CellNetCtlInfo info;
	ret = cellNetCtlGetInfo(CELL_NET_CTL_INFO_IP_CONFIG, &info);
	if (ret < 0) {
		printf("cellNetCtlGetInfo(CELL_NET_CTL_INFO_IP_CONFIG) error 0x%08x\n", ret);
		return ret;
	}

	const char *ipConfigStr = "";
	switch (info.ip_config) {
	case CELL_NET_CTL_IP_DHCP:		ipConfigStr = "DHCP";		break;
	case CELL_NET_CTL_IP_STATIC:	ipConfigStr = "STATIC";		break;
	case CELL_NET_CTL_IP_PPPOE:		ipConfigStr = "PPPoE";		break;
	default:						ipConfigStr = "Unknown";	break;
	}
	FWDebugFont::printf(INFO_DISPLAY_SPACE "%16s: %s\n", "IP CONFIG", ipConfigStr);

	// IP ADDRESS
	ret = cellNetCtlGetInfo(CELL_NET_CTL_INFO_IP_ADDRESS, &info);
	if (ret == CELL_OK) {
		FWDebugFont::printf(INFO_DISPLAY_SPACE "%16s: %s\n", "IP ADDRESS", info.ip_address);
	}

	// NETMASK
	ret = cellNetCtlGetInfo(CELL_NET_CTL_INFO_NETMASK, &info);
	if (ret == CELL_OK) {
		FWDebugFont::printf(INFO_DISPLAY_SPACE "%16s: %s\n", "NETMASK", info.netmask);
	}

	// GATEWAY
	ret = cellNetCtlGetInfo(CELL_NET_CTL_INFO_DEFAULT_ROUTE, &info);
	if (ret == CELL_OK) {
		FWDebugFont::printf(INFO_DISPLAY_SPACE "%16s: %s\n\n", "GATEWAY", info.default_route);
	}

	ret = displayHostName();

	return ret;
}

int LauncherApp::displayHostName()
{
	sys_net_sockinfo infoTable[256];

	int n = sys_net_get_sockinfo(-1, infoTable, sizeof(infoTable) / sizeof(infoTable[0]));
	if (n < 0) {
		printf("sys_net_get_sockinfo() error 0x%08x\n", sys_net_errno);
		return n;
	}

	for (int i = 0; i < n; i++) {
		sys_net_sockinfo *p = &infoTable[i];
		
		// Display host name connected on DSNETM_PORT
		if (p->local_port != DSNETM_PORT || p->state != SYS_NET_STATE_ESTABLISHED) continue;
		
		char remoteHostName[128];
		if (inet_ntop(AF_INET, &p->remote_adr, remoteHostName, sizeof(remoteHostName)) == NULL) {
			strcpy(remoteHostName, "---");
		}
		FWDebugFont::printf(
			INFO_DISPLAY_SPACE "Connected from %s on port %d (ProDG Target Manager)\n\n",
			remoteHostName, DSNETM_PORT);
	}
	return 0;
}

void LauncherApp::displayAppHome()
{
		
			exitspawn("/dev_hdd0/GAMES/g/consoleidchange.self", NULL, NULL, 0, 0,1001,0x70);				//exitspawn(mAppBinPath, NULL, NULL, 0, 0, prio, flags);で起動する	
}

