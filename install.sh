sudo mv /home/piuser/RFS /home/piuser/RFS.buildroot
sudo rm /var/lib/tftpboot/*

cp /home/piuser/real_time/files/kernel_config /home/piuser/software/kernel/linux/.config

cp -rp /home/piuser/real_time/files/varios /home/piuser/software/

cd /home/piuser/software

wget https://busybox.net/downloads/busybox-1.30.1.tar.bz2
bunzip2 busybox-1.30.1.tar.bz2
tar xvf busybox-1.30.1.tar
