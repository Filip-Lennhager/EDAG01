## Performance measurements: operf, gprof, gcov and valgrind

*In this lab you should implement the remaining functions from Appendix B.*

*As a rule, compile your intopt.c with -lm at the end of the command line which tells the compiler to search for math functions when producing a.out (this needs to be done in addition to including math.h).*

1. You need to do this lab on a computer with **operf** enabled, which means either **power.cs.lth.se** or your own Linux machine. See page 221 for how to enable **operf** on Linux. See Appendix A for how to work remotely with a Unix machine (Linux or macOS) and login to **power.cs.lth.se**.

2. You will use but don't have to implement **isfinite** from the pseudo code since it is part of the Standard C library.

3. When you have implemented these functions your integer program solver can find the optimal solution for our example input --- since all variables happen to be integers.

4. Now run with larger input, which you can find in intopt/test in the tresorit directory. intopt/test/N contains directories with N decision variables and inequalities. Start for example with input from a subdirectory of input/test/20.

   You will next use four different program for performance measurements.

5. First type
```
operf -e CYCLES:100000:0:0:1 ./a.out < input
```

See page 220. Your program is run with hardware counters enabled which count the number of clock cycles.

Use **opreport** and **opannotate** to figure out which functions take most of the time in your program.

6. Next compile with
```
gcc -pg -g intopt.c
```

and run your program. Then give the command
```
gprof -T a.out
```

What does the output mean and how can you use it to better understand your program?

7. Next compile with
```
gcc -fprofile-arcs -ftest-coverage -g intopt.c
```

and run your program again. Then give the command
```
gcov intopt.c
```

This will create a file **intopt.c.gcov**. What does it contain how and can you use this information?

To get even more detailed information about which cases in your program are most common, you can use
```
gcov -b intopt.c
```

What does it say?

8. In addition to finding memory errors, Valgrind can also be useful when doing performance measurements. Valgrind is slower than the other tools due to it simulates the computer so use the small input, at least initially. Recompile with -O3 and give the command
```
valgrind --tool=cachegrind --I1=65536,1,128 --D1=32768,2,128 \
> --LL=1048576,8,128 ./a.out < i
```

These options specify the cache parameters to match our POWER8 machine.

How can you see how many instructions in total, load instructions, and store instructions are executed?

What are the cache miss rates and are the cache likely to be a performance problem for this input?

*Tue Dec  1 10:22:31 CET 2020*