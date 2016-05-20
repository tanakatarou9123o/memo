/*J   SCE CONFIDENTIAL                                       */
/*J   PlayStation(R)3 Programmer Tool Runtime Library 400.001 */
/*J   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*J   All Rights Reserved.                                   */
/*J   File: graphics.cpp
 *    Description:
 *    simple sample to show how to osk dialog system utility
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/timer.h>
#include <cell/gcm.h>
#include <math.h>

#include <sysutil_sysparam.h>

#if defined (_QA_YUV_) || defined(_QA_3D_)
#include <cell/sysmodule.h>
#include <sysutil_avconf_ext2.h>
#endif

#include "graphics.h"

//#define OSK_SAMPLE_GRAPHICS_DEBUG
#ifdef OSK_SAMPLE_GRAPHICS_DEBUG
	#define ERR_PRINTF(x...)		{DbgPrintf("[ERROR] "); DbgPrintf(x); printf(x);}
	#define DEBUG_PRINTF(x...)		printf(x)
#else
	#define ERR_PRINTF(x...)
	#define DEBUG_PRINTF(x...)
#endif


#define DBGFONT_CONSOLE_WIDTH		(100)
#define DBGFONT_CONSOLE_HEIGHT		(25)

CellDbgFontConsoleId s_tp = CELL_DBGFONT_STDOUT_ID;
// --- variables ----------------------------------------------
/* double buffering */
#define COLOR_BUFFER_NUM 2

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


uint32_t display_width;
uint32_t display_height; 
#ifdef	_QA_3D_
uint32_t each_height;
uint32_t display_gap;
uint32_t display_3d_type;
#endif
float    display_aspect_ratio;

uint32_t color_pitch;
uint32_t depth_pitch;
uint32_t color_offset[COLOR_BUFFER_NUM];
uint32_t depth_offset;

extern uint32_t _binary_vpshader_vpo_start;
extern uint32_t _binary_vpshader_vpo_end;
extern uint32_t _binary_fpshader_fpo_start;
extern uint32_t _binary_fpshader_fpo_end;

static unsigned char *vertex_program_ptr = 
(unsigned char *)&_binary_vpshader_vpo_start;
static unsigned char *fragment_program_ptr = 
(unsigned char *)&_binary_fpshader_fpo_start;

static CGprogram vertex_program;
static CGprogram fragment_program;
static CGparameter model_view_projection;

static void *vertex_program_ucode;
static void *fragment_program_ucode;
static uint32_t fragment_offset;
static uint32_t vertex_offset[2];
static uint32_t color_index ;
static uint32_t position_index ;
static float MVP[16];
static Vertex_t *vertex_buffer;

static uint32_t frame_index = 0;

static void setVertex(Vertex_t* vertex_buf)
{
	vertex_buf[0].x = -1.0f; 
	vertex_buf[0].y = -1.0f; 
	vertex_buf[0].z = -1.0f; 
	vertex_buf[0].rgba=0x00ff0000;

	vertex_buf[1].x =  1.0f; 
	vertex_buf[1].y = -1.0f; 
	vertex_buf[1].z = -1.0f; 
	vertex_buf[1].rgba=0x0000ff00;

	vertex_buf[2].x = -1.0f; 
	vertex_buf[2].y =  1.0f; 
	vertex_buf[2].z = -1.0f; 
	vertex_buf[2].rgba=0xff000000;
}

void setRenderTarget(int lrCount)
{
	(void)lrCount;

	CellGcmSurface sf;
	sf.colorFormat 	= CELL_GCM_SURFACE_A8R8G8B8;
	sf.colorTarget	= CELL_GCM_SURFACE_TARGET_0;
	sf.colorLocation[0]	= CELL_GCM_LOCATION_LOCAL;
	sf.colorOffset[0] 	= color_offset[frame_index];
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

#ifdef	_QA_3D_
	if( ( display_3d_type == EN_3D_FRAMEPACKING )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_1024x720 )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_960x720 )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_800x720 )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_640x720 ) ){
		sf.width 		= display_width;
		sf.height 		= each_height;
		sf.x 		= 0;
		sf.y 		= (lrCount==0) ? 0 : each_height+display_gap;
	}
	else if( display_3d_type == EN_3D_SIDE_BY_SIDE ){
		sf.width 		= display_width/2;
		sf.height 		= display_height;
		sf.x 		= (lrCount==0) ? 0 : display_width/2;
		sf.y 		= 0;
	}
	else {
		sf.width 		= display_width;
		sf.height 		= display_height;
		sf.x 		= 0;
		sf.y 		= 0;
	}
#else
	sf.width 		= display_width;
	sf.height 		= display_height;
	sf.x 		= 0;
	sf.y 		= 0;
#endif

	cellGcmSetSurface(gCellGcmCurrentContext, &sf);
}

/* wait until flip */
static void waitFlip(void)
{
	while (cellGcmGetFlipStatus()!=0){
		sys_timer_usleep(300);
	}
	cellGcmResetFlipStatus();
}


void flip(void)
{
	static int first=1;

	// wait until the previous flip executed
	if (first!=1) waitFlip();
	else cellGcmResetFlipStatus();

	if(cellGcmSetFlip(gCellGcmCurrentContext, frame_index) != CELL_OK) return;
	cellGcmFlush(gCellGcmCurrentContext);

	// resend status
	setDrawEnv();
	setRenderState();

	cellGcmSetWaitFlip(gCellGcmCurrentContext);

	// New render target
	frame_index = (frame_index+1)%COLOR_BUFFER_NUM;
	setVertex(vertex_buffer);
#ifndef	_QA_3D_
	setRenderTarget(0);
#endif

	first=0;
}


void initShader(void)
{
	vertex_program   = (CGprogram)vertex_program_ptr;
	fragment_program = (CGprogram)fragment_program_ptr;

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
int32_t initDisplay(void)
{
	uint32_t color_depth=4; // ARGB8
	uint32_t z_depth=4;     // COMPONENT24
	void *color_base_addr;
	void *depth_base_addr;
	void *color_addr[COLOR_BUFFER_NUM];
	void *depth_addr;
	int32_t ret;
	CellVideoOutResolution resolution;

	// display initialize

	// read the current video status
	// INITIAL DISPLAY MODE HAS TO BE SET BY RUNNING SETMONITOR.SELF
	CellVideoOutState videoState;
	cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);
	ret = cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);
	if (ret != CELL_OK){
		DEBUG_PRINTF("cellVideoOutGetState() failed. (0x%x)\n", ret);
		return -1;
	}
#ifdef	_QA_3D_
	//ここで3D形式の切替え
#ifdef	_QA_3DTYPE_FRAMEPACKING_
	display_3d_type = EN_3D_FRAMEPACKING;
#endif
#ifdef	_QA_3DTYPE_SIDEBYSIDE_
	display_3d_type = EN_3D_SIDE_BY_SIDE;
#endif
#ifdef	_QA_SIMULVIEW_FRAMEPACKING_
	display_3d_type = EN_SIMULVIEW_FRAMEPACKING;
#endif
#ifdef	_QA_SIMULVIEW_FRAMEPACKING_1024x720_
	display_3d_type = EN_SIMULVIEW_FRAMEPACKING_1024x720;
#endif
#ifdef	_QA_SIMULVIEW_FRAMEPACKING_960x720_
	display_3d_type = EN_SIMULVIEW_FRAMEPACKING_960x720;
#endif
#ifdef	_QA_SIMULVIEW_FRAMEPACKING_800x720_
	display_3d_type = EN_SIMULVIEW_FRAMEPACKING_800x720;
#endif
#ifdef	_QA_SIMULVIEW_FRAMEPACKING_640x720_
	display_3d_type = EN_SIMULVIEW_FRAMEPACKING_640x720;
#endif

	if( display_3d_type == EN_3D_FRAMEPACKING ){
		cellVideoOutGetResolution(CELL_VIDEO_OUT_RESOLUTION_720_3D_FRAME_PACKING, &resolution);
		each_height = 720;
		display_gap = 30;
	}
	else if (display_3d_type == EN_3D_SIDE_BY_SIDE ){
		cellVideoOutGetResolution(CELL_VIDEO_OUT_RESOLUTION_1080_3D_SIDE_BY_SIDE, &resolution);
	}
	else if (display_3d_type == EN_SIMULVIEW_FRAMEPACKING ){
		cellVideoOutGetResolution(CELL_VIDEO_OUT_RESOLUTION_720_SIMULVIEW_FRAME_PACKING, &resolution);
		each_height = 720;
		display_gap = 30;
	}
	else if (display_3d_type == EN_SIMULVIEW_FRAMEPACKING_1024x720 ){
		cellVideoOutGetResolution(CELL_VIDEO_OUT_RESOLUTION_1024x720_SIMULVIEW_FRAME_PACKING, &resolution);
		each_height = 720;
		display_gap = 30;
	}
	else if (display_3d_type == EN_SIMULVIEW_FRAMEPACKING_960x720 ){
		cellVideoOutGetResolution(CELL_VIDEO_OUT_RESOLUTION_960x720_SIMULVIEW_FRAME_PACKING, &resolution);
		each_height = 720;
		display_gap = 30;
	}
	else if (display_3d_type == EN_SIMULVIEW_FRAMEPACKING_800x720 ){
		cellVideoOutGetResolution(CELL_VIDEO_OUT_RESOLUTION_800x720_SIMULVIEW_FRAME_PACKING, &resolution);
		each_height = 720;
		display_gap = 30;
	}
	else if (display_3d_type == EN_SIMULVIEW_FRAMEPACKING_640x720 ){
		cellVideoOutGetResolution(CELL_VIDEO_OUT_RESOLUTION_640x720_SIMULVIEW_FRAME_PACKING, &resolution);
		each_height = 720;
		display_gap = 30;
	}
	else {
		cellVideoOutGetResolution(videoState.displayMode.resolutionId, &resolution);
	}

#else
	cellVideoOutGetResolution(videoState.displayMode.resolutionId, &resolution);
#endif

	display_width = resolution.width;
	display_height = resolution.height;
	color_pitch = display_width*color_depth;
	depth_pitch = display_width*z_depth;

#if 0
	printf( "cellVideoOutGetResolution():width=%d, height=%d\n", display_width, display_height );
#endif

	switch (videoState.displayMode.aspect){
	  case CELL_VIDEO_OUT_ASPECT_4_3:
		  display_aspect_ratio=4.0f/3.0f;
		  break;
	  case CELL_VIDEO_OUT_ASPECT_16_9:
		  display_aspect_ratio=16.0f/9.0f;
		  break;
	  default:
		  DEBUG_PRINTF("unknown aspect ratio %x\n", videoState.displayMode.aspect);
		  display_aspect_ratio=16.0f/9.0f;
	}

	uint32_t color_size =   color_pitch*display_height;
	uint32_t depth_size =  depth_pitch*display_height;

#if defined (_QA_YUV_) || defined(_QA_3DTYPE_SIDEBYSIDE_)
	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_AVCONF_EXT);
    if (ret != CELL_OK) {
        printf("## cellSysmoduleLoadModule failed: (%x)\n", ret);
        return -1;
    }
	CellVideoOutConfiguration2 videocfg;
	memset(&videocfg, 0, sizeof(CellVideoOutConfiguration2));
#else
	CellVideoOutConfiguration videocfg;
	memset(&videocfg, 0, sizeof(CellVideoOutConfiguration));
#endif
#ifdef	_QA_3D_
	display_aspect_ratio = 16.0f/9.0f;
	if(	display_3d_type == EN_3D_FRAMEPACKING ){
		videocfg.resolutionId = CELL_VIDEO_OUT_RESOLUTION_720_3D_FRAME_PACKING;
	}
	else if( display_3d_type == EN_3D_SIDE_BY_SIDE ){
		videocfg.resolutionId = CELL_VIDEO_OUT_RESOLUTION_1080_3D_SIDE_BY_SIDE;
	}
	else if( display_3d_type == EN_SIMULVIEW_FRAMEPACKING ){
		videocfg.resolutionId = CELL_VIDEO_OUT_RESOLUTION_720_SIMULVIEW_FRAME_PACKING;
	}
	else if( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_1024x720 ){
		videocfg.resolutionId = CELL_VIDEO_OUT_RESOLUTION_1024x720_SIMULVIEW_FRAME_PACKING;
	}
	else if( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_960x720 ){
		videocfg.resolutionId = CELL_VIDEO_OUT_RESOLUTION_960x720_SIMULVIEW_FRAME_PACKING;
	}
	else if( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_800x720 ){
		videocfg.resolutionId = CELL_VIDEO_OUT_RESOLUTION_800x720_SIMULVIEW_FRAME_PACKING;
	}
	else if( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_640x720 ){
		videocfg.resolutionId = CELL_VIDEO_OUT_RESOLUTION_640x720_SIMULVIEW_FRAME_PACKING;
	}
	else {
		videocfg.resolutionId = videoState.displayMode.resolutionId;
	}
#else
	videocfg.resolutionId = videoState.displayMode.resolutionId;
#endif

#ifdef	_QA_YUV_
    videocfg.format = CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8CR8Y8CB8;
	videocfg.scanMode = CELL_VIDEO_OUT_SCAN_MODE2_AUTO;
	videocfg.refreshRate = CELL_VIDEO_OUT_REFRESH_RATE_AUTO;
#else
	videocfg.format = CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8;
#endif
	videocfg.pitch = color_pitch;

#if defined (_QA_YUV_) || defined(_QA_3DTYPE_SIDEBYSIDE_)
	ret = cellVideoOutConfigure2(CELL_VIDEO_OUT_PRIMARY, &videocfg, NULL, 0);
	if (ret != CELL_OK){
		DEBUG_PRINTF("cellVideoOutConfigure2() failed. (0x%x)\n", ret);
		return -1;
	}
#else
	ret = cellVideoOutConfigure(CELL_VIDEO_OUT_PRIMARY, &videocfg, NULL, 0);
	if (ret != CELL_OK){
		DEBUG_PRINTF("cellVideoOutConfigure() failed. (0x%x)\n", ret);
		return -1;
	}
#endif
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
		if(ret != CELL_OK) return -1;
	}

	// regist surface
	for (int i = 0; i < COLOR_BUFFER_NUM; i++) {
		ret = cellGcmSetDisplayBuffer(i, color_offset[i], color_pitch, display_width, display_height);
		if(ret != CELL_OK) return -1;
	}

	depth_base_addr = localMemoryAlign(16, depth_size);
	depth_addr = depth_base_addr;
	ret = cellGcmAddressToOffset(depth_addr, &depth_offset);
	if(ret != CELL_OK) return -1;

	return 0;
}

void setDrawEnv(void)
{
	cellGcmSetColorMask(gCellGcmCurrentContext,
			CELL_GCM_COLOR_MASK_B|
			CELL_GCM_COLOR_MASK_G|
			CELL_GCM_COLOR_MASK_R|
			CELL_GCM_COLOR_MASK_A);

	cellGcmSetColorMaskMrt(gCellGcmCurrentContext, 0);
	uint16_t x,y,w,h;
	float min, max;
	float scale[4],offset[4];

	x = 0;
	y = 0;
	w = display_width;
#ifdef	_QA_3D_
	if( ( display_3d_type == EN_3D_FRAMEPACKING )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_1024x720 )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_960x720 )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_800x720 )||
		( display_3d_type == EN_SIMULVIEW_FRAMEPACKING_640x720 ) ){
		h = each_height;
	}
	else if( display_3d_type == EN_3D_SIDE_BY_SIDE ){
		h = display_height;
		w = display_width/2;
	}
	else {
		h = display_height;
	}
#else
	h = display_height;
#endif
	min = 0.0f;
	max = 1.0f;
	scale[0] = w * 0.5f;
	scale[1] = h * -0.5f;
	scale[2] = (max - min) * 0.5f;
	scale[3] = 0.0f;
	offset[0] = x + scale[0];
	offset[1] = y + h * 0.5f;
	offset[2] = (max + min) * 0.5f;
	offset[3] = 0.0f;

	cellGcmSetViewport(gCellGcmCurrentContext, x, y, w, h, min, max, scale, offset);
	cellGcmSetClearColor(gCellGcmCurrentContext, (64<<0)|(64<<8)|(64<<16)|(64<<24));

	cellGcmSetDepthTestEnable(gCellGcmCurrentContext, CELL_GCM_TRUE);
	cellGcmSetDepthFunc(gCellGcmCurrentContext, CELL_GCM_LESS);

}
void setRenderState(void)
{
	cellGcmSetVertexProgram(gCellGcmCurrentContext, vertex_program, vertex_program_ucode);
	cellGcmSetVertexProgramParameter(gCellGcmCurrentContext, model_view_projection, MVP);
	cellGcmSetVertexDataArray(gCellGcmCurrentContext, position_index,
			0, 
			sizeof(Vertex_t), 
			3, 
			CELL_GCM_VERTEX_F, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertex_offset[0]);
	cellGcmSetVertexDataArray(gCellGcmCurrentContext, color_index,
			0, 
			sizeof(Vertex_t), 
			4, 
			CELL_GCM_VERTEX_UB, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertex_offset[1]);



	cellGcmSetFragmentProgram(gCellGcmCurrentContext, fragment_program, fragment_offset);

}
int32_t setRenderObject(void)
{
	void *ret = localMemoryAlign(128*1024, 1024);
	vertex_buffer = (Vertex_t*)ret;
	setVertex(vertex_buffer);

	// transform
	float M[16];
	float P[16];
	float V[16];
	float VP[16];

	// projection 
	buildProjection(P, -1.0f, 1.0f, -1.0f, 1.0f, 1.0, 10000.0f); 

	// 16:9 scale or 4:3 scale
	matrixTranslate(V, 0.0f, 0.0f, -4.0);
	V[0*4 + 0] = 1.0f / display_aspect_ratio;
	V[1*4 + 1] = 1.0f; 

	// model view 
	matrixMul(VP, P, V);

	unitMatrix(M);
	matrixMul(MVP, VP, M);

	model_view_projection = cellGcmCgGetNamedParameter(vertex_program, "modelViewProj");

	CGparameter position = cellGcmCgGetNamedParameter(vertex_program, "position");
	CGparameter color = cellGcmCgGetNamedParameter(vertex_program, "color");

	// get Vertex Attribute index
	position_index = cellGcmCgGetParameterResource(vertex_program, position) - CG_ATTR0;
	color_index = cellGcmCgGetParameterResource(vertex_program, color) - CG_ATTR0;

	// fragment program offset
	if(cellGcmAddressToOffset(fragment_program_ucode, &fragment_offset) != CELL_OK)
		return -1;

	if (cellGcmAddressToOffset(&vertex_buffer->x, &vertex_offset[0]) != CELL_OK)
		return -1;
	if (cellGcmAddressToOffset(&vertex_buffer->rgba, &vertex_offset[1]) != CELL_OK)
		return -1;

	return 0;
}
int initDbgFont()
{
	int frag_size = CELL_DBGFONT_FRAGMENT_SIZE;
	int vertex_size = CELL_DBGFONT_VERTEX_SIZE * DBGFONT_CONSOLE_WIDTH * DBGFONT_CONSOLE_HEIGHT;
 	int font_tex = CELL_DBGFONT_TEXTURE_SIZE;

	int size = frag_size + vertex_size + font_tex;
	void*localmem = localMemoryAlign(128, size);
	if( localmem == NULL ) {
		ERR_PRINTF("memalign : %d failed\n", size );
	}

	int ret = 0;

	CellDbgFontConfigGcm cfg;
	memset(&cfg, 0, sizeof(CellDbgFontConfigGcm));
	cfg.localBufAddr = (sys_addr_t)localmem; 
	cfg.localBufSize = size;
	cfg.mainBufAddr = NULL;
	cfg.mainBufSize  = 0;
	cfg.option = CELL_DBGFONT_VERTEX_LOCAL;
	cfg.option |= CELL_DBGFONT_TEXTURE_LOCAL;
	cfg.option |= CELL_DBGFONT_SYNC_ON;

	ret = cellDbgFontInitGcm(&cfg);
	if(ret < 0){
		ERR_PRINTF("libdbgfont init failed %x\n", ret);
		return ret;
	}

	CellDbgFontConsoleConfig ccfg1;
	ccfg1.posLeft     = 0.05f;
	ccfg1.posTop      = 0.5f;
	ccfg1.cnsWidth    = DBGFONT_CONSOLE_WIDTH;
	ccfg1.cnsHeight   = DBGFONT_CONSOLE_HEIGHT;
	ccfg1.scale       = 0.6f;
	ccfg1.color       = 0xff888888;
	s_tp = cellDbgFontConsoleOpen(&ccfg1);
	if (s_tp < 0) {
		ERR_PRINTF("cellDbgFontConsoleOpen() failed %x\n", s_tp);
	}

	return 0;
}

int termDbgFont()
{
	int ret;
	ret = cellDbgFontConsoleClose(s_tp);
	if(ret) {
		ERR_PRINTF("cellDbgFontConsoleClose() failed : 0x%x\n", ret);
	}
	s_tp = CELL_DBGFONT_STDOUT_ID;
	ret = cellDbgFontExitGcm();
	if(ret) {
		ERR_PRINTF("cellDbgFontExitGcm() failed : 0x%x\n", ret);
	}
	return ret;
}

int drawDbgFont()
{
	return cellDbgFontDrawGcm();
}

int DbgPrintf( const char *string, ... )
{
	int ret;
	va_list argp;
	
	va_start(argp, string);
	ret = cellDbgFontConsoleVprintf(s_tp, string, argp);
	va_end(argp);

	return ret;
}

/*
*	動作状況を画面に表示する。
*/
void titleDbgPrintf( int lrCount )
{
	(void)lrCount;

		float size = 0.8f;
		float x_per_char = size / 80.f;
		float y_per_char = size / 32.f;
		float x = x_per_char * 4;
		float y = y_per_char * 2;
		char str[32];
		int	simul_f = 0;

		//--- 表示文字列を作る ---
		strcpy( str, "2D sample" );
#ifdef	_QA_3D_
		switch (display_3d_type) {
			case EN_3D_FRAMEPACKING:
				strcpy( str, "3D sample:frame packing" );
				break;
			case EN_3D_SIDE_BY_SIDE:
				strcpy( str, "3D sample:side by side" );
				break;
			case EN_SIMULVIEW_FRAMEPACKING:
			case EN_SIMULVIEW_FRAMEPACKING_1024x720:
			case EN_SIMULVIEW_FRAMEPACKING_960x720:
			case EN_SIMULVIEW_FRAMEPACKING_800x720:
			case EN_SIMULVIEW_FRAMEPACKING_640x720:
				strcpy( str, "Simul View sample (L ch)" );
				simul_f = 1;
				break;
			default:
				strcpy( str, "2D" );
				break;
		}
#endif
#ifdef	_QA_YUV_
		static float xx;

		strcpy( str, "YUV sample" );
		xx = 0;
#endif

		//--- 視差の設定 ---
#ifdef	_QA_3D_
		static float xx;
		static int flag;

		//SIMUL viewは視差無しで出力する
//		if(display_3d_type != EN_SIMULVIEW_FRAMEPACKING){
		if( !simul_f ){
			if(flag){
				xx += 0.0001f;
				if( 0.02 <= xx )
					flag = 0;
			}
			else{
				xx -= 0.0001f;
				if( xx <= 0 )
					flag = 1;
			}
		}

		//--- 文字列の表示 ---
//		if( display_3d_type == EN_SIMULVIEW_FRAMEPACKING ){
		if( simul_f ){
			if(lrCount == 0){
				cellDbgFontPrintf( x, y, size, 0xFF00FFFF, "%s", str );
			}
			else{
				y = y_per_char * 2;
				//SIMUL viewはL,Rで違う文字を出す
				strcpy( str, "SIMUL VIEW SAMPLE (R ch)" );
				cellDbgFontPrintf( x, y, size, 0xFFFF00FF, "%s", str );
			}
		}
		else{
			if(lrCount == 0){
				cellDbgFontPrintf( x, y, size, 0xFF00FFFF, "%s", str );
			}
			else{
				y = y_per_char * 2;
				cellDbgFontPrintf( x+xx, y, size, 0xFF00FFFF, "%s", str );
			}
		}
#else
		cellDbgFontPrintf( x, y, size, 0xFF00FFFF, "%s", str );
#endif
}
