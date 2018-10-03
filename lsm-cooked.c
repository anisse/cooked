/*
 * Cooked Linux Security Module
 *
 * Author: Anisse Astier <anisse@astier.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 */

#include <linux/lsm_hooks.h>
#include <linux/net.h>
#include <linux/socket.h>

#include "lsm-sym.h"

static int cooked_sock_sendmsg(struct socket *sock, struct msghdr *msg, int size) {
	if (sock->type == SOCK_RAW)
		return -1;
}

static struct security_hook_list cooked_hooks[] __lsm_ro_after_init = {
	LSM_HOOK_INIT_COOKED(socket_sendmsg, cooked_sock_sendmsg),
};


void __init cooked_add_hooks(void)
{
	pr_info("Cooked: blocking raw sockets TX and creation.\n");
	security_add_hooks_cooked(cooked_hooks, ARRAY_SIZE(cooked_hooks), "cooked");
}
