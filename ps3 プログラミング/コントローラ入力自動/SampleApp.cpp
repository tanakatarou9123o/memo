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

//-----------------------------------------------------------------------------
// Description: Constructor
// Parameters: 
// Returns:
// Notes: 
//-----------------------------------------------------------------------------
SampleApp::SampleApp()
{
	
}

// Description: Initialization callback
// Parameters: 
// Returns:
// Notes: 
//-----------------------------------------------------------------------------
bool SampleApp::onInit(int argc, char **ppArgv)
{
    //FWGCMCamControlApplication::onInit(argc, ppArgv);
    mFrameCount = 0;

    //handle = cellPadLddRegisterController();
    //if (handle<0)
    //{
    //    printf("### LDD device add faile %08X\n", handle);
    //    return(handle);
    //}
    //printf("### Added LDD device handle %d\n", handle);

    return true;
}

//-----------------------------------------------------------------------------
// Description: Render callback
// Parameters: 
// Returns:
// Notes: 
//-----------------------------------------------------------------------------
void SampleApp::onRender()
{
    // base implementation clears screen and sets up camera
   // FWGCMCamControlApplication::onRender();

    FWTimeVal   time = FWTime::getCurrentTime();
    float fFPS = 1.f / (float)(time - mLastTime);
    mLastTime = time;
    mFrameCount++;

    FWDebugFont::setPosition(0, 32);
    FWDebugFont::setColor(1.f, 1.f, 1.f, 1.f);
    FWDebugFont::printf("oh\n");
    FWDebugFont::printf("FRAME:%d  FPS: %.2f\n", mFrameCount, fFPS);

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
        //data.button[0] |= CELL_PAD_CTRL_LDD_PS;
    }
    if(frame == 200)
    {
        /* Release PS BUTTON */
        //data.button[0] &= ~CELL_PAD_CTRL_LDD_PS;
    }

    /* cursor move up */
    if(frame > 300 && frame<= 400)
    {
        if (frame%60 < 30)
        {
            /* Press */
            data.button[2] = CELL_PAD_CTRL_DOWN;
        }
        else
        {
            /* Release */
            data.button[2] = 0x00;
        }
    }

    /* cursor move down */
    if(frame > 500 && frame<= 600)
    {
        if (frame%60 < 30)
        {
            /* Press */
            data.button[2] = CELL_PAD_CTRL_UP;
        }
        else
        {
            /* Release */
            data.button[2] = 0x00;
        }
    }
	

    /* CLSOE DIALOG */
    if(frame == 800)
    {
        /* Press PS BUTTON */
        //data.button[0] |= CELL_PAD_CTRL_LDD_PS;
    }
    if(frame == 804)
    {
        /* Release PS BUTTON */
        //data.button[0] &= ~CELL_PAD_CTRL_LDD_PS;
    }

    //cellPadLddDataInsert(handle, &data);

}


//-----------------------------------------------------------------------------
// Description: Resize callback
// Parameters: 
// Returns:
// Notes: 
//-----------------------------------------------------------------------------
void SampleApp::onSize(const FWDisplayInfo& rDispInfo)
{
    //FWGCMCamControlApplication::onSize(rDispInfo);
}

//-----------------------------------------------------------------------------
// Description: Shutdown callback
// Parameters: 
// Returns:
// Notes: 
//-----------------------------------------------------------------------------
void SampleApp::onShutdown()
{
    //FWGCMCamControlApplication::onShutdown();

    if (handle>=0)
    {
       // printf("### Removed LDD device handle %d\n", handle);
       // cellPadLddUnregisterController(handle);
    }
}
