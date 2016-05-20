/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <sys/synchronization.h>


#include <stdio.h>
#include <string.h>

#include <cell/rtc.h>
#include <np.h>

#include "np_gui.h"
#include "np_gui_draw.h"
#include "strings.h"


extern struct np_gui_info g_info;

static const char *action_menu_str[] = {
	"CREATE CLAN",
	"GET CLAN LIST",
	"GET CLAN LIST BY NPID",
	"SEARCH BY PROFILE",
	"SEARCH BY NAME",
	NULL
};

static const char *clanmode_action_menu_str[] = {
	"DISBAND",
	"GET CLAN INFO",
	"UPDATE CLAN INFO",
	"GET AUTO ACCEPT STATUS",
	"UPDATE AUTO ACCEPT STATUS",
	"GET MEMBER LIST",
	"GET MEMBER INFO",
	"UPDATE MEMBER INFO",
	"CHANGE MEMBER ROLE",
	"KICK MEMBER",
	"JOIN CLAN",
	"LEAVE CLAN",
	"SEND INVITATION REQ",
	"CANCEL INVITATION",
	"SEND INVITATION RES",
	"SEND MEMBERSHIP REQ",
	"CANCEL MEMBERSHIP",
	"SEND MEMBERSHIP RES",
	"GET BLACKLIST",
	"ADD BLACKLIST",
	"REMOVE BLACKLIST ENTRY",
	"RETRIEVE ANNOUNCEMENTS",
	"POST ANNOUNCEMENT",
	"REMOVE ANNOUNCEMENT",
	"POST CHALLENGE",
	"RETRIEVE POSTED CHALLENGES",
	"REMOVE POSTED CHALLENGE",
	"RETRIEVE CHALLENGES",
	"REMOVE CHALLENGE",
	NULL
};


static const char *search_op_str[] = {
	"---",
	"EQUAL TO",
	"NOT EQUAL TO",
	"GREATER THAN",
	"GREATER THAN OR EQUAL TO",
	"LESS THAN",
	"LESS THAN OR EQUAL TO",
	"LIKE",
	NULL
};


static const char *member_list_status_str[] = {
	"UNKNOWN",
	"NORMAL",
	"INVITED",
	"PENDING",
	NULL
};

static const char *member_list_role_str[] = {
	"UNKNOWN",
	"NON MEMBER",
	"MEMBER",
	"SUB LEADER",
	"LEADER",
	NULL
};



static char buf[256];

static const char *
SearchOp2Str(SceNpClansSearchOperator op)
{
	if (op < 0 || op > 6)
		op = -1;

	return search_op_str[op+1];
}

void
clans_draw_clanlist(void)
{
	unsigned int i;
	SceNpClansEntry *clans;
	SceNpClansClanBasicInfo *info;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_CLANLIST) {
		a = UNFOCUSED_A;
	}

	memset(buf, 0x00, sizeof(buf));

	switch (g_info.pos_act_y) {
	case NP_GUI_MODE_CLANS_ACT_POS_Y_CREATE_CLAN:
	case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST:
	case NP_GUI_MODE_CLANS_ACT_POS_Y_GET_CLAN_LIST_BY_NPID:
		snprintf(buf, sizeof(buf)-1,
				"[ Clan List ] (%d/%d)",
				g_info.getClanList_PagingResult.count,
				g_info.getClanList_PagingResult.total);

		ypos = YSTART + DEFAULT_YCHAR_INTERVAL * 6;
		cellDbgFontPuts(STRING_X_CLAN_LIST_TITLE, ypos,
			DEFAULT_SCALE, (a|COLOR_WHITE), buf);

		clans = g_info.clanList;
		for (i=0; i<(unsigned int)g_info.clanlist_num; i++) {
			ypos += S_DEFAULT_YCHAR_INTERVAL(DEFAULT_SCALE) * 2;
			cellDbgFontPrintf(STRING_X_CLAN_LIST_TITLE,
				ypos, DEFAULT_SCALE,
				MENU_COLOR_WITH_A((int)i, g_info.pos_slot_y, (a|FOCUSED_COLOR), a),
				"[%s] %s", clans[i].info.tag, clans[i].info.name);
		}
		break;
	case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_PROFILE:
	case NP_GUI_MODE_CLANS_ACT_POS_Y_SEARCH_BY_NAME:
		snprintf(buf, sizeof(buf)-1,
			"[ Search Result ] (%d/%d)",
			g_info.search_PagingResult.count,
			g_info.search_PagingResult.total);

		ypos = YSTART + DEFAULT_YCHAR_INTERVAL * 6;
		cellDbgFontPuts(STRING_X_CLAN_LIST_TITLE, ypos,
			DEFAULT_SCALE, (a|COLOR_WHITE), buf);

		info = g_info.clanBasicInfoList;
		for (i=0; i<(unsigned int)g_info.basicInfo_num; i++) {
			ypos += S_DEFAULT_YCHAR_INTERVAL(DEFAULT_SCALE);
			cellDbgFontPrintf(STRING_X_CLAN_LIST_TITLE,
				ypos, DEFAULT_SCALE,
				MENU_COLOR_WITH_A((int)i, g_info.pos_slot_y, (a|FOCUSED_COLOR), a),
				"[%s] %s", info[i].tag, info[i].name);
		}
		break;
	default:
		break;
	}

	return;
}


void
clans_draw_anlist(void)
{
	unsigned int i;
	SceNpClansMessageEntry *entry;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST) {
		a = UNFOCUSED_A;
	}

	snprintf(buf, sizeof(buf)-1,
		"[ Announcements ] (%d/%d)",
		g_info.retrieveAnnouncements_PagingResult.count,
		g_info.retrieveAnnouncements_PagingResult.total);

	ypos = YSTART + DEFAULT_YCHAR_INTERVAL * 6;
	cellDbgFontPuts(STRING_X_CLAN_LIST_TITLE, ypos,
		DEFAULT_SCALE, (a|COLOR_WHITE), buf);

	entry = g_info.messageEntry;
	for (i=0; i<(unsigned int)g_info.announcementsList_num; i++) {
		ypos += S_DEFAULT_YCHAR_INTERVAL(DEFAULT_SCALE);
		cellDbgFontPrintf(STRING_X_CLAN_LIST_TITLE,
			ypos, DEFAULT_SCALE,
			MENU_COLOR_WITH_A((int)i, g_info.pos_slot_y, (a|FOCUSED_COLOR), a),
			"[%d:%s] %s", entry[i].mId, entry[i].npid.handle.data, entry[i].message.subject);
	}

	return;
}


void
clans_draw_chlist(void)
{
	unsigned int i;
	SceNpClansMessageEntry *entry;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST) {
		a = UNFOCUSED_A;
	}

	snprintf(buf, sizeof(buf)-1,
		"[ Challenges ] (%d/%d)",
		g_info.retrieveChallenges_PagingResult.count,
		g_info.retrieveChallenges_PagingResult.total);

	ypos = YSTART + DEFAULT_YCHAR_INTERVAL * 6;
	cellDbgFontPuts(STRING_X_CLAN_LIST_TITLE, ypos,
		DEFAULT_SCALE, (a|COLOR_WHITE), buf);

	entry = g_info.challengeEntry;
	for (i=0; i<(unsigned int)g_info.challengeList_num; i++) {
		ypos += S_DEFAULT_YCHAR_INTERVAL(DEFAULT_SCALE);
		cellDbgFontPrintf(STRING_X_CLAN_LIST_TITLE,
			ypos, DEFAULT_SCALE,
			MENU_COLOR_WITH_A((int)i, g_info.pos_slot_y, (a|FOCUSED_COLOR), a),
			"[%d:%s] %s", entry[i].mId, entry[i].npid.handle.data, entry[i].message.subject);
	}

	return;
}


void
clans_draw_posted_chlist(void)
{
	unsigned int i;
	SceNpClansMessageEntry *entry;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST) {
		a = UNFOCUSED_A;
	}

	snprintf(buf, sizeof(buf)-1,
		"[ Posted Challenges ] (%d/%d)",
		g_info.retrievePostedChallenges_PagingResult.count,
		g_info.retrievePostedChallenges_PagingResult.total);

	ypos = YSTART + DEFAULT_YCHAR_INTERVAL * 6;
	cellDbgFontPuts(STRING_X_CLAN_LIST_TITLE, ypos,
		DEFAULT_SCALE, (a|COLOR_WHITE), buf);

	entry = g_info.postedChallengeEntry;
	for (i=0; i<(unsigned int)g_info.postedChallengeList_num; i++) {
		ypos += S_DEFAULT_YCHAR_INTERVAL(DEFAULT_SCALE);
		cellDbgFontPrintf(STRING_X_CLAN_LIST_TITLE,
			ypos, DEFAULT_SCALE,
			MENU_COLOR_WITH_A((int)i, g_info.pos_slot_y, (a|FOCUSED_COLOR), a),
			"[%d:%s] %s", entry[i].mId, entry[i].npid.handle.data, entry[i].message.subject);
	}

	return;
}


void
clans_draw_blacklist(void)
{
	unsigned int i;
	SceNpClansBlacklistEntry *entry;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST) {
		a = UNFOCUSED_A;
	}
	snprintf(buf, sizeof(buf)-1,
		"[ Blacklist ] (%d/%d)",
		g_info.getBlacklist_PagingResult.count,
		g_info.getBlacklist_PagingResult.total);

	ypos = YSTART + DEFAULT_YCHAR_INTERVAL * 6;
	cellDbgFontPuts(STRING_X_CLAN_LIST_TITLE, ypos,
		DEFAULT_SCALE, (a|COLOR_WHITE), buf);

	entry = g_info.blacklistEntry;
	for (i=0; i<(unsigned int)g_info.getBlacklist_PagingResult.count; i++) {
		ypos += S_DEFAULT_YCHAR_INTERVAL(DEFAULT_SCALE);
		cellDbgFontPrintf(STRING_X_CLAN_LIST_TITLE,
			ypos, DEFAULT_SCALE,
			(a|COLOR_WHITE),
			"%s (by %s)", entry[i].entry.handle.data, entry[i].registeredBy.handle.data);
	}

	return;
}

void
clans_draw_memberlist(void)
{
	unsigned int i;
	SceNpClansMemberEntry *entry;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_LIST) {
		a = UNFOCUSED_A;
	}

	snprintf(buf, sizeof(buf)-1,
		"[ Member List ] (%d/%d)",
		g_info.getMemberList_PagingResult.count,
		g_info.getMemberList_PagingResult.total);

	ypos = YSTART + DEFAULT_YCHAR_INTERVAL * 6;
	cellDbgFontPuts(STRING_X_CLAN_LIST_TITLE, ypos,
		DEFAULT_SCALE, (a|COLOR_WHITE), buf);

	entry = g_info.memberEntry;
	for (i=0; i<(unsigned int)g_info.getMemberList_PagingResult.count; i++) {
		ypos += S_DEFAULT_YCHAR_INTERVAL(DEFAULT_SCALE);
		cellDbgFontPrintf(STRING_X_CLAN_LIST_TITLE,
			ypos, DEFAULT_SCALE,
			(a|COLOR_WHITE),
			"%s", entry[i].npid.handle.data);
	}

	return;
}


void
clans_draw_action_menu(
	void
	)
{
	int i, row;
	uint32_t a = DEF_A;

	row = 6;

	a = DEF_A;
	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_ACT) {
		a = UNFOCUSED_A;
	}
	cellDbgFontPuts(XSTART,
					YSTART + DEFAULT_YCHAR_INTERVAL * row, CLANS_ACTION_SCALE,
					(a|COLOR_WHITE),
					"[ Clans Action]");


	a = DEF_A;
	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}
	cellDbgFontPuts(CENTERING_X_POS("[ Parameters ]"),
					YSTART + DEFAULT_YCHAR_INTERVAL * row, CLANS_ACTION_SCALE,
					(a|COLOR_WHITE),
					"[ Parameters ]");

	row = 8;

	a = DEF_A;
	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_ACT) {
		a = UNFOCUSED_A;
	}
	for (i = 0; i <= NP_GUI_MODE_CLANS_ACT_POS_Y_MAX; i++) {
		cellDbgFontPrintf(XSTART,
						  YSTART + DEFAULT_YCHAR_INTERVAL * (row+i), CLANS_ACTION_SCALE,
						  MENU_COLOR_WITH_A(i, g_info.pos_act_y, (a|FOCUSED_COLOR), a),
						  "%s",
						  action_menu_str[i]);
	}

	return;
}

static const char *clans_bottom_menu_str[2] = {
	MODE_CLANS_TOP_STRING_BOTTOM_MENU_ENTER_CIRCLE,
	MODE_CLANS_TOP_STRING_BOTTOM_MENU_ENTER_CROSS
};

void
clans_draw_bottom_menu(
	void
	)
{
	switch (g_info.clans_state) {
	case NP_GUI_CLANS_STATE_TOP:
		memset(buf, 0, sizeof(buf));
		snprintf(buf, 256, clans_bottom_menu_str[g_info.button_assign]);
		cellDbgFontPuts(CENTERING_X_POS(buf),
						LINE_BOTTOM_Y, DEFAULT_SCALE,
						(DEF_A|COLOR_LIGHT_BLUE),
						buf);
		break;
	default:
		break;
	}

	return;
}


void
clans_draw_create_clan(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "NAME:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NAME:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_NAME,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clans_create_clan_name);

	/* print "TAG:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "TAG:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_TAG,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clans_create_clan_tag);

	/* print "CRAETE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 4;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CREATE_CLAN_ACT_POS_Y_CREATE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"CREATE");

	return;
}


void
clans_draw_get_clan_list(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "StrtPos:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.getClanList_PagingRequest.startPos);

	/* print "Max:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.getClanList_PagingRequest.max);

	/* print "TOTAL" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 4;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print "GET" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_ACT_POS_Y_GET,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"GET");

	return;
}

void
clans_draw_get_clan_list_by_npid(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "NpId:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) *2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_NPID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.getClanListByNpId_onlineId.data);

	/* print "StrtPos:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.getClanListByNpId_PagingRequest.startPos);

	/* print "Max:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.getClanListByNpId_PagingRequest.max);

	/* print "TOTAL" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 4;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print "GET" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_GET_CLAN_LIST_BY_NPID_ACT_POS_Y_GET,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"GET");

	return;
}

void
clans_draw_search_by_profile(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE) * row;
	/* intAttr1 */
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "intAttr1:");
	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT1,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%d", g_info.searchProf.attr.intAttr1);

	/* intAttr1 OP */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST+S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*5,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT1_OP,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", SearchOp2Str(g_info.searchProf.intAttr1SearchOp));

	/* intAttr2 */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "intAttr2:");
	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT2,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%d", g_info.searchProf.attr.intAttr2);

	/* intAttr2 OP */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST+S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*5,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT2_OP,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", SearchOp2Str(g_info.searchProf.intAttr2SearchOp));

	/* intAttr3 */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "intAttr3:");
	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT3,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%d", g_info.searchProf.attr.intAttr3);

	/* intAttr3 OP */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST+S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*5,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_INT3_OP,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", SearchOp2Str(g_info.searchProf.intAttr3SearchOp));

	/* binAttr1 */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "binAttr1:");
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*10,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_BIN1,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.searchProf.attr.binAttr1);

	/* binAttr3 OP */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST+S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*5,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_BIN1_OP,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", SearchOp2Str(g_info.searchProf.binAttr1SearchOp));

	/* numMembers */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "numMembers:");
	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MEMBER,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%d", g_info.searchProf.numMembers);

	/* numMembers OP */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST+S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*5,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_MEMBER_OP,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", SearchOp2Str(g_info.searchProf.numMembersSearchOp));

	/* tag */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "tag:");
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*4,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TAG,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.searchProf.tag);

	/* tag OP */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST+S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*5,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TAG_OP,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", SearchOp2Str(g_info.searchProf.tagSearchOp));

	/* print "StrtPos:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.searchByProfile_PagingRequest.startPos);

	/* print "Max:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.searchByProfile_PagingRequest.max);

	/* print TOTAL */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE) * 4,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print SEARCH */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE) * 4,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_PROFILE_ACT_POS_Y_SEARCH,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"SEARCH");

	return;
}


void
clans_draw_search_by_name(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE) * row;

	/* Name */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "Name:");
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*6,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_NAME,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.searchName.name);

	/* Name OP */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST+S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE)*5,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_NAME_OP,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", SearchOp2Str(g_info.searchName.nameSearchOp));

	/* print "StrtPos:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.searchByName_PagingRequest.startPos);

	/* print "Max:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, CLANS_SEARCH_COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.searchByName_PagingRequest.max);

	/* print TOTAL */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE) * 4,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print SEARCH */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(CLANS_SEARCH_COND_SCALE) * 4,
		ypos, CLANS_SEARCH_COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_SEARCH_BY_NAME_ACT_POS_Y_SEARCH,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"SEARCH");

	return;
}



///// clan mode
void
clans_draw_clanmode_action_menu(void)
{
	int i, k, row;
	uint32_t a = DEF_A;

	row = 6;

	if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_ACT)
		a = DEF_A;
	else
		a = UNFOCUSED_A;
	cellDbgFontPuts(XSTART,
					YSTART + DEFAULT_YCHAR_INTERVAL * row, CLANS_ACTION_SCALE,
					(a|COLOR_WHITE),
					"[ Clans Mode Action ]");


	if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_COND)
		a = DEF_A;
	else
		a = UNFOCUSED_A;
	cellDbgFontPuts(CENTERING_X_POS("[ Parameters ]"),
					YSTART + DEFAULT_YCHAR_INTERVAL * row, CLANS_ACTION_SCALE,
					(a|COLOR_WHITE),
					"[ Parameters ]");

	row+=2;
	if (g_info.pos_x == NP_GUI_MODE_CLANS_TOP_POS_X_ACT)
		a = DEF_A;
	else
		a = UNFOCUSED_A;
	if (g_info.pos_y_clans_menu_top != 0)
		cellDbgFontPrintf(XSTART, YSTART + DEFAULT_YCHAR_INTERVAL * (row), CLANS_ACTION_SCALE,
						  (a|COLOR_WHITE), "         ^         ");

	row++;
	for (i = g_info.pos_y_clans_menu_top, k=0;
		i <= NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_MAX && k< NP_GUI_MODE_CLANS_CLANMODE_ACT_DISP_MAX ;
		i++, k++) {
		cellDbgFontPrintf(XSTART,
						  YSTART + DEFAULT_YCHAR_INTERVAL * (row+k), CLANS_ACTION_SCALE,
						  MENU_COLOR_WITH_A(i, g_info.pos_act_y, (a|FOCUSED_COLOR), a),
						  "%s",
						  clanmode_action_menu_str[i]);
	}

	if (i != NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_MAX + 1)
		cellDbgFontPrintf(XSTART, YSTART + DEFAULT_YCHAR_INTERVAL * (row+k), CLANS_ACTION_SCALE,
						  (a|COLOR_WHITE), "         v         ");

	return;
}


void
clans_draw_clanmode_disband(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS + 5;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* print "DISBAND" */
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 5,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"DISBAND");

	return;
}


void
clans_draw_clanmode_get_info(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	if (g_info.clanInfo.info.name[0] == '\0')
		return;

	row = STRING_ROW_CONDITIONS;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* print date: */
	memset(buf, 0x00, sizeof(buf));
	cellRtcFormatRfc2822LocalTime(buf, &g_info.clanInfo.dateCreated);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"date: %s", buf);

	/* print clan id: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"clan id: %d", g_info.clanInfo.info.clanId);

	/* print id num mem: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"num mem: %d", g_info.clanInfo.info.numMembers);

	/* print name: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"name: %s", g_info.clanInfo.info.name);

	/* print tag: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"tag: %s", g_info.clanInfo.info.tag);

	/* print updatable fields: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"updatable field: 0x%x", g_info.clanInfo.updatable.fields);

	/* print description: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"desc: %s", g_info.clanInfo.updatable.description);

	/* print attr fields: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"attr fields: 0x%x", g_info.clanInfo.updatable.attr.fields);

	/* print intAttr1: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"intAttr1: %d", g_info.clanInfo.updatable.attr.intAttr1);

	/* print intAttr2: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"intAttr2: %d", g_info.clanInfo.updatable.attr.intAttr2);

	/* print intAttr3: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"intAttr3: %d", g_info.clanInfo.updatable.attr.intAttr3);

	/* print binAttr1: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"binAttr1: %s", g_info.clanInfo.updatable.attr.binAttr1);

	/* print binData1: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"binData1: %s", g_info.clanInfo.updatable.binData1);

	/* print binData1 size: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"binData1 size: %d", g_info.clanInfo.updatable.binData1Size);

	return;
}

void
clans_draw_clanmode_update_clan_info(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* description */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "description:");
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE)*13,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_DESC,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.updatableClanInfo.description);

	/* intAttr1 */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "intAttr1:");
	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT1,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%d", g_info.updatableClanInfo.attr.intAttr1);

	/* intAttr2 */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "intAttr2:");
	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT2,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%d", g_info.updatableClanInfo.attr.intAttr2);

	/* intAttr3 */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "intAttr3:");
	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_INT3,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%d", g_info.updatableClanInfo.attr.intAttr3);

	/* binAttr1 */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "binAttr1:");
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE)*10,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_BIN1,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.updatableClanInfo.attr.binAttr1);

	/* print UPDATE */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 4,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_CLAN_INFO_ACT_POS_Y_UPDATE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"UPDATE");

	return;
}

void
clans_draw_clanmode_get_auto_accept(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	if (g_info.haveAutoAcceptStatus == false)
		return;

	row = STRING_ROW_CONDITIONS;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* print date: */
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"auto-accept: %s", (g_info.autoAccept)?"true":"false");

	return;
}

void
clans_draw_clanmode_update_auto_accept(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* description */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "auto-accept:");
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE)*13,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_AUTO_ACCEPT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", (g_info.updateAutoAccept) ? "true" : "false");

	/* print UPDATE */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 4,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_AUTO_ACCEPT_ACT_POS_Y_UPDATE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"UPDATE");

	return;
}

void
clans_draw_clanmode_send_invitation(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "NpId:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_ONLINEID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.sendInvitation_onlineId.data);

	/* print "SUBJECT:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "SUBJECT:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_SUBJECT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.subject);

	/* print "BODY:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "BODY:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_BODY,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.body);

	/* print "CENSOR:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CENSOR:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_CENSOR,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.sendInvitation_censor)?"true":"false");

	/* print "SEND" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_ACT_POS_Y_SEND,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"SEND");

	return;
}

void
clans_draw_clanmode_cancel_invitation(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_ONLINEID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.sendInvitation_onlineId.data);

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	/* print "CANCEL INVITATION" */
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 5,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_CANCEL_INVITATION_ACT_POS_Y_CANCEL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"CANCEL INVITATION");

	return;
}

void
clans_draw_clanmode_send_membership_request(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "SUBJECT:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "SUBJECT:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_SUBJECT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.subject);

	/* print "BODY:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "BODY:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_BODY,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.body);

	/* print "CENSOR:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CENSOR:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_CENSOR,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.sendMembershipRequest_censor)?"true":"false");

	/* print "SEND" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEM_REQ_ACT_POS_Y_SEND,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"SEND");

	return;
}

void
clans_draw_clanmode_cancel_membership(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS + 5;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* print "CANCEL MEMBERSHIP" */
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 5,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"CANCEL MEMBERSHIP");

	return;
}


void
clans_draw_clanmode_add_blacklist_entry(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "NpId:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_NPID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.addBlacklistEntry_onlineId.data);

	/* print "ADD" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_ADD_BLACKLIST_ACT_POS_Y_ADD,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"ADD");

	return;
}

void
clans_draw_clanmode_remove_blacklist_entry(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "NpId:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_NPID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.removeBlacklistEntry_onlineId.data);

	/* print "REMOVE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_BLACKLIST_ACT_POS_Y_REMOVE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"REMOVE");

	return;
}

void
clans_draw_clanmode_post_announcement(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "SUBJECT:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "SUBJECT:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_SUBJECT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.subject);

	/* print "BODY:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "BODY:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_BODY,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.body);

	/* print "Duration:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "DURATION:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_DURATION,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.postMessage_duration);

	/* print "CENSOR:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CENSOR:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_CENSOR,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.postMessage_censor)?"true":"false");

	/* print "POST" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_ANNOUNCEMENT_ACT_POS_Y_POST,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"POST");

	return;
}

void
clans_draw_clanmode_remove_announcement(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "ANNOUNCEMENT ID:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "ANNOUNCEMENT ID:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_ID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.messageId);

	/* print "REMOVE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_ANNOUNCEMENT_ACT_POS_Y_REMOVE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"REMOVE");

	return;
}

void
clans_draw_clanmode_retrieve_announcements(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "StrtPos:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.retrieveAnnouncements_PagingRequest.startPos);

	/* print "Max:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.retrieveAnnouncements_PagingRequest.max);

	/* print "TOTAL" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 4;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print "RETRIEVE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_ANNOUNCEMENTS_ACT_POS_Y_RETRIEVE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"RETRIEVE");

	return;
}

void
clans_draw_clanmode_post_challenge(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "CLAN:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CLAN:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_CLAN,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.postChallengeClan);

	/* print "SUBJECT:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "SUBJECT:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_SUBJECT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.subject);

	/* print "BODY:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "BODY:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_BODY,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.body);

	/* print "Duration:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "DURATION:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_DURATION,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.postChallenge_duration);

	/* print "CENSOR:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CENSOR:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_CENSOR,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.postChallenge_censor)?"true":"false");

	/* print "POST" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_POST_CHALLENGE_ACT_POS_Y_POST,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"POST");

	return;
}

void
clans_draw_clanmode_remove_posted_challenge(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "CLAN:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CLAN:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_CLAN,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.removeChallengeClan);

	/* print "CHALLENGE ID:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CHALLENGE ID:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_ID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.postedChallengeId);

	/* print "REMOVE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_POSTED_CHALLENGE_ACT_POS_Y_REMOVE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"REMOVE");

	return;
}

void
clans_draw_clanmode_remove_challenge(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "CHALLENGE ID:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CHALLENGE ID:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_ID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.challengeId);

	/* print "REMOVE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_REMOVE_CHALLENGE_ACT_POS_Y_REMOVE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"REMOVE");

	return;
}

void
clans_draw_clanmode_retrieve_posted_challenges(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "CLAN:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CLAN:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_CLAN,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.retrieveChallengesClan);

	/* print "StrtPos:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.retrievePostedChallenges_PagingRequest.startPos);

	/* print "Max:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.retrievePostedChallenges_PagingRequest.max);

	/* print "TOTAL" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 4;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print "RETRIEVE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_POSTED_CHALLENGES_ACT_POS_Y_RETRIEVE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"RETRIEVE");

	return;
}

void
clans_draw_clanmode_retrieve_challenges(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "StrtPos:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.retrieveChallenges_PagingRequest.startPos);

	/* print "Max:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.retrieveChallenges_PagingRequest.max);

	/* print "TOTAL" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 4;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print "RETRIEVE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_RETRIEVE_CHALLENGES_ACT_POS_Y_RETRIEVE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"RETRIEVE");

	return;
}

void
clans_draw_clanmode_get_blacklist(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "StrtPos:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.getBlacklist_PagingRequest.startPos);

	/* print "MAX:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.getBlacklist_PagingRequest.max);

	/* print "TOTAL" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 4;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print "GET" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_BLACKLIST_ACT_POS_Y_GET,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"GET");

	return;
}

void
clans_draw_clanmode_get_member_list(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "StrtPos:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "StartPos:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_PAGE_START,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.getMemberList_PagingRequest.startPos);

	/* print "Max:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "MAX:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_PAGE_MAX,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%u", g_info.getMemberList_PagingRequest.max);

	/* print "STATUS:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "STATUS:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_STATUS,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", member_list_status_str[g_info.memberStatus]);

	/* print "TOTAL" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 4;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_TOTAL,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"TOTAL");

	/* print "GET" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBERLIST_ACT_POS_Y_GET,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"GET");

	return;
}


void
clans_draw_clanmode_get_member_info(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* print NpId: */
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) *2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_NPID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.getMemberInfo_onlineId.data);

	/* print "GET" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_GET_MEMBER_INFO_ACT_POS_Y_GET,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"GET");

	if (g_info.getMemberInfoEntry.npid.handle.data[0] == '\0')
		return;

	/* print NpId: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"NpId: %s", g_info.getMemberInfoEntry.npid.handle.data);

	/* print role: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"role: %s", member_list_role_str[g_info.getMemberInfoEntry.role]);

	/* print status: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"status: %s", member_list_status_str[g_info.getMemberInfoEntry.status]);

	/* print updatable field: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"updatable field: 0x%x", g_info.getMemberInfoEntry.updatable.fields);

	/* print binData1: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"binData1: %s", g_info.getMemberInfoEntry.updatable.binData1);

	/* print binData1Size: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"binData1Size: %d", g_info.getMemberInfoEntry.updatable.binData1Size);

	/* print binAttr1: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"binAttr1: %s", g_info.getMemberInfoEntry.updatable.binAttr1);

	/* print descr: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"descr: %s", g_info.getMemberInfoEntry.updatable.description);

	/*J allowMsg の値は sceNpClansGetMemberInfo() では取得することができません。*/
#if 0
	/* print allowMsg: */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST, ypos, COND_SCALE, (a|COLOR_WHITE),
		"allowMsg: %s", (g_info.getMemberInfoEntry.updatable.allowMsg)?"true":"false");
#endif

	return;
}

void
clans_draw_clanmode_update_member_info(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* binAttr1 */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "binAttr1:");
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE)*10,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_BIN1,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.updatableMemberInfo.binAttr1);

	/* description */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "description:");
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE)*13,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_DESC,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.updatableMemberInfo.description);


	/* allowMsg */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "allowMsg:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_ALLOW_MSG,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.updatableMemberInfo.allowMsg)?"true":"false");

	/* print UPDATE */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(
		STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 4,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_UPDATE_MEMBER_INFO_ACT_POS_Y_UPDATE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"UPDATE");

	return;
}


void
clans_draw_clanmode_change_member_role(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_TOP_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print NpId: */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) *2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_NPID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.changeMemberRole_onlineId.data);

	/* print "ROLE:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "ROLE:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_ROLE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", member_list_role_str[g_info.memberRole]);

	/* print "CHANGE" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_CHANGE_MEMBER_ROLE_ACT_POS_Y_CHANGE,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"CHANGE");

	return;
}

void
clans_draw_clanmode_kick_member(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "NpId:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) *2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_NPID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.kickMember_onlineId.data);

	/* print "SUBJECT:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "SUBJECT:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_SUBJCET,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.kickMemberMessage.subject);

	/* print "BODY:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "BODY:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_BODY,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.kickMemberMessage.body);

	/* print "CENSOR:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CENSOR:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_CENSOR,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.kickMember_censor)?"true":"false");

	/* print "KICK:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_KICK_MEMBER_ACT_POS_Y_KICK,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"KICK");

	return;
}


void
clans_draw_clanmode_join(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS + 5;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* print "JOIN" */
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 5,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"JOIN");

	return;
}

void
clans_draw_clanmode_leave(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS + 5;
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	/* print "JOIN" */
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 5,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_ACT_POS_Y_DISBAND,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"LEAVE");

	return;
}



void
clans_draw_clanmode_send_invitation_response(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "SUBJECT:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "SUBJECT:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_SUBJECT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.subject);

	/* print "BODY:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "BODY:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_BODY,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.body);

	/* print "ACCEPT:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "ACCEPT:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_ACCEPT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.sendInvitationResponse_accept)?"true":"false");

	/* print "CENSOR:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CENSOR:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_CENSOR,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.sendInvitationResponse_censor)?"true":"false");

	/* print "SEND" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_INVITATION_RESPONSE_ACT_POS_Y_SEND,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"SEND");

	return;
}

void
clans_draw_clanmode_send_membership_response(void)
{
	int row;
	uint32_t a = DEF_A;
	float ypos;

	if (g_info.pos_x != NP_GUI_MODE_CLANS_CLANMODE_POS_X_COND) {
		a = UNFOCUSED_A;
	}

	row = STRING_ROW_CONDITIONS;

	/* print "NpId:" */
	ypos = YSTART + S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * row;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "NpId:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) *2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_NPID,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.sendMembershipResponse_onlineId.data);

	/* print "SUBJECT:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "SUBJECT:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_SUBJECT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.subject);

	/* print "BODY:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "BODY:");

	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE);
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_BODY,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"[%s]", g_info.clansMessage.body);

	/* print "ACCEPT:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "ACCEPT:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_ACCEPT,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.sendMembershipResponse_accept)?"true":"false");

	/* print "CENSOR:" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 1;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST,
		ypos, COND_SCALE, (a|COLOR_WHITE), "CENSOR:");

	cellDbgFontPrintf(STRING_X_CONDITION_VALUE,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_CENSOR,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"%s", (g_info.sendMembershipResponse_censor)?"true":"false");

	/* print "SEND" */
	ypos += S_DEFAULT_YCHAR_INTERVAL(COND_SCALE) * 3;
	cellDbgFontPrintf(STRING_X_CONDITION_LIST + S_DEFAULT_XCHAR_INTERVAL(COND_SCALE) * 2,
		ypos, COND_SCALE,
		MENU_COLOR_WITH_A(NP_GUI_MODE_CLANS_CLANMODE_SEND_MEMBERSHIP_RESPONSE_ACT_POS_Y_SEND,
			g_info.pos_y, (a|FOCUSED_COLOR), a),
		"SEND");

	return;
}


