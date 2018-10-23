/*
 * Copyright (C) 1994-2018 Altair Engineering, Inc.
 * For more information, contact Altair at www.altair.com.
 *
 * This file is part of the PBS Professional ("PBS Pro") software.
 *
 * Open Source License Information:
 *
 * PBS Pro is free software. You can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * PBS Pro is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Commercial License Information:
 *
 * For a copy of the commercial license terms and conditions,
 * go to: (http://www.pbspro.com/UserArea/agreement.html)
 * or contact the Altair Legal Department.
 *
 * Altair’s dual-license business model allows companies, individuals, and
 * organizations to create proprietary derivative works of PBS Pro and
 * distribute them - whether embedded or bundled with other software -
 * under a commercial license agreement.
 *
 * Use of Altair’s trademarks, including but not limited to "PBS™",
 * "PBS Professional®", and "PBS Pro™" and Altair’s logos is subject to Altair's
 * trademark licensing policies.
 *
 */
/**
 * @file	cnt2server
 *	Connect to the server, and if there is an error, print a more
 * 	descriptive message.
 *
 * @par	Synopsis:
 *	int cnt2server( char *server )
 *
 *	server	The name of the server to connect to. A NULL or null string
 *		for the default server.
 *
 * @par	Returns:
 *	The connection returned by pbs_connect().
 */

#include <pbs_config.h>   /* the master config generated by configure */

#if defined(PBS_SECURITY) && (PBS_SECURITY == KRB5)
#include "pbsgss.h"
#endif

#include <errno.h>
#include "cmds.h"


/**
 * @brief
 *	Makes a connection to the server, returning the pbs_connect() result.
 *
 * @param[in]	server	- hostname of the pbs server to connect to
 * @param[in]	extend  - extend data to send along with the connection.
 *
 * @return	int
 * @retval	connection	success
 * @retval	0		fail
 */
int
cnt2server_extend(char *server, char *extend)
{
	int connect;

#if defined(PBS_SECURITY) && (PBS_SECURITY == KRB5)
	if (!getenv("PBSPRO_IGNORE_KERBEROS") && !pbsgss_can_get_creds()) {
		fprintf(stderr, "No Kerberos credentials found.\n");
		exit(1);
	}
#endif
	
	connect = pbs_connect_extend(server, extend);
	if (connect <= 0) {
		if (pbs_errno > PBSE_) {
			switch (pbs_errno) {

				case PBSE_BADHOST:
					fprintf(stderr, "Unknown Host.\n");
					break;

				case PBSE_NOCONNECTS:
					fprintf(stderr, "Too many open connections.\n");
					break;

				case PBSE_NOSERVER:
					fprintf(stderr, "No default server name.\n");
					break;

				case PBSE_SYSTEM:
					fprintf(stderr, "System call failure.\n");
					break;

				case PBSE_PERM:
					fprintf(stderr, "No Permission.\n");
					break;

				case PBSE_PROTOCOL:
					fprintf(stderr, "Communication failure.\n");
					break;

			}
		} else if (errno != 0)
			perror(NULL);

		return (connect);
	}

	/*
	 * Disable Nagle's algorithm on the TCP connection to server.
	 * Nagle's algorithm is hurting cmd-server communication.
	 */
	if (pbs_connection_set_nodelay(connect) == -1) {
		fprintf(stderr, "Cannot set nodelay on connection %d (errno=%d)\n",
			connect, pbs_errno);
		return (PBSE_SYSTEM);
	}

	return (connect);
}

/**
 * @brief
 *	A wrapper function to the cnt2server_extend() call where there's
 *	no 'extend' parameter passed.
 *
 * @param[in]	server	- hostname of the pbs server to connect to
 *
 * @return      int
 * @retval      connection      success
 * @retval	0		fail
 */
int
cnt2server(char *server)
{
	return (cnt2server_extend(server, NULL));
}
