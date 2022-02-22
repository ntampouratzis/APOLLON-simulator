cd linux-arm32-gem5

echo Compile ARM KENEL ...
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- vexpress_gem5_server_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -j4

cp vmlinux ../../kernels/binaries/vmlinux.aarch32.Apollon

echo Mount the .img ...
sudo mount -o loop,offset=32256 ../../kernels/disks/linux-aarch32-ael.img /mnt/

echo Compile UserSpace Driver ...
arm-linux-gnueabi-g++ -std=c++11 -c kernel/ApollonDriver.c

echo Add this file to a library...
ar rvs ApollonDriverC++.a ApollonDriver.o

echo Copy the ApollonDriverC++.a to Application directory...
cp ApollonDriverC++.a ../Application

echo Copy the Header files to Application directory...
cp kernel/ApollonDriver.h ../Application


echo Compile APP using library...
cd ../Application
make all32

echo Copy the executable in /mnt...
#sudo cp Node0 /mnt/
#sudo cp Node1 /mnt/
#sudo cp Node2 /mnt/
sudo cp App /mnt/
echo Umount the .img ...
sudo umount /mnt
