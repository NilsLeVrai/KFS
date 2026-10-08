global activate_segments
activate_segments:
    ; Chargement des segments de données Kernel (Index 2 = 0x10)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far Jump pour charger CS avec le segment de Code Kernel (Index 1 = 0x08)
    jmp 0x08:.reload_cs
.reload_cs:
    ret

; global switch_to_user_mode
; switch_to_user_mode:
;     cli                 ; Désactive les interruptions pendant la transition
;
;     ; 1. Pousser le sélecteur de segment de pile Utilisateur (SS) avec privilège 3
;     push 0x23
;     push esp            ; Pousser le pointeur de pile actuel (ou de l'application)
;
;     ; 2. Pousser les EFLAGS (le registre d'état du CPU)
;     pushfd
;     pop eax
;     or eax, 0x200       ; Réactiver les interruptions une fois en Ring 3
;     push eax
;
;     ; 3. Pousser le sélecteur de segment de Code Utilisateur (CS) avec privilège 3
;     push 0x1B
;
;     ; 4. Pousser l'adresse de la fonction utilisateur à exécuter (EIP)
;     push user_function_address
;
;     ; 5. Charger les registres de données avec le segment Utilisateur (0x23)
;     mov ax, 0x23
;     mov ds, ax
;     mov es, ax
;     mov fs, ax
;     mov gs, ax
;
;     ; 6. Execution magique du saut de privilège
;     iretd
