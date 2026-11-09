/*
 * shell/shell.c
 * Shell (squelette).
 */

#include "shell.h"
#include "../utils/kreadline.h"
#include "../utils/string.h"
#include "../tty/tty.h"
#include "../tty/tty_manager.h"
#include "../utils/printk.h"
#include "builtins/builtins.h"

#define BUILTINS_NB 3
#define BUILTINS_NAME_MAX 32

typedef struct builtins_s {
	int8_t		str[BUILTINS_NAME_MAX];
	void		(*adress)(void);
} builtins_t;

static builtins_t builtins[BUILTINS_NB];

void	shell_init() {
	kstrcpy(builtins[0].str, "42");
	builtins[0].adress = &bonjour42;
	kstrcpy(builtins[1].str, "reboot");
	builtins[1].adress = &kreboot;
	kstrcpy(builtins[2].str, "shutdown");
	builtins[2].adress = &kshutdown;
}

void	shell_start() {
	for (;;) {
		static int8_t b[64];
		kreadline(tty_current(), b, 64, "neumanOs >> ");
		tty_write(tty_current(), "\n", 0, 0, 0);
		for (int i = 0; i < BUILTINS_NB; i++) {
			if (!kstrcmp(b, builtins[i].str)) {
				builtins[i].adress();
			}
		}
	}
}
