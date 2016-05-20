/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <sys/memory.h>
#include <sysutil/sysutil_oskdialog.h>

#include "np_gui.h"

#include <stdio.h>
#include <string.h>

#define ucs2_to_ascii(c, w, charsNum) { \
	int i; \
	char *ptr = (char *)w; \
	for (i=0; i<charsNum; i++) { \
		c[i] = ptr[i*2 + 1]; \
	} \
}

extern struct np_gui_info g_info;
static enum np_gui_osk_input osk_mode = NP_GUI_OSK_INPUT_INVALID;
int numCharLimit = 0;

void
start_osk_dialog(
	enum np_gui_osk_input input,
	uint16_t *guide_message,
	int limit
	)
{
	CellOskDialogInputFieldInfo fieldInfo;
	CellOskDialogParam param;
	CellOskDialogPoint pos;
	int ret = 0;

	if (osk_mode != NP_GUI_OSK_INPUT_INVALID)
		return;

	fieldInfo.message = guide_message;
	fieldInfo.init_text = (uint16_t *)"";
	fieldInfo.limit_length = limit;

	pos.x = 0.0;
	pos.y = 0x0;

	param.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_ALPHABET |
							 CELL_OSKDIALOG_PANELMODE_ALPHABET |
							 CELL_OSKDIALOG_PANELMODE_ENGLISH;
	param.firstViewPanel = CELL_OSKDIALOG_PANELMODE_ALPHABET;
	param.controlPoint = pos;
	param.prohibitFlgs = 0;

	osk_mode = input;
	numCharLimit = limit;

	ret = cellOskDialogLoadAsync(SYS_MEMORY_CONTAINER_ID_INVALID, &param, &fieldInfo);
	if(ret < 0){
		printf("cellOskDialogLoadAsync() failed. ret = 0x%x\n", ret);
		goto error;
	}

	return;

error:
	osk_mode = NP_GUI_OSK_INPUT_INVALID;
	numCharLimit = 0;

	return;
}

static uint16_t tmp_buff[256];
void
finish_osk_dialog(
	void
	)
{
	CellOskDialogCallbackReturnParam outputInfo;
	int ret = 0;

	memset(tmp_buff, 0x00, sizeof(tmp_buff));
	memset(&outputInfo, 0x00, sizeof(outputInfo));
	outputInfo.numCharsResultString = numCharLimit;
	outputInfo.pResultString =  tmp_buff;

	ret = cellOskDialogUnloadAsync(&outputInfo);
	if(ret < 0){
		printf("cellOskDialogUnloadAsync() failed. ret = 0x%x\n", ret);
		goto error;
	}
	if(outputInfo.result != CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK){
		printf("result of osk dialog = 0x%x\n", outputInfo.result);
		goto error;
	}

	switch (osk_mode) {
	case NP_GUI_OSK_INPUT_CREATE_CLAN_NAME:
		ucs2_to_ascii(g_info.clans_create_clan_name,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_CREATE_CLAN_TAG:
		ucs2_to_ascii(g_info.clans_create_clan_tag,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_GET_CLAN_LIST_BY_NPID_NPID:
		ucs2_to_ascii(g_info.getClanListByNpId_onlineId.data,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_SEARCH_BIN_ATTR:
		ucs2_to_ascii(g_info.searchProf.attr.binAttr1,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_SEARCH_TAG:
		ucs2_to_ascii(g_info.searchProf.tag,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_SEARCH_NAME:
		ucs2_to_ascii(g_info.searchName.name,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_UPDATE_BIN_ATTR:
		ucs2_to_ascii(g_info.updatableClanInfo.attr.binAttr1,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_UPDATE_BIN_DATA:
		ucs2_to_ascii(g_info.updatableClanInfo.binData1,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_UPDATE_DESC:
		ucs2_to_ascii(g_info.updatableClanInfo.description,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_SEND_INVITATION_NPID:
		ucs2_to_ascii(g_info.sendInvitation_onlineId.data,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_MESSAGE_SUBJECT:
		ucs2_to_ascii(g_info.clansMessage.subject,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_MESSAGE_BODY:
		ucs2_to_ascii(g_info.clansMessage.body,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_ADD_BLACKLIST_NPID:
		ucs2_to_ascii(g_info.addBlacklistEntry_onlineId.data,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_REMOVE_BLACKLIST_NPID:
		ucs2_to_ascii(g_info.removeBlacklistEntry_onlineId.data,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_GET_MEMBER_INFO_ONLINEID:
		ucs2_to_ascii(g_info.getMemberInfo_onlineId.data,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_UPDATE_MEMBER_INFO_DESC:
		ucs2_to_ascii(g_info.updatableMemberInfo.description,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_UPDATE_MEMBER_INFO_BIN_ATTR:
		ucs2_to_ascii(g_info.updatableMemberInfo.binAttr1,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_CHANGE_MEMBER_ROLE_ONLINEID:
		ucs2_to_ascii(g_info.changeMemberRole_onlineId.data,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_SEND_MEMBERSHIP_NPID:
		ucs2_to_ascii(g_info.sendMembershipResponse_onlineId.data,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_KICK_MEMBER_NPID:
		ucs2_to_ascii(g_info.kickMember_onlineId.data,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_KICK_MEMBER_SUBJECT:
		ucs2_to_ascii(g_info.kickMemberMessage.subject,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_KICK_MEMBER_BODY:
		ucs2_to_ascii(g_info.kickMemberMessage.body,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_POST_CHALLENGE_NAME:
		ucs2_to_ascii(g_info.postChallengeClan,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_REMOVE_POSTED_CHALLENGE_NAME:
		ucs2_to_ascii(g_info.removeChallengeClan,
			outputInfo.pResultString, numCharLimit);
		break;
	case NP_GUI_OSK_INPUT_RETRIEVE_POSTED_CHALLENGES_NAME:
		ucs2_to_ascii(g_info.retrieveChallengesClan,
			outputInfo.pResultString, numCharLimit);
		break;
	default:
		break;
	}

error:
	osk_mode = NP_GUI_OSK_INPUT_INVALID;
	numCharLimit= 0;
	return;
}

void
term_osk_dialog(
	void
	)
{
}


