/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 400.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __SAMPLEAPP_H__
#define __SAMPLEAPP_H__

#include <string>
#include "FWGCMCamControlApplication.h"

class SampleApp : public FWGCMCamControlApplication
{
public:
                    SampleApp();
    virtual bool    onInit(int argc, char **ppArgv);
    virtual void    onRender();
    virtual void    onShutdown();
    virtual void    onSize(const FWDisplayInfo & dispInfo);

private:
    FWTimeVal mLastTime;
    uint32_t mFrameCount;
    int32_t handle;
};

#endif // __SAMPLEAPP_H__
