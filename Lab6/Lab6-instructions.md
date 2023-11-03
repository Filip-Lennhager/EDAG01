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

`size --common *.o`

The size of the read-only section is printed below **text** and includes instructions and constants. Global variables which should be initialized to zero are listed under **bss** which means block started by symbol (for historical reasons).

To see which variables bss refers to, type the command:

`nm 0.o`

Try to figure out what T, U, G (or D), and C mean. The explanation is given in the manual page for `nm`. Type:

`man nm`

What does `-Os` mean? All files compiled at levels 3--5 should have identical sizes. To compare whether two files are identical, use the command `diff file1 file2`

If `diff` prints no output, the files are identical.

5. Now run the following shell command:
```
for x in s 0 1 2 3
do
gcc -O$x -o $x mipsx.c tbr.s timebase.c
./$x
done
```

6. The remaining compilations should use -O3 and some other flags.

7. To understand what happened, type the following:
`objdump -d mipsx > x`

8. **Feedback-directed optimizations** uses statistics (usually about branches) from previous executions and exploits that information when optimizing a file.

9. Related to **-fprofile-generate** are the options used in the following command:
`gcc -g -fprofile-arcs -ftest-coverage mipsx.c tbr.s timebase.c`

10. A recent addition to GCC is **link-time optimization**.

11. Now take the matrix multiplication program **clang-matmul.c** and try to get clang to vectorize the inner loop.

12. Next compare clang, gcc, IBM xlc, and Nvidia pgcc on the original matmul.c

13. You can read more about IBM xlc [here](https://www.ibm.com/us-en/marketplace/xl-cpp-linux-compiler-power).

14. You can learn more about optimizing compilers [here](http://cs.lth.se/edan75).

_Thu Dec 17 13:24:14 CET 2020_




