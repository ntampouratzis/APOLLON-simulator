# ARM gem5-fiendly Kernels
The OS can be represented as a layered structure, as in the following Figure. It contains the User Space with the user applications and all the appropriate libraries, and the Kernel Space (in our case a Linux Kernel), which is strictly reserved for running
a privileged operating system kernel and most of the device drivers. In order to incorporate efficiently our accelerator module we have developed a set of device drivers and we have integrated them in two ARM gem5-configured 3.x kernels (ARM-32 and ARM-64 - gcc ver.5 supported).

## Build the ARM-32 Kernel
The following script compliles the kernel with [User Application](Application/TestApp.c) and mount it inside the linux-aarch32-ael.img. In addition, it creates vmlinux.aarch32.Apollon and put it in kernels/binaries directory.
```
./build32.sh
```

## Build the ARM-64 Kernel
The following script compliles the kernel with [User Application](Application/TestApp.c) and mount it inside the aarch64-ubuntu-trusty-headless.img. In addition, it creates vmlinux.aarch64.Apollon and put it in kernels/binaries directory.
```
./build64.sh
```
