#include "builtins.h"

void	kshutdown() {
	port_byte_out(0x604, 0x2000);
}
