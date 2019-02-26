/*
 * pim_zmqw.h
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

int pim_zmqw_register_notifications(void);
int pimd_igmpv3_add(struct igmp_group *group, struct in_addr src_addr);
int pimd_igmpv3_del(struct igmp_group *group, struct in_addr src_addr);
