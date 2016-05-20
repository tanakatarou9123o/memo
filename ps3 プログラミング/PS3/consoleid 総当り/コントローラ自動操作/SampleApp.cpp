/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 400.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#include <string>
#include <stdio.h>
#include <assert.h>
#include <sys/sys_time.h>
#include <sys/time_util.h>

#include "FWDebugFont.h"
#include "SampleApp.h"
#include "FWTime.h"

// instantiate the class
SampleApp app;

SampleApp::SampleApp()
{
}

bool SampleApp::onInit(int argc, char **ppArgv)
{
    FWGCMCamControlApplication::onInit(argc, ppArgv);
    mFrameCount = 0;

    handle = cellPadLddRegisterController();
    

    return true;
}


void SampleApp::onRender()
{
       
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



    /* OPEN SYSTEM UTILITY DIALOG */
    uint32_t frame = mFrameCount%1100;
    if(frame == 180)
    {
        /* Press PS BUTTON */
        data.button[0] |= CELL_PAD_CTRL_LDD_PS;
    }
    if(frame == 200)
    {
        /* Release PS BUTTON */
        data.button[0] &= ~CELL_PAD_CTRL_LDD_PS;
    }


	if(frame == 400)
    {
   	/* Press */
    	data.button[3] = CELL_PAD_CTRL_CIRCLE;	//maru
    }

	if(frame == 440)	
    {	
    /* Release */
    data.button[3] = 0x00; 
    }
    

    /* CLSOE DIALOG */
    if(frame == 800)
    {
        /* Press PS BUTTON */
        data.button[0] |= CELL_PAD_CTRL_LDD_PS;
    }
    if(frame == 804)
    {
        /* Release PS BUTTON */
        data.button[0] &= ~CELL_PAD_CTRL_LDD_PS;
    }

    cellPadLddDataInsert(handle, &data);

}



void SampleApp::onSize(const FWDisplayInfo& rDispInfo)
{
}
void SampleApp::onShutdown()
{
}
