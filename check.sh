#!/bin/bash
cd $HOME/apps/qemu/bin/debug/native/
ls
cd $HOME/apps/qemu/bin/debug/native/x86_64-softmmu
ls
sudo ln -s $HOME/apps/qemu/bin/debug/native/x86_64-softmmu/qemu-system-x86_64 /bin/qemu
which qemu
