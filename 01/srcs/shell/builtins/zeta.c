#include "builtins.h"

struct zeta_s {
	uint32_t	nb;
	uint32_t	result;
};

struct zeta_s lol[8] = {{1, 2}};

void	zeta(const uint32_t nb) {
	printk(tty_current(), "la valeur de la fonction zeta pour %d est %d\n", lol[nb].nb, lol[nb].result);
}
