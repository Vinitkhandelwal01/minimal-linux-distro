#!/bin/bash
set -e

INITRAMFS_DIR=initramfs
KERNEL_SRC=/path/to/linux-6.6
ISO_NAME=minimal.iso

# Create initramfs directory
mkdir -p $INITRAMFS_DIR

# Copy shell binary
cp init $INITRAMFS_DIR/
chmod +x $INITRAMFS_DIR/init

# Optional: Add static Lua binary
# cp /path/to/lua $INITRAMFS_DIR/lua

# Create initramfs
cd $INITRAMFS_DIR
find . | cpio -o -H newc > ../initramfs.cpio
cd ..

# Compile kernel with initramfs
cd $KERNEL_SRC
make tinyconfig
# Manually enable CONFIG_BLK_DEV_INITRD, CONFIG_TTY, CONFIG_64BIT, CONFIG_BINFMT_ELF in .config
echo "CONFIG_INITRAMFS_SOURCE=\"/path/to/initramfs.cpio\"" >> .config
make -j$(nproc)
cp arch/x86/boot/bzImage ..

# Create ISO
genisoimage -o $ISO_NAME -b bzImage -c boot.cat .

echo "ISO created: $ISO_NAME"
echo "Run 'qemu-system-x86_64 -cdrom $ISO_NAME -m 512' to test"
