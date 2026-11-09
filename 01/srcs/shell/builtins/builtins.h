#pragma once
#include "../../tty/tty.h"
#include "../../tty/tty_manager.h"
#include "../../utils/printk.h"
#include "../../drivers/ports.h"
#include "../../cpu/timer.h"

void	bonjour42();
void	zeta(const uint32_t nb);
void	kreboot();
void	kshutdown();
void    kpoppy();
