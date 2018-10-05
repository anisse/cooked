/*
 * Copyright (C) 2018 Anisse Astier <anisse@astier.eu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/livepatch.h>

/*
 * This live patch attempts to disable raw sockets
 */

#include <linux/socket.h>
#include <linux/net.h>
#include <linux/security.h>
#include <net/sock.h>

static int cooked_rawv6_sendmsg(struct sock *sk, struct msghdr *msg, size_t len)
{
	return -EOPNOTSUPP;
}
static int cooked_raw_sendmsg(struct sock *sk, struct msghdr *msg, size_t len)
{
	return -EOPNOTSUPP;
}
static int cooked_raw_init(struct sock *sk)
{
	return -EOPNOTSUPP;
}
static int cooked_rawv6_init_sk(struct sock *sk)
{
	return -EOPNOTSUPP;
}
/* AF_PACKET bypass is untested */
static int cooked_packet_sendmsg_spkt(struct socket *sock, struct msghdr *msg,
                               size_t len)
{
	return -EOPNOTSUPP;
}
static int cooked_packet_sendmsg(struct socket *sock, struct msghdr *msg, size_t len)
{
	return -EOPNOTSUPP;
}
static struct klp_func funcs[] = {
        {
                .old_name = "raw_init",
                .new_func = cooked_raw_init,
        },
        {
                .old_name = "rawv6_init_sk",
                .new_func = cooked_rawv6_init_sk,
        },
        {
                .old_name = "raw_sendmsg",
                .new_func = cooked_raw_sendmsg,
        },
        {
                .old_name = "rawv6_sendmsg",
                .new_func = cooked_rawv6_sendmsg,
        },
        {
                .old_name = "packet_sendmsg_spkt",
                .new_func = cooked_packet_sendmsg_spkt,
        },
        {
                .old_name = "packet_sendmsg",
                .new_func = cooked_packet_sendmsg,
        },
	{ }
};

static struct klp_object objs[] = {
        {
                /* name being NULL means vmlinux */
                .funcs = funcs,
        }, { }
};

static struct klp_patch patch = {
        .mod = THIS_MODULE,
        .objs = objs,
};

static int cooked_init(void)
{
        int ret;

        ret = klp_register_patch(&patch);
        if (ret)
                return ret;
        ret = klp_enable_patch(&patch);
        if (ret) {
                WARN_ON(klp_unregister_patch(&patch));
                return ret;
        }
        return 0;
}

static void cooked_exit(void)
{
        WARN_ON(klp_unregister_patch(&patch));
}

module_init(cooked_init);
module_exit(cooked_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");
