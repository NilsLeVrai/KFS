#include "./gdt.h"

struct gdt_entry gdt[5];
/* 	◦ Kernel Code
	◦ Kernel Data
	◦ Kernel stack
	◦ User code
	◦ User data
	◦ User stack	*/

struct gdt_ptr gp;


void init_gdt_entry(int32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].base_low    = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high   = (base >> 24) & 0xFF;
    gdt[index].limit_low   = (limit & 0xFFFF);

    // 3. Configuration de la granularité et des 4 bits restants de la limite (bits 16-19)
    // On combine les drapeaux de granularité et les 4 bits de poids fort de la limite
    gdt[index].granularity = (limit >> 16) & 0x0F;     // Stocke les bits 16-19 de la limite
    gdt[index].granularity |= (granularity & 0xF0);    // Ajoute les flags (ex: 0xCF -> Page-granular, 32-bit)

    // 4. Configuration de l'octet d'accès
    gdt[index].access = access;
}

void gdt_flush() {
    // On passe l'adresse de la structure 'gp' à l'instruction 'lgdt'
    asm volatile (
        "lgdt %0\n\t"          // Charge le pointeur GDT dans le registre GDTR

        // Mises à jour des segments de données (0x10 est l'index 2 du Kernel : 2 * 8 = 16 = 0x10)
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"

        // Far Jump pour recharger CS (0x08 est l'index 1 du Kernel : 1 * 8 = 8 = 0x08)
        // 'ljmp' sous la syntaxe AT&T prend la forme : ljmp $section, $offset
        "ljmp $0x08, $.reload_cs\n\t"
        ".reload_cs:\n\t"
        :
        : "m" (gp)             // Entrée : la structure gp en mémoire
        : "ax"                 // On indique à GCC que le registre AX est modifié
    );
}

void setup_gdt() {
    // 0x00 : Null Descriptor
    init_gdt_entry(0, 0, 0, 0, 0);

    // 0x08 : Code Kernel (Index 1 -> 1 * 8 = 0x08)
    init_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // 0x10 : Données Kernel (Index 2 -> 2 * 8 = 0x10)
    init_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // 0x18 : Code Utilisateur (Index 3 -> 3 * 8 = 0x18 | Privilège 3 = 0x1B)
    init_gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // 0x20 : Données Utilisateur (Index 4 -> 4 * 8 = 0x20 | Privilège 3 = 0x23)
    init_gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base  = (unsigned int)&gdt;

    gdt_flush();
    activate_segments();
}
