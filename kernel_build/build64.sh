cd linux-arm64-gem5

echo Compile ARM KENEL ...
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- gem5_defconfig
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j4

cp vmlinux ../../kernels/binaries/vmlinux.aarch64.Apollon

echo Mount the .img ...
sudo mount -o loop,offset=32256 ../../kernels/disks/aarch64-ubuntu-trusty-headless.img /mnt/

echo Compile UserSpace Driver ...
aarch64-linux-gnu-gcc -c kernel/ApollonDriver.c

echo Add this file to a library...
ar rvs ApollonDriver.a ApollonDriver.o

echo Copy the ApollonDriver.a to Application directory...
cp ApollonDriver.a ../Application

echo Copy the Header file to Application directory...
cp kernel/ApollonDriver.h ../Application


echo Compile APP.C using library...
cd ../Application
make all64

echo Copy the executable in /mnt...
sudo cp App /mnt/
echo Umount the .img ...
sudo umount /mnt
