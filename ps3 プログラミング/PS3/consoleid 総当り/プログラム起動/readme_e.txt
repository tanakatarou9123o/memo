   SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 400.001
   Copyright (C) 2007 Sony Computer Entertainment Inc.
   All Rights Reserved.


Sample of Activating a Program from the Host PC


<Description of the Sample>
    This program is a sample program that demonstrates how to activate 
    a program that is located on the host PC that is connected to 
    the Debugging Station (/app_home/EBOOT.BIN). 

<Files>
    main.cpp        : Source file of the sample of activating a program 
                      from the host PC
    main.h          : Header file of the sample of activating a program 
                      from the host PC
    launcher.gp3    : Project file of the Disc Image Generator for 
                      PLAYSTATION(R)3
    launcher.conf   : NPDRM PACKAGE setting file
    PS3_GAME/       : Directory to which the package data will be stored

<How to Operate>
    When the program is activated, it will be checked whether 
    the file /app_home/EBOOT.BIN is accessible, and if the file exists, 
    the program will be activated with exitspawn(...) 
    after waiting for three seconds. 
    In order to access /app_home of the host PC, it is necessary that 
    dtnetm/dtcfilesv of the host PC, or ProDG Target Manager is connected 
    to the Debugging Station. 

    If the EBOOT.BIN file does not exist or cannot be accessed, 
    the IP address information of the Debugging Station and the IP address 
    of the host PC that is connected with dtnetm will be displayed, 
    and the sample program will wait for the file to become accessible. 

<launcher_args.txt>
    When the program is activated, a file called 
    /app_home/launcher_args.txt is read.
    Specify the filename of the boot program and the arguments to be passed
    at the boot timing, with spaces in between the specifications, in 
    launcher_args.txt.
    If a space is included in the filename or boot argument, encase that
    character string in quotation marks "".

    Example 1: Activate /app_home/EBOOT.BIN and pass a and b as boot 
               arguments.
    launcher_args.txt:
      EBOOT.BIN a b

    Example 2: Activate /app_home/myprogram.self and pass "a b" and c as 
               boot arguments.
    launcher_args.txt:
      myprogram.self "a b" c

<How to Generate a Disc Image>
    Execute $ make gen_image, open launcher.gp3 with the Disc Image 
    Generator for PLAYSTATION(R)3, and a disc image will be generated. 
    The generated DVD-SL ISO image is in: 
      $CELL_SDK/sample_data/util/debug/launcher/laucher-DVD-SL.iso.zip

<How to Generate NPDRM PACKAGE>
    When $ make npdrm_package is executed, a package file named 
    UP0001-SXXX00001_00-0000111122223333.pkg will be generated. 
    This package file is used for installing this program to the HDD 
    of the Debugging Station. 
    This generated NPDRM PACKAGE can be found at
      $CELL_SDK/sample_data/util/debug/launcher/UP0001-SXXX00001_00-0000111122223333.pkg

