/* SCE CONFIDENTIAL
* PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include <sys/synchronization.h>

#include <netex/net.h>
#include <netex/libnetctl.h>

#include <cell/http.h>
#include <cell/ssl.h>

#include <sysutil_common.h>

#include "np_gui.h"
#include "np_gui_draw.h"
#include "strings.h"
#include "np_conf.h"

#define RESULT_PRIO			1001
#define RESULT_STACKSIZE	(16*1024)
#define UDPP2P_PRIO			1002
#define UDPP2P_STACKSIZE	(16*1024)

#define HTTP_POOL_SIZE      (64 * 1024U)
#define SSL_POOL_SIZE       (256 * 1024U)


static uint8_t httpPool[HTTP_POOL_SIZE];
static uint8_t sslPool[SSL_POOL_SIZE];
uint8_t np_pool[SCE_NP_MIN_POOL_SIZE];

extern struct np_gui_info g_info;
extern sys_lwmutex_t lwmutex;

const char *conn_status_str[] = {
	"", "PENDING", "ACTIVE"
};

const char *netinfo_nat_status_str[] = {
	"Unknown", "Type1", "Type2", "Type3"
};

const char *netinfo_upnp_status_str[] = {
	"Unknown", "Invalid", "Valid"
};

const char *netinfo_npport_status_str[] = {
	"Closed", "Open"
};

static void managerCb(int event, int result, void *arg);

static int loadCerts(size_t *numBufPtr, CellHttpsData **caListPtr)
{
	CellHttpsData *caList = NULL;
	size_t size = 0;
	int ret = 0;
	char *buf = NULL;

	caList = (CellHttpsData *)malloc(sizeof(CellHttpsData)*2);
	if (NULL == caList) {
		printf("failed to malloc cert data\n");
		ret = -1;
		goto error;
	}

	ret = cellSslCertificateLoader(CELL_SSL_LOAD_CERT_ALL, NULL, 0, &size);
	if (ret < 0) {
		printf("cellSslCertifacateLoader() failed(1). ret = %x\n", ret);
		goto error;
	}

	buf = (char*)malloc(size);
	if (NULL == buf) {
		printf("failed to malloc cert buffer\n");
		free(caList);
		ret = -1;
		goto error;
	}

	ret = cellSslCertificateLoader(CELL_SSL_LOAD_CERT_ALL, buf, size, NULL);
	if (ret < 0) {
		printf("cellSslCertifacateLoader() failed(2). ret = %x\n", ret);
		free(buf);
		free(caList);
		goto error;
	}

	(&caList[0])->ptr = buf;
	(&caList[0])->size = size;

	*caListPtr = caList;
	*numBufPtr = 1;

error:
	return ret;
}

static int32_t
initHttps(void)
{
	int ret;
	size_t numBuf = 0;
	CellHttpsData *caList = NULL;

	/*E startup procedures */
	ret = cellSslInit(sslPool, SSL_POOL_SIZE);
	if (ret < 0) {
		printf("unable to start libssl... (0x%x)\n", ret);
		goto end;
	}


	ret = cellHttpInit(httpPool, HTTP_POOL_SIZE);
	if (ret < 0) {
		printf("unable to start libhttp... (0x%x)\n", ret);
		goto end;
	}

	ret = loadCerts(&numBuf, &caList);
	if (ret < 0) {
		printf("failed to load certs\n");
		goto end;
	}

	ret = cellHttpsInit(numBuf, caList);
	free(caList->ptr);
	free(caList);
	if (ret < 0) {
		printf("unable to start https... (0x%x)\n", ret);
		goto end;
	}

end:
	return(ret);
}





static void
managerCb(int event, int result, void *arg)
{
	(void)result;
	(void)arg;

	DBG("<MANAGER CB> event:%d, result:0x%x, arg:0x%p\n", event, result, arg);
	
	switch (event) {
	case SCE_NP_MANAGER_STATUS_OFFLINE:
		g_info.flag |= NP_GUI_FLAG_OFFLINE;
		break;
	case SCE_NP_MANAGER_STATUS_GETTING_TICKET:
	case SCE_NP_MANAGER_STATUS_GETTING_PROFILE:
	case SCE_NP_MANAGER_STATUS_LOGGING_IN:
		break;
	case SCE_NP_MANAGER_STATUS_ONLINE:
		g_info.flag &= ~NP_GUI_FLAG_OFFLINE;
		break;
	default:
		break;
	}
}

int32_t
memoryContainerCheck(sys_memory_container_t mc, size_t size)
{
	sys_memory_info minfo;
	int ret;

	ret = sys_memory_container_get_size(&minfo, mc);
	if (ret < 0) {
		return ret;
	}
	/* check if we have an empty memory container */
	if(minfo.total_user_memory != minfo.available_user_memory) {
		return -1;
	}
	/* check if we have the full required size */
	if (minfo.total_user_memory < size) {
		return -1;
	}
	return 0;
}

void
check_result(void)
{
	cellSysutilCheckCallback();
}

int
np_init(void)
{
	int ret;
	sys_lwmutex_attribute_t lwmutex_attr;

	/************************************************************
	 * main function
	 ************************************************************/

	sys_lwmutex_attribute_initialize(lwmutex_attr);
	sys_lwmutex_create(&lwmutex, &lwmutex_attr);

	ret = sys_net_initialize_network();
	if (ret < 0) {
		DBG("sys_net_initialize_network() failed (%d)\n", ret);
		goto error;
	}

	ret = cellNetCtlInit();
	if (ret < 0) {
		DBG("cellNetCtlInit() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpInit(SCE_NP_MIN_POOL_SIZE, np_pool);
	if (ret < 0) {
		DBG("sceNpInit() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpManagerRegisterCallback(managerCb, NULL);
	if (ret < 0) {
		DBG("sceNpManagerRegisterCallback() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = initHttps();
	if (ret < 0) {
		DBG("initHttps() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpLookupInit();
	if (ret < 0) {
		DBG("sceNpLookupInit() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpClansInit(
		NpConf::npCommId(), NpConf::npCommPassphrase(), NULL, 0, 0);
	if (ret < 0) {
		DBG("sceNpClansInit() failed. ret = 0x%x\n", ret);
		goto error;
	}

error:
	return ret;
}


static void
mode_init(
	enum np_gui_mode mode
	)
{
	switch (mode) {
	case NP_GUI_MODE_TOP:
		break;
	case NP_GUI_MODE_NETSTART:
		change_netstart_state(NP_GUI_NETSTART_STATE_TOP);
		break;
	case NP_GUI_MODE_CLANS:
		change_clans_state(NP_GUI_CLANS_STATE_TOP);
		break;
	default:
		break;
	}

	g_info.pos_act_y = 0;
	g_info.pos_y = 0;
	g_info.pos_x = 0;
	
	return;
}

void
change_mode(
	enum np_gui_mode mode
	)
{
	g_info.mode = mode;

	mode_init(g_info.mode);
	
	return;
}

static void
netstart_state_init(
	enum np_gui_netstart_state state
	)
{
	switch (state) {
	case NP_GUI_NETSTART_STATE_TOP:
		start_netstart_dialog();
		break;
	default:
		break;
	}
	
	return;
}

void
change_netstart_state(
	enum np_gui_netstart_state state
	)
{
	g_info.netstart_state = state;

	netstart_state_init(g_info.netstart_state);
	
	return;
}

static void
clans_state_init(
	enum np_gui_clans_state state
	)
{
	switch (state) {
	case NP_GUI_CLANS_STATE_TOP:
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		g_info.pos_act_y = 0;
		g_info.pos_y_clans_menu_top = 0;
		break;
	default:
		break;
	}

	return;
}

void
change_clans_state(
	enum np_gui_clans_state state
	)
{
	g_info.clans_state = state;
	g_info.pos_x = 0;
	g_info.pos_y = 0;
	g_info.pos_act_y = 0;
	g_info.pos_slot_y = 0;

	clans_state_init(g_info.clans_state);
	
	return;
}


