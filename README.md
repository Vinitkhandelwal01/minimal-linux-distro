# Minimal Linux Distro with ASCII Garden 

This project creates a **minimal Linux distro** for x64, inspired by [Nir Lichtman's YouTube tutorial](https://www.youtube.com/watch?v=u2Juz5sQyYQ).  
It includes a custom shell with an interactive **ASCII garden** feature, where users can "plant" and "grow" ASCII art like trees and flowers.  
The distro is **bootable via QEMU** and fits in just a few megabytes.

---

##  Features

- Minimal Linux kernel (based on `tinyconfig`).
- Static shell with syscalls for:
  - `read`, `write`, `fork`, `execv`, `waitid`, `exit`.
- **ASCII garden**:
  - `plant tree` → Displays an ASCII tree.
  - `plant flower` → Displays an ASCII flower.
  - `grow` → Evolves plants (seed → sprout → tree).
- Optional: Static Lua binary for scripting (not included by default).
- Bootable ISO for testing in **QEMU**.

---

##  Prerequisites

Make sure you have the following installed on an **Ubuntu/Debian-based system**:

```bash
sudo apt install genisoimage qemu-system-x86 gcc make
```
Also download:

Linux kernel source (e.g., linux-6.6 from [kernel.org](https://kernel.org/?utm_source=chatgpt.com)).

GCC for compiling the shell.

## Setup

1. Clone this repository:

git clone <your-repo-url>
cd minimal-linux-distro

2. Download and extract the Linux kernel source (e.g., linux-6.6) to /path/to/linux-6.6.

3. Update KERNEL_SRC in Makefile and create_initramfs.sh to point to your kernel source path.

4. Build the distro:
```
make
```
5. Test in QEMU:
```
make test
```
---
## Usage

Once inside the shell (> prompt), try:

plant tree → Shows an ASCII tree 

plant flower → Shows an ASCII flower 

grow → Cycles through plant growth stages

exit → Quit the shell

Other commands (e.g., lua if included) are executed if available in the initramfs.

## Customization

Add a static Lua binary to initramfs/lua for scripting (update create_initramfs.sh).

Modify shell.c to add new ASCII art or commands.

Edit the kernel .config to enable additional features (e.g., networking).

## Notes

The ASCII garden adds a playful twist, making this distro a fun learning tool.

Reference code: Nir Lichtman’s GitHub
.

Debug issues like "init not found" by ensuring /init is executable:

```
chmod +x initramfs/init
```
