$GEM5/build/ARM/gem5.opt --listener-mode=on -r -d $GEM5/node0 $GEM5/configs/example/fs.py --kernel=vmlinux.aarch32.Apollon --disk-image=linux-aarch32-ael.img --mem-size=512MB --SynchTime=1ms --RxPacketTime=100us --TotalNodes=2 --nodeNum=0 --machine-type=VExpress_EMM --dtb=vexpress.aarch32.ll_20131205.0-gem5.1cpu.dtb --script=$GEM5/configs/boot/COSSIM/script0.rcS --PtolemySynchTime=100us --cossim &
$GEM5/build/ARM/gem5.opt --listener-mode=on -r -d $GEM5/node1 $GEM5/configs/example/fs.py --kernel=vmlinux.aarch32.Apollon --disk-image=linux-aarch32-ael.img --mem-size=512MB --SynchTime=1ms --RxPacketTime=100us --TotalNodes=2 --nodeNum=1 --machine-type=VExpress_EMM --dtb=vexpress.aarch32.ll_20131205.0-gem5.1cpu.dtb --script=$GEM5/configs/boot/COSSIM/script1.rcS --cossim &