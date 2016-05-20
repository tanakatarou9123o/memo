  SCE CONFIDENTIAL                                      
  PlayStation(R)3 Programmer Tool Runtime Library 400.001 
  Copyright (C) 2009 Sony Computer Entertainment Inc.   
  All Rights Reserved.                                  


<Description of the Sample>
    This sample is provided to intorduce and demonstrate the
    NP Clans library.

<Files>
    main.c                : Main process of sample
    np_gui.cpp            : Processing including callback results
    np_gui.h              : Main header of sample
    np_gui_draw.h         : Rendering definitions for sample
    strings.h             : String definitions
    np_top.cpp            : Top screen processing
    np_top_pad.cpp        : Top screen pad processing
    netstart_dialog.cpp   : Netstart dialog processing
    osk_dialog.cpp        : OSK dialog processing
    np_clans.cpp          : Clan mode main processing
    np_clans_draw.cpp     : Clan mode rendering processing
    np_clans_pad.cpp      : Clan mode pad processing
    np_gui_async_pad.cpp  : Suspension processing while calling the block 
                            API

<How to Run the Sample>
    When the sample is started up, the Top screen will appear.

    The behavior of the Circle button and Cross button in this sample 
    corresponds to the button assignment of the system. In the
    following explanation, the button assigned in the system as
    the Enter button will be called the Enter button, and the
    other button will be called the Back button.

<Top Screen>
    Select "CLAN MODE".

    <Operation>
    
    Up, Down buttons : Move cursor (select mode)
    Enter button     : Enter

    To transition from the Top screen to the different modes, it is
    necessary for NP sign-in to be complete and for the NP status 
    to be Online. If the NP status is not Online, the Network Start 
    Dialog utility will start up and execute the NP sign-in.

<CLANS MODE>

        The clan mode is consisted of 2 operation screens such as "Clan 
        Selection Screen" and the "Clan Operation Screen". On the "Clan 
        Selection Screen", search for and creation of a clan can be 
        performed. When a clan is selected from the search result, it 
        transits to the "Clan Operation Screen" on which a processing can 
        be performed for the selected clan. It is possible to return back 
        to the "Clan Selection Screen" by pressing the cancel button on 
        the "Clan Operation Screen".

        <Clan Selection Screen>

                The following operations can be selected on the "Clan 
                Selection Screen".

                The results of each operation are listed in the right most
                column.
                When a clan is selected and the Enter button is pressed, 
                it transits to the "Clan Operation Screen".

                CREATE CLAN
                        Create a new clan
                        The list is not updated by this operation.
                GET CLAN LIST
                        Obtain a clan associated with this user
                        When TOTAL is executed, it obtains only the total 
                        count.
                        When GET is executed, the list is updated.
                GET CLAN LIST BY NPID
                        Obtain a clan associated with other user of the 
                        specified NP ID
                        Basically, the behavior is same as GET CLAN LIST.
                SEARCH BY PROFILE
                        Search a clan based on the clan profile
                        The search result includes a clan that is not 
                        associated with the user.
                        Search operators are displayed immediately under 
                        each parameter.
                        When you do not want to specify a search operator 
                        to each parameter, specify "---".
                        When TOTAL is executed, it obtains only the total 
                        count.
                        When SEARCH is executed, the list is updated.
                SEARCH BY NAME
                        Search a clan based on the clan name
                        The search result includes a clan that is not 
                        associated with the user.
                        Search operators are displayed immediately under 
                        each parameter.
                        When TOTAL is executed, it obtains only the total 
                        count.
                        When SEARCH is executed, the list is updated.

        <Clan Operation Screen>

                The following operations can be performed for the clan 
                that is selected on the "Clan Selection Screen":

                DISBAND
                        Release the selected clan
                GET CLAN INFO
                        Display the information for the selected clan
                UPDATE CLAN INFO
                        Update the information for the selected clan
                        Enter each parameter and execute UPDATE
                GET AUTO ACCEPT STATUS
                        Display the auto-accept status for the selected clan
                UPDATE AUTO ACCEPT STATUS
                        Update the auto-accept status for the selected clan
                        Enter the parameter and execute UPDATE
                GET MEMBER LIST
                        Obtain a member list for the selected clan
                        The result is displayed in the right most column.
                        When TOTAL is executed, it obtains only the total 
                        count.
                        When GET is executed, the list is updated.
                GET MEMBER INFO
                        Obtain the detailed member information of the 
                        selected clan of the specified NP ID
                UPDATE MEMBER INFO
                        Update the information of the user of the selected
                        clan
                        Enter each parameter and execute UPDATE
                CHANGE MEMBER ROLE
                        Change the ROLE of the member of the specified 
                        NP ID for the selected clan
                JOIN CLAN
                        Join the selected clan
                LEAVE CLAN
                        Leave from the selected clan
                SEND INVITATION REQ
                        Send an invitation message of the selected clan to
                        a user of the specified NP ID
                CANCEL INVITATION
                        Cancel the invitation message that is sent to the 
                        user of the specified NP ID
                SEND INVITATION RES
                        Send a response to the invitation message sent to 
                        the user
                SEND MEMBERSHIP REQ
                        Send a membership request message to the selected 
                        clan
                CANCEL MEMBERSHIP
                        Cancel the membership request message sent to the 
                        selected clan                        
                SEND MEMBERSHIP RES
                        Send a response to the membership request message 
                        sent to the clan associated with the user
                GET BLACKLIST
                        Obtain a black list for the selected clan
                        When TOTAL is executed, it obtains only the total 
                        count.
                        When GET is executed, the list is updated.
                ADD BLACKLIST
                        Add a user of the specified NP ID to the black 
                        list of the selected clan
                REMOVE BLACKLIST ENTRY
                        Delete a user of the specified NP ID from the 
                        black list of the selected clan
                RETRIEVE ANNOUNCEMENTS
                        Obtain an announcement of the selected clan
                        When TOTAL is executed, it obtains only the total 
                        count.
                        When RETRIEVE is executed, the list is updated.
                POST ANNOUNCEMENTS
                        Send an announcement to the selected clan
                REMOVE ANNOUNCEMENTS
                        Delete an announcement of the specified 
                        ANNOUNCEMENTS ID from the selected clan
                POST CHALLENGE
                        Send a clan challenge from the selected clan
                        to a target clan
                RETRIEVE POSTED CHALLENGES
                        Obtain challenges posted by the selected clan
                        When TOTAL is executed, it obtains only the total 
                        count.
                        When RETRIEVE is executed, the list is updated.
                REMOVE POSTED CHALLENGE
                        Delete clan challenge of the specified 
                        CHALLENGE ID from the selected clan to the
                        specified target
                RETRIEVE CHALLENGES
                        Obtain challenges posted to the selected clan
                        When TOTAL is executed, it obtains only the total 
                        count.
                        When RETRIEVE is executed, the list is updated.
                REMOVE CHALLENGE
                        Delete clan challenge of the specified 
                        CHALLENGE ID for the selected clan
