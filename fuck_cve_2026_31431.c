// SPDX-License-Identifier: GPL-2.0-or-later

#include <crypto/internal/aead.h>
#include <crypto/scatterwalk.h>
#include <crypto/if_alg.h>
#include <crypto/skcipher.h>
#include <crypto/null.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/net.h>
#include <net/sock.h>

#define FAKE_ERROR ENOENT

static void *fuck_bind(const char *name, u32 type, u32 mask)
{
	// do nothing
	(void) name;
	(void) type;
	(void) mask;

	return NULL;
}

static void fuck_release(void *private)
{
	// do nothing
	(void) private;
}

static int fuck_setkey(void *private, const u8 *key, unsigned int keylen)
{
	// do nothing
	(void) private;
	(void) key;
	(void) keylen;

	return -FAKE_ERROR;
}

static int fuck_setauthsize(void *private, unsigned int authsize)
{
	// do nothing
	(void) private;
	(void) authsize;

	return -FAKE_ERROR;
}

static int fuck_accept(void *private, struct sock *sk)
{
	// do nothing
	(void) private;
	(void) sk;

	return -FAKE_ERROR;
}

static int fuck_sendmsg(struct socket *sock, struct msghdr *m, size_t total_len)
{
	// do nothing
	(void) sock;
	(void) m;
	(void) total_len;

	return -FAKE_ERROR;
}

static int fuck_recvmsg (struct socket *sock, struct msghdr *m, size_t total_len, int flags)
{
	// do nothing
	(void) sock;
	(void) m;
	(void) total_len;
	(void) flags;

	return -FAKE_ERROR;
}

static struct proto_ops fuck_ops = {
	.family		=	PF_ALG,

	.connect	=	sock_no_connect,
	.socketpair	=	sock_no_socketpair,
	.getname	=	sock_no_getname,
	.ioctl		=	sock_no_ioctl,
	.listen		=	sock_no_listen,
	.shutdown	=	sock_no_shutdown,
	.mmap		=	sock_no_mmap,
	.bind		=	sock_no_bind,
	.accept		=	sock_no_accept,

	.release	=	af_alg_release,
	.sendmsg	=	fuck_sendmsg,
	.recvmsg	=	fuck_recvmsg,
	.poll		=	af_alg_poll,
};

static const struct af_alg_type algif_fuck_cve_2026_31431 = {
	.bind		=	fuck_bind,
	.release	=	fuck_release,
	.setkey		=	fuck_setkey,
	.setauthsize	=	fuck_setauthsize,
	.accept		=	fuck_accept,
	.accept_nokey	=	fuck_accept,
	.ops		=	&fuck_ops,
	.ops_nokey	=	&fuck_ops,
	.name		=	"aead",
	.owner		=	THIS_MODULE
};

static int reg_fake_impl = 0;

static int __init fuck_cve_2026_31431_init(void)
{
	int err = af_alg_unregister_type(&algif_fuck_cve_2026_31431);
	pr_info("Remove insecure algif_aead implement: %d", err);

	if (reg_fake_impl) {
		pr_info("register fake implement to avoid load");
		return af_alg_register_type(&algif_fuck_cve_2026_31431);
	}
	return 0;
}

static void __exit fuck_cve_2026_31431_exit(void)
{
	if (reg_fake_impl) {
		// int err = 
		// best effort to remove
		af_alg_unregister_type(&algif_fuck_cve_2026_31431);
		// BUG_ON(err);
	}
}

module_init(fuck_cve_2026_31431_init);
module_exit(fuck_cve_2026_31431_exit);

module_param(reg_fake_impl, int, 0644);
MODULE_PARM_DESC(reg_fake_impl, "register a fake implement to avoid load, default=0");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Fuck algif_aead for CVE-2026-31431");
