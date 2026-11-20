/*
 * shell/shell.c
 * Shell (squelette).
 */

#include "shell.h"
#include "../utils/kreadline.h"
#include "../utils/string.h"
#include "../tty/tty_manager.h"
#include "builtins/builtins.h"

#define BUILTINS_NB 4
#define BUILTINS_NAME_MAX 32

typedef struct builtins_s {
	int8_t		str[BUILTINS_NAME_MAX];
	void		(*adress)(void);
} builtins_t;

static builtins_t builtins[BUILTINS_NB];

#define PROMPT "neumanOs >> "

void	shell_init() {
	kstrcpy(builtins[0].str, "42");
	builtins[0].adress = &bonjour42;
	kstrcpy(builtins[1].str, "reboot");
	builtins[1].adress = &kreboot;
	kstrcpy(builtins[2].str, "shutdown");
	builtins[2].adress = &kshutdown;
	kstrcpy(builtins[3].str, "poppy");
	builtins[3].adress = &kpoppy;
}

void	shell_start() {
	static int8_t b[TTY_LINE_MAX];

	for (uint32_t i = 0; i < TERMINAL_NB; i++)
		tty_write(tty_get(i), PROMPT, 0, 0, 0);

	for (;;) {
		kreadline(b, sizeof(b));
		tty_t *t = tty_current();
		tty_write(t, "\n", 0, 0, 0);
		for (int i = 0; i < BUILTINS_NB; i++) {
			if (!kstrcmp(b, builtins[i].str)) {
				builtins[i].adress();
				break ;
			}
		}
		tty_write(t, PROMPT, 0, 0, 0);
	}
}
