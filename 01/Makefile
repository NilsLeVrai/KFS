NAME		:= myos
ISO		:= myos.iso

# Directories
SRC_DIR		:= srcs
OBJ_DIR		:= objs
ISO_DIR		:= isodir

# Tools
NASM		:= nasm
CC		:= ./utils/i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-gcc
GRUB		:= grub-mkrescue
KVM		:= kvm

# Flags
NASMFLAGS	:= -f elf32
CFLAGS		:= -m32 -std=gnu99 -ffreestanding
LDFLAGS		:= -T linker.ld -m32 -ffreestanding -nostdlib

# Find all sources recursively
ASM_SRCS	:= $(shell find $(SRC_DIR) -type f -name '*.asm')
C_SRCS		:= $(shell find $(SRC_DIR) -type f -name '*.c')

# Convert sources/xxx/foo.asm -> objs/xxx/foo.o
ASM_OBJS	:= $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_SRCS))
C_OBJS		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

OBJS		:= $(ASM_OBJS) $(C_OBJS)


.PHONY: all clean fclean re run iso

all: $(ISO)

# --------------------------------------------------
# Linking
# --------------------------------------------------

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lgcc

# --------------------------------------------------
# ASM compilation
# --------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

# --------------------------------------------------
# C compilation
# --------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# --------------------------------------------------
# ISO
# --------------------------------------------------

iso: $(ISO)

$(ISO): $(NAME)
	@mkdir -p $(ISO_DIR)/boot
	cp $(NAME) $(ISO_DIR)/boot/$(NAME)
	$(GRUB) -o $(ISO) $(ISO_DIR)

# --------------------------------------------------
# Run
# --------------------------------------------------

run: $(ISO)
	$(KVM) $(ISO)

# --------------------------------------------------
# Cleaning
# --------------------------------------------------

clean:
	rm -rf $(OBJ_DIR)/*.o
	find $(OBJ_DIR) -type d -empty -delete

fclean: clean
	rm -f $(NAME)
	rm -f $(ISO)
	rm -f $(ISO_DIR)/boot/$(NAME)

re: fclean all
