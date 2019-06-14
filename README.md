# toppers-asp-for-linux

This is TOPPERS/ASP 1.9.2 Linux_gcc version.

* [TOPPERS/ASP kernel](http://toppers.jp/en/asp-kernel.html) is a real-time kernel based on the standard profile of uITRON4.0.
* Add a new target "linux_gcc" based on "macosx_gcc."
* Test & pass "sample1" and test programs on Ubuntu 14.04LTS (x86 and x86_64).

## Platform

* Ubuntu 16.04LTS (x86, x86_64)

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

## (Note) the pointer guard feature in Linux

The pointer guard feature should be disabled to rewrite the setjmp/longjmp stack for task-switching.
The pointer guard feature is disabled, when the LD_POINTER_GUARD enviornment variable is set to 0,
A user can run the TOPPERS apps, like this:

    % env LD_POINTER_GUARD=0 ./asp      (csh)
    % export LD_POINTER_GUARD=0 ./asp   (sh/bsh)

After the vulnerability CVE-2015-8777 was reported and fixed, the LD_POINTER_GUARD environment variable 
can no longer be used to disable the pointer guard feature. The pointer guard feature is always 
enabled at least glibc-2.23 or later (,and some prior versions in some Linux distributions).

* [CVE - CVE-2015-8777](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2015-8777)
* [The GNU C Library version 2.23 is now available](https://www.sourceware.org/ml/libc-alpha/2016-02/msg00502.html)
* [ld.so(8) - Linux manual page](http://man7.org/linux/man-pages/man8/ld.so.8.html)

We have to emulate the behavior of the pointer guard feature by adding the following compile option;

    -DWORKAROUND_CVE_2015_8777_FIX     (in target/linux_gcc/Makefile.target)

The emulation code is the pointer mangling/de-mangling code, from the glibc source code. 
This may make the whole code (or at least the target dependent part) GPL'ed?

