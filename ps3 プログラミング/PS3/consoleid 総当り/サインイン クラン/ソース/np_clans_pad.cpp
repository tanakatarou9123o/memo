/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sysutil/sysutil_common.h>
#include "np_gui.h"


extern struct np_gui_info g_info;

static void
clans_pad_init_cond_param(
	void
	)
{
	g_info.pos_y = 0;
	g_info.pos_slot_y = 0;
	memset(&g_info.clansMessage, 0x00, sizeof(g_info.clansMessage));
	memset(&g_info.getMemberInfoEntry, 0x00, sizeof(g_info.getMemberInfoEntry));
}

static int
check_and_move_clanmode(void)
{
	switch (g_info.pos_act_y) {
	case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
	case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST &&
			g_info.clanlist_num > 0) {
			g_info.selectedClanId = g_info.clanList[g_info.pos_slot_y].info.clanId;
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Clan mode <%s>\n", g_info.clanList[g_info.pos_slot_y].info.name);
			change_clans_state(NP_GUI_CLANS_STATE_CLANMODE);
			return 1;
		}
		break;
	case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
	case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST &&
			g_info.basicInfo_num > 0) {
			g_info.selectedClanId = g_info.clanBasicInfoList[g_info.pos_slot_y].clanId;
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Clan mode <%s>\n", g_info.clanBasicInfoList[g_info.pos_slot_y].name);
			change_clans_state(NP_GUI_CLANS_STATE_CLANMODE);
			return 1;
		}
		break;
	default:
		break;
	}
	return 0;
}



static int
check_clanmode_list(void)
{
	switch (g_info.pos_act_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST &&
			g_info.announcementsList_num > 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"body: %s\n", g_info.messageEntry[g_info.pos_slot_y].message.body);
			return 1;
		}
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST &&
			g_info.challengeList_num > 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"body: %s\n", g_info.challengeEntry[g_info.pos_slot_y].message.body);
			return 1;
		}
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST &&
			g_info.postedChallengeList_num > 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"body: %s\n", g_info.postedChallengeEntry[g_info.pos_slot_y].message.body);
			return 1;
		}
		break;
	default:
		break;
	}
	return 0;
}




static void
clans_pad_enter(
	void
	)
{
	int ret = 0;

	ret = async_pad_start();
	if (ret < 0) {
		printf("ERROR : async_pad_start() failed. ret = 0x%x\n", ret);
		return;
	}

	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		if (check_and_move_clanmode())
			break;

		switch (g_info.pos_act_y) {
		case NP_GUI_MODE_CLANS_ACT_POS_Y_CREATE_CLAN:
			doClansCreateClan();
			if (ret < 0)
				DBG("doClansCreateClan() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
			ret = doClansGetClanList();
			if (ret < 0)
				DBG("doClansGetClanList() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
			ret = doClansGetClanListByNpId();
			if (ret < 0)
				DBG("doClansGetClanListByNpId() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
			ret = doClansSearchByProfile();
			if (ret < 0)
				DBG("doClansSearchByProfile() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
			ret = doClansSearchByName();
			if (ret < 0)
				DBG("doClansSearchByName() failed. ret = 0x%x\n", ret);
			break;
		default:
			break;
		}
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		if (check_clanmode_list())
			break;

		switch (g_info.pos_act_y) {
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND:
			ret = doClansDisband();
			if (ret < 0)
				DBG("doClansDisband() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_INFO:
			ret = doClansGetClanInfo();
			if (ret < 0)
				DBG("doClansGetClanInfo() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_CLAN_INFO:
			ret = doClansUpdateClanInfo();
			if (ret < 0)
				DBG("doClansUpdateClanInfo() failed. ret = 0x%x\n", ret);
			break;

		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_AUTO_ACCEPT:
			ret = doClansGetAutoAcceptStatus();
			if (ret < 0)
				DBG("doClansGetAutoAcceptStatus() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_AUTO_ACCEPT:
			ret = doClansUpdateAutoAcceptStatus();
			if (ret < 0)
				DBG("doClansUpdateAutoAcceptStatus() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_KICK_MEMBER:
			ret = doClansKickMember();
			if (ret < 0)
				DBG("doClansKickMember() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_JOIN_CLAN:
			ret = doClansJoinClan();
			if (ret < 0)
				DBG("doClansJoinClan() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_LEAVE_CLAN:
			ret = doClansLeaveClan();
			if (ret < 0)
				DBG("doClansLeaveClan() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION:
			ret = doClansSendInvitation();
			if (ret < 0)
				DBG("doClansSendInvitation() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CANCEL_INVITATION:
			ret = doClansCancelInvitation();
			if (ret < 0)
				DBG("doClansCancelInvitation() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_REQUEST:
			ret = doClansSendMembershipRequest();
			if (ret < 0)
				DBG("doClansSendMembershipRequest() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CANCEL_MEMBERSHIP:
			ret = doClansCancelMembershipRequest();
			if (ret < 0)
				DBG("doClansCancelMembershipRequest() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_ADD_BLACKLIST_ENTRY:
			ret = doClansAddBlacklistEntry();
			if (ret < 0)
				DBG("doClansAddBlacklistEntry() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_BLACKLIST_ENTRY:
			ret = doClansRemoveBlacklistEntry();
			if (ret < 0)
				DBG("doClansRemoveBlacklistEntry() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_ANNOUNCEMENTS:
			ret = doClansPostAnnouncement();
			if (ret < 0)
				DBG("doClansPostAnnouncement() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_ANNOUNCEMENTS:
			ret = doClansRemoveAnnouncement();
			if (ret < 0)
				DBG("doClansRemoveAnnouncement() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS:
			ret = doClansRetrieveAnnouncements();
			if (ret < 0)
				DBG("doClansRetrieveAnnouncements() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_CHALLENGE:
			ret = doClansPostChallenge();
			if (ret < 0)
				DBG("doClansPostChallenge() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_POSTED_CHALLENGE:
			ret = doClansRemovePostedChallenge();
			if (ret < 0)
				DBG("doClansRemovePostedChallenge() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES:
			ret = doClansRetrievePostedChallenges();
			if (ret < 0)
				DBG("doClansRetrievePostedChallenges() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_CHALLENGE:
			ret = doClansRemoveChallenge();
			if (ret < 0)
				DBG("doClansRemoveChallenge() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES:
			ret = doClansRetrieveChallenges();
			if (ret < 0)
				DBG("doClansRetrieveChallenges() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_BLACKLIST:
			ret = doClansGetBlacklist();
			if (ret < 0)
				DBG("doClansGetBlacklist() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_LIST:
			ret = doClansGetMemberList();
			if (ret < 0)
				DBG("doClansGetMemberList() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_INFO:
			ret = doClansGetMemberInfo();
			if (ret < 0)
				DBG("doClansGetMemberInfo() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_MEMBER_INFO:
			ret = doClansUpdateMemberInfo();
			if (ret < 0)
				DBG("doClansUpdateClanInfo() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CHANGE_MEMBER_ROLE:
			ret = doClansChangeMemberRole();
			if (ret < 0)
				DBG("doClansChangeMemberRole() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION_RESPONSE:
			ret = doClansSendInvitationResponse();
			if (ret < 0)
				DBG("doClansSendInvitationResponse() failed. ret = 0x%x\n", ret);
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_RESPONSE:
			ret = doClansSendMembershipResponse();
			if (ret < 0)
				DBG("doClansSendMembershipResponse() failed. ret = 0x%x\n", ret);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	async_pad_stop();

	return;
}

static void
clans_pad_back(
	void
	)
{
	sceNpClansAbortRequest(g_info.clansHandle);
	g_info.clanlist_num = 0;
	g_info.basicInfo_num = 0;
	g_info.selectedClanId = 0;
	memset(&g_info.getClanList_PagingResult, 0x00, sizeof(g_info.getClanList_PagingResult));

	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		change_mode(NP_GUI_MODE_TOP);
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		change_clans_state(NP_GUI_CLANS_STATE_TOP);
		break;
	default:
		break;
	}

	return;
}

void
clans_pad_circle(
	void
	)
{
	if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE)
		clans_pad_enter();
	else if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS)
		clans_pad_back();

	return;
}

void
clans_pad_cross(
	void
	)
{
	if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE)
		clans_pad_back();
	else if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS)
		clans_pad_enter();

	return;
}

void
clans_pad_square(
	void
	)
{
	return;
}

void
clans_pad_triangle(
	void
	)
{
	return;
}

static void
clans_pad_cond_down(
	int pos_y_max
	)
{
	if (g_info.pos_y < pos_y_max) {
		g_info.pos_y++;
	}

	return;
}

void
clans_pad_down(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_ACT) {
			if (g_info.pos_act_y < (int)NP_GUI_MODE_CLANS_ACT_POS_Y_MAX) {
				g_info.pos_act_y++;
				clans_pad_init_cond_param();
			}
		}
		else if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
			switch (g_info.pos_act_y) {
			case NP_GUI_MODE_CLANS_ACT_POS_Y_CREATE_CLAN:
				clans_pad_cond_down(NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
				clans_pad_cond_down(NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
				clans_pad_cond_down(NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
				clans_pad_cond_down(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
				clans_pad_cond_down(NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_MAX);
				break;
			default:
				clans_pad_cond_down(0);
				break;
			}
		}
		else if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST) {
			switch (g_info.pos_act_y) {
			case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
			case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
				if (g_info.pos_slot_y < (int)(g_info.clanlist_num -1)) {
					g_info.pos_slot_y++;
				}
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
			case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
				if (g_info.pos_slot_y < (int)(g_info.basicInfo_num -1)) {
					g_info.pos_slot_y++;
				}
				break;
			default:
				break;
			}
		}
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_ACT) {
			if (g_info.pos_act_y < (int)NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_MAX) {
				g_info.pos_act_y++;
				if (g_info.pos_act_y - g_info.pos_y_clans_menu_top == NP_GUI_MODE_CLANS_CLANMODE_ACT_DISP_MAX)
					g_info.pos_y_clans_menu_top++;
				clans_pad_init_cond_param();
			}
		}
		else if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
			switch (g_info.pos_act_y ) {
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND:
				clans_pad_cond_down(NP_GUI_MODE_CLANS_CLANMODE_DISBAND_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_CLAN_INFO:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_AUTO_ACCEPT:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CANCEL_INVITATION:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_REQUEST:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_ADD_BLACKLIST_ENTRY:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_BLACKLIST_ENTRY:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_ANNOUNCEMENTS:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_ANNOUNCEMENTS:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_MAX);
				break;

			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_CHALLENGE:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_POSTED_CHALLENGE:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_CHALLENGE:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_BLACKLIST:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_LIST:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_INFO:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_MEMBER_INFO:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_KICK_MEMBER:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CHANGE_MEMBER_ROLE:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION_RESPONSE:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_MAX);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_RESPONSE:
				clans_pad_cond_down(
					NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_MAX);
				break;
			default:
				break;
			}
		}
		else if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST)
		{
			switch (g_info.pos_act_y ) {
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS:
				if (g_info.pos_slot_y < (int)(g_info.announcementsList_num -1)) {
					g_info.pos_slot_y++;
				}
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES:
				if (g_info.pos_slot_y < (int)(g_info.challengeList_num -1)) {
					g_info.pos_slot_y++;
				}
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES:
				if (g_info.pos_slot_y < (int)(g_info.postedChallengeList_num -1)) {
					g_info.pos_slot_y++;
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	return;
}

static void
clans_pad_cond_up(
	int pos_y_min
	)
{
	if (g_info.pos_y > pos_y_min) {
		g_info.pos_y--;
	}

	return;
}

void
clans_pad_up(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_ACT) {
			if (g_info.pos_act_y > (int)NP_GUI_MODE_CLANS_ACT_POS_Y_MIN) {
				g_info.pos_act_y--;
				clans_pad_init_cond_param();
			}
		}
		else if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
			switch (g_info.pos_act_y) {
			case NP_GUI_MODE_CLANS_ACT_POS_Y_CREATE_CLAN:
				clans_pad_cond_up(NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
				clans_pad_cond_up(NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
				clans_pad_cond_up(NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
				clans_pad_cond_up(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
				clans_pad_cond_up(NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_MIN);
				break;
			default:
				clans_pad_cond_up(0);
				break;
			}
		}
		else if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST)
		{
			if (g_info.pos_slot_y > 0) {
				g_info.pos_slot_y--;
			}
		}
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_ACT) {
			if (g_info.pos_act_y > (int)NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_MIN) {
				if (g_info.pos_act_y == g_info.pos_y_clans_menu_top)
					g_info.pos_y_clans_menu_top--;
				g_info.pos_act_y--;
				clans_pad_init_cond_param();
			}
		}
		else if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
			switch (g_info.pos_act_y) {
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND:
				clans_pad_cond_up(NP_GUI_MODE_CLANS_CLANMODE_DISBAND_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_CLAN_INFO:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_AUTO_ACCEPT:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CANCEL_INVITATION:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_REQUEST:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_ADD_BLACKLIST_ENTRY:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_BLACKLIST_ENTRY:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_ANNOUNCEMENTS:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_ANNOUNCEMENTS:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_CHALLENGE:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_POSTED_CHALLENGE:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_CHALLENGE:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_BLACKLIST:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_LIST:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_INFO:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_MEMBER_INFO:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_KICK_MEMBER:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CHANGE_MEMBER_ROLE:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION_RESPONSE:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_MIN);
				break;
			case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_RESPONSE:
				clans_pad_cond_up(
					NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_MIN);
				break;
			default:
				break;
			}
		}
		else if (g_info.pos_x == NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST)
		{
			if (g_info.pos_slot_y > 0) {
				g_info.pos_slot_y--;
			}
		}
		break;
	default:
		break;
	}

	return;
}

static void
post_announcement_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_DURATION:
		g_info.postMessage_duration += 10;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_CENSOR:
		g_info.postMessage_censor = true;
		break;
	default:
		break;
	}
	return;
}

static void
post_challenge_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_DURATION:
		g_info.postChallenge_duration += 10;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_CENSOR:
		g_info.postChallenge_censor = true;
		break;
	default:
		break;
	}
	return;
}

static void
search_by_prof_pad_right(void)
{
	switch(g_info.pos_y) {
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT1:
		g_info.searchProf.attr.intAttr1++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT2:
		g_info.searchProf.attr.intAttr2++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT3:
		g_info.searchProf.attr.intAttr3++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MEMBER:
		g_info.searchProf.numMembers++;
		break;

	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT1_OP:
		if (g_info.searchProf.intAttr1SearchOp < 6)
			g_info.searchProf.intAttr1SearchOp++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT2_OP:
		if (g_info.searchProf.intAttr2SearchOp < 6)
			g_info.searchProf.intAttr2SearchOp++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT3_OP:
		if (g_info.searchProf.intAttr3SearchOp < 6)
			g_info.searchProf.intAttr3SearchOp++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_BIN1_OP:
		if (g_info.searchProf.binAttr1SearchOp < 6)
			g_info.searchProf.binAttr1SearchOp++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MEMBER_OP:
		if (g_info.searchProf.numMembersSearchOp < 6)
			g_info.searchProf.numMembersSearchOp++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TAG_OP:
		if (g_info.searchProf.tagSearchOp < 6)
			g_info.searchProf.tagSearchOp++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_PAGE_START:
		g_info.searchByProfile_PagingRequest.startPos++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_PAGE_MAX:
		if (g_info.searchByProfile_PagingRequest.max < CLANS_BASICINFO_ENTRY_MAX)
			g_info.searchByProfile_PagingRequest.max++;
		break;
	default:
		break;
	}
	return;
}

static void
search_by_name_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_NAME_OP:
		if (g_info.searchName.nameSearchOp < 6)
			g_info.searchName.nameSearchOp++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_PAGE_START:
		g_info.searchByName_PagingRequest.startPos++;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_PAGE_MAX:
		if (g_info.searchByName_PagingRequest.max < CLANS_BASICINFO_ENTRY_MAX)
			g_info.searchByName_PagingRequest.max++;
		break;
	default:
		break;
	}
	return;
}

static void
update_clan_info_pad_right(void)
{
	switch(g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT1:
		g_info.updatableClanInfo.attr.intAttr1++;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT2:
		g_info.updatableClanInfo.attr.intAttr2++;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT3:
		g_info.updatableClanInfo.attr.intAttr3++;
		break;
	default:
		break;
	}
	return;
}

static void
update_auto_accept_pad_right(void)
{
	switch(g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_AUTO_ACCEPT:
		g_info.updateAutoAccept = true;
		break;
	default:
		break;
	}
	return;
}

static void
retrieve_announcements_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_PAGE_START:
		g_info.retrieveAnnouncements_PagingRequest.startPos++;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_PAGE_MAX:
		if (g_info.retrieveAnnouncements_PagingRequest.max < CLANS_AN_ENTRY_MAX)
			g_info.retrieveAnnouncements_PagingRequest.max++;
		break;
	default:
		break;
	}
	return;
}

static void
retrieve_challenges_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_PAGE_START:
		g_info.retrieveChallenges_PagingRequest.startPos++;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_PAGE_MAX:
		if (g_info.retrieveChallenges_PagingRequest.max < CLANS_AN_ENTRY_MAX)
			g_info.retrieveChallenges_PagingRequest.max++;
		break;
	default:
		break;
	}
	return;
}

static void
retrieve_posted_challenges_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_PAGE_START:
		g_info.retrievePostedChallenges_PagingRequest.startPos++;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_PAGE_MAX:
		if (g_info.retrievePostedChallenges_PagingRequest.max < CLANS_AN_ENTRY_MAX)
			g_info.retrievePostedChallenges_PagingRequest.max++;
		break;
	default:
		break;
	}
	return;
}

static void
remove_posted_challenges_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_ID:
		g_info.postedChallengeId++;
		break;
	default:
		break;
	}
	return;
}

static void
get_blacklist_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_PAGE_START:
		g_info.getBlacklist_PagingRequest.startPos++;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_PAGE_MAX:
		if (g_info.getBlacklist_PagingRequest.max < CLANS_BLACKLIST_ENTRY_MAX)
			g_info.getBlacklist_PagingRequest.max++;
		break;
	default:
		break;
	}
	return;
}

static void
get_memberlist_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_STATUS:
		if (g_info.memberStatus < SCE_NP_CLANS_MEMBER_STATUS_PENDING)
			g_info.memberStatus++;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_PAGE_START:
		g_info.getMemberList_PagingRequest.startPos++;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_PAGE_MAX:
		if (g_info.getMemberList_PagingRequest.max < CLANS_MEMNBERLIST_ENTRY_MAX)
			g_info.getMemberList_PagingRequest.max++;
		break;
	default:
		break;
	}
	return;
}

static void
update_member_info_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_ALLOW_MSG:
		g_info.updatableMemberInfo.allowMsg = true;
		break;
	default:
		break;
	}
	return;
}

static void
kick_member_info_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_CENSOR:
		g_info.kickMember_censor = true;
		break;
	default:
		break;
	}
	return;
}

static void
change_member_role_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_ROLE:
		if (g_info.memberRole < SCE_NP_CLANS_ROLE_LEADER)
			g_info.memberRole++;
		break;
	default:
		break;
	}
	return;
}

static void
send_invitation_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_CENSOR:
		g_info.sendInvitation_censor = true;
		break;
	default:
		break;
	}
	return;
}

static void
send_invitation_response_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_ACCEPT:
		g_info.sendInvitationResponse_accept = true;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_CENSOR:
		g_info.sendInvitationResponse_censor = true;
		break;
	default:
		break;
	}
	return;
}

static void
send_membership_request_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_CENSOR:
		g_info.sendMembershipRequest_censor = true;
		break;
	default:
		break;
	}
	return;
}

static void
send_membership_response_pad_right(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_ACCEPT:
		g_info.sendMembershipResponse_accept = true;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_CENSOR:
		g_info.sendMembershipResponse_censor = true;
		break;
	default:
		break;
	}
	return;
}

void
clans_pad_right(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		switch (g_info.pos_act_y) {
		case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
			if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND)
				break;

			if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_PAGE_START)
				g_info.getClanList_PagingRequest.startPos++;
			else if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_PAGE_MAX)
				if (g_info.getClanList_PagingRequest.max < CLANS_CLAN_ENTRY_MAX)
					g_info.getClanList_PagingRequest.max++;
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
			if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND)
				break;

			if (g_info.pos_y ==
				NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_PAGE_START)
				g_info.getClanListByNpId_PagingRequest.startPos++;
			else if (g_info.pos_y ==
				NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_PAGE_MAX)
				if (g_info.getClanListByNpId_PagingRequest.max < CLANS_CLAN_ENTRY_MAX)
					g_info.getClanListByNpId_PagingRequest.max++;
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
			search_by_prof_pad_right();
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
			search_by_name_pad_right();
		default:
			break;
		}
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		switch (g_info.pos_act_y) {
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_CLAN_INFO:
			update_clan_info_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_AUTO_ACCEPT:
			update_auto_accept_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_ANNOUNCEMENTS:
			post_announcement_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_ANNOUNCEMENTS:
			g_info.messageId++;
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS:
			retrieve_announcements_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_CHALLENGE:
			post_challenge_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_POSTED_CHALLENGE:
			remove_posted_challenges_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES:
			retrieve_posted_challenges_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_CHALLENGE:
			g_info.challengeId++;
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES:
			retrieve_challenges_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_BLACKLIST:
			get_blacklist_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_LIST:
			get_memberlist_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_MEMBER_INFO:
			update_member_info_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_KICK_MEMBER:
			kick_member_info_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CHANGE_MEMBER_ROLE:
			change_member_role_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION:
			send_invitation_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION_RESPONSE:
			send_invitation_response_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_REQUEST:
			send_membership_request_pad_right();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_RESPONSE:
			send_membership_response_pad_right();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return;
}

static void
post_announcement_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_DURATION:
		g_info.postMessage_duration -= 10;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_CENSOR:
		g_info.postMessage_censor = false;
		break;
	default:
		break;
	}
	return;
}

static void
post_challenge_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_DURATION:
		g_info.postChallenge_duration -= 10;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_CENSOR:
		g_info.postChallenge_censor = false;
		break;
	default:
		break;
	}
	return;
}

static void
search_by_prof_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT1:
		g_info.searchProf.attr.intAttr1--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT2:
		g_info.searchProf.attr.intAttr2--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT3:
		g_info.searchProf.attr.intAttr3--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MEMBER:
		g_info.searchProf.numMembers--;
		break;

	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT1_OP:
		if (g_info.searchProf.intAttr1SearchOp >= 0)
			g_info.searchProf.intAttr1SearchOp--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT2_OP:
		if (g_info.searchProf.intAttr2SearchOp >= 0)
			g_info.searchProf.intAttr2SearchOp--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT3_OP:
		if (g_info.searchProf.intAttr3SearchOp >= 0)
			g_info.searchProf.intAttr3SearchOp--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_BIN1_OP:
		if (g_info.searchProf.binAttr1SearchOp >= 0)
			g_info.searchProf.binAttr1SearchOp--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MEMBER_OP:
		if (g_info.searchProf.numMembersSearchOp >= 0)
			g_info.searchProf.numMembersSearchOp--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TAG_OP:
		if (g_info.searchProf.tagSearchOp >= 0)
			g_info.searchProf.tagSearchOp--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_PAGE_START:
		g_info.searchByProfile_PagingRequest.startPos--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_PAGE_MAX:
		if (g_info.searchByProfile_PagingRequest.max > 0)
			g_info.searchByProfile_PagingRequest.max--;
		break;
	default:
		break;
	}
}

static void
search_by_name_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_NAME_OP:
		if (g_info.searchName.nameSearchOp >= 0)
			g_info.searchName.nameSearchOp--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_PAGE_START:
		g_info.searchByName_PagingRequest.startPos--;
		break;
	case NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_PAGE_MAX:
		if (g_info.searchByName_PagingRequest.max > 0)
			g_info.searchByName_PagingRequest.max--;
		break;
	default:
		break;
	}
	return;
}

static void
update_clan_info_pad_left(void)
{
	switch(g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT1:
		g_info.updatableClanInfo.attr.intAttr1--;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT2:
		g_info.updatableClanInfo.attr.intAttr2--;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT3:
		g_info.updatableClanInfo.attr.intAttr3--;
		break;
	default:
		break;
	}
	return;
}

static void
update_auto_accept_pad_left(void)
{
	switch(g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_AUTO_ACCEPT:
		g_info.updateAutoAccept = false;
		break;
	default:
		break;
	}
	return;
}

static void
retrieve_announcements_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_PAGE_START:
		g_info.retrieveAnnouncements_PagingRequest.startPos--;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_PAGE_MAX:
		if (g_info.retrieveAnnouncements_PagingRequest.max > 0)
			g_info.retrieveAnnouncements_PagingRequest.max--;
		break;
	default:
		break;
	}
	return;
}

static void
retrieve_challenges_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_PAGE_START:
		g_info.retrieveChallenges_PagingRequest.startPos--;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_PAGE_MAX:
		if (g_info.retrieveChallenges_PagingRequest.max > 0)
			g_info.retrieveChallenges_PagingRequest.max--;
		break;
	default:
		break;
	}
	return;
}

static void
retrieve_posted_challenges_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_PAGE_START:
		g_info.retrievePostedChallenges_PagingRequest.startPos--;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_PAGE_MAX:
		if (g_info.retrievePostedChallenges_PagingRequest.max > 0)
			g_info.retrievePostedChallenges_PagingRequest.max--;
		break;
	default:
		break;
	}
	return;
}

static void
remove_posted_challenges_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_ID:
		g_info.postedChallengeId--;
		break;
	default:
		break;
	}
	return;
}

static void
get_blacklist_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_PAGE_START:
		g_info.getBlacklist_PagingRequest.startPos--;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_PAGE_MAX:
		if (g_info.getBlacklist_PagingRequest.max > 0)
			g_info.getBlacklist_PagingRequest.max--;
		break;
	default:
		break;
	}
	return;
}

static void
get_memberlist_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_STATUS:
		if (g_info.memberStatus > SCE_NP_CLANS_MEMBER_STATUS_UNKNOWN)
			g_info.memberStatus--;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_PAGE_START:
		g_info.getMemberList_PagingRequest.startPos--;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_PAGE_MAX:
		if (g_info.getMemberList_PagingRequest.max > 0)
			g_info.getMemberList_PagingRequest.max--;
		break;
	default:
		break;
	}
	return;
}

static void
update_member_info_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_ALLOW_MSG:
		g_info.updatableMemberInfo.allowMsg = false;
		break;
	default:
		break;
	}
	return;
}

static void
kick_member_info_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_CENSOR:
		g_info.kickMember_censor = false;
		break;
	default:
		break;
	}
	return;
}

static void
change_member_role_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_ROLE:
		if ((int)g_info.memberRole > SCE_NP_CLANS_ROLE_UNKNOWN)
			g_info.memberRole--;
		break;
	default:
		break;
	}
	return;
}

static void
send_invitation_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_CENSOR:
		g_info.sendInvitation_censor = false;
		break;
	default:
		break;
	}
	return;
}

static void
send_invitation_response_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_ACCEPT:
		g_info.sendInvitationResponse_accept = false;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_CENSOR:
		g_info.sendInvitationResponse_censor = false;
		break;
	default:
		break;
	}
	return;
}

static void
send_membership_request_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_CENSOR:
		g_info.sendMembershipRequest_censor = false;
		break;
	default:
		break;
	}
	return;
}

static void
send_membership_response_pad_left(void)
{
	switch (g_info.pos_y) {
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_ACCEPT:
		g_info.sendMembershipResponse_accept = false;
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_CENSOR:
		g_info.sendMembershipResponse_censor = false;
		break;
	default:
		break;
	}
	return;
}

void
clans_pad_left(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		switch (g_info.pos_act_y) {
		case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
			if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND)
				break;

			if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_PAGE_START)
				g_info.getClanList_PagingRequest.startPos--;
			else if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_PAGE_MAX)
				if (g_info.getClanList_PagingRequest.max > 0)
					g_info.getClanList_PagingRequest.max--;
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
			if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND)
				break;

			if (g_info.pos_y ==
				NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_PAGE_START)
				g_info.getClanListByNpId_PagingRequest.startPos--;
			else if (g_info.pos_y ==
				NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_PAGE_MAX)
				if (g_info.getClanListByNpId_PagingRequest.max > 0)
					g_info.getClanListByNpId_PagingRequest.max--;
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
			search_by_prof_pad_left();
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
			search_by_name_pad_left();
		default:
			break;
		}
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		switch (g_info.pos_act_y) {
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_CLAN_INFO:
			update_clan_info_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_AUTO_ACCEPT:
			update_auto_accept_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_ANNOUNCEMENTS:
			post_announcement_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_ANNOUNCEMENTS:
			g_info.messageId--;
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS:
			retrieve_announcements_pad_left();
			break;

		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_CHALLENGE:
			post_challenge_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_POSTED_CHALLENGE:
			remove_posted_challenges_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES:
			retrieve_posted_challenges_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_CHALLENGE:
			g_info.challengeId--;
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES:
			retrieve_challenges_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_BLACKLIST:
			get_blacklist_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_LIST:
			get_memberlist_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_MEMBER_INFO:
			update_member_info_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_KICK_MEMBER:
			kick_member_info_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CHANGE_MEMBER_ROLE:
			change_member_role_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION:
			send_invitation_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION_RESPONSE:
			send_invitation_response_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_REQUEST:
			send_membership_request_pad_left();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_RESPONSE:
			send_membership_response_pad_left();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return;
}

void
clans_pad_r1(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		if (g_info.pos_x < NP_GUI_MODE_CLANS_TOP_POS_X_MAX) {
			g_info.pos_x++;
		}
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		if (g_info.pos_x < NP_GUI_MODE_CLANS_CLANMODE_POS_X_MAX) {
			g_info.pos_x++;
		}
		break;
	default:
		break;
	}

	return;
}

void
clans_pad_l1(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		if (g_info.pos_x > NP_GUI_MODE_CLANS_TOP_POS_X_MIN) {
			g_info.pos_x--;
		}
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		if (g_info.pos_x > NP_GUI_MODE_CLANS_CLANMODE_POS_X_MIN) {
			g_info.pos_x--;
		}
		break;
	default:
		break;
	}

	return;
}

