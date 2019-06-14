# toppers-asp-for-linux

https://www.toppers.jp/asp-kernel.html

## Platform

* Ubuntu 16.04LTS (i386, x64)
  * 32bit binary available
  * 64bit binary available also

## Required packages

* git
* gcc-multilib (5.3.1-1ubuntu1)
* libboost-dev (1.58.0.1ubuntu1)
* libboost-system-dev (1.58.0.1ubuntu1)
* libboost-filesystem-dev (1.58.0.1ubuntu1)
* libboost-program-options-dev (1.58.0.1ubuntu1)
* libboost-regex-dev (1.58.0.1ubuntu1)

## Quick Start (sample1)
    sudo apt-get install git gcc-multilib libboost-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-regex-dev
    git clone https://github.com/morioka/toppers-asp-for-linux
    cd toppers-asp-for-linux/asp
    cd cfg
    make clean
    make depend
    make
    cd ..
    mkdir obj
    cd obj
    ../configure -T linux_gcc
    make depend
    make
    ./asp
