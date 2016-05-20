/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_NP_SAMPLE_NP_GUI_H_
#define _SCE_NP_SAMPLE_NP_GUI_H_

/*
 * for DEBUG
 */
#ifdef NP_GUI_DEBUG
#include <stdio.h>
#define ERR(format, arg...) \
	printf(format, ## arg)
#define INFO(format, arg...) \
	printf(format, ## arg)
#define DBG(format, arg...) \
	printf(format, ## arg)
#else
#define ERR(format, arg...) (void)0
#define INFO(format, arg...) (void)0
#define DBG(format, arg...) (void)0
#endif

#include <cell/dbgfont.h>
#include <sys/ppu_thread.h>
#include <np.h>
#include "np_gui_draw.h"

enum np_gui_mode {
	NP_GUI_MODE_TOP = 0,
	NP_GUI_MODE_NETSTART,
	NP_GUI_MODE_CLANS,
	NP_GUI_MODE_MAX
};

enum np_gui_netstart_state {
	NP_GUI_NETSTART_STATE_TOP = 0,
	NP_GUI_NETSTART_STATE_RUNNING,
	NP_GUI_NETSTART_STATE_UNLOADING,
	NP_GUI_NETSTART_STATE_UNLOADED
};

enum np_gui_osk_input {
	NP_GUI_OSK_INPUT_INVALID = 0,
	NP_GUI_OSK_INPUT_CREATE_CLAN_NAME,
	NP_GUI_OSK_INPUT_CREATE_CLAN_TAG,
	NP_GUI_OSK_INPUT_GET_CLAN_LIST_BY_NPID_NPID,
	NP_GUI_OSK_INPUT_SEARCH_BIN_ATTR,
	NP_GUI_OSK_INPUT_SEARCH_TAG,
	NP_GUI_OSK_INPUT_SEARCH_NAME,
	NP_GUI_OSK_INPUT_UPDATE_BIN_ATTR,
	NP_GUI_OSK_INPUT_UPDATE_BIN_DATA,
	NP_GUI_OSK_INPUT_UPDATE_DESC,
	NP_GUI_OSK_INPUT_SEND_INVITATION_NPID,
	NP_GUI_OSK_INPUT_MESSAGE_SUBJECT,
	NP_GUI_OSK_INPUT_MESSAGE_BODY,
	NP_GUI_OSK_INPUT_ADD_BLACKLIST_NPID,
	NP_GUI_OSK_INPUT_REMOVE_BLACKLIST_NPID,
	NP_GUI_OSK_INPUT_GET_MEMBER_INFO_ONLINEID,
	NP_GUI_OSK_INPUT_UPDATE_MEMBER_INFO_DESC,
	NP_GUI_OSK_INPUT_UPDATE_MEMBER_INFO_BIN_ATTR,
	NP_GUI_OSK_INPUT_CHANGE_MEMBER_ROLE_ONLINEID,
	NP_GUI_OSK_INPUT_SEND_MEMBERSHIP_NPID,
	NP_GUI_OSK_INPUT_KICK_MEMBER_NPID,
	NP_GUI_OSK_INPUT_KICK_MEMBER_SUBJECT,
	NP_GUI_OSK_INPUT_KICK_MEMBER_BODY,
	NP_GUI_OSK_INPUT_POST_CHALLENGE_NAME,
	NP_GUI_OSK_INPUT_REMOVE_POSTED_CHALLENGE_NAME,
	NP_GUI_OSK_INPUT_RETRIEVE_POSTED_CHALLENGES_NAME
};


enum np_gui_clans_state {
	NP_GUI_CLANS_STATE_TOP = 0,
	NP_GUI_CLANS_STATE_CLANMODE
};

typedef void (*np_gui_update_func) (void);
typedef void (*np_gui_draw_func) (void);
typedef void (*np_gui_pad_func) (void);

struct np_gui_info {
	uint32_t padd;

	enum np_gui_mode mode;
	enum np_gui_mode mode_after_netstart;
	enum np_gui_netstart_state netstart_state;
	enum np_gui_clans_state clans_state;

	np_gui_update_func pad_func[NP_GUI_MODE_MAX];
	np_gui_update_func draw_func[NP_GUI_MODE_MAX];
	np_gui_update_func update_func[NP_GUI_MODE_MAX];

	int button_assign;
	int pos_x;
	int pos_y;
	int pos_act_y;
	int pos_slot_y;
//	int pos_protocol_y;
	int pos_y_clans_menu_top;
	int flag;
	int progress_count;

	SceNpId npId;
	SceNpPsHandle psHandle;
	CellDbgFontConsoleId mDbgFontID_info;

	/* clans */
	SceNpClansRequestHandle clansHandle;
	char clans_create_clan_name[SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH + 1];
	char clans_create_clan_tag[SCE_NP_CLANS_CLAN_TAG_MAX_LENGTH + 1];

	int clanlist_num;
	SceNpClansPagingRequest getClanList_PagingRequest;
	SceNpClansPagingResult getClanList_PagingResult;
	SceNpClansPagingRequest getClanListByNpId_PagingRequest;
	SceNpOnlineId getClanListByNpId_onlineId;
#define CLANS_CLAN_ENTRY_MAX			(SCE_NP_CLANS_PAGING_REQUEST_PAGE_MAX + 1)
	/*J sceNpClansGetClanList{ByNpId}() で共通 */
	SceNpClansEntry clanList[CLANS_CLAN_ENTRY_MAX];

	int basicInfo_num;
	SceNpClansSearchableProfile searchProf;
	SceNpClansPagingRequest searchByProfile_PagingRequest;
	SceNpClansPagingRequest searchByName_PagingRequest;
	SceNpClansPagingResult search_PagingResult;
#define CLANS_BASICINFO_ENTRY_MAX		(SCE_NP_CLANS_PAGING_REQUEST_PAGE_MAX + 1)
	/*J sceNpClansSearchBy{Profile,Name}() で共通 */
	SceNpClansClanBasicInfo clanBasicInfoList[CLANS_BASICINFO_ENTRY_MAX];
	SceNpClansSearchableName searchName;

	SceNpClanId selectedClanId;

	SceNpClansClanInfo clanInfo;
	SceNpClansUpdatableClanInfo updatableClanInfo;
	bool autoAccept;
	bool haveAutoAcceptStatus;
	bool updateAutoAccept;

	SceNpOnlineId sendInvitation_onlineId;
	bool sendInvitation_censor;
	SceNpClansMessage clansMessage;

	SceNpOnlineId addBlacklistEntry_onlineId;
	SceNpOnlineId removeBlacklistEntry_onlineId;

	int announcementsList_num;
	uint32_t postMessage_duration;
	bool postMessage_censor;
	SceNpClansMessageId messageId;
#define CLANS_AN_ENTRY_MAX				(SCE_NP_CLANS_PAGING_REQUEST_PAGE_MAX + 1)
	SceNpClansMessageEntry messageEntry[CLANS_AN_ENTRY_MAX];
	SceNpClansPagingRequest retrieveAnnouncements_PagingRequest;
	SceNpClansPagingResult retrieveAnnouncements_PagingResult;

	int challengeList_num;
	int postedChallengeList_num;
	uint32_t postChallenge_duration;
	bool postChallenge_censor;
	SceNpClansMessageId challengeId;
	SceNpClansMessageId postedChallengeId;
	char postChallengeClan[SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH+1];
	char removeChallengeClan[SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH+1];
	char retrieveChallengesClan[SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH+1];
#define CLANS_CH_ENTRY_MAX				(SCE_NP_CLANS_PAGING_REQUEST_PAGE_MAX + 1)
	SceNpClansMessageEntry challengeEntry[CLANS_CH_ENTRY_MAX];
	SceNpClansPagingRequest retrieveChallenges_PagingRequest;
	SceNpClansPagingResult retrieveChallenges_PagingResult;

#define CLANS_POSTED_CH_ENTRY_MAX				(SCE_NP_CLANS_PAGING_REQUEST_PAGE_MAX + 1)
	SceNpClansMessageEntry postedChallengeEntry[CLANS_POSTED_CH_ENTRY_MAX];
	SceNpClansPagingRequest retrievePostedChallenges_PagingRequest;
	SceNpClansPagingResult retrievePostedChallenges_PagingResult;

#define CLANS_BLACKLIST_ENTRY_MAX		(SCE_NP_CLANS_PAGING_REQUEST_PAGE_MAX + 1)
	SceNpClansBlacklistEntry blacklistEntry[CLANS_BLACKLIST_ENTRY_MAX];
	SceNpClansPagingRequest getBlacklist_PagingRequest;
	SceNpClansPagingResult getBlacklist_PagingResult;

#define CLANS_MEMNBERLIST_ENTRY_MAX		(SCE_NP_CLANS_PAGING_REQUEST_PAGE_MAX + 1)
	SceNpOnlineId getMemberInfo_onlineId;
	SceNpClansMemberEntry getMemberInfoEntry;
	SceNpClansMemberEntry memberEntry[CLANS_MEMNBERLIST_ENTRY_MAX];
	SceNpClansPagingRequest getMemberList_PagingRequest;
	SceNpClansPagingResult getMemberList_PagingResult;
	SceNpClansMemberStatus memberStatus;

	SceNpOnlineId kickMember_onlineId;
	SceNpClansMessage kickMemberMessage;
	bool kickMember_censor;

	SceNpClansUpdatableMemberInfo updatableMemberInfo;
	SceNpClansRole memberRole;
	SceNpOnlineId changeMemberRole_onlineId;

	bool sendInvitationResponse_accept;
	bool sendInvitationResponse_censor;

	SceNpOnlineId sendMembershipResponse_onlineId;
	bool sendMembershipResponse_accept;
	bool sendMembershipResponse_censor;

	bool sendMembershipRequest_censor;

	int lookupTransId;
};

/*E button[2] */
#define CTRL_SELECT     (1<<0)
#define CTRL_L3         (1<<1)
#define CTRL_R3         (1<<2)
#define CTRL_START      (1<<3)
#define CTRL_UP         (1<<4)
#define CTRL_RIGHT      (1<<5)
#define CTRL_DOWN       (1<<6)
#define CTRL_LEFT       (1<<7)
/*E button[3] */
#define CTRL_L2         (1<<8)
#define CTRL_R2         (1<<9)
#define CTRL_L1         (1<<10)
#define CTRL_R1         (1<<11)
#define CTRL_TRIANGLE   (1<<12)
#define CTRL_CIRCLE     (1<<13)
#define CTRL_CROSS      (1<<14)
#define CTRL_SQUARE     (1<<15)

#define NP_GUI_FLAG_PAD_LOCK				0x01
#define NP_GUI_FLAG_OFFLINE					0x02
#define NP_GUI_FLAG_THREAD_TERM				0x04
#define NP_GUI_FLAG_DISPLAY_FILTER			0x08

/* key position for mode top */
#define NP_GUI_MODE_TOP_POS_Y_MIN			0
#define NP_GUI_MODE_TOP_POS_Y_MODE_CLANS	0
#define NP_GUI_MODE_TOP_POS_Y_MAX			0

/* key position for mode clans / state top */
#define NP_GUI_MODE_CLANS_TOP_POS_X_MIN			0
#define NP_GUI_MODE_CLANS_TOP_POS_X_ACT			0
#define NP_GUI_MODE_CLANS_TOP_POS_X_COND		1
#define NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST	2
#define NP_GUI_MODE_CLANS_TOP_POS_X_MAX			2
/* key position for mode clans/top/action menu */
#define NP_GUI_MODE_CLANS_ACT_POS_Y_MIN						0
#define NP_GUI_MODE_CLANS_ACT_POS_Y_CREATE_CLAN				0
#define NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST			1
#define NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID	2
#define NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE		3
#define NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME			4
#define NP_GUI_MODE_CLANS_ACT_POS_Y_MAX						4
/* key position for mode clans/top/create clan */
#define NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_NAME		0
#define NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_TAG			1
#define NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_CREATE		2
#define NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_MAX			2
/* key position for mode clans/top/get clan list */
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_PAGE_START	0
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_PAGE_MAX		1
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_TOTAL			2
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_GET			3
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_MAX			3
/* key position for mode clans/top/get clan list by npId */
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_NPID			0
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_PAGE_START	1
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_PAGE_MAX		2
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_TOTAL			3
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_GET			4
#define NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_MAX			4
/* key position for mode clans/top/search by profile */
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MIN				0
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT1				0
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT1_OP			1
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT2				2
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT2_OP			3
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT3				4
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT3_OP			5
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_BIN1				6
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_BIN1_OP			7
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MEMBER			8
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MEMBER_OP			9
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TAG				10
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TAG_OP			11
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_PAGE_START		12
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_PAGE_MAX			13
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TOTAL				14
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_SEARCH			15
#define NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MAX				15
/* key position for mode clans/top/search by name */
#define NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_MIN					0
#define NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_NAME					0
#define NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_NAME_OP				1
#define NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_PAGE_START			2
#define NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_PAGE_MAX				3
#define NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_TOTAL				4
#define NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_SEARCH				5
#define NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_MAX					5

/* key position for mode clans / state clanmode */
#define NP_GUI_MODE_CLANS_CLANMODE_POS_X_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_POS_X_ACT		0
#define NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND		1
#define NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST		2
#define NP_GUI_MODE_CLANS_CLANMODE_POS_X_MAX		2
/* key position for mode clans/clanmode/action menu */
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_MIN						0
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND					0
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_INFO					1
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_CLAN_INFO			2
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_AUTO_ACCEPT			3
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_AUTO_ACCEPT			4
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_LIST			5
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_INFO			6
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_MEMBER_INFO			7
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CHANGE_MEMBER_ROLE			8
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_KICK_MEMBER				9
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_JOIN_CLAN					10
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_LEAVE_CLAN					11
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION			12
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CANCEL_INVITATION			13
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION_RESPONSE	14
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_REQUEST	15
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CANCEL_MEMBERSHIP			16
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_RESPONSE	17
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_BLACKLIST				18
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_ADD_BLACKLIST_ENTRY		19
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_BLACKLIST_ENTRY		20
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS		21
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_ANNOUNCEMENTS			22
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_ANNOUNCEMENTS		23
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_CHALLENGE				24
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES	25
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_POSTED_CHALLENGE	26
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES		27
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_CHALLENGE			28
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_MAX						28
#define NP_GUI_MODE_CLANS_CLANMODE_ACT_DISP_MAX							10
/* key position for mode clans/clanmode/delete */
#define NP_GUI_MODE_CLANS_CLANMODE_DISBAND_ACT_POS_Y_MIN				0
#define NP_GUI_MODE_CLANS_CLANMODE_DISBAND_ACT_POS_Y_DISBAND			0
#define NP_GUI_MODE_CLANS_CLANMODE_DISBAND_ACT_POS_Y_MAX				0
/* key position for mode clans/clanmode/update clan info */
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_DESC		0
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT1		1
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT2		2
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT3		3
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_BIN1		4
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_UPDATE	5
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_MAX		5
/* key position for mode clans/clanmode/update auto-accept status */
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_AUTO_ACCEPT	0
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_UPDATE		1
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_MAX			1
/* key position for mode clans/clanmode/send invitation */
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_ONLINEID	0
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_SUBJECT	1
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_BODY		2
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_CENSOR		3
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_SEND		4
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_MAX		4
/* key position for mode clans/clanmode/cancel invitation */
#define NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_ONLINEID	0
#define NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_CANCEL	1
#define NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_MAX		1
/* key position for mode clans/clanmode/send membership request */
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_SUBJECT		0
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_BODY			1
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_CENSOR		2
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_SEND			3
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_MAX			3
/* key position for mode clans/clanmode/add blacklist entry */
#define NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_NPID			0
#define NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_ADD			1
#define NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_MAX			1
/* key position for mode clans/clanmode/remove blacklist entry */
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_NPID			0
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_REMOVE		1
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_MAX			1
/* key position for mode clans/clanmode/post announcement */
#define NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_SUBJECT		0
#define NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_BODY			1
#define NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_DURATION		2
#define NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_CENSOR		3
#define NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_POST			4
#define NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_MAX			4
/* key position for mode clans/clanmode/remove announcement */
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_ID			0
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_REMOVE		1
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_MAX		1
/* key position for mode clans/clanmode/retrieve announcements */
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_PAGE_START	0
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_PAGE_MAX	1
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_TOTAL		2
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_RETRIEVE	3
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_MAX			3
/* key position for mode clans/clanmode/get blacklist */
#define NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_PAGE_START	0
#define NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_PAGE_MAX		1
#define NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_TOTAL		2
#define NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_GET			3
#define NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_MAX			3
/* key position for mode clans/clanmode/get member list */
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_PAGE_START	0
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_PAGE_MAX	1
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_STATUS		2
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_TOTAL		3
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_GET			4
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_MAX			4
/* key position for mode clans/clanmode/get member info */
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_NPID		0
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_GET		1
#define NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_MAX		1
/* key position for mode clans/clanmode/update member info */
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_BIN1		0
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_DESC		1
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_ALLOW_MSG	2
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_UPDATE		3
#define NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_MAX			3
/* key position for mode clans/clanmode/kick member */
#define NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_NPID		0
#define NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_SUBJCET	1
#define NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_BODY		2
#define NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_CENSOR		3
#define NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_KICK		4
#define NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_MAX		4
/* key position for mode clans/clanmode/change member role */
#define NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_NPID		0
#define NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_ROLE		1
#define NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_CHANGE		2
#define NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_MAX			2
/* key position for mode clans/clanmode/send invitation response */
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_SUBJECT	0
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_BODY		1
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_ACCEPT	2
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_CENSOR	3
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_SEND		4
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_MAX		4
/* key position for mode clans/clanmode/send membership response */
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_NPID		0
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_SUBJECT	1
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_BODY		2
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_ACCEPT	3
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_CENSOR	4
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_SEND		5
#define NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_MAX		5
/* key position for mode clans/clanmode/post challenge */
#define NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_CLAN		0
#define NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_SUBJECT		1
#define NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_BODY		2
#define NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_DURATION	3
#define NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_CENSOR		4
#define NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_POST		5
#define NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_MAX			5
/* key position for mode clans/clanmode/remove posted challenge */
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_CLAN		0
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_ID			1
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_REMOVE		2
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_MAX		2
/* key position for mode clans/clanmode/retrieve posted challenge */
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_MIN			0
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_CLAN			0
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_PAGE_START	1
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_PAGE_MAX		2
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_TOTAL		3
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_RETRIEVE		4
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_MAX			4
/* key position for mode clans/clanmode/remove challenge */
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_ID		0
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_REMOVE	1
#define NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_MAX		1
/* key position for mode clans/clanmode/retrieve challenge */
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_MIN		0
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_PAGE_START	0
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_PAGE_MAX	1
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_TOTAL		2
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_RETRIEVE	3
#define NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_MAX		3

#define POP_WINDOW_WIDTH	16
#define POP_WINDOW_HEIGHT	3
#define POP_WINDOW_POS_LEFT	\
	(XSTART + ((1.0f - (MARGIN_LEFT_RIGHT * 2)) - (POP_WINDOW_WIDTH + 1) * DEFAULT_XCHAR_INTERVAL))
#define POP_WINDOW_POS_TOP \
	(YSTART + ((1.0f - (MARGIN_TOP_BOTTOM * 2)) - (POP_WINDOW_HEIGHT + 2) * DEFAULT_YCHAR_INTERVAL))

#define INFO_FONT_SCALE	(DEFAULT_SCALE * 0.8f)
#define INFO_WINDOW_WIDTH	64
#define INFO_WINDOW_HEIGHT	5
#define INFO_WINDOW_POS_LEFT			XSTART
#define INFO_WINDOW_POS_TOP	\
	(LINE_BOTTOM_Y - DEFAULT_YCHAR_INTERVAL - (S_DEFAULT_YCHAR_INTERVAL(INFO_FONT_SCALE) * INFO_WINDOW_HEIGHT))
#define INFO_FONT_COLOR		(DEF_A|COLOR_GREEN)

#define COND_SCALE				(DEFAULT_SCALE * 1.0f)
#define CLANS_ACTION_SCALE		(DEFAULT_SCALE * 0.9f)
#define CLANS_SEARCH_COND_SCALE	(DEFAULT_SCALE * 0.9f)

#define STRING_ROW_CONDITIONS_TOP		6
#define STRING_ROW_CONDITIONS			9

#define STRING_X_CONDITION_LIST		(XSTART + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 26)
#define STRING_X_CONDITION_VALUE	(XSTART + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 42)

#define STRING_X_CLAN_LIST_TITLE	(XSTART + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 50)

/* np_gui.cpp */
void change_mode(enum np_gui_mode mode);
void change_netstart_state(enum np_gui_netstart_state state);
void change_clans_state(enum np_gui_clans_state state);
void check_result(void);
int np_init(void);
int32_t memoryContainerCheck(sys_memory_container_t mc, size_t size);

/* np_top.cpp*/
void top_pad_func(void);
void top_draw_func(void);
void top_update_func(void);

/* np_top_pad.cpp */
void top_pad_circle(void);
void top_pad_cross(void);
void top_pad_down(void);
void top_pad_up(void);

/* netstart_dialog.cpp */
void start_netstart_dialog(void);
void finish_netstart_dialog(void);
void term_netstart_dialog(void);
void netstart_pad_func(void);
void netstart_draw_func(void);
void netstart_update_func(void);

/* osk_dialog.cpp */
void start_osk_dialog(enum np_gui_osk_input input, uint16_t *guide_message, int limit);
void finish_osk_dialog(void);
void term_osk_dialog(void);

/* np_clans.cpp */
void clans_pad_func(void);
void clans_draw_func(void);
void clans_update_func(void);
int doClansCreateClan(void);
int doClansGetClanList(void);
int doClansGetClanListByNpId(void);
int doClansSearchByProfile(void);
int doClansSearchByName(void);
int doClansDisband(void);
int doClansGetClanInfo(void);
int doClansUpdateClanInfo(void);
int doClansGetAutoAcceptStatus(void);
int doClansUpdateAutoAcceptStatus(void);
int doClansKickMember(void);
int doClansJoinClan(void);
int doClansLeaveClan(void);
int doClansSendInvitation(void);
int doClansCancelInvitation(void);
int doClansSendMembershipRequest(void);
int doClansCancelMembershipRequest(void);
int doClansAddBlacklistEntry(void);
int doClansRemoveBlacklistEntry(void);
int doClansPostAnnouncement(void);
int doClansRemoveAnnouncement(void);
int doClansRetrieveAnnouncements(void);
int doClansGetBlacklist(void);
int doClansGetMemberList(void);
int doClansGetMemberInfo(void);
int doClansUpdateMemberInfo(void);
int doClansChangeMemberRole(void);
int doClansSendInvitationResponse(void);
int doClansSendMembershipResponse(void);
int doClansPostChallenge(void);
int doClansRemovePostedChallenge(void);
int doClansRetrievePostedChallenges(void);
int doClansRemoveChallenge(void);
int doClansRetrieveChallenges(void);

/* np_clans_draw.cpp */
void clans_draw_clanlist(void);
void clans_draw_anlist(void);
void clans_draw_chlist(void);
void clans_draw_posted_chlist(void);
void clans_draw_blacklist(void);
void clans_draw_memberlist(void);
void clans_draw_action_menu(void);
void clans_draw_bottom_menu(void);
void clans_draw_create_clan(void);
void clans_draw_get_clan_list(void);
void clans_draw_get_clan_list_by_npid(void);
void clans_draw_search_by_profile(void);
void clans_draw_search_by_name(void);
void clans_draw_clanmode_action_menu(void);
void clans_draw_clanmode_disband(void);
void clans_draw_clanmode_get_info(void);
void clans_draw_clanmode_update_clan_info(void);
void clans_draw_clanmode_get_auto_accept(void);
void clans_draw_clanmode_update_auto_accept(void);
void clans_draw_clanmode_send_invitation(void);
void clans_draw_clanmode_cancel_invitation(void);
void clans_draw_clanmode_send_membership_request(void);
void clans_draw_clanmode_cancel_membership(void);
void clans_draw_clanmode_add_blacklist_entry(void);
void clans_draw_clanmode_remove_blacklist_entry(void);
void clans_draw_clanmode_post_announcement(void);
void clans_draw_clanmode_remove_announcement(void);
void clans_draw_clanmode_retrieve_announcements(void);
void clans_draw_clanmode_post_challenge(void);
void clans_draw_clanmode_remove_posted_challenge(void);
void clans_draw_clanmode_remove_challenge(void);
void clans_draw_clanmode_retrieve_posted_challenges(void);
void clans_draw_clanmode_retrieve_challenges(void);
void clans_draw_clanmode_get_blacklist(void);
void clans_draw_clanmode_get_member_list(void);
void clans_draw_clanmode_get_member_info(void);
void clans_draw_clanmode_update_member_info(void);
void clans_draw_clanmode_change_member_role(void);
void clans_draw_clanmode_kick_member(void);
void clans_draw_clanmode_join(void);
void clans_draw_clanmode_leave(void);
void clans_draw_clanmode_send_invitation_response(void);
void clans_draw_clanmode_send_membership_response(void);
	
/* np_clans_pad.cpp */
void clans_pad_circle(void);
void clans_pad_cross(void);
void clans_pad_square(void);
void clans_pad_triangle(void);
void clans_pad_down(void);
void clans_pad_up(void);
void clans_pad_right(void);
void clans_pad_left(void);
void clans_pad_r1(void);
void clans_pad_l1(void);

/* np_clans_async_pad.cpp */
int async_pad_start(void);
void async_pad_stop(void);
	

#endif //_SCE_NP_SAMPLE_NP_GUI_H_
