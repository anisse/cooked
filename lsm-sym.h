#include <linux/lsm_hooks.h>


#define LSM_HOOK_INIT_COOKED(HEAD, HOOK) \
        { .head = &security_hook_heads_cooked->HEAD, .hook = { .HEAD = HOOK } }

const void (*security_add_hooks_cooked)(struct security_hook_list *hooks, int count, char *lsm) = (void*)0xffffffff827a631c;
struct security_hook_heads const *security_hook_heads_cooked = (void*)0xffffffff82a7d4c0;

