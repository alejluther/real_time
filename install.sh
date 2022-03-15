#mv /home/piuser/software/uboot/u-boot.good /home/piuser/software/uboot/u-boot

#rm /home/piuser/software/varios/RFS.tar
#cp /home/piuser/.updates/curso_git/2022/varios/RFS.tar /home/piuser/software/varios/

# Configuracion busybox: clean up
#cp /home/piuser/.updates/curso_git/2022/busybox/config /home/piuser/software/busybox/.config
#cd /home/piuser/software/busybox;make clean

# Configuracion kernel : ramfilesystems ramdisk, .config de backup
#cp /home/piuser/.updates/curso_git/2022/kernel/config /home/piuser/software/kernel/linux/.config
#cp /home/piuser/.updates/curso_git/2022/kernel/zImage /home/piuser/software/kernel/linux/arch/arm/boot/

# script_trace.sh  --->  fix script enbling/disabling traces
#cp /home/piuser/.updates/curso_git/2022/varios/script_trace.sh /home/piuser/software/varios/

# uSD con uboot para NFS
#cp /home/piuser/software/uboot/u-boot/u-boot.bin /home/piuser/software/varios/uboot_NFS.bin
#cp /home/piuser/software/uboot/u-boot.initrd/u-boot.bin /home/piuser/software/varios/uboot_initrd.bin
#cp /home/piuser/software/uboot/u-boot.pivot_root/u-boot.bin /home/piuser/software/varios/uboot_uSD.bin
#cp /home/piuser/.updates/curso_git/2022/varios/*.patch /home/piuser/software/varios/

# fdisk commands for user? Do they have uSD reader in the laptop?

cp -rp /home/piuser/.updates/curso_git/2022/hints /home/piuser/software/varios/
