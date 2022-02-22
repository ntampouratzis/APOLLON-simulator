#!/bin/bash

#Clone the APOLLON sub-parts
cd $HOME
git clone https://github.com/ntampouratzis/APOLLON.git
mv -f $HOME/APOLLON/cMcPAT/ $HOME/APOLLON/cgem5/McPat

# Installing cCERTI & Our SynchServer
mkdir $HOME/pthla
cd $HOME/pthla
git clone -b br_jbch_4.0.0 https://git.savannah.nongnu.org/git/certi.git
mkdir $HOME/pthla/certi-tools
cd $HOME/pthla/certi
mkdir $HOME/pthla/certi/build-certi
cd $HOME/pthla/certi/build-certi
cmake -DCMAKE_INSTALL_PREFIX=$HOME/pthla/certi-tools $CMAKE_FLAGS ../
make
make install

mv $HOME/APOLLON/SynchServer $HOME/pthla/certi
cd $HOME/pthla/certi/SynchServer
./build.sh
cp Federation.fed $HOME/pthla/certi-tools/share/federations

echo "#cCERTI exports" >> ~/.bashrc
echo "export CERTI_HOME=$HOME/pthla/certi-tools" >> ~/.bashrc
echo "export CERTI_FOM_PATH=$HOME/pthla/certi-tools/share/federations" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/pthla/certi-tools/lib" >> ~/.bashrc
echo "export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/pthla/certi-tools/lib" >> ~/.bashrc
echo "export PATH=$HOME/pthla/certi-tools/share/scripts:$PATH" >> ~/.bashrc
echo "export PATH=$HOME/pthla/certi-tools/bin:$PATH" >> ~/.bashrc
echo "export CERTI_SOURCE_DIRECTORY=$HOME/pthla/certi" >> ~/.bashrc
echo "export CERTI_BINARY_DIRECTORY=$HOME/pthla/certi/build-certi" >> ~/.bashrc
echo "#change CERTI_HOST if you want to use HLA Server (rtig) and SynchServer from another machine" >> ~/.bashrc
echo "export CERTI_HOST=127.0.0.1" >> ~/.bashrc


export CERTI_HOME=$HOME/pthla/certi-tools
export CERTI_FOM_PATH=$HOME/pthla/certi-tools/share/federations
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/pthla/certi-tools/lib
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/pthla/certi-tools/lib
export PATH=$HOME/pthla/certi-tools/share/scripts:$PATH
export PATH=$HOME/pthla/certi-tools/bin:$PATH
export CERTI_SOURCE_DIRECTORY=$HOME/pthla/certi
export CERTI_BINARY_DIRECTORY=$HOME/pthla/certi/build-certi
export CERTI_HOST=127.0.0.1

# Installing cgem5
export GEM5=$HOME/APOLLON/cgem5
export M5_PATH=$HOME/APOLLON/kernels

echo "#GEM5 exports" >> ~/.bashrc
echo "export GEM5=$HOME/APOLLON/cgem5" >> ~/.bashrc
echo "export M5_PATH=$HOME/APOLLON/kernels" >> ~/.bashrc
cd $HOME/APOLLON/cgem5
source ~/.bashrc
scons build/ARM/gem5.opt -j4
scons build/X86/gem5.opt -j4

mv $HOME/kernels.tar.gz $HOME/APOLLON
cd $HOME/APOLLON
tar -zxvf kernels.tar.gz

# Installing cMcPat
cd $HOME/APOLLON/cgem5/McPat/mcpat
make all
cd $HOME/APOLLON/cgem5/McPat/Scripts
chmod +x GEM5ToMcPAT.py
chmod +x print_energy.py


# Installing cOMNET++
cd $HOME
tar xvfz omnetpp-5.0-src.tgz
cd omnetpp-5.0
export PATH=$PATH:$HOME/omnetpp-5.0/bin
export OMNETWP=$HOME/APOLLON/OMNETPP_COSSIM_workspace/OMNET_WORKSPACE

./configure && make

cp -R $HOME/APOLLON/COSSIM_GUI/* $HOME/omnetpp-5.0/ide/dropins

mkdir $HOME/APOLLON/OMNETPP_COSSIM_workspace/OMNET_WORKSPACE/HLANode/simulations
echo "export PATH=$PATH:$HOME/omnetpp-5.0/bin" >> ~/.bashrc
echo "export OMNETWP=$HOME/APOLLON/OMNETPP_COSSIM_workspace/OMNET_WORKSPACE" >> ~/.bashrc
source ~/.bashrc
