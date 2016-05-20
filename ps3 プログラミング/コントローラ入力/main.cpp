/*  SCE CONFIDENTIAL
 *  PlayStation(R)3 Programmer Tool Runtime Library 400.001
 *  Copyright (C) 2007 Sony Computer Entertainment Inc.
 *  All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/process.h>
#include <cell/sysmodule.h>

// libgcm
#include <cell/gcm.h>
#include <sysutil/sysutil_sysparam.h>

// libdbgfont
#include <cell/dbgfont.h>

// using vectormath
#include <vectormath/cpp/vectormath_aos.h>
using namespace Vectormath::Aos;

// need padutil
#include "padutil.h"

#include "main.h"

using namespace cell::Gcm;


// for debug
#ifdef NDEBUG
#define ASSERT(x)
static void DPRINTF(const char* format, ...){
	(void) format;
}
#else
#define ASSERT(x) assert(x)
static void DPRINTF(const char* format, ...){
	va_list marker;
	va_start(marker,format);
	vfprintf(stderr, format, marker);
	va_end(marker);
}
#endif



extern uint8_t _binary_vs_simple_vpo_start[];
extern uint8_t _binary_fs_simple_fpo_start[];

namespace{
	Shader_t sSimpleShader = {0, 0, 0, 0};

	bool sKeepRunning = false;

	// pad data
	uint32_t sButtonData = 0;
	CellPadUtilAxis sAxisLeft = {0x80, 0x80};
	CellPadUtilAxis sAxisRight = {0x80, 0x80};
	CellPadUtilPress sPadPressData = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	CellPadUtilSensor sSensorValue = {0x0200, 0x0200, 0x0200, 0x0200};
	CellPadUtilSensor sFilterdSensorValue = {0x0200, 0x0200, 0x0200, 0x0200};

	// scene param
	bool sEditMode = false;
	uint32_t sActiveMenu = 0;
	uint32_t sActiveFilter = 0;
	uint32_t sTargetPad = 0;
	uint32_t sControlPad = 0;
}

static void sysutil_exit_callback(uint64_t status, uint64_t param, void* userdata)
{
	(void) param;
	(void) userdata;

	switch(status) {
	case CELL_SYSUTIL_REQUEST_EXITGAME:
		sKeepRunning = false;	
		break;
	case CELL_SYSUTIL_DRAWING_BEGIN:
	case CELL_SYSUTIL_DRAWING_END:
		break;
	default:
		break;
	}
}

void draw_quad(float x, float y, float z, float width, float height, uint32_t color)
{
	CellVideoOutResolution resolution = get_resolution();

	Viewport_t vp;
	vp.x = 0;
	vp.y = 0;
	vp.width= resolution.width ;
	vp.height= resolution.height;
	vp.min = 0.0f;
	vp.max = 1.0f;
	vp.scale[0] = vp.width;
	vp.scale[1] = vp.height;
	vp.scale[2] = (vp.max - vp.min) * 0.5f;
	vp.scale[3] = 0.0f;
	vp.offset[0] = 0.0f;
	vp.offset[1] = 0.0f;
	vp.offset[2] = (vp.max + vp.min) * 0.5f;
	vp.offset[3] = 0.0f;
	cellGcmSetViewport(vp.x, vp.y, vp.width, vp.height, vp.min, vp.max, vp.scale, vp.offset);
	
	const uint32_t VTX_NUMBER = 4;
	struct MyVertex_t{
		float x, y, z;
		uint32_t color;
	};

	// setup vertex attribute
	const uint32_t ATTR_POSITION = CG_ATTR0 - CG_ATTR0; // vertex position (float4)
	const uint32_t ATTR_DIFFUSE  = CG_ATTR3 - CG_ATTR0; // vertex color (float4)

	cellGcmSetVertexDataArray(ATTR_POSITION, 0, sizeof(MyVertex_t), 3, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	cellGcmSetVertexDataArray(ATTR_DIFFUSE,  0, sizeof(MyVertex_t), 4, CELL_GCM_VERTEX_UB, CELL_GCM_LOCATION_LOCAL, 0);

	// set vertex
	MyVertex_t vtx[VTX_NUMBER] = {
		{ x, y, z, color},
		{ x + width, y, z, color},
		{ x + width, y + height, z, color},
		{ x, y + height, z, color}
	};

	// draw
	const int VTX_WORD_SIZE = sizeof(MyVertex_t) / sizeof(float);
	cellGcmSetDrawInlineArray(CELL_GCM_PRIMITIVE_QUADS, VTX_NUMBER * VTX_WORD_SIZE, vtx);

	// invalidate vertex attribute
	cellGcmSetVertexDataArray(ATTR_POSITION, 0, 0, 0, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	cellGcmSetVertexDataArray(ATTR_DIFFUSE,  0, 0, 0, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);

	// restore viewport
	Viewport_t def_vp = get_default_viewport();
	cellGcmSetViewport(def_vp.x, def_vp.y, def_vp.width, def_vp.height, def_vp.min, def_vp.max, def_vp.scale, def_vp.offset);
}

void draw_pointer(uint32_t x, uint32_t y, uint32_t size, float value_x, float value_y)
{
	float aspect = get_display_aspect_ratio();
	float sx = FONT_MARGIN_X + FONT_WIDTH * x;
	float sy = FONT_MARGIN_Y + FONT_HEIGHT * y;
	float h = FONT_HEIGHT * size;
	float w = h / aspect;
	
	const float point_size = 0.05f;
	float px = (w * (1.0f - point_size)) * value_x;
	float py = (h * (1.0f - point_size)) * value_y;

	draw_quad(sx, sy, 0.5f, w, h, 0x777777ff);
	draw_quad(sx + px, sy + py, 0.2f, w * point_size, h * point_size, 0xffffffff);
}

void draw_meter(const char* label, uint32_t x, uint32_t y, float value)
{
	uint32_t len = strlen(label);
	dbgfont_puts(label, x, y, 1.0f, 0xffffffff, TEXT_LEFT);

	const float meter_width = 10.0f;
	const float meter_height = 0.8f;

	float sx = FONT_MARGIN_X + FONT_WIDTH * (x + len);
	float sy = FONT_MARGIN_Y + FONT_HEIGHT * y + FONT_HEIGHT * ((1.0f - meter_height) / 2.0f);
	float w = FONT_WIDTH * meter_width;
	float h = FONT_HEIGHT * meter_height;

	draw_quad(sx, sy, 0.5f, w, h, 0x777777ff);
	draw_quad(sx, sy, 0.2f, w * value, h, 0xffffffff);
}


bool on_init(void)
{
	// init pad
	cellPadUtilPadInit();
	cellPadUtilSetSensorMode(true);
	cellPadUtilSetPressMode(true);
	cellPadUtilSetSensorFilter(CELL_PADFILTER_IIR_CUTOFF_2ND_LPF_BT_010);

	// init dbgfont
	const int32_t word_count = 32 * 80;
	if(init_dbgfont(word_count) == false){
		return false;
	}

	return true;
}

bool on_update(void)
{
	if(!cellPadUtilUpdate()) return true;

	//ボタンを押すと終了する
	if(cellPadUtilButtonPressed(sControlPad, CELL_UTIL_BUTTON_R1))
	{
		if(cellPadUtilButtonPressed(sControlPad, CELL_UTIL_BUTTON_L1))
		{
			if(cellPadUtilButtonPressed(sControlPad, CELL_UTIL_BUTTON_SELECT))
			{
				if(cellPadUtilButtonPressed(sControlPad,CELL_UTIL_BUTTON_START))
				{exit(1);}
			}
		}
	}
	//ボタンを押すと終了する

	
	
	return true;
}

bool on_draw(void)
{
	setup_surface();

	// set shader
	cellGcmSetVertexProgram(sSimpleShader.vertex_program, sSimpleShader.vp_ucode);
	cellGcmSetFragmentProgram(sSimpleShader.fragment_program, sSimpleShader.fp_offset);

	uint32_t line = 0;

	dbgfont_puts("PadUtil Sample", 0, line, 2.0f, 0xfff00000, TEXT_LEFT);
	line += 2;

	dbgfont_puts("[Menu]", 0, ++line, 1.0f, 0xffffffff, TEXT_LEFT);
	if(!sEditMode){
		dbgfont_puts("Push SELECT button", 7, line, 1.0f, 0xff3333ff, TEXT_LEFT);
	}

	dbgfont_puts("Pad:", 2, ++line, 1.0f, (sEditMode && sActiveMenu == 0 ? 0xff3333ff: 0xffffffff), TEXT_LEFT);
	for(uint32_t i = 0; i < CELL_UTIL_PAD_MAX; ++i){
		uint32_t color = 0xff777777;

		if(cellPadUtilIsConnected(i)){
			if(sEditMode && sTargetPad == i){
				color = 0xff3333ff;
			}else if(sControlPad == i){
				color = 0xff33ff33;
			}else{
				color = 0xffffffff;
			}
		}
		dbgfont_printf(8 + i * 2, line, 1.0f, color, TEXT_LEFT, "%d", i + 1);
	}

	dbgfont_printf(2, ++line, 1.0f, (sEditMode && sActiveMenu == 1 ? 0xff3333ff: 0xffffffff), TEXT_LEFT, "Press Mode: %s",
		(cellPadUtilGetPressMode() ? "ON": "OFF")
	);

	dbgfont_printf(2, ++line, 1.0f, (sEditMode && sActiveMenu == 2 ? 0xff3333ff: 0xffffffff), TEXT_LEFT, "Sensor Mode: %s",
		(cellPadUtilGetSensorMode() ? "ON": "OFF")
	);

	const char *filter_name_list[] = {
        "10% Cut Off", 
		"20% Cut Off", 
		"50% Cut Off", 
	};
	dbgfont_printf(2, ++line, 1.0f, (sEditMode && sActiveMenu == 3 ? 0xff3333ff: 0xffffffff), TEXT_LEFT, "Sensor Filter: %s",
		filter_name_list[sActiveFilter]
	);

	dbgfont_printf(2, ++line, 1.0f, (sEditMode && sActiveMenu == 4 ? 0xff3333ff: 0xffffffff), TEXT_LEFT, "Actuator Mode: %s",
		(cellPadUtilGetActuatorMode() ? "ON (Push R1,R2)": "OFF")
	);

	++line;

	if(cellPadUtilIsConnected(sTargetPad)){
        dbgfont_printf(0, ++line, 1.0f, 0xffffffff, TEXT_LEFT, "[Pad Data] target = %d", sTargetPad + 1);
	}else{
        dbgfont_puts("[Pad Data]", 0, ++line, 1.0f, 0xffffffff, TEXT_LEFT);
	}

	++line;
	dbgfont_puts("Capability: ", 2, line, 1.0f, 0xffffffff, TEXT_LEFT);
	dbgfont_printf(14, line, 1.0f, 0xffffffff, TEXT_LEFT, "%3s %5s %6s %8s %8s",
		(cellPadUtilIsPs3Conformity(sTargetPad) ? "PS3": ""),
		(cellPadUtilIsSupportedPressMode(sTargetPad) ? "Press": ""),
		(cellPadUtilIsSupportedSensorMode(sTargetPad) ? "Sensor": ""),
		(cellPadUtilIsSupportedHpAnalogStick(sTargetPad) ? "HpAnalog": ""),
		(cellPadUtilIsSupportedActuator(sTargetPad) ? "Actuator": "")
	);
	dbgfont_printf(14, line, 1.0f, 0xff777777, TEXT_LEFT, "%3s %5s %6s %8s %8s",
		(!cellPadUtilIsPs3Conformity(sTargetPad) ? "PS3": ""),
		(!cellPadUtilIsSupportedPressMode(sTargetPad) ? "Press": ""),
		(!cellPadUtilIsSupportedSensorMode(sTargetPad) ? "Sensor": ""),
		(!cellPadUtilIsSupportedHpAnalogStick(sTargetPad) ? "HpAnalog": ""),
		(!cellPadUtilIsSupportedActuator(sTargetPad) ? "Actuator": "")
	);

	
	dbgfont_puts("Status: ", 2, ++line, 1.0f, 0xffffffff, TEXT_LEFT);
	
	dbgfont_printf(6, ++line, 1.0f, 0xff777777, TEXT_LEFT, "L1 L2 L3 R1 R2 R3 Select Start");
	dbgfont_printf(6,   line, 1.0f, 0xffffffff, TEXT_LEFT, "%2s %2s %2s %2s %2s %2s %6s %5s",
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_L1) ? "L1": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_L2) ? "L2": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_L3) ? "L3": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_R1) ? "R1": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_R2) ? "R2": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_R3) ? "R3": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_SELECT) ? "Select": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_START) ? "Start": "")
	);

	dbgfont_printf(6, ++line, 1.0f, 0xff777777, TEXT_LEFT, "Up Down Left Right Circle Cross Square Triangle");
	dbgfont_printf(6,   line, 1.0f, 0xffffffff, TEXT_LEFT, "%2s %4s %4s %5s %6s %5s %6s %8s",
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_UP) ? "Up": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_DOWN) ? "Down": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_LEFT) ? "Left": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_RIGHT) ? "Right": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_CIRCLE) ? "Circle": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_CROSS) ? "Cross": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_SQUARE) ? "Square": ""),
		(cellPadUtilDigitalButtonPressed(sButtonData, CELL_UTIL_BUTTON_TRIANGLE) ? "Triangle": "")
	);

	// draw analog values
	line += 2;
	draw_pointer(6, line, 3, sAxisLeft.x / 255.0f, sAxisLeft.y / 255.0f);
	draw_pointer(12, line, 3, sAxisRight.x / 255.0f, sAxisRight.y / 255.0f);
	line += 4;

	// draw press values
	dbgfont_puts("Press:", 2, line, 1.0f, 0xffffffff, TEXT_LEFT);
	draw_meter("L1:",  4, line + 1, sPadPressData.L1 / 255.0f);
	draw_meter("L2:",  4, line + 2, sPadPressData.L2 / 255.0f);
	draw_meter("R1:", 19, line + 1, sPadPressData.R1 / 255.0f);
	draw_meter("R2:", 19, line + 2, sPadPressData.R2 / 255.0f);
	draw_meter("U :",  4, line + 3, sPadPressData.up / 255.0f);
	draw_meter("R :",  4, line + 4, sPadPressData.right / 255.0f);
	draw_meter("D :", 19, line + 3, sPadPressData.down / 255.0f);
	draw_meter("L :", 19, line + 4, sPadPressData.left / 255.0f);
	draw_meter("Ci:",  4, line + 5, sPadPressData.circle / 255.0f);
	draw_meter("Cr:",  4, line + 6, sPadPressData.cross / 255.0f);
	draw_meter("Sq:", 19, line + 5, sPadPressData.square / 255.0f);
	draw_meter("Tr:", 19, line + 6, sPadPressData.triangle / 255.0f);

	// draw sensor values
	dbgfont_puts("Sensor:", 34, line, 1.0f, 0xffffffff, TEXT_LEFT);
	draw_meter("X:", 36, line + 1, sSensorValue.x / 1023.0f);
	draw_meter("Y:", 36, line + 2, sSensorValue.y / 1023.0f);
	draw_meter("Z:", 36, line + 3, sSensorValue.z / 1023.0f);
	draw_meter("G:", 36, line + 4, sSensorValue.g / 1023.0f);

	// draw filtered sensor Values
	dbgfont_puts("Filtered:", 49, line, 1.0f, 0xffffffff, TEXT_LEFT);
	draw_meter("X:", 51, line + 1, sFilterdSensorValue.x / 1023.0f);
	draw_meter("Y:", 51, line + 2, sFilterdSensorValue.y / 1023.0f);
	draw_meter("Z:", 51, line + 3, sFilterdSensorValue.z / 1023.0f);
	draw_meter("G:", 51, line + 4, sFilterdSensorValue.g / 1023.0f);

	// draw dbgfont
	cellDbgFontDrawGcm();

	flip();

	return true;
}

bool on_finish(void)
{
	// end dbgfont
	end_dbgfont();

	// end pad
	cellPadUtilPadEnd();

	return true;
}


SYS_PROCESS_PARAM(1001, 0x10000);


int main()
{

	if(!init_gcm()){
		DPRINTF("failed: init_gcm\n");
		return -1;
	}

	if(!init_display()){
		DPRINTF("failed: init_display\n");
		return -1;
	}

	if(!init_shader(_binary_vs_simple_vpo_start, _binary_fs_simple_fpo_start, &sSimpleShader)){
		DPRINTF("failed: init_shader\n");
		return -1;
	}

	// setup exit callback
	cellSysutilRegisterCallback(0, sysutil_exit_callback, NULL);

	if(!on_init()){
		DPRINTF("failed: on_init\n");
		return -1;
	}

	sKeepRunning = true;
	while(sKeepRunning){
		cellSysutilCheckCallback();

		//ボタン処理
		on_update();
		//ボタン処理

		//文字書き込み
		on_draw();
		//文字書き込み
	}

	on_finish();
	finish_gcm();

	return 0;
}



/*
int main()
{

	cellSysutilRegisterCallback(0, sysutil_exit_callback, NULL);
	return 0;
}
*/