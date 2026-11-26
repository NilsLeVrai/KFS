#include "builtins.h"

/*
chopper le terminal
verifier si on peut afficher le caractere
faire la boucle principal
*/

extern uint8_t stack_top[];
extern uint8_t stack_bottom[];

static void printk_kernel_stack(tty_t *t, uint32_t esp, uint32_t ebp) {

    
    uint32_t stack_size;
    uint32_t bytes_used;
    
    asm volatile ("mov $stack_top, %0\n\tsub $stack_bottom, %0" : "=r"(stack_size));

    bytes_used = (uint32_t)stack_top - esp;
    
    printk(t, "-- stack --\n");
    printk(t, "bottom %p  top %p  (%d bytes)\n", stack_bottom, stack_top, stack_size);
    printk(t, "esp    %p  ebp %p  used %d bytes\n", (void*)esp, (void*)ebp, bytes_used);
}

static void printk_backtrace(tty_t *t, uint32_t ebp) {
    
    uint32_t *frame;
    uint32_t ret;
    int      i;

    frame = (uint32_t*)ebp;
    i = 0;
    printk(t, "-- backtrace --\n");

    while (frame != 0 && (uint8_t*)frame >= stack_bottom && (uint8_t*)(frame + 2) <= stack_top) {
        ret = *(frame + 1);
        printk(t, "#%d  frame %p  ret %p\n", i, frame, (void*)ret);
        if (frame[0] != 0 && frame[0] <= (uint32_t)frame)
            break;
        frame = (uint32_t*) frame[0];
        i++;
    }
}

static void printk_stack(tty_t *t, uint32_t esp) {
    
   // uint32_t tmp;
    (void)esp;
    printk(t, "-- stack (esp --> top) --\n");
    //tmp = esp;
    //for (i in range)
}

void	kdump() {
    
    tty_t *t = tty_current();
    uint32_t esp;
    uint32_t ebp;

    asm volatile ("mov %%esp, %0" : "=r"(esp));
    asm volatile ("mov %%ebp, %0" : "=r"(ebp));

    printk_kernel_stack(t, esp, ebp);
    printk_backtrace(t, ebp);
    printk_stack(t, esp);
}

