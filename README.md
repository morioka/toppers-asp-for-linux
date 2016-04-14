# toppers-asp-for-linux

* Add a new target "linux_gcc" based on "macosx_gcc."
* Test & pass "sample1" and test programs on Ubuntu 14.04LTS (x86_64).

* Quick Start (sample1)

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

* about the Linux's pointer guard feature

The pointer guard feature should be disabled to
rewrite the setjmp/longjmp stack for task-switching.
The pointer guard feature was disabled, when
the LD_POINTER_GUARD enviornment variable value was 0.
A user could run the TOPPERS apps, like this:

  % env LD_POINTER_GUARD=0 ./asp      (csh)
  % export LD_POINTER_GUARD=0 ./asp   (sh/bsh)

Unfortunately,after the vulnerability CVE-2015-8777 
was fixed, the LD_POINTER_GUARD environment variable 
can no longer be used to disable the pointer guard feature. 
The pointer guard feature is always enabled.

In such system, we have to emulate the behavior of 
the pointer guard feature by -DWORKAROUND_CVE_2015_8777_FIX.
The code is the pointer mangling/de-mangling code, 
from the glibc source code. This may make the whole 
code (or at least the target dependent part) GPL'ed?


if macro DISABLE_PTRMANGLE_SELF_IMPL is defined,
then  set LD_POINTER_GUARD=0 before run ./asp

see http://d.hatena.ne.jp/yupo5656/20060820/p1

On the other hand, due to CVE-2015-8777 vulnerability, 
glibc-2.23 (or some earlier versions)  does not allow LD_POINTER_GUARD to be set to 0.
