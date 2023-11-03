# Lab 5: Performance measurements: the IBM POWER8 pipeline simulator

*In this lab you will study your intopt using a cycle-accurate pipeline simulator from IBM, which you can find the in sim_ppc.tar in the tresorit directory.*

### Introduction

- The IBM pipeline simulator takes as input an *instruction trace*, which is a file with a sequence of:
  - instruction address
  - instruction opcode
  - optional data address
  
  Thus neither register nor memory contents are included in the trace.

- For efficiency, this file is in a binary format, but a human-readable ASCII file can also be created.

- The instruction trace is created by a special version of valgrind, using the IBM tool *itrace*.

- The output from itrace is then converted to another format for the simulator.

- Then the simulator is run, and it analyzes essentially all details relevant for performance of the pipeline and produces an output file which can be used by another IBM program, *scrollpv* to visualize the execution, with remarks about what is happening in the pipeline such as that an instruction is waiting for input operands.

- The scrollpv program is written in Java and is most easily used at home (but in theory you can login with ssh -Y to power.cs.lth.se but I suspect it will be far too slow). The scrollpv program can be downloaded from [IBM](https://developer.ibm.com/linuxonpower/sdk-download/), or alternatively, you can download it from the Tresorit directory (see email).

- You need to create the trace files on power.cs.lth.se so be sure you have setup ssh to avoid having to type your password (use **ssh-keygen** and then **ssh-copy-id stilid@power.cs.lth.se** --- see appendix A (also in Tresorit)).

- Note that what we call **reorder buffer** in the course is called **global completion table** in IBM terminology.

### Lab instructions

1. Copy the following directory on power.cs.lth.se to your home directory on that machine:
/usr/local/cs/edag01/labs/lab5

bash
Copy code

2. There you can find the following script as the file **t**
```
export FILE=$1
rm -rf a.out *.vgi *.qt *.dir && \
gcc $FILE.c -fno-tree-vectorize -O3 -mcpu=power8 -fno-inline && \
/opt/at13.0/bin/valgrind/valgrind --tool=itrace --fn-start=main --binary-outfile=$FILE.vgi --num-insns-to-collect=1k ./a.out && \
/opt/at13.0/bin/valgrind/vgi2qt -f $FILE.vgi -o $FILE.qt && \
/opt/ibm/sim_ppc/sim_p8/bin/run_timer $FILE.qt 1000 100 1 $FILE -inf_all -p 1 -b 1 -e 2000
```
It can be used to compile a C file, making the trace and running the simulator.

To analyze a file a.c, invoke the script with the parameter a, such as with ./t a.

It is easier to learn about the simulator if only simple instructions (no SIMD) are used so we compile with -fno-tree-vectorization.

Always tell valgrind in which function tracing should start as is done above with --fn-start=main (about 200,000 instructions are executed before main is reached with dynamic linking, and valgrind dislikes static linking).

The number of instructions to collect can be specified with k, m, or g.

The simulator is called run_timer and takes the following parameters above:

 - a qt-trace file ($FILE.qt),
 - the number of instructions to simulate (1000),
 - how often it should print on stdout its progress (once every 100 instructions),
 - when certain counters should be reset, (1 = not reset),
 - a name X, use for two files X.pipe and X.results, ($FILE),
 - -inf_all to ignore the memory hierarchy which assumes all cache accesses hit L1 caches. This avoids having hundreds of wasted cycles in the pipeline visualizer but must of course be used with caution.
 - -p 1 visualize based on real instructions (and not e.g. cycles),
 - -b 1 begin visualization at instruction 1,
 - -e 2000 end visualization at instruction 2000 (which is more than enough).

3. Next look at the program a.c. First make a rough guess how many clock cycles each iteration takes, and then use the simulator and scrollpv to check in the next few tasks.

4. Create a trace file with ./t a

5. Disassemble the a.out with the command objdump -d a.out > x

6. Download the directory lab5 (at least the files x, a.config, a.pipe, and a.results).

Open x in an editor and search for the instructions in the main-function. You can find them using the pattern:

```
main>:
```
7. You should see the lines:
```
    10000420:   02 10 40 3c     lis     r2,4098
    10000424:   00 7f 42 38     addi    r2,r2,32512
    10000428:   64 00 c0 38     li      r6,100
    1000042c:   00 00 00 60     nop
    10000430:   78 87 e2 38     addi    r7,r2,-30856
    10000434:   00 00 00 60     nop
    10000438:   38 81 02 39     addi    r8,r2,-32456
    1000043c:   00 00 00 60     nop
    10000440:   58 84 42 39     addi    r10,r2,-31656
    10000444:   00 00 20 39     li      r9,0
    10000448:   a6 03 c9 7c     mtctr   r6
    1000044c:   14 00 00 48     b       10000460 
    10000450:   00 00 00 60     nop
    10000454:   00 00 00 60     nop
    10000458:   00 00 00 60     nop
    1000045c:   00 00 42 60     ori     r2,r2,0
    10000460:   ae 4c 08 7c     lfdx    f0,r8,r9
    10000464:   ae 4c 8a 7d     lfdx    f12,r10,r9
    10000468:   2a 60 00 fc     fadd    f0,f0,f12
    1000046c:   ae 4d 07 7c     stfdx   f0,r7,r9
    10000470:   08 00 29 39     addi    r9,r9,8
    10000474:   ec ff 00 42     bdnz    10000460 
    10000478:   00 00 60 38     li      r3,0
    1000047c:   20 00 80 4e     blr
```


The loop starts at **10000460** and ends at **10000474** so six instructions per iteration.

8. Start scrollpv and in the upper left corner select the File menu and open the file a.pipe (you can have four different traces opened at a time).

9. You will see a matrix filled with dots and some characters. The X-axis is time or more exactly clock cycles and the Y-axis is instructions.

In scrollpv and the column Mnemonic you should see the same instructions (except that lis and li are not seen since they are so called synthetic instructions and actually variants of or instructions).

You can also see that each store instruction stfdx appears twice and this is due to it is split into two internal operations: one for adding the registers to produce the data address and another for writing to memory.

In the matrix, you can see what happens to each instruction each clock cycle. You can see lots of D which mean the instruction is in one of the decode pipeline stages.

You can also see that the floating point add fadd has many E which mean execute, i.e. a floating point unit is actually working on the instruction.

In the Info bar the character is explained which you can see when you hover the mouse over a character (such as an E).

When the mouse is in the matrix and you press the left mouse button, you will see a crosshair that you can move around and easier find the clock cycle (at the top of the window) when an event happened.

Use it to find the clock cycle when the first load lfdx left the reorder buffer of the pipeline, i.e. completed, which is marked as a C (it does not matter much which event you look for but the last is easy to find).

Write down the cycle.

Now calculate which internal operation the last floating point load instruction is (i.e. the Iop Id in the first column) using the number of iterations in the loop (see source code) and the number of internal operations in each iteration.

Scroll down to this internal operation (i.e. instruction) and look a bit below to see that the main function is returning so that you know you found the last iteration (you should find the blr at address 1000047c --- blr means branch-to-contents-of link register).

Find the clock cycle when the load in the last iteration completed and calculate the number of cycles per iteration.

10. Now redo the same with the file b.c and explain why each iteration takes more clock cycles (not so easy as you might first think by counting the number of arithmetic operations in the source code, but not the least difficult).

11. Now change the t script and use your intopt with the file in. Start tracing in pivot and view the result in scrollpv.

12. Look for the worst cases of pipeline stalls, i.e. red characters. Try to explain what they are caused by. This will differ between different programs, obviously, but the purposes are

 - that you should see which arithmetic operations should be avoided, if possible, and

 - you should get a better understanding of what happens in pivot.

***Warning: important rule about tuning floating point software: even if you identify an optimization which is mathematically equivalent, it might not work with floating point types due to limited precision, which means testing on a few examples only is a bad idea.***

13. Finally, what is a rename register, and do both speculative and non-speculative execution need rename registers and why?

*Tue Dec 1 14:29:44 CET 2020*