/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/synchronization.h>
#include <sys/memory.h>
#include <sysutil/sysutil_common.h>
#include <sysutil/sysutil_oskdialog.h>

#include "np_gui.h"
#include "np_gui_draw.h"
#include "strings.h"
#include "np_conf.h"

#define LOOKUP_THREAD_PRIO			1500


extern struct np_gui_info g_info;

static int call_sceNpLookupNpIdAsync(SceNpId *npId, SceNpOnlineId *onlineId);

static int
call_sceNpLookupNpIdAsync(SceNpId *npId, SceNpOnlineId *onlineId)
{
	int ret;
	int32_t lookupTitleCtxId = -1;

	ret = sceNpManagerGetNpId(&g_info.npId);
	if (ret < 0) {
		DBG("sceNpManagerGetNpId() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpLookupCreateTitleCtx(NpConf::npCommId(), &g_info.npId);
	if (ret < 0) {
		DBG("sceNpLookupCreateTitleCtx() failed. ret = 0x%x\n", ret);
		goto error;
	}
	lookupTitleCtxId = ret;

	ret = sceNpLookupCreateTransactionCtx(lookupTitleCtxId);
	if (ret < 0) {
		DBG("sceNpLookupCreateTransactionCtx() failed. ret = 0x%x\n", ret);
		goto error;
	}
	g_info.lookupTransId = ret;

	ret = sceNpLookupNpIdAsync(g_info.lookupTransId,
			onlineId, npId, LOOKUP_THREAD_PRIO, NULL);
	if (ret < 0) {
		DBG("sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
		goto error;
	}

	while (sceNpLookupPollAsync(g_info.lookupTransId, &ret)) {
		cellSysutilCheckCallback();
		sys_timer_usleep(10 * 1000);
	}
	if (ret < 0) {
		DBG("sceNpLookupPollAsync() result = 0x%x\n", ret);
		goto error;
	}

error:
	if (g_info.lookupTransId >= 0) {
		ret = sceNpLookupDestroyTransactionCtx(g_info.lookupTransId);
		if (ret < 0) {
			DBG("sceNpLookupDestroyTransactionCtx() failed. ret = 0x%x\n", ret);
		}
		g_info.lookupTransId = 0;
	}

	if (lookupTitleCtxId >= 0) {
		ret = sceNpLookupDestroyTitleCtx(lookupTitleCtxId);
		if (ret < 0) {
			DBG("sceNpLookupDestroyTitleCtx() failed. ret = 0x%x\n", ret);
		}
	}

	return ret;
}



void
clans_pad_func(
	void
	)
{
	if (g_info.flag & NP_GUI_FLAG_PAD_LOCK)
		return;

	if ( g_info.padd & CTRL_CIRCLE ) {
		clans_pad_circle();
	}

	if ( g_info.padd & CTRL_CROSS ) {
		clans_pad_cross();
	}

	if ( g_info.padd & CTRL_SQUARE ) {
		clans_pad_square();
	}

	if ( g_info.padd & CTRL_TRIANGLE ) {
		clans_pad_triangle();
	}

	if ( g_info.padd & CTRL_UP ) {
		clans_pad_up();
	}

	if ( g_info.padd & CTRL_DOWN ) {
		clans_pad_down();
	}

	if ( g_info.padd & CTRL_RIGHT ) {
		clans_pad_right();
	}

	if ( g_info.padd & CTRL_LEFT ) {
		clans_pad_left();
	}

	if ( g_info.padd & CTRL_R1 ) {
		clans_pad_r1();
	}

	if ( g_info.padd & CTRL_L1 ) {
		clans_pad_l1();
	}
	return;
}

void
clans_draw_func(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		switch (g_info.pos_act_y) {
		case NP_GUI_MODE_CLANS_ACT_POS_Y_CREATE_CLAN:
			clans_draw_create_clan();
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
			clans_draw_get_clan_list();
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
			clans_draw_get_clan_list_by_npid();
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
			clans_draw_search_by_profile();
			break;
		case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
			clans_draw_search_by_name();
			break;
		default:
			break;
		}

		clans_draw_clanlist();
		clans_draw_action_menu();
		clans_draw_bottom_menu();
		break;
	case NP_GUI_CLANS_STATE_CLANMODE:
		switch (g_info.pos_act_y) {
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND:
			clans_draw_clanmode_disband();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_INFO:
			clans_draw_clanmode_get_info();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_CLAN_INFO:
			clans_draw_clanmode_update_clan_info();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_AUTO_ACCEPT:
			clans_draw_clanmode_get_auto_accept();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_AUTO_ACCEPT:
			clans_draw_clanmode_update_auto_accept();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION:
			clans_draw_clanmode_send_invitation();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CANCEL_INVITATION:
			clans_draw_clanmode_cancel_invitation();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_REQUEST:
			clans_draw_clanmode_send_membership_request();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CANCEL_MEMBERSHIP:
			clans_draw_clanmode_cancel_membership();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_ADD_BLACKLIST_ENTRY:
			clans_draw_clanmode_add_blacklist_entry();
			clans_draw_blacklist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_BLACKLIST_ENTRY:
			clans_draw_clanmode_remove_blacklist_entry();
			clans_draw_blacklist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_ANNOUNCEMENTS:
			clans_draw_clanmode_post_announcement();
			clans_draw_anlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_ANNOUNCEMENTS:
			clans_draw_clanmode_remove_announcement();
			clans_draw_anlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_ANNOUNCEMENTS:
			clans_draw_clanmode_retrieve_announcements();
			clans_draw_anlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_POST_CHALLENGE:
			clans_draw_clanmode_post_challenge();
			clans_draw_posted_chlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_POSTED_CHALLENGE:
			clans_draw_clanmode_remove_posted_challenge();
			clans_draw_posted_chlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_POSTED_CHALLENGES:
			clans_draw_clanmode_retrieve_posted_challenges();
			clans_draw_posted_chlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_REMOVE_CHALLENGE:
			clans_draw_clanmode_remove_challenge();
			clans_draw_chlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_RETRIEVE_CHALLENGES:
			clans_draw_clanmode_retrieve_challenges();
			clans_draw_chlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_BLACKLIST:
			clans_draw_clanmode_get_blacklist();
			clans_draw_blacklist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_LIST:
			clans_draw_clanmode_get_member_list();
			clans_draw_memberlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_GET_MEMBER_INFO:
			clans_draw_clanmode_get_member_info();
			clans_draw_memberlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_UPDATE_MEMBER_INFO:
			clans_draw_clanmode_update_member_info();
			clans_draw_memberlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_CHANGE_MEMBER_ROLE:
			clans_draw_clanmode_change_member_role();
			clans_draw_memberlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_KICK_MEMBER:
			clans_draw_clanmode_kick_member();
			clans_draw_memberlist();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_JOIN_CLAN:
			clans_draw_clanmode_join();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_LEAVE_CLAN:
			clans_draw_clanmode_leave();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_INVITATION_RESPONSE:
			clans_draw_clanmode_send_invitation_response();
			break;
		case NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_SEND_MEMBERSHIP_RESPONSE:
			clans_draw_clanmode_send_membership_response();
			break;
		default:
			break;
		}
		clans_draw_clanmode_action_menu();
		clans_draw_bottom_menu();
		break;
	default:
		break;
	}

	return;
}

void
clans_update_func(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		if (g_info.flag & NP_GUI_FLAG_OFFLINE) {
			DBG("NP OFFLINE\n");
			g_info.flag &= ~NP_GUI_FLAG_OFFLINE;
			change_mode(NP_GUI_MODE_TOP);
		}
		break;
	default:
		break;
	}
	
	return;
}


int
doClansCreateClan(void)
{
	int ret=0;
	SceNpClanId clanId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_NAME) {
		start_osk_dialog(
			NP_GUI_OSK_INPUT_CREATE_CLAN_NAME,
			(uint16_t *)L"Clan name",
			SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH);
	} else if (g_info.pos_y == NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_TAG) {
		start_osk_dialog(
			NP_GUI_OSK_INPUT_CREATE_CLAN_TAG,
			(uint16_t *)L"Clan tag",
			SCE_NP_CLANS_CLAN_TAG_MAX_LENGTH);
	} else if (g_info.pos_y == NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_CREATE) {
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateClan(
			g_info.clansHandle,
			g_info.clans_create_clan_name, 
			g_info.clans_create_clan_tag,
			&clanId
			);
		if (ret < 0) {
			DBG("sceNpClansCreateClan() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateClan() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success. clanId = %d\n", clanId);
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansGetClanList(void)
{
	int ret=0;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_TOP_POS_X_COND:
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_TOTAL) {
			ret = sceNpClansGetClanList(
					g_info.clansHandle, NULL, NULL, &result);
			if (ret < 0) {
				DBG("sceNpClansGetClanList() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansGetClanList() failed. ret = 0x%x\n", ret);
				goto error;
			}
	
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Clan List total = %d\n", result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_GET)
		{
			g_info.clanlist_num = 0;
			ret = sceNpClansGetClanList(
					g_info.clansHandle,
					&g_info.getClanList_PagingRequest,
					g_info.clanList,
					&g_info.getClanList_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansGetClanList() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansGetClanList() failed. ret = 0x%x\n", ret);
				goto error;
			}

			g_info.clanlist_num = g_info.getClanList_PagingResult.count;
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Success.\n");
		}
		break;
	case NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}

int
doClansGetClanListByNpId(void)
{
	int ret=0;
	SceNpId npId;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_TOP_POS_X_COND:
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_NPID) {
			start_osk_dialog(
				NP_GUI_OSK_INPUT_GET_CLAN_LIST_BY_NPID_NPID,
				(uint16_t *)L"OnlineId",
				SCE_NET_NP_ONLINEID_MAX_LENGTH	
				);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_TOTAL)
		{
			ret = call_sceNpLookupNpIdAsync(&npId, &g_info.getClanListByNpId_onlineId);
			if (ret < 0) {
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
				goto error;
			}

			ret = sceNpClansGetClanListByNpId(
					g_info.clansHandle, NULL, &npId, NULL, &result);
			if (ret < 0) {
				DBG("sceNetClansGetClanListByNpId() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansGetClanListByNpId() failed. ret = 0x%x\n", ret);
				goto error;
			}

			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"<%s> Clan List total = %d\n",
				g_info.getClanListByNpId_onlineId.data, result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_GET)
		{
			g_info.clanlist_num = 0;
			ret = call_sceNpLookupNpIdAsync(&npId, &g_info.getClanListByNpId_onlineId);
			if (ret < 0)
				goto error;

			ret = sceNpClansGetClanListByNpId(
					g_info.clansHandle,
					&g_info.getClanListByNpId_PagingRequest,
					&npId,
					g_info.clanList,
					&g_info.getClanList_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansGetClanListByNpId() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansGetClanListByNpId() failed. ret = 0x%x\n", ret);
				goto error;
			}
			g_info.clanlist_num = g_info.getClanList_PagingResult.count;
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Success.\n");
		}
		break;
	case NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}


static void
fill_search_field(void)
{
	g_info.searchProf.attr.fields = 0;
	g_info.searchProf.fields = 0;

	if (g_info.searchProf.intAttr1SearchOp >= 0)
		g_info.searchProf.attr.fields |= SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR1;
	if (g_info.searchProf.intAttr2SearchOp >= 0)
		g_info.searchProf.attr.fields |= SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR2;
	if (g_info.searchProf.intAttr3SearchOp >= 0)
		g_info.searchProf.attr.fields |= SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR3;
	if (g_info.searchProf.binAttr1SearchOp >= 0)
		g_info.searchProf.attr.fields |= SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_BIN_ATTR1;
	if (g_info.searchProf.numMembersSearchOp >= 0)
		g_info.searchProf.fields |= SCE_NP_CLANS_FIELDS_SEARCHABLE_PROFILE_NUM_MEMBERS;
	if (g_info.searchProf.tagSearchOp >= 0)
		g_info.searchProf.fields |= SCE_NP_CLANS_FIELDS_SEARCHABLE_PROFILE_TAG;

	return;
}


int
doClansSearchByProfile(void)
{
	int ret;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	fill_search_field();

	ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
	if (ret < 0) {
		DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_TOP_POS_X_COND:
		if (g_info.pos_y == NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_BIN1)
		{
			start_osk_dialog(
				NP_GUI_OSK_INPUT_SEARCH_BIN_ATTR,
				(uint16_t *)L"binAttr1",
				SCE_NP_CLANS_CLAN_BINARY_ATTRIBUTE1_MAX_SIZE);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TAG)
		{
			start_osk_dialog(
				NP_GUI_OSK_INPUT_SEARCH_TAG,
				(uint16_t *)L"tag",
				SCE_NP_CLANS_CLAN_TAG_MAX_LENGTH);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TOTAL)
		{
			ret = sceNpClansSearchByProfile(
					g_info.clansHandle,
					NULL,
					&g_info.searchProf,
					NULL,
					&result
					);
			if (ret < 0) {
				DBG("sceNpClansSearchByProfile() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansSearchByProfile() failed. ret = 0x%x\n", ret);
				goto error;
			}

			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Search Clan List total = %d\n",
				result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_SEARCH)
		{
			g_info.basicInfo_num = 0;
			ret = sceNpClansSearchByProfile(
					g_info.clansHandle,
					&g_info.searchByProfile_PagingRequest,
					&g_info.searchProf,
					g_info.clanBasicInfoList,
					&g_info.search_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansSearchByProfile() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansSearchByProfile() failed. ret = 0x%x\n", ret);
				goto error;
			}
			g_info.basicInfo_num = g_info.search_PagingResult.count;
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Success.\n");
		}
		break;
	case NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}

int
doClansSearchByName(void)
{
	int ret;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
	if (ret < 0) {
		DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_TOP_POS_X_COND:
		if (g_info.pos_y == NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_NAME)
		{
			start_osk_dialog(
				NP_GUI_OSK_INPUT_SEARCH_NAME,
				(uint16_t *)L"name",
				SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_TOTAL)
		{
			ret = sceNpClansSearchByName(
					g_info.clansHandle,
					NULL,
					&g_info.searchName,
					NULL,
					&result
					);
			if (ret < 0) {
				DBG("sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
				goto error;
			}

			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Search Clan List total = %d\n",
				result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_SEARCH)
		{
			g_info.basicInfo_num = 0;
			ret = sceNpClansSearchByName(
					g_info.clansHandle,
					&g_info.searchByName_PagingRequest,
					&g_info.searchName,
					g_info.clanBasicInfoList,
					&g_info.search_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
				goto error;
			}
			g_info.basicInfo_num = g_info.search_PagingResult.count;
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Success.\n");
		}
		break;
	case NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}


//// clan mode

int
doClansDisband(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
	if (ret < 0) {
		DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpClansDisbandClan(g_info.clansHandle, g_info.selectedClanId);
	if (ret < 0) {
		DBG("sceNpClansDisbandClan() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansDisbandClan() failed. ret = 0x%x\n", ret);
		goto error;
	}
	cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
		"Success.\n");

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

static uint8_t binData1[SCE_NP_CLANS_CLAN_BINARY_DATA_MAX_SIZE];
int
doClansGetClanInfo(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
	if (ret < 0) {
		DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	memset(&g_info.clanInfo, 0x00, sizeof(g_info.clanInfo));
	g_info.clanInfo.updatable.binData1 = binData1;
	g_info.clanInfo.updatable.binData1Size = sizeof(binData1);
	ret = sceNpClansGetClanInfo(
			g_info.clansHandle,
			g_info.selectedClanId,
			&g_info.clanInfo);
	if (ret < 0) {
		DBG("sceNpClansGetClanInfo() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansGetClanInfo() failed. ret = 0x%x\n", ret);
		goto error;
	}
	cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
		"Success.\n");

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}


static void
fill_update_clan_info_field(void)
{
	g_info.updatableClanInfo.fields = 0;
	g_info.updatableClanInfo.attr.fields = SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR1 |
		                                   SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR2 |
		                                   SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR3;
	if (g_info.updatableClanInfo.attr.binAttr1[0] != '\0')
		g_info.updatableClanInfo.attr.fields |= SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_BIN_ATTR1;
	
	if (g_info.updatableClanInfo.description[0] != '\0')
		g_info.updatableClanInfo.fields |= SCE_NP_CLANS_FIELDS_UPDATABLE_CLAN_INFO_DESCR;
	if ((g_info.updatableClanInfo.binData1) && g_info.updatableClanInfo.binData1[0] != '\0')
		g_info.updatableClanInfo.fields |= SCE_NP_CLANS_FIELDS_UPDATABLE_CLAN_INFO_BIN_DATA1;
	return;
}


int
doClansUpdateClanInfo(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	fill_update_clan_info_field();

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_DESC)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_UPDATE_DESC,
			(uint16_t *)L"Description",
			SCE_NP_CLANS_CLAN_DESCRIPTION_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_BIN1)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_UPDATE_BIN_ATTR,
			(uint16_t *)L"binAttr1",
			SCE_NP_CLANS_CLAN_BINARY_ATTRIBUTE1_MAX_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_UPDATE)
	{
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansUpdateClanInfo(
				g_info.clansHandle,
				g_info.selectedClanId, 
				&g_info.updatableClanInfo
				);
		if (ret < 0) {
			DBG("sceNpClansUpdateClanInfo() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansUpdateClanInfo() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansGetAutoAcceptStatus(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
	if (ret < 0) {
		DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpClansGetAutoAcceptStatus(
		g_info.clansHandle,
		g_info.selectedClanId,
		&g_info.autoAccept);
	if (ret < 0) {
		DBG("sceNpClansGetAutoAcceptStatus() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansGetAutoAcceptStatus() failed. ret = 0x%x\n", ret);
		goto error;
	}
	cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
		"Success.\n");

	g_info.haveAutoAcceptStatus = true;

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansUpdateAutoAcceptStatus(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_UPDATE) {
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansUpdateAutoAcceptStatus(
			g_info.clansHandle,
			g_info.selectedClanId,
			g_info.updateAutoAccept);
		if (ret < 0) {
			DBG("sceNpClansUpdateAutoAcceptStatus() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansUpdateAutoAcceptStatus() failed. ret = 0x%x\n", ret);
			goto error;
		}
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansKickMember(void)
{
	int ret=0;
	SceNpId npId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_NPID)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_KICK_MEMBER_NPID,
			(uint16_t *)L"NpId",
			SCE_NET_NP_ONLINEID_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_SUBJCET)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_KICK_MEMBER_SUBJECT,
			(uint16_t *)L"Subject",
			SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_BODY)
	{
		/*J SceNpClansMessage.body のサイズは OSK で入力できる最大長よりも大きい
		 *  ので、ここでは OSK に合わせます。
		 */
		start_osk_dialog(
			NP_GUI_OSK_INPUT_KICK_MEMBER_BODY,
			(uint16_t *)L"Body",
			CELL_OSKDIALOG_STRING_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_KICK)
	{
		ret = call_sceNpLookupNpIdAsync(&npId, &g_info.kickMember_onlineId);
		if (ret < 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.kickMember_censor) {
			g_info.kickMemberMessage.options |= SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		} else {
			g_info.kickMemberMessage.options &= ~SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		}

		ret = sceNpClansKickMember(
				g_info.clansHandle,
				g_info.selectedClanId, 
				&npId,
				&g_info.kickMemberMessage
				);
		if (ret < 0) {
			DBG("sceNpClansKickMember() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansKickMember() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}



int
doClansJoinClan(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
	if (ret < 0) {
		DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpClansJoinClan(
			g_info.clansHandle,
			g_info.selectedClanId 
			);
	if (ret < 0) {
		DBG("sceNpClansJoinClan() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansJoinClan() failed. ret = 0x%x\n", ret);
		goto error;
	}

	cellDbgFontConsolePrintf(g_info.mDbgFontID_info, "Success.\n");

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansLeaveClan(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
	if (ret < 0) {
		DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpClansLeaveClan(
			g_info.clansHandle,
			g_info.selectedClanId 
			);
	if (ret < 0) {
		DBG("sceNpClansLeaveClan() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansLeaveClan() failed. ret = 0x%x\n", ret);
		goto error;
	}

	cellDbgFontConsolePrintf(g_info.mDbgFontID_info, "Success.\n");

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansSendInvitation(void)
{
	int ret=0;
	SceNpId npId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_ONLINEID)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_SEND_INVITATION_NPID,
			(uint16_t *)L"OnlineId",
			SCE_NET_NP_ONLINEID_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_SUBJECT)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_SUBJECT,
			(uint16_t *)L"Subject",
			SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_BODY)
	{
		/*J SceNpClansMessage.body のサイズは OSK で入力できる最大長よりも大きい
		 *  ので、ここでは OSK に合わせます。
		 */
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_BODY,
			(uint16_t *)L"Body",
			CELL_OSKDIALOG_STRING_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_SEND)
	{
		ret = call_sceNpLookupNpIdAsync(&npId, &g_info.sendInvitation_onlineId);
		if (ret < 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.sendInvitation_censor) {
			g_info.clansMessage.options |= SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		} else {
			g_info.clansMessage.options &= ~SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		}

		ret = sceNpClansSendInvitation(
				g_info.clansHandle,
				g_info.selectedClanId, 
				&npId,
				&g_info.clansMessage
				);
		if (ret < 0) {
			DBG("sceNpClansSendInvitation() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansSendInvitation() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansCancelInvitation(void)
{
	int ret=0;
	SceNpId npId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_ONLINEID)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_SEND_INVITATION_NPID,
			(uint16_t *)L"OnlineId",
			SCE_NET_NP_ONLINEID_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_CANCEL)
	{
		ret = call_sceNpLookupNpIdAsync(&npId, &g_info.sendInvitation_onlineId);
		if (ret < 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCancelInvitation(
				g_info.clansHandle,
				g_info.selectedClanId, 
				&npId
				);
		if (ret < 0) {
			DBG("sceNpClansCancelInvitation() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCancelInvitation() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}



int
doClansSendMembershipRequest(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_SUBJECT)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_SUBJECT,
			(uint16_t *)L"Subject",
			SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_BODY)
	{
		/*J SceNpClansMessage.body のサイズは OSK で入力できる最大長よりも大きい
		 *  ので、ここでは OSK に合わせます。
		 */
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_BODY,
			(uint16_t *)L"Body",
			CELL_OSKDIALOG_STRING_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_SEND)
	{
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.sendMembershipRequest_censor) {
			g_info.clansMessage.options |= SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		} else {
			g_info.clansMessage.options &= ~SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		}

		ret = sceNpClansSendMembershipRequest(
				g_info.clansHandle,
				g_info.selectedClanId, 
				&g_info.clansMessage
				);
		if (ret < 0) {
			DBG("sceNpClansSendMembershipRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansSendMembershipRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansCancelMembershipRequest(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
	if (ret < 0) {
		DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	ret = sceNpClansCancelMembershipRequest(
			g_info.clansHandle,
			g_info.selectedClanId
			);
	if (ret < 0) {
		DBG("sceNpClansCancelMembershipRequest() failed. ret = 0x%x\n", ret);
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"sceNpClansCancelMembershipRequest() failed. ret = 0x%x\n", ret);
		goto error;
	}

	cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
		"Success.\n");

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}




int
doClansAddBlacklistEntry(void)
{
	int ret=0;
	SceNpId npId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_NPID)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_ADD_BLACKLIST_NPID,
			(uint16_t *)L"OnlineId",
			SCE_NET_NP_ONLINEID_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_ADD)
	{
		ret = call_sceNpLookupNpIdAsync(&npId, &g_info.addBlacklistEntry_onlineId);
		if (ret < 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansAddBlacklistEntry(
				g_info.clansHandle,
				g_info.selectedClanId,
				&npId
				);
		if (ret < 0) {
			DBG("sceNpClansAddBlacklistEntry() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansAddBlacklistEntry() failed. ret = 0x%x\n", ret);
			goto error;
		}
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansRemoveBlacklistEntry(void)
{
	int ret=0;
	SceNpId npId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_NPID)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_REMOVE_BLACKLIST_NPID,
			(uint16_t *)L"OnlineId",
			SCE_NET_NP_ONLINEID_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_REMOVE)
	{
		ret = call_sceNpLookupNpIdAsync(&npId, &g_info.removeBlacklistEntry_onlineId);
		if (ret < 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansRemoveBlacklistEntry(
				g_info.clansHandle,
				g_info.selectedClanId,
				&npId
				);
		if (ret < 0) {
			DBG("sceNpClansRemoveBlacklistEntry() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansRemoveBlacklistEntry() failed. ret = 0x%x\n", ret);
			goto error;
		}
		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansPostAnnouncement(void)
{
	int ret=0;
	SceNpClansMessageId messageId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_SUBJECT)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_SUBJECT,
			(uint16_t *)L"Subject",
			SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_BODY)
	{
		/*J SceNpClansMessage.body のサイズは OSK で入力できる最大長よりも大きい
		 *  ので、ここでは OSK に合わせます。
		 */
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_BODY,
			(uint16_t *)L"Body",
			CELL_OSKDIALOG_STRING_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_POST)
	{
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.postMessage_censor) {
			g_info.clansMessage.options |= SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		} else {
			g_info.clansMessage.options &= ~SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		}

		ret = sceNpClansPostAnnouncement(
				g_info.clansHandle,
				g_info.selectedClanId,
				&g_info.clansMessage,
				NULL,
				g_info.postMessage_duration,
				&messageId
				);
		if (ret < 0) {
			DBG("sceNpClansPostAnnouncement() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansPostAnnouncement() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success. messageId = %d\n", messageId);
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansRemoveAnnouncement(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_REMOVE)
	{
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansRemoveAnnouncement(
				g_info.clansHandle,
				g_info.selectedClanId,
				g_info.messageId
				);
		if (ret < 0) {
			DBG("sceNpClansRemoveAnnouncement() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansRemoveAnnouncement() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success. messageId = %d\n", g_info.messageId);
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansRetrieveAnnouncements(void)
{
	int ret=0;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND:
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_TOTAL) {
			ret = sceNpClansRetrieveAnnouncements(
					g_info.clansHandle, g_info.selectedClanId, NULL, NULL, &result);
			if (ret < 0) {
				DBG("sceNpClansRetrieveAnnouncements() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansRetrieveAnnouncements() failed. ret = 0x%x\n", ret);
				goto error;
			}
	
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Announcments total = %d\n", result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_RETRIEVE)
		{
			g_info.announcementsList_num = 0;

			ret = sceNpClansRetrieveAnnouncements(
					g_info.clansHandle,
					g_info.selectedClanId,
					&g_info.retrieveAnnouncements_PagingRequest,
					g_info.messageEntry,
					&g_info.retrieveAnnouncements_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansRetrieveAnnouncements() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansRetrieveAnnouncements() failed. ret = 0x%x\n", ret);
				goto error;
			}

			g_info.announcementsList_num = g_info.retrieveAnnouncements_PagingResult.count;
		}
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}

int
doClansGetBlacklist(void)
{
	int ret=0;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND:
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_TOTAL) {
			ret = sceNpClansGetBlacklist(
					g_info.clansHandle, g_info.selectedClanId, NULL, NULL, &result);
			if (ret < 0) {
				DBG("sceNpClansGetBlacklist() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansGetBlacklist() failed. ret = 0x%x\n", ret);
				goto error;
			}
	
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Blacklist total = %d\n", result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_GET)
		{
			ret = sceNpClansGetBlacklist(
					g_info.clansHandle,
					g_info.selectedClanId,
					&g_info.getBlacklist_PagingRequest,
					g_info.blacklistEntry,
					&g_info.getBlacklist_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansGetBlacklist() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansGetBlacklist() failed. ret = 0x%x\n", ret);
				goto error;
			}
		}
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}

int
doClansGetMemberList(void)
{
	int ret=0;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND:
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_TOTAL) {
			ret = sceNpClansGetMemberList(
					g_info.clansHandle, g_info.selectedClanId, NULL, g_info.memberStatus, NULL, &result);
			if (ret < 0) {
				DBG("sceNpClansGetMemberList() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansGetMemberList() failed. ret = 0x%x\n", ret);
				goto error;
			}
	
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Member List total = %d\n", result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_GET)
		{
			ret = sceNpClansGetMemberList(
					g_info.clansHandle,
					g_info.selectedClanId,
					&g_info.getMemberList_PagingRequest,
					g_info.memberStatus,
					g_info.memberEntry,
					&g_info.getMemberList_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansGetMemberList() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansGetMemberList() failed. ret = 0x%x\n", ret);
				goto error;
			}
		}
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}

int
doClansGetMemberInfo(void)
{
	int ret=0;
	SceNpId npId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_NPID)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_GET_MEMBER_INFO_ONLINEID,
			(uint16_t *)L"OnlineId",
			SCE_NET_NP_ONLINEID_MAX_LENGTH);
	} else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_GET)
	{
		ret = call_sceNpLookupNpIdAsync(&npId, &g_info.getMemberInfo_onlineId);
		if (ret < 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		memset(&g_info.getMemberInfoEntry, 0x00, sizeof(g_info.getMemberInfoEntry));

		ret = sceNpClansGetMemberInfo(
				g_info.clansHandle,
				g_info.selectedClanId,
				&npId,
				&g_info.getMemberInfoEntry
				);
		if (ret < 0) {
			DBG("sceNpClansGetMemberInfo() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansGetMemberInfo() failed. ret = 0x%x\n", ret);
			goto error;
		}
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}


static void
fill_update_member_info_field(void)
{
	g_info.updatableMemberInfo.fields = 
		SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_DESCR |
		SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_BIN_ATTR1 |
		SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_ALLOW_MSG;
}


int
doClansUpdateMemberInfo(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	fill_update_member_info_field();

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_DESC)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_UPDATE_MEMBER_INFO_DESC,
			(uint16_t *)L"Description",
			SCE_NP_CLANS_CLAN_DESCRIPTION_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_BIN1)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_UPDATE_MEMBER_INFO_BIN_ATTR,
			(uint16_t *)L"binAttr1",
			SCE_NP_CLANS_CLAN_BINARY_ATTRIBUTE1_MAX_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_UPDATE)
	{
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansUpdateMemberInfo(
				g_info.clansHandle,
				g_info.selectedClanId, 
				&g_info.updatableMemberInfo
				);
		if (ret < 0) {
			DBG("sceNpClansUpdateMemberInfo() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansUpdateMemberInfo() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansChangeMemberRole(void)
{
	int ret=0;
	SceNpId npId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_NPID)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_CHANGE_MEMBER_ROLE_ONLINEID,
			(uint16_t *)L"OnlineId",
			SCE_NET_NP_ONLINEID_MAX_LENGTH);
	} else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_CHANGE)
	{
		ret = call_sceNpLookupNpIdAsync(&npId, &g_info.changeMemberRole_onlineId);
		if (ret < 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansChangeMemberRole(
				g_info.clansHandle,
				g_info.selectedClanId,
				&npId,
				g_info.memberRole
				);
		if (ret < 0) {
			DBG("sceNpClansChangeMemberRole() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansChangeMemberRole() failed. ret = 0x%x\n", ret);
			goto error;
		}
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}


int
doClansSendInvitationResponse(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_SUBJECT)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_SUBJECT,
			(uint16_t *)L"Subject",
			SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_BODY)
	{
		/*J SceNpClansMessage.body のサイズは OSK で入力できる最大長よりも大きい
		 *  ので、ここでは OSK に合わせます。
		 */
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_BODY,
			(uint16_t *)L"Body",
			CELL_OSKDIALOG_STRING_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_SEND)
	{
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.sendInvitationResponse_censor) {
			g_info.clansMessage.options |= SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		} else {
			g_info.clansMessage.options &= ~SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		}

		ret = sceNpClansSendInvitationResponse(
				g_info.clansHandle,
				g_info.selectedClanId, 
				&g_info.clansMessage,
				g_info.sendInvitationResponse_accept
				);
		if (ret < 0) {
			DBG("sceNpClansSendInvitationResponse() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansSendInvitationResponse() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansSendMembershipResponse(void)
{
	int ret=0;
	SceNpId npId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_NPID)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_SEND_MEMBERSHIP_NPID,
			(uint16_t *)L"NpId",
			SCE_NET_NP_ONLINEID_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_SUBJECT)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_SUBJECT,
			(uint16_t *)L"Subject",
			SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_BODY)
	{
		/*J SceNpClansMessage.body のサイズは OSK で入力できる最大長よりも大きい
		 *  ので、ここでは OSK に合わせます。
		 */
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_BODY,
			(uint16_t *)L"Body",
			CELL_OSKDIALOG_STRING_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_SEND)
	{
		ret = call_sceNpLookupNpIdAsync(&npId, &g_info.sendMembershipResponse_onlineId);
		if (ret < 0) {
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"call_sceNpLookupNpIdAsync() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.sendMembershipResponse_censor) {
			g_info.clansMessage.options |= SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		} else {
			g_info.clansMessage.options &= ~SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		}

		ret = sceNpClansSendMembershipResponse(
				g_info.clansHandle,
				g_info.selectedClanId, 
				&npId,
				&g_info.clansMessage,
				g_info.sendMembershipResponse_accept
				);
		if (ret < 0) {
			DBG("sceNpClansSendMembershipResponse() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansSendMembershipResponse() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success.\n");
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}


int
doClansPostChallenge(void)
{
	int ret=0;
	SceNpClansMessageId messageId;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;
	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_CLAN)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_POST_CHALLENGE_NAME,
			(uint16_t *)L"name",
			SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_SUBJECT)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_SUBJECT,
			(uint16_t *)L"Subject",
			SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_BODY)
	{
		/*J SceNpClansMessage.body のサイズは OSK で入力できる最大長よりも大きい
		 *  ので、ここでは OSK に合わせます。
		 */
		start_osk_dialog(
			NP_GUI_OSK_INPUT_MESSAGE_BODY,
			(uint16_t *)L"Body",
			CELL_OSKDIALOG_STRING_SIZE);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_POST)
	{
		SceNpClansPagingRequest paging;
		SceNpClansSearchableName search;
		SceNpClansClanBasicInfo info;
		SceNpClansPagingResult result;

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		paging.startPos = 0;
		paging.max = 1;
		search.nameSearchOp = SCE_NP_CLANS_SEARCH_OPERATOR_EQUAL_TO;
		strcpy(search.name, g_info.postChallengeClan);
		memset(&info, 0x00, sizeof(info));
		result.count = 1;
		result.total = 0;

		ret = sceNpClansSearchByName(g_info.clansHandle, &paging, &search, &info, &result);
		if (ret < 0) {
			DBG("sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
			goto error;
		}

		sceNpClansDestroyRequest(g_info.clansHandle);
		g_info.clansHandle = NULL;

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.postChallenge_censor) {
			g_info.clansMessage.options |= SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		} else {
			g_info.clansMessage.options &= ~SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR;
		}

		ret = sceNpClansPostChallenge(
				g_info.clansHandle,
				g_info.selectedClanId,
				info.clanId,
				&g_info.clansMessage,
				NULL,
				g_info.postChallenge_duration,
				&messageId
				);
		if (ret < 0) {
			DBG("sceNpClansPostChallenge() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansPostChallenge() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success. messageId = %d\n", messageId);
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansRemovePostedChallenge(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_CLAN)
	{
		start_osk_dialog(
			NP_GUI_OSK_INPUT_REMOVE_POSTED_CHALLENGE_NAME,
			(uint16_t *)L"name",
			SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH);
	}
	else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_REMOVE)
	{
		SceNpClansPagingRequest paging;
		SceNpClansSearchableName search;
		SceNpClansClanBasicInfo info;
		SceNpClansPagingResult result;

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		paging.startPos = 0;
		paging.max = 1;
		search.nameSearchOp = SCE_NP_CLANS_SEARCH_OPERATOR_EQUAL_TO;
		strcpy(search.name, g_info.removeChallengeClan);
		memset(&info, 0x00, sizeof(info));
		result.count = 1;
		result.total = 0;

		ret = sceNpClansSearchByName(g_info.clansHandle, &paging, &search, &info, &result);
		if (ret < 0) {
			DBG("sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
			goto error;
		}

		sceNpClansDestroyRequest(g_info.clansHandle);
		g_info.clansHandle = NULL;

		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansRemovePostedChallenge(
				g_info.clansHandle,
				g_info.selectedClanId,
				info.clanId,
				g_info.postedChallengeId
				);
		if (ret < 0) {
			DBG("sceNpClansRemovePostedChallenge() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansRemovePostedChallenge() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success. messageId = %d\n", g_info.postedChallengeId);
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansRetrievePostedChallenges(void)
{
	int ret=0;
	SceNpClansPagingRequest paging;
	SceNpClansSearchableName search;
	SceNpClansClanBasicInfo info;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND:
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_CLAN) {
			start_osk_dialog(
				NP_GUI_OSK_INPUT_RETRIEVE_POSTED_CHALLENGES_NAME,
				(uint16_t *)L"name",
				SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_TOTAL) {
			paging.startPos = 0;
			paging.max = 1;
			search.nameSearchOp = SCE_NP_CLANS_SEARCH_OPERATOR_EQUAL_TO;
			strcpy(search.name, g_info.retrieveChallengesClan);
			memset(&info, 0x00, sizeof(info));
			result.count = 1;
			result.total = 0;

			ret = sceNpClansSearchByName(g_info.clansHandle, &paging, &search, &info, &result);
			if (ret < 0) {
				DBG("sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansSearchByNAme() failed. ret = 0x%x\n", ret);
				goto error;
			}

			sceNpClansDestroyRequest(g_info.clansHandle);
			g_info.clansHandle = NULL;

			ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
			if (ret < 0) {
				DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
				goto error;
			}

			result.count = 0;
			result.total = 0;

			ret = sceNpClansRetrievePostedChallenges(
					g_info.clansHandle, g_info.selectedClanId, info.clanId, NULL, NULL, &result);
			if (ret < 0) {
				DBG("sceNpClansRetrievePostedChallenges() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansRetrievePostedChallenges() failed. ret = 0x%x\n", ret);
				goto error;
			}
	
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Posted Challenge total = %d\n", result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_RETRIEVE)
		{
			paging.startPos = 0;
			paging.max = 1;
			search.nameSearchOp = SCE_NP_CLANS_SEARCH_OPERATOR_EQUAL_TO;
			strcpy(search.name, g_info.retrieveChallengesClan);
			memset(&info, 0x00, sizeof(info));
			result.count = 1;
			result.total = 0;

			ret = sceNpClansSearchByName(g_info.clansHandle, &paging, &search, &info, &result);
			if (ret < 0) {
				DBG("sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansSearchByName() failed. ret = 0x%x\n", ret);
				goto error;
			}

			sceNpClansDestroyRequest(g_info.clansHandle);
			g_info.clansHandle = NULL;

			ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
			if (ret < 0) {
				DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
				goto error;
			}

			g_info.challengeList_num = 0;

			ret = sceNpClansRetrievePostedChallenges(
					g_info.clansHandle,
					g_info.selectedClanId,
					info.clanId,
					&g_info.retrievePostedChallenges_PagingRequest,
					g_info.postedChallengeEntry,
					&g_info.retrievePostedChallenges_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansRetrievePostedChallenges() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansRetrievePostedChallenges() failed. ret = 0x%x\n", ret);
				goto error;
			}

			g_info.postedChallengeList_num = g_info.retrievePostedChallenges_PagingResult.count;
		}
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}

int
doClansRemoveChallenge(void)
{
	int ret=0;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_REMOVE)
	{
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		ret = sceNpClansRemoveChallenge(
				g_info.clansHandle,
				g_info.selectedClanId,
				g_info.challengeId
				);
		if (ret < 0) {
			DBG("sceNpClansRemoveChallenge() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansRemoveChallenge() failed. ret = 0x%x\n", ret);
			goto error;
		}

		cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
			"Success. messageId = %d\n", g_info.challengeId);
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;
	return ret;
}

int
doClansRetrieveChallenges(void)
{
	int ret=0;
	SceNpClansPagingResult result;

	if (g_info.clansHandle != NULL) {
		DBG("Clans Request is busy...\n");
		return -1;
	}

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND)
		return -1;

	switch (g_info.pos_x) {
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND:
		ret = sceNpClansCreateRequest(&g_info.clansHandle, 0);
		if (ret < 0) {
			DBG("sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"sceNpClansCreateRequest() failed. ret = 0x%x\n", ret);
			goto error;
		}

		if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_TOTAL) {
			ret = sceNpClansRetrieveChallenges(
					g_info.clansHandle, g_info.selectedClanId, NULL, NULL, &result);
			if (ret < 0) {
				DBG("sceNpClansRetrieveChallenges() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansRetrieveChallenges() failed. ret = 0x%x\n", ret);
				goto error;
			}
	
			cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
				"Challenge total = %d\n", result.total);
		}
		else if (g_info.pos_y == NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_RETRIEVE)
		{
			g_info.challengeList_num = 0;

			ret = sceNpClansRetrieveChallenges(
					g_info.clansHandle,
					g_info.selectedClanId,
					&g_info.retrieveChallenges_PagingRequest,
					g_info.challengeEntry,
					&g_info.retrieveChallenges_PagingResult
					);
			if (ret < 0) {
				DBG("sceNpClansRetrieveChallenges() failed. ret = 0x%x\n", ret);
				cellDbgFontConsolePrintf(g_info.mDbgFontID_info,
					"sceNpClansRetrieveChallenges() failed. ret = 0x%x\n", ret);
				goto error;
			}

			g_info.challengeList_num = g_info.retrieveChallenges_PagingResult.count;
		}
		break;
	case NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST:
		break;
	default:
		break;
	}

error:
	if (g_info.clansHandle)
		sceNpClansDestroyRequest(g_info.clansHandle);
	g_info.clansHandle = NULL;

	return ret;
}
