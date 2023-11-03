## The GNU Debugger GDB

*You can read about **gdb** in Section 6.5 (on page 212) in the book.*

1. Create a struct **simplex_t** according to page 718 but with proper C syntax.

2. In this lab you should implement the following functions from Appendix B.
   - **simplex**
   - **xsimplex**
   - **pivot**
   - **initial** and assume **b ≥ 0** so skip the call to **prepare** and the rest of **initial**
   - **init**
   - **select_nonbasic**

3. It is expected that your will encounter various problems and to fix them you should use a combination of debugging with printing output and the GNU debugger **gdb**. Both are equally important to be familiar with.

4. When you have implemented these, your program should be able to solve the example input from Lab 1 (and Appendix B). The optimal value should be **16**.

5. To use **gdb** your program should be compiled with the **-g** option.

6. If your program crashes, start the debugger:
```
gdb a.out
```
and type
```
run < i
```
or just
```
r < i
```
which will start the program with input from the file **i**.

You should now see where in the source code the program crashed.

7. To print out the value of a variable, say **x**, type
```
print x
```
8. Use commands such as:
```
break xsimplex
break 100
```
to set a so-called breakpoint at the beginning of function or certain line number.

9. If you run the program again it should stop at the breakpoint.

10. Try to find out what the difference is between the two commands:
 ```
 next
 step
 ```

11. To continue execution, type
 ```
 c
 ```

12. Now call your function which prints your linear program from **gdb**. If that function is named **print** and your linear program is represented in a struct named **s**, you can type
 ```
 p print(s)
 ```

13. You will now learn a very useful trick to find out when a variable changes value. Add a global variable **int glob;** before any function.
 
 Then add for instance **glob += 1** to any function.
 
 Recompile, start **gdb** and type
 ```
 watch glob
 ```
 Run your program. It should stop when **glob** is modified.
 
 It is easy to use watch for global variables but to stop when data allocated with **malloc** or **calloc** is modified, you need to know the address of that data.
 
 Put a breakpoint in **pivot**
 
 Assuming you have a struct **s** with the array **b**, type
 ```
 p &s.b[1]
 ```
 That should give you the address of element one of the **b** array.
 
 The address will be a hexadecimal number (base 16). Assume it is **1234567890abc0** and the type of the element is **double**.
 
 Now type
 ```
 watch *(double*)0x1234567890abc0
 ```
 Continue the program and see what happens.

14. Find out what the following commands mean
 - **up**
 - **down**
 - **display**

*Wed Nov 18 17:37:58 CET 2020*