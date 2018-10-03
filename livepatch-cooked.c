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

static int cooked_sock_sendmsg(struct socket *sock, struct msghdr *msg) {
        int ret;
        if (sock->sk->sk_family == AF_PACKET)
                return -EPERM;
        if (sock->type == SOCK_RAW && (sock->sk->sk_family == AF_INET || sock->sk->sk_family == AF_INET6))
                return -EPERM;
        /* original implementation. WARNING: bypasses LSM security check*/
        ret = sock->ops->sendmsg(sock, msg, msg_data_left(msg));
        BUG_ON(ret == -EIOCBQUEUED);
        return ret;
}

static int cooked_sock_create(int family, int type, int protocol, struct socket **res) {
        if (family == AF_PACKET)
                return -EPERM;
        if (type == SOCK_RAW && (family == AF_INET || family == AF_INET6))
                return -EPERM;
        /* original implementation */
        return __sock_create(current->nsproxy->net_ns, family, type, protocol, res, 0);
}

static struct klp_func funcs[] = {
        {
                .old_name = "sock_sendmsg",
                .new_func = cooked_sock_sendmsg,
        },
        {
                .old_name = "sock_create", /* we should probably hook the lower level sock_create_lite, but this one is simpler to implement */
                .new_func = cooked_sock_create,
        }, { }
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
