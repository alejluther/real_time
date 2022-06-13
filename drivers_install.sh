rm /home/piuser/software/kernel/linux/drivers/char/driver_03.*
cd /home/piuser/software/kernel/linux
git checkout drivers/char/Makefile

make ARCH=arm zImage

sudo cp arch/arm/boot/zImage /var/lib/tftpboot/

cd /home/piuser/.updates/curso_git
git pull

cp -rp 2019/DRIVERS /home/piuser/

cp  -rp /home/alucero/RFS_BUILDROOT /home/piuser/
mv /home/piuser/RFS_BUILDROOT /home/piuser/RFS
rm -rf /home/piuser/RFS/home/*
