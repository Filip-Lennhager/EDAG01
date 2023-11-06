# EDAG01 Lab 6: Optimizing compilers

_The purpose of this lab is to learn more about the optimization options of GCC and optimizing compilers in general. We will use a benchmark called the Stanford benchmarks, used for the MIPSX project in the beginning of the 1980's. The input sizes have been adapted for our POWER8 machine, however._

Online documentation about the optimization options of GCC is available [here](http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html).

1. Log in to **power.cs.lth.se** and start **bash** in your terminal (to make sure the script below work).

2. Copy lab6 from
```
/usr/local/cs/edag01/labs/lab6
```

3. Go to **lab6**

4. The first task in the lab is to determine which optimization levels have any additional effect with gcc. Compile the program **mipsx.c** with different optimization levels and save each relocatable file, using the command:

```
for x in s 0 1 2 3 4 5
do
gcc -c -O$x -o $x.o mipsx.c
done
```

The `for` loop is equivalent to writing the following commands:
```
gcc -c -Os -o s.o mipsx.c
gcc -c -O0 -o 0.o mipsx.c
gcc -c -O1 -o 1.o mipsx.c
gcc -c -O2 -o 2.o mipsx.c
gcc -c -O3 -o 3.o mipsx.c
gcc -c -O4 -o 4.o mipsx.c
gcc -c -O5 -o 5.o mipsx.c
```

Level zero is the same as no optimization. To determine the size of the instructions in a file, the command `size` is used: 

```
size --common *.o
```

The size of the read-only section is printed below **text** and includes instructions and constants. Global variables which should be initialized to zero are listed under **bss** which means block started by symbol (for historical reasons).

To see which variables bss refers to, type the command:

```
nm 0.o
```

Try to figure out what T, U, G (or D), and C mean. The explanation is given in the manual page for `nm`. Type:

```
man nm
```

What does `-Os` mean? All files compiled at levels 3--5 should have identical sizes. To compare whether two files are identical, use the command 
```
diff file1 file2
```

If `diff` prints no output, the files are identical.

5. Now run the following shell command:
```
for x in s 0 1 2 3
do
gcc -O$x -o $x mipsx.c tbr.s timebase.c
./$x
done
```

POWER processors have a special register called the timebase register which can be used for very accurate timing (avoid making a system call to ask the operating system kernel what time it is, which normally is what happens when you use the ISO C clock function).

You can check the current CPU frequences using:
```
cat /proc/cpuinfo
```

The files `tbr.s` and `timebase.c` are used for this. Note that the POWER8 processor sometimes changes its clock frequency but that is usually not a big problem. Run your programs a few times to see if it appears to remain the same.

Then run the programs optimized at the different levels and note whether they improve in speed.

6. The remaining compilations should use -O3 and some other flags. It is sometimes useful to tell the compiler for which pipeline it should optimize that code such as with `-mcpu=power8`. It has no effect on gcc on our machine but the IBM xlc compiler can sometimes produce different code.

Try now to figure out if gcc could vectorize the mipsx.c program.

7. To understand what happened, type the following:
```
objdump -d mipsx > x
```

This disassembles the program and writes to the file `x`. Edit that file and search for `stvx` which is the machine instruction for storing a vector register to memory. Can you find any? Many? Can you see many other vector instructions? Examples of other vector instructions are `vmaddfp`, `vperm`, and `lvx`.

8. **Feedback-directed optimizations** uses statistics (usually about branches) from previous executions and exploits that information when optimizing a file.

Read about the options **-fprofile-generate** and **-fprofile-use** online or on page 208 of the course book. Compile with:

```
gcc -O3 -fprofile-generate mipsx.c tbr.s timebase.c
```

then run the program and then recompile with:

```
gcc -O3 -fprofile-use mipsx.c tbr.s timebase.c -mcpu=power8
```

What is the effect and what can have happened? You need to run the programs a few times.

9. Related to **-fprofile-generate** are the options used in the following command:
```
gcc -g -fprofile-arcs -ftest-coverage mipsx.c tbr.s timebase.c
```

Compile and run. These options do not give feedback to GCC but rather to you. The command:

```
gcov mipsx.c
```

will produce a file `mips.c.gcov` with statistics about how many times each line was executed. Adding `-b` will in addition print statistics about branches, as explained on page 224 in the book.

10. A recent addition to GCC is **link-time optimization**. It means that optimization is performed during linking, i.e. when all relocatable files of a program are available. Issue the following commands:
```
cat a.c b.c
```
```
gcc -O3 a.c b.c
```
(dump the file and disassemble)
```
objdump -d a.out > x 
```


Then open the file `x` to see what the function `main` looks like.

Search for
```
main>:
```

Redo the same thing but add `-flto` to GCC. Check what `main` now was compiled to!

11. Now take the matrix multiplication program **clang-matmul.c** and try to get clang to vectorize the inner loop. Is there any array reference which would make it difficult to put the elements in a vector register? If so, what can you do about it? What else would be affected by that change?

You should modify the **clang-matmul.c** file!

Compile with:

```
clang -O3 clang-matmul.c tbr.s timebase.c -mcpu=power8 && ./a.out
```

Verify that the code was vectorized.

12. Next compare clang, gcc, IBM xlc, and Nvidia pgcc on the original matmul.c

```
clang -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.clang
```

```
gcc -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.gcc
```

```
xlc -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.ibm
```
```
/opt/nvidia/hpc_sdk/Linux_ppc64le/20.9/compilers/bin/pgcc -O3 matmul.c tbr.s timebase.c -tp=pwr8 -o matmul.nvidia
```

```
echo -n "clang: "; ./matmul.clang
```

```
echo -n "gcc: "; ./matmul.gcc
```

```
echo -n "ibm: "; ./matmul.ibm
```

```
echo -n "nvidia: "; ./matmul.nvidia
```

13. You can read more about IBM xlc [here](https://www.ibm.com/us-en/marketplace/xl-cpp-linux-compiler-power).

14. You can learn more about optimizing compilers [here](http://cs.lth.se/edan75).

_Thu Dec 17 13:24:14 CET 2020_




