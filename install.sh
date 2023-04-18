#!/bin/bash
sudo apt-get update
sudo apt-get upgrade gcc
sudo apt-get install binutils
sudo apt-get install pkg-config
sudo apt-get install zlib1g-dev
sudo apt-get install libglib2.0-dev
sudo apt-get install libfdt-dev
sudo apt-get install libpixman-1-dev
sudo apt-get install gcc libc6-dev
sudo apt-get install autoconf
sudo apt-get install libtool
sudo apt-get install libsdl1.2-dev
sudo apt-get install g++
sudo apt-get install libx11-dev
sudo apt-get install libxrandr-dev
sudo apt-get install libxi-dev
sudo apt-get install libnfs-dev libiscsi-dev
sudo apt-get install valgrind
sudo apt-get install ninja-build
sudo apt-get upgrade perl
sudo apt-get install libc6-dbg gdb
sudo apt-get install make
sudo apt-get install git
mkdir $HOME/apps
cd $HOME/apps
git clone https://gitlab.com/qemu-project/qemu.git
cd $HOME/apps/qemu
mkdir -p bin/debug/native
cd bin/debug/native
../../../configure --target-list=x86_64-softmmu --enable-debug
make
