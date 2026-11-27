#include "builtins.h"
#include "../../utils/string.h"

static inline unsigned char port_byte_in_int(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

static inline void port_byte_out_int(unsigned short port, unsigned char data) {
    __asm__ volatile ("out %%al, %%dx" : : "a" (data), "d" (port));
}

static unsigned char bcd_to_bin(unsigned char value) {
	unsigned char ret = ( (value & 0xF0) >> 1) + ( (value & 0xF0) >> 3) + (value & 0xf); //osdev
	return ret;
}

static uint8_t	bdc_to_int(uint8_t value) {
	return (value >> 4) * 10 + (value & 0x0F);
}

static const char *get_weekday(unsigned char idx) {
	static const char *weekdays[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
	return (weekdays[idx % 7]); // idx : 0 = Dimanche ... 6 = Samedi
}
static const char *get_month(unsigned char value) {
	static const char *months[] = {"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"};
	unsigned char ret = bcd_to_bin(value);
	return (months[ret - 1]);
}

static unsigned int get_year(unsigned char value) {
	unsigned char year = bcd_to_bin(value);
	return year + 2000;
}

static void init_time(time_t *time) {
    port_byte_out_int(0x70, 0x00);
    time->seconds = port_byte_in_int(0x71);

    port_byte_out_int(0x70, 0x02);
    time->minutes = port_byte_in_int(0x71);

    port_byte_out_int(0x70, 0x04);
    time->hours = port_byte_in_int(0x71);
    time->hours = (bcd_to_bin(time->hours) + 1) % 24;

    port_byte_out_int(0x70, 0x06);
    time->weekday = get_weekday(bcd_to_bin(port_byte_in_int(0x71)) - 1); // RTC : 1 = Dimanche

    port_byte_out_int(0x70, 0x07);
    time->day = port_byte_in_int(0x71);

    port_byte_out_int(0x70, 0x08);
    time->month = get_month(port_byte_in_int(0x71));

    port_byte_out_int(0x70, 0x09);
    time->year = get_year(port_byte_in_int(0x71));

}

static void printk_month(tty_t *t, time_t *time) {

	int	i;
	uint32_t	len_month;
	uint32_t	len_padding;

	i = 0;
	len_month = kstrlen((uint8_t *)time->month) + 5;
	len_padding = (20 - len_month) / 2;
	while (i++ < len_padding)
		printk(t, " ");
	printk(t, "%s %d\n", time->month, time->year);
	printk(t, " Di Lu Ma Me Je Ve Sa\n");

	port_byte_out_int(0x70, 0x06);
	int wday = bcd_to_bin(port_byte_in_int(0x71)) - 1; // 5 (27 nov)
	int day_int = bdc_to_int(time->day);               // 27
	int first_day_int = (wday - (day_int - 1) % 7 + 7) % 7;  // 0
	i = 0;
	while (i++ < first_day_int) {
		printk(t, "   ");
	}
	i = 1;
	while (i <= 30) {
		printk(t, i < 10 ? "  " : " ");
		if (i == day_int) {
			t->bg_color_default = 1;
			t->fg_color_default = 0;
		}
		printk(t, "%d", i);
		t->bg_color_default = 0;
		t->fg_color_default = 1;
		if ((first_day_int + i) % 7 == 0)
			printk(t, "\n");
		i++;
	}
	printk(t, "\n");
}

/*
trouver jour + date ex: 6 + dimanche
trouver le premier jour du mois ex: mardi

*/

void klendar() {

	tty_t	*t;
	time_t	time;

	t = tty_current();
	init_time(&time);

    printk(t, "%s %02x %s %d %02u:%02x:%02x UTC+1\n", time.weekday, time.day, time.month, time.year, time.hours, time.minutes, time.seconds);
	printk_month(t, &time);
}
