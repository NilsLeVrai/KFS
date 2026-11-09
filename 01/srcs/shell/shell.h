/*
 * shell/shell.h
 * Shell (squelette). Passe par tty/ et kreadline, jamais par vga.
 */

#pragma once
#include "../drivers/vga.h"

void	shell_init();
void	shell_start();
