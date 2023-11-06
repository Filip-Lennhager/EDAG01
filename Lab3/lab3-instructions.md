## Lab 3: Advanced Valgrind and Google Sanitizer

*In this lab you should implement the following functions from Appendix B which will then make your linear program solver complete.*

- **prepare**
- make **initial** complete (Appendix B, page 720)

1. When all elements of the **b** array are non-negative, we can solve the linear program starting in the origin, i.e. vertex **0**.

2. Use the same input as in the previous labs but add the constraint that all feasible points should be on or above the line _x_1 = 5 - x_0_. See Figure B.1.

   How do you express this constraint in your linear program from the previous labs?

   Will this new constraint affect the value of the optimal solution?

3. Implement the function **prepare** and add the call to it in **initial**.
   Verify that your program reaches the **prepare** function.

4. Implement the rest of the function **initial** make sure it works for the example input.

5. It is now time to use Valgrind to check that all memory allocated from the heap also is deallocated.
   Run your program with
```
valgrind ./a.out < i
```

Valgrind will most likely complain about memory leaks due to not freeing everything allocated.

Try to figure out what was not freed and modify your program accordingly.

If that turns out to be complicated, use instead
```
valgrind --leak-check=full --show-reachable=yes ./a.out < i
```

6. Now destroy your program by commenting out the first for-loop in the function **init**.
If you allocated memory for the **var** array with **calloc**, then switch to **malloc**.

The purpose is to see that Valgrind can warn you about using uninitialized memory.  Run with
```
valgrind ./a.out < i
```

Does Valgrind complain?

We would like to know where that memory was allocated, and therefore run with
```
valgrind --track-origins=yes ./a.out < i
```

This collects more information and is slower but can be crucial.
In **Section 6.9.1 memcheck / Reading uninitialized memory** (on page 228) you can see that Valgrind keeps track of individual bits in memory.

7. Repair your program so it works again and then make another disaster: take a for-loop and skip initializing the loop index variable. Run again with the previous command.

Does Valgrind keep track of local variables?

8. Now add the following to any function in your program:
```
int local_array[10];

for (i = 0; i < 11; i += 1)
   local_array[i] = i;
```

Does Valgrind check index-out-of-bounds for local variables allocated on the stack?

9. Repeat the last problem with a global array. What does Valgrind do?

10. Now compile with
 ```
 gcc -g -fno-common -fsanitize=address intopt.c
 ```
 Repeat the experiments with local and global arrays. What is the result?

*Wed May  6 20:08:01 CEST 2020*