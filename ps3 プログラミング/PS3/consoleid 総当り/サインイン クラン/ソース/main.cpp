/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#include <sys/process.h>
#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include <sys/synchronization.h>
#include <cell/sysmodule.h>

#include <cell/http.h>
#include <cell/ssl.h>

#include <netex/net.h>
#include <netex/libnetctl.h>

#include <sysutil_common.h>
#include <sysutil_sysparam.h>
#include <sysutil/sysutil_oskdialog.h>
#include <cell/gcm.h>
#include <cell/pad.h>

#include "np_gui.h"
#include "strings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int cidseikou=0;



//////////////////////////////////////////////////////
static void
setViewAndScissor(
	uint32_t X,
	uint32_t Y,
	uint32_t width,
	uint32_t height,
	uint32_t surfaceheight,
	bool flip
	);

//////////////////////////////////////////////////////

struct np_gui_info g_info;
sys_lwmutex_t lwmutex;

static int exit_game = 0;

static int
load_modules(
	void
	)
{
	int ret = 0;

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_NET);
	if (ret < 0) {
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_NET) failed (0x%x)\n", ret);
		goto error;
	}

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_IO);
	if (ret < 0) {
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_IO) failed (0x%x)\n", ret);
		goto error;
	}

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_GCM_SYS);
	if (ret < 0) {
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_GCM_SYS) failed (0x%x)\n", ret);
		goto error;
	}

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP);
	if (ret < 0) {
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP) failed (0x%x)\n", ret);
		goto error;
	}

	/*J NP Clans のために必要 */
	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_HTTPS);
	if (ret < 0) {
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_HTTPS) failed (0x%x\n", ret);
		goto error;
	}

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP_CLANS);
	if (ret < 0) {
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP_CLANS) failed (0x%x\n", ret);
		goto error;
	}

error:
	return ret;
}

static void
unload_modules(
	void
	)
{
	int ret = 0;

	ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_NP_CLANS);
	if (ret < 0)
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP_CLANS) failed (0x%x)\n", ret);

	ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_HTTPS);
	if (ret < 0)
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_HTTPS) failed (0x%x)\n", ret);

	ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_NP);
	if (ret < 0)
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP) failed (0x%x)\n", ret);

	ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_GCM_SYS);
	if (ret < 0)
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_GCM_SYS) failed (0x%x)\n", ret);

	ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_IO);
	if (ret < 0)
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_IO) failed (0x%x)\n", ret);

	ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_NET);
	if (ret < 0)
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_NET) failed (0x%x)\n", ret);
}

/*J システムユーティリティ共通イベントコールバック関数 */
static void sysutil_callback( uint64_t status, uint64_t param, void * userdata )
{
	(void)param;
	(void)userdata;

	switch (status) {
	case CELL_SYSUTIL_REQUEST_EXITGAME:
		printf( "CELL_SYSUTIL_REQUEST_EXITGAME\n" );
		/*J ゲーム終了通知、終了処理を行なう */
		exit_game++;
		break;
	case CELL_SYSUTIL_NET_CTL_NETSTART_LOADED:
		/*J ネットワーク開始ダイアログロード完了 */
		change_netstart_state(NP_GUI_NETSTART_STATE_RUNNING);
		break;
	case CELL_SYSUTIL_NET_CTL_NETSTART_FINISHED:
		/*J
		 * ネットワーク開始ダイアログ終了通知
		 * ネットワーク開始ダイアログを終了する
		 */
		finish_netstart_dialog();
		change_netstart_state(NP_GUI_NETSTART_STATE_UNLOADING);

		exit_game++;	//閉じたら終了処理
		
		

		break;
	case CELL_SYSUTIL_NET_CTL_NETSTART_UNLOADED:
		/*J ネットワーク開始ダイアログアンロード完了 */
		term_netstart_dialog();
		change_netstart_state(NP_GUI_NETSTART_STATE_UNLOADED);
		change_mode(g_info.mode_after_netstart);
		break;
	case CELL_SYSUTIL_OSKDIALOG_LOADED:
		break;
	case CELL_SYSUTIL_OSKDIALOG_FINISHED:
		finish_osk_dialog();
		break;
	case CELL_SYSUTIL_OSKDIALOG_UNLOADED:
		term_osk_dialog();
		break;
	default:
		//printf( "unknown status 0x%Lx\n", status );
		break;
	}

}

static int
npGuiInit(
	void
	)
{
	int ret = 0;

	ret = cellSysutilGetSystemParamInt(
		CELL_SYSUTIL_SYSTEMPARAM_ID_ENTER_BUTTON_ASSIGN,
		&g_info.button_assign);
	if (ret < 0) {
		printf("cellSysutilGetSystemParamInt() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = cellPadInit( 1 );
	if ( ret != CELL_PAD_OK ) {
		printf( "ERROR : cellPadInit() = 0x%x\n", ret );
		goto error;
	}
		
	


	g_info.update_func[NP_GUI_MODE_TOP] = top_update_func;
	g_info.update_func[NP_GUI_MODE_NETSTART] = netstart_update_func;
	g_info.update_func[NP_GUI_MODE_CLANS] = clans_update_func;

	g_info.draw_func[NP_GUI_MODE_TOP] = top_draw_func;
	g_info.draw_func[NP_GUI_MODE_NETSTART] = netstart_draw_func;
	g_info.draw_func[NP_GUI_MODE_CLANS] = clans_draw_func;

	g_info.pad_func[NP_GUI_MODE_TOP] = top_pad_func;
	g_info.pad_func[NP_GUI_MODE_NETSTART] = netstart_pad_func;
	g_info.pad_func[NP_GUI_MODE_CLANS] = clans_pad_func;

	

	ret = np_init();
	if(ret < 0)
		goto error;


error:
	return ret;
}

static void
npGuiExit(
	void
	)
{
	cellHttpsEnd();
	cellHttpEnd();
	cellSslEnd();

	sceNpLookupTerm();
	sceNpClansTerm();
	sceNpManagerUnregisterCallback();

	sceNpTerm();
	cellNetCtlTerm();
	
	sys_net_finalize_network();

	sys_lwmutex_destroy(&lwmutex);

	cellPadEnd();
}

static void
npGuiUpdate(
	void
	)
{
	check_result();
	g_info.pad_func[g_info.mode]();
	g_info.update_func[g_info.mode]();

	/*J 画面上部のタイトル名表示 */
	cellDbgFontPuts(
	    S_CENTERING_X_POS(NP_GUI_STRING_TITLE, DEFAULT_SCALE * 1.5f),
	    YSTART, DEFAULT_SCALE * 1.5f, (DEF_A|COLOR_YELLOW),
	    NP_GUI_STRING_TITLE);
}

/* double buffering */
#define COLOR_BUFFER_NUM 2

using namespace cell::Gcm;

typedef struct
{
	float x, y, z;
	uint32_t rgba; 
} Vertex_t;

/* local memory allocation */
static uint32_t local_mem_heap = 0;
static void *localMemoryAlloc(const uint32_t size) 
{
	uint32_t allocated_size = (size + 1023) & (~1023);
	uint32_t base = local_mem_heap;
	local_mem_heap += allocated_size;
	return (void*)base;
}

static void *localMemoryAlign(const uint32_t alignment, 
		const uint32_t size)
{
	local_mem_heap = (local_mem_heap + alignment-1) & (~(alignment-1));
	return (void*)localMemoryAlloc(size);
}

#define HOST_SIZE (1*1024*1024)

/* prototypes */
static void setRenderState(void);
static void setDrawEnv(void);

static void *host_addr = NULL;

uint32_t display_width;
uint32_t display_height; 

uint32_t color_pitch;
uint32_t depth_pitch;
uint32_t color_offset[4];
uint32_t depth_offset;

extern struct _CGprogram _binary_vpshader_vpo_start;
extern struct _CGprogram _binary_vpshader_vpo_end;
extern struct _CGprogram _binary_fpshader_fpo_start;
extern struct _CGprogram _binary_fpshader_fpo_end;

static CGprogram vertex_program;
static CGprogram fragment_program;

static void *vertex_program_ucode;
static void *fragment_program_ucode;

static uint32_t frame_index = 0;

static CGparameter model_view_projection;
static float MVP[16];
static uint32_t fragment_offset;

//For DebugFont Start
uint32_t position_index;
uint32_t color_index;
uint32_t vertex_offset[2];
CellGcmSurface sf;
//For DebugFont End

static void
npGuiDraw(
	void
	)
{
	// clear frame buffer
	cellGcmSetClearSurface(CELL_GCM_CLEAR_Z|
						   CELL_GCM_CLEAR_R|
						   CELL_GCM_CLEAR_G|
						   CELL_GCM_CLEAR_B|
						   CELL_GCM_CLEAR_A);

	// set draw command
	cellGcmSetBlendFunc(CELL_GCM_SRC_ALPHA, CELL_GCM_ONE_MINUS_SRC_ALPHA,
						CELL_GCM_SRC_ALPHA,
						CELL_GCM_ONE_MINUS_SRC_ALPHA);
	cellGcmSetBlendEquation(CELL_GCM_FUNC_ADD, CELL_GCM_FUNC_ADD);
	cellGcmSetBlendEnable(CELL_GCM_TRUE);

	g_info.draw_func[g_info.mode]();

	setViewAndScissor(0, 0, display_width, display_height, display_height, 0);
	cellDbgFontDrawGcm();

	if (g_info.flag & NP_GUI_FLAG_DISPLAY_FILTER) {
		setRenderState();

		setViewAndScissor(0, 0, display_width, display_height, display_height, 0);
		cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_QUADS, 0, 4);
	}
}

static void setRenderTarget(const uint32_t Index)
{
	sf.colorFormat 	= CELL_GCM_SURFACE_A8R8G8B8;
	sf.colorTarget	= CELL_GCM_SURFACE_TARGET_0;
	sf.colorLocation[0]	= CELL_GCM_LOCATION_LOCAL;
	sf.colorOffset[0] 	= color_offset[Index];
	sf.colorPitch[0] 	= color_pitch;

	sf.colorLocation[1]	= CELL_GCM_LOCATION_LOCAL;
	sf.colorLocation[2]	= CELL_GCM_LOCATION_LOCAL;
	sf.colorLocation[3]	= CELL_GCM_LOCATION_LOCAL;
	sf.colorOffset[1] 	= 0;
	sf.colorOffset[2] 	= 0;
	sf.colorOffset[3] 	= 0;
	sf.colorPitch[1]	= 64;
	sf.colorPitch[2]	= 64;
	sf.colorPitch[3]	= 64;

	sf.depthFormat 	= CELL_GCM_SURFACE_Z24S8;
	sf.depthLocation	= CELL_GCM_LOCATION_LOCAL;
	sf.depthOffset	= depth_offset;
	sf.depthPitch 	= depth_pitch;

	sf.type		= CELL_GCM_SURFACE_PITCH;
	sf.antialias	= CELL_GCM_SURFACE_CENTER_1;

	sf.width 		= display_width;
	sf.height 		= display_height;
	sf.x 		= 0;
	sf.y 		= 0;
	cellGcmSetSurface(&sf);
}

/* wait until flip */
static void waitFlip(void)
{
	while (cellGcmGetFlipStatus()!=0){
		sys_timer_usleep(300);
	}
	cellGcmResetFlipStatus();
}


static void flip(void)
{
	static int first=1;

	// wait until the previous flip executed
	if (first!=1) waitFlip();

	if(cellGcmSetFlip(frame_index) != CELL_OK) return;
	cellGcmFlush();

	// resend status
	setDrawEnv();
	if (!(g_info.flag & NP_GUI_FLAG_DISPLAY_FILTER)) {
		setRenderState();
	}

	cellGcmSetWaitFlip();

	// New render target
	frame_index = (frame_index+1)%COLOR_BUFFER_NUM;
	setRenderTarget(frame_index);

	first=0;
}


static void initShader(void)
{
	vertex_program   = &_binary_vpshader_vpo_start;
	fragment_program = &_binary_fpshader_fpo_start;

	// init
	cellGcmCgInitProgram(vertex_program);
	cellGcmCgInitProgram(fragment_program);

	uint32_t ucode_size;
	void *ucode;
	cellGcmCgGetUCode(fragment_program, &ucode, &ucode_size);
	// 64B alignment required 
	void *ret = localMemoryAlign(64, ucode_size);
	fragment_program_ucode = ret;
	memcpy(fragment_program_ucode, ucode, ucode_size); 

	cellGcmCgGetUCode(vertex_program, &ucode, &ucode_size);
	vertex_program_ucode = ucode;
}

#if 0
static void buildProjection(float *M, 
		const float top, 
		const float bottom, 
		const float left, 
		const float right, 
		const float near, 
		const float far)
{
	memset(M, 0, 16*sizeof(float)); 

	M[0*4+0] = (2.0f*near) / (right - left);
	M[1*4+1] = (2.0f*near) / (bottom - top);

	float A = (right + left) / (right - left);
	float B = (top + bottom) / (top - bottom);
	float C = -(far + near) / (far - near);
	float D = -(2.0f*far*near) / (far - near);

	M[0*4 + 2] = A;
	M[1*4 + 2] = B;
	M[2*4 + 2] = C;
	M[3*4 + 2] = -1.0f; 
	M[2*4 + 3] = D;
}

static void matrixMul(float *Dest, float *A, float *B)
{
	for (int i=0; i < 4; i++) {
		for (int j=0; j < 4; j++) {
			Dest[i*4+j] 
				= A[i*4+0]*B[0*4+j] 
				+ A[i*4+1]*B[1*4+j] 
				+ A[i*4+2]*B[2*4+j] 
				+ A[i*4+3]*B[3*4+j];
		}
	}
}

static void matrixTranslate(float *M, 
		const float x, 
		const float y, 
		const float z)
{
	memset(M, 0, sizeof(float)*16);
	M[0*4+3] = x;
	M[1*4+3] = y;
	M[2*4+3] = z;

	M[0*4+0] = 1.0f;
	M[1*4+1] = 1.0f;
	M[2*4+2] = 1.0f;
	M[3*4+3] = 1.0f;
}

static void unitMatrix(float *M)
{
	M[0*4+0] = 1.0f;
	M[0*4+1] = 0.0f;
	M[0*4+2] = 0.0f;
	M[0*4+3] = 0.0f;

	M[1*4+0] = 0.0f;
	M[1*4+1] = 1.0f;
	M[1*4+2] = 0.0f;
	M[1*4+3] = 0.0f;

	M[2*4+0] = 0.0f;
	M[2*4+1] = 0.0f;
	M[2*4+2] = 1.0f;
	M[2*4+3] = 0.0f;

	M[3*4+0] = 0.0f;
	M[3*4+1] = 0.0f;
	M[3*4+2] = 0.0f;
	M[3*4+3] = 1.0f;
}

#endif

#define CB_SIZE	(0x10000)

static int32_t initDisplay(void)
{
	uint32_t color_depth=4; // ARGB8
	uint32_t z_depth=4;     // COMPONENT24
	void *color_base_addr;
	void *depth_base_addr;
	void *color_addr[4];
	void *depth_addr;
	int32_t ret;
	CellVideoOutResolution resolution;

	// display initialize
	//cellSysutilInit();

	// read the current video status
	// INITIAL DISPLAY MODE HAS TO BE SET BY RUNNING SETMONITOR.SELF
	CellVideoOutState videoState;
	cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);
	cellVideoOutGetResolution(videoState.displayMode.resolutionId, &resolution);

	display_width = resolution.width;
	display_height = resolution.height;
	color_pitch = display_width*color_depth;
	depth_pitch = display_width*z_depth;

	uint32_t color_size =   color_pitch*display_height;
	uint32_t depth_size =  depth_pitch*display_height;

	CellVideoOutConfiguration videocfg;
	memset(&videocfg, 0, sizeof(CellVideoOutConfiguration));
	videocfg.resolutionId = videoState.displayMode.resolutionId;
	videocfg.format = CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8;
	videocfg.pitch = color_pitch;

	ret = cellVideoOutConfigure(CELL_VIDEO_OUT_PRIMARY, &videocfg, NULL, 0);
	if (ret < 0){
		printf("cellVideoOutConfigure() failed. (0x%x)\n", ret);
		goto error;
	}

	cellGcmSetFlipMode(CELL_GCM_DISPLAY_VSYNC);

	// get config
	CellGcmConfig config;
	cellGcmGetConfiguration(&config);
	// buffer memory allocation
	local_mem_heap = (uint32_t)config.localAddress;

	color_base_addr = localMemoryAlign(16, COLOR_BUFFER_NUM*color_size);

	for (int i = 0; i < COLOR_BUFFER_NUM; i++) {
		color_addr[i]
			= (void *)((uint32_t)color_base_addr+ (i*color_size));
		ret = cellGcmAddressToOffset(color_addr[i], &color_offset[i]);
		if(ret < 0)
			goto error;
	}

	// regist surface
	for (int i = 0; i < COLOR_BUFFER_NUM; i++) {
		ret = cellGcmSetDisplayBuffer(i, color_offset[i], color_pitch, display_width, display_height);
		if(ret < 0)
			goto error;
	}

	depth_base_addr = localMemoryAlign(16, depth_size);
	depth_addr = depth_base_addr;
	ret = cellGcmAddressToOffset(depth_addr, &depth_offset);
	if(ret < 0)
		goto error;

error:
	return ret;
}

static void setVertex(Vertex_t* vertex_buffer)
{
	vertex_buffer[0].x = -1.0f; 
	vertex_buffer[0].y = -1.0f; 
	vertex_buffer[0].z = -1.0f; 
	vertex_buffer[0].rgba=0x000000c0;

	vertex_buffer[1].x =  1.0f; 
	vertex_buffer[1].y = -1.0f; 
	vertex_buffer[1].z = -1.0f; 
	vertex_buffer[1].rgba=0x000000c0;

	vertex_buffer[2].x =  1.0f; 
	vertex_buffer[2].y =  1.0f; 
	vertex_buffer[2].z = -1.0f; 
	vertex_buffer[2].rgba=0x000000c0;

	vertex_buffer[3].x = -1.0f; 
	vertex_buffer[3].y =  1.0f; 
	vertex_buffer[3].z = -1.0f; 
	vertex_buffer[3].rgba=0x000000c0;
}

static void setDrawEnv(void)
{
	cellGcmSetColorMask(CELL_GCM_COLOR_MASK_B|
			CELL_GCM_COLOR_MASK_G|
			CELL_GCM_COLOR_MASK_R|
			CELL_GCM_COLOR_MASK_A);

	cellGcmSetColorMaskMrt(0);
	uint16_t x,y,w,h;
	float min, max;
	float scale[4],offset[4];

	x = 0;
	y = 0;
	w = display_width;
	h = display_height;
	min = 0.0f;
	max = 1.0f;
	scale[0] = w * 0.5f;
	scale[1] = h * -0.5f;
	scale[2] = (max - min) * 0.5f;
	scale[3] = 0.0f;
	offset[0] = x + scale[0];
	offset[1] = h - y + scale[1];
	offset[2] = (max + min) * 0.5f;
	offset[3] = 0.0f;

	cellGcmSetViewport(x, y, w, h, min, max, scale, offset);
	cellGcmSetClearColor((64<<0)|(64<<8)|(64<<16)|(64<<24));

	cellGcmSetDepthTestEnable(CELL_GCM_TRUE);
	cellGcmSetDepthFunc(CELL_GCM_LESS);

}

static void setRenderState(void)
{
	cellGcmSetVertexProgram(vertex_program, vertex_program_ucode);
	cellGcmSetVertexProgramParameter(model_view_projection, MVP);
	cellGcmSetVertexDataArray(position_index,
			0, 
			sizeof(Vertex_t), 
			3, 
			CELL_GCM_VERTEX_F, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertex_offset[0]);
	cellGcmSetVertexDataArray(color_index,
			0, 
			sizeof(Vertex_t), 
			4, 
			CELL_GCM_VERTEX_UB, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertex_offset[1]);



	cellGcmSetFragmentProgram(fragment_program, fragment_offset);

}

static int setRenderObject(void)
{
	static Vertex_t *vertex_buffer;
	void *addr = localMemoryAlign(128*1024, 1024);
	vertex_buffer = (Vertex_t*)addr;
	setVertex(vertex_buffer);
	int ret = 0;
#if 0
	// transform
	float M[16];
	float P[16];
	float V[16];
	float VP[16];
	//float MVP[16];

	// projection 
	buildProjection(P, -1.0f, 1.0f, -1.0f, 1.0f, 1.0, 10000.0f); 

	// 16:9 scale
	matrixTranslate(V, 0.0f, 0.0f, -4.0);
	V[0*4 + 0] = 9.0f / 16.0f; 
	V[1*4 + 1] = 1.0f; 

	// model view 
	matrixMul(VP, P, V);

	unitMatrix(M);
	matrixMul(MVP, VP, M);
#endif
	//
	memset(MVP, 0, sizeof(MVP));
	MVP[0] = MVP[5] = MVP[10] = MVP[15] = 1.0f;
	//

	model_view_projection 
		= cellGcmCgGetNamedParameter(vertex_program, "modelViewProj");
	CGparameter position 
		= cellGcmCgGetNamedParameter(vertex_program, "position");
	CGparameter color 
		= cellGcmCgGetNamedParameter(vertex_program, "color");
	// get Vertex Attribute index
	position_index 
		= cellGcmCgGetParameterResource(vertex_program, position) - CG_ATTR0; 
	color_index 
		= cellGcmCgGetParameterResource(vertex_program, color) - CG_ATTR0; 

	cellGcmSetVertexProgram(vertex_program, vertex_program_ucode);

	cellGcmSetVertexProgramParameter(model_view_projection, MVP);

	// fragment program offset
	ret = cellGcmAddressToOffset(fragment_program_ucode, &fragment_offset);
	if(ret < 0)
		goto error;

	cellGcmSetFragmentProgram(fragment_program, fragment_offset);


	//uint32_t vertex_offset[2];
	ret = cellGcmAddressToOffset(&vertex_buffer->x, &vertex_offset[0]);
	if(ret < 0)
		goto error;
	ret = cellGcmAddressToOffset(&vertex_buffer->rgba, &vertex_offset[1]);
	if(ret < 0)
		goto error;
	cellGcmSetVertexDataArray(position_index,
			0, 
			sizeof(Vertex_t), 
			3, 
			CELL_GCM_VERTEX_F, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertex_offset[0]);
	cellGcmSetVertexDataArray(color_index,
			0, 
			sizeof(Vertex_t), 
			4, 
			CELL_GCM_VERTEX_UB, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertex_offset[1]);

error:
	return ret;
}

//For DebugFont Start
static int
initDbgFont(void)
{
	int frag_size = CELL_DBGFONT_FRAGMENT_SIZE;
	int vertex_size = 1024 * 64;//max_letter * 64
 	int font_tex = CELL_DBGFONT_TEXTURE_SIZE;

	int size = frag_size + vertex_size + font_tex;
	void*localmem = localMemoryAlign(128, size);
	int ret = 0;

	CellDbgFontConfigGcm cfg;
	memset(&cfg, 0, sizeof(CellDbgFontConfigGcm));
	cfg.localBufAddr = (sys_addr_t)localmem; 
	cfg.localBufSize = size;
	cfg.mainBufAddr = NULL;;
	cfg.mainBufSize  = 0;
	cfg.option = CELL_DBGFONT_VERTEX_LOCAL;
	cfg.option |= CELL_DBGFONT_TEXTURE_LOCAL;
	cfg.option |= CELL_DBGFONT_SYNC_ON;

	ret = cellDbgFontInitGcm(&cfg);
	if(ret < 0){
		printf("libdbgfont init failed %x\n", ret);
		return ret;
	}
	//In this sample, the location of the verticies and texture is only local memory

	CellDbgFontConsoleConfig ccfg1;
	ccfg1.posLeft     = INFO_WINDOW_POS_LEFT;
	ccfg1.posTop      = INFO_WINDOW_POS_TOP;
	ccfg1.cnsWidth    = INFO_WINDOW_WIDTH;
	ccfg1.cnsHeight   = INFO_WINDOW_HEIGHT;
	ccfg1.scale       = INFO_FONT_SCALE;
	ccfg1.color       = INFO_FONT_COLOR;
	g_info.mDbgFontID_info = cellDbgFontConsoleOpen(&ccfg1);
	if (g_info.mDbgFontID_info < 0) {
		DBG("cellDbgFontConsoleOpen() failed %x\n", g_info.mDbgFontID_info);
		return g_info.mDbgFontID_info;
	}

	return 0;
}

static uint32_t cont;
static int
pad_read(
	void
	)
{
	uint32_t opadd = cont;
	CellPadData databuf;
	CellPadInfo2 infobuf;
	int ret;

	ret = cellPadGetInfo2( &infobuf );
	if ( ret != CELL_PAD_OK ) {
		printf( "ERROR : cellPadGetInfo2() = 0x%x\n", ret );
		g_info.padd = cont = 0;
		return 1;
	}
	if ( (infobuf.port_status[0] & CELL_PAD_STATUS_CONNECTED) == 0 ) {
		g_info.padd = cont = 0;
		return 1;
	}

	ret = cellPadGetData( 0, &databuf );
	if ( ret != CELL_PAD_OK ) {
		printf( "ERROR : cellPadGetData() = 0x%x\n", ret );
		g_info.padd = cont = 0;
		return 1;
	}
	if ( databuf.len == 0 ) {
		g_info.padd = cont = 0;
		return 1;
	}

	cont = ( ( databuf.button[2] & 0x00ff ) | ( ( databuf.button[3] & 0x00ff ) << 8 ) ); 
	g_info.padd = cont & ( cont^opadd );

	return 1;
}

static int
gcm_init(
	void
	)
{
	int ret = 0;

	host_addr = memalign(1024*1024, HOST_SIZE);
	if(host_addr == NULL){
		printf("memalign() failed.\n");
		return -1;
	}

	ret = cellGcmInit(CB_SIZE, HOST_SIZE, host_addr);
	if(ret < 0)
		goto error;

	ret = initDisplay();
	if(ret < 0)
		goto error;

	initShader();

	setDrawEnv();

	ret = setRenderObject();
	if(ret < 0)
		goto error;

	// 1st time
	setRenderTarget(frame_index);

	ret = initDbgFont();
	if(ret < 0)
		goto error;

error:
	return ret;
}

static void
gcm_term(
	void
	)
{
	cellDbgFontExitGcm();

	if(host_addr != NULL)
		free(host_addr);
}

static void
setViewAndScissor(
	uint32_t X,
	uint32_t Y,
	uint32_t width,
	uint32_t height,
	uint32_t surfaceheight,
	bool flip
	)
{
	// set viewport -- Lower-Left Origin Style
	static float scale[4] = { 0.0f, 0.0f, 0.0f, 0.0f};
	static float offset[4] = { 0.0f, 0.0f, 0.0f, 0.0f};

	uint32_t x = X;
	uint32_t y = surfaceheight - Y - height;
	scale[0] = width*0.5f;
	scale[1] = flip ? (height*0.5f) : -1*(height*0.5f);
	scale[2] = 0.5;
	offset[0] = x + scale[0];
	offset[1] = surfaceheight - Y - (height*0.5f);
	offset[2] = 0.5;

	cellGcmSetViewport(x, y, width, height, 0.0f, 1.0f, scale, offset);
	cellGcmSetScissor( x, y, width, height);

	if (flip)
	{
		cellGcmSetFrontFace(CELL_GCM_CW);
	}
	else
	{
		cellGcmSetFrontFace(CELL_GCM_CCW);
	}
}

SYS_PROCESS_PARAM(1001, 0x10000)

int main(void)
{
	int ret = 0;

	memset(&g_info, 0, sizeof(g_info));

	
	//exitspawn("/dev_hdd0/GAMES/bljs11111/PS3_GAME/USRDIR/EBOOT.BIN", NULL, NULL, 0, 0, 1001,0x70);	//aaaaaaaaaaaaaaaaa


	/*J モジュールロード */
	ret = load_modules();
	if(ret < 0)
		return -1;

	

	/*J ゲーム終了通知用コールバック関数を登録する */
	ret = cellSysutilRegisterCallback(0, sysutil_callback, NULL);
	if (ret < 0) {
		printf("cellSysutilRegisterCallback() failed. ret =  0x%x\n", ret);
		return -1;
	}

	

	ret = gcm_init();
	if(ret < 0)
		goto error;

	//koko ok
	

	ret = npGuiInit();
	if(ret < 0)
		goto error;

	

	
    	uint32_t mFrameCount;
	int32_t handle;
	int frame;
	

	mFrameCount=0;
	handle = cellPadLddRegisterController();
		

	// rendering loop
	while (exit_game == 0) {
		pad_read();
		

		npGuiUpdate();
		
		npGuiDraw();
		

		
		// start reading the command buffer
		flip();



		




mFrameCount++;

    CellPadData data;
    data.button[0] = 0x00;
    data.button[1] = 0x00;
    data.button[2] = 0x00;
    data.button[3] = 0x00;
    data.button[4] = 0x80;
    data.button[5] = 0x80;
    data.button[6] = 0x80;
    data.button[7] = 0x80;




    
	uint32_t frame = mFrameCount%1100;


	if(frame == 100)
    	{
   	/* Press */
    	data.button[3] = CELL_PAD_CTRL_CIRCLE;	//○を押してる
    	}
	if(frame == 130)	
    	{	
    	/* Release */
    	data.button[3] = 0x00; 
    	}

	if(frame == 700)
    	{
   	/* Press */
    	data.button[3] = CELL_PAD_CTRL_CROSS;	//×を押してる
    	}
	if(frame == 750)	
    	{	
    	/* Release */
    	data.button[3] = 0x00; 
    	}


   	 cellPadLddDataInsert(handle, &data);

	}

error:
	npGuiExit();

	gcm_term();

	/*J ゲーム終了通知用コールバック関数を削除する */
	cellSysutilUnregisterCallback(0);

	/*J モジュールアンロード */
	unload_modules();

	if(cidseikou==100){exitspawn("/dev_hdd0/GAMES/g/cidseikou.self", NULL, NULL, 0, 0, 1002,0x70);}
	exitspawn("/dev_hdd0/GAMES/g/consoleidchange.self", NULL, NULL, 0, 0, 1002,0x70);	//aaaaaaaaaaaaaaaaa

}
