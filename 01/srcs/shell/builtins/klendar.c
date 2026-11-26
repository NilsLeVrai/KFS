#include "builtins.h"

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

static const char *get_weekday(unsigned char value) {
    static const char *weekdays[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
    unsigned char ret = bcd_to_bin(value);
    return (weekdays[ret - 1]);
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

void klendar() {

    const char *month;
    const char *weekday;
    unsigned int year;
    unsigned char seconds;
    unsigned char minutes;
    unsigned char hours;
    unsigned char day;

    port_byte_out_int(0x70, 0x00);
    seconds = port_byte_in_int(0x71);

    port_byte_out_int(0x70, 0x02);
    minutes = port_byte_in_int(0x71);

    port_byte_out_int(0x70, 0x04);
    hours = port_byte_in_int(0x71);

    port_byte_out_int(0x70, 0x06);
    weekday = get_weekday(port_byte_in_int(0x71));

    port_byte_out_int(0x70, 0x07);
    day = port_byte_in_int(0x71);

    port_byte_out_int(0x70, 0x08);
    month = get_month(port_byte_in_int(0x71));

    port_byte_out_int(0x70, 0x09);
    year = get_year(port_byte_in_int(0x71));

    printk(tty_current(), "%s %02x %s %d %02x:%02x:%02x UTC\n", weekday, day, month, year, hours + 1, minutes, seconds);
}
