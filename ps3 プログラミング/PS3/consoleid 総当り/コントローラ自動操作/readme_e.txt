   SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 400.001
   Copyright (C) 2007 Sony Computer Entertainment Inc.
   All Rights Reserved.


Custom Controller Sample


<Description>
    This sample program uses the Controller library (libpad) and
    shows how to operate the system dialog with a custom controller.

<Files>
        SampleApp.cpp   Custom controller sample

<Operation Instructions>
    When the sample is launched, the following sequence of operations 
    will be executed according to the time scale.

    1. Notify the PS button press to the system and open the system dialog
    2. Send the down directional key
    3. Send the up directional key
    4. Notify the PS button press and close the system dialog
    5. Go to Step 1 (and repeat)

