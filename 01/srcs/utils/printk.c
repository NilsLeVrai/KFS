#include "printk.h"
#include "stdint.h"

typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type)   __builtin_va_arg(ap, type)
#define va_end(ap)         __builtin_va_end(ap)

static int32_t	put_unsigned(tty_t *t, uint32_t n, uint32_t base, bool upper, int32_t min_len) {
	const int8_t	*digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
	int8_t			buf[32];
	int32_t			i = 0;
	int32_t			cnt = 0;

	do {
		buf[i++] = digits[n % base];
		n /= base;
	} while (n);
	while (i < min_len)
		buf[i++] = '0';
	while (i--)
		cnt += tty_putchar(t, buf[i], 0, 0, 0);
	return (cnt);
}

static int32_t	put_signed(tty_t *t, int32_t n) {
	int32_t	cnt = 0;

	if (n < 0) {
		cnt += tty_putchar(t, '-', 0, 0, 0);
		// cast avant la negation pour gerer INT32_MIN
		return (cnt + put_unsigned(t, -(uint32_t)n, 10, FALSE, 0));
	}
	return (put_unsigned(t, (uint32_t)n, 10, FALSE, 0));
}

/*
 * %d %i : int signe
 * %u    : unsigned
 * %x %X : hexa (minuscule / majuscule)
 * %b    : binaire
 * %p    : pointeur (0x + 8 chiffres hexa)
 * %s %c %%
 * flag '#' : prefixe 0x / 0X pour %x %X, 0b pour %b
 */
uint32_t	printk(tty_t *t, const int8_t* f, ...) {
	va_list	args;
	int32_t	cnt = 0;
	bool	alt;

	va_start(args, f);

	while (*f) {
		if (*f != '%') {
			cnt += tty_putchar(t, *f++, 0, 0, 0);
			continue;
		}
		f++;
		alt = FALSE;
		if (*f == '#')
			alt = TRUE, f++;
		switch (*f) {
			case 'd':
			case 'i':
				cnt += put_signed(t, va_arg(args, int32_t));
				break;
			case 'u':
				cnt += put_unsigned(t, va_arg(args, uint32_t), 10, FALSE, 0);
				break;
			case 'x':
			case 'X':
				if (alt)
					cnt += tty_write(t, *f == 'X' ? "0X" : "0x", 0, 0, 0);
				cnt += put_unsigned(t, va_arg(args, uint32_t), 16, *f == 'X', 0);
				break;
			case 'b':
				if (alt)
					cnt += tty_write(t, "0b", 0, 0, 0);
				cnt += put_unsigned(t, va_arg(args, uint32_t), 2, FALSE, 0);
				break;
			case 'p':
				cnt += tty_write(t, "0x", 0, 0, 0);
				cnt += put_unsigned(t, (uintptr_t)va_arg(args, void*), 16, FALSE, sizeof(void*) * 2);
				break;
			case 's': {
				const int8_t	*s = va_arg(args, const int8_t*);
				cnt += tty_write(t, s ? s : "(null)", 0, 0, 0);
				break;
			}
			case 'c':
				cnt += tty_putchar(t, (int8_t)va_arg(args, int32_t), 0, 0, 0);
				break;
			case '%':
				cnt += tty_putchar(t, '%', 0, 0, 0);
				break;
			case '\0':
				va_end(args);
				return (cnt);
			default:
				cnt += tty_putchar(t, '%', 0, 0, 0);
				if (alt)
					cnt += tty_putchar(t, '#', 0, 0, 0);
				cnt += tty_putchar(t, *f, 0, 0, 0);
		}
		f++;
	}
	va_end(args);
	return (cnt);
}
