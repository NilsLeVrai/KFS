#include "builtins.h"

void kpoppy() {
	for (int i = 0; i < 20; i++) {
		printk(tty_current(), "Poppy de %d\n", i);
        ksleep(1000);
	}
}