KERNEL_SRC = /path/to/linux-6.6
INITRAMFS_DIR = initramfs
ISO_NAME = minimal.iso

all: shell initramfs iso

shell:
	gcc -static shell.c -o $(INITRAMFS_DIR)/init

initramfs:
	mkdir -p $(INITRAMFS_DIR)
	cp shell.c $(INITRAMFS_DIR)/init
	chmod +x $(INITRAMFS_DIR)/init
	# Optional: Add static Lua binary
	# cp /path/to/lua $(INITRAMFS_DIR)/lua
	cd $(INITRAMFS_DIR) && find . | cpio -o -H newc > ../initramfs.cpio

kernel:
	cd $(KERNEL_SRC) && make tinyconfig
	# Edit .config to enable CONFIG_BLK_DEV_INITRD, CONFIG_TTY, CONFIG_64BIT, CONFIG_BINFMT_ELF
	cd $(KERNEL_SRC) && make -j$(shell nproc)
	cp $(KERNEL_SRC)/arch/x86/boot/bzImage .

iso:
	genisoimage -o $(ISO_NAME) -b bzImage -c boot.cat .

clean:
	rm -rf $(INITRAMFS_DIR) initramfs.cpio bzImage $(ISO_NAME)

test:
	qemu-system-x86_64 -cdrom $(ISO_NAME) -m 512

.PHONY: all shell initramfs kernel iso clean test
