/*
 * zmqw.c
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

#include <stddef.h>
#include "lib/hook.h"
#include "lib/libfrr.h"
#include "lib/frr_zmq.h"
#include "lib/module.h"
#include "lib/frrstr.h"
#include "lib/vector.h"
#include "zmqw/zmqw.h"

static void *zmqsock;

int zmqw_process_notification(const char *notification)
{
	return zmq_send(zmqsock, notification, strlen(notification) + 1, 0);
}

static int zmqw_late_init(struct thread_master *tm)
{
	const char *format = THIS_MODULE->load_args;
/* c doesn't like initializers on dynamic length stack variables */
#define ZMQCONNECTSTRLEN 14
	char zmqConnectStr[ZMQCONNECTSTRLEN] = {
		0,
	}; /* tcp:XX*:65536 */
	vector formatVector = frrstr_split_vec(format, ";");

	for (size_t x = 0; x < vector_active(formatVector); x++) {
		vector parameterVector =
			frrstr_split_vec(vector_lookup(formatVector, x), "=");
		char *parameter = vector_lookup(parameterVector, 0);

		if (strcmp(parameter, "port") == 0) {
			snprintf(zmqConnectStr, ZMQCONNECTSTRLEN, "tcp://*:%s",
				 (char *)vector_lookup(parameterVector, 1));
		}
	}

	vector_free(formatVector);

	if (*zmqConnectStr == '\0') {
		zlog_err(
			"could not parse the zmq port number off the command line.  please specify one.  will not initialize the hooks without it\n");
		return -1;
	}

	/* we need to initialize up the zmq library */
	frrzmq_init();

	zmqsock = zmq_socket(frrzmq_context, ZMQ_PUB);
	if (zmq_bind(zmqsock, zmqConnectStr)) {
		perror("zmq_bind() failed!");
		exit(1);
	}

	return 0;
}

static int zmqw_init(void)
{
	hook_register(frr_late_init, zmqw_late_init);
	return 0;
}

FRR_MODULE_SETUP(.name = "zmqw", .version = "zmqw", .description = "zmqw",
		 .init = zmqw_init)
