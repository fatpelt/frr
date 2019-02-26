/*
 * pim_zmqw.c
 * Copyright (C) 2018-2019  Patrick Felt
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; see the file COPYING; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

//#include "zmqw/zmqw.h"
#include <stddef.h>
#include "lib/libfrr.h"
#include "pimd/pim_igmp.h"
#include "pimd/pim_igmpv3.h"
#include "pimd/pimd.h"
#include "pimd/pim_zmqw.h"
#include "lib/frr_zmq.h"
#include "lib/hook.h"
#include "lib/module.h"
#include "lib/frrstr.h"
#include "lib/vector.h"
#include "lib/json.h"

int pimd_igmpv3_add(struct igmp_group *group, struct in_addr src_addr)
{
	char g[16], s[16];
	struct json_object *out = json_object_new_object();
	char *jsonOut;

	inet_ntop(AF_INET, &group->group_addr.s_addr, g, 15);
	inet_ntop(AF_INET, &src_addr, s, 15);

	json_object_string_add(out, "action", "add");
	json_object_string_add(out, "group", g);
	json_object_string_add(out, "source", s);

	jsonOut = XSTRDUP(MTYPE_TMP, json_object_to_json_string_ext(
					     out, JSON_C_TO_STRING_PRETTY));
	zmqw_send_notification(jsonOut, strlen(jsonOut));
	XFREE(MTYPE_TMP, jsonOut);
	json_object_free(out);

	return 0;
}

int pimd_igmpv3_del(struct igmp_group *group, struct in_addr src_addr)
{
	char g[16], s[16];
	struct json_object *out = json_object_new_object();
	char *jsonOut;

	inet_ntop(AF_INET, &group->group_addr.s_addr, g, 15);
	inet_ntop(AF_INET, &src_addr, s, 15);

	json_object_string_add(out, "action", "delete");
	json_object_string_add(out, "group", g);
	json_object_string_add(out, "source", s);

	jsonOut = XSTRDUP(MTYPE_TMP, json_object_to_json_string_ext(
					     out, JSON_C_TO_STRING_PRETTY));
	zmqw_send_notification(jsonOut, strlen(jsonOut));
	XFREE(MTYPE_TMP, jsonOut);
	json_object_free(out);

	return 0;
}

int pim_zmqw_register_notifications(void)
{
	hook_register(pimd_igmpv3_add, pimd_igmpv3_add);
	hook_register(pimd_igmpv3_del, pimd_igmpv3_del);
	return 0;
}
