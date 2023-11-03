## File input and memory allocation in C

*The purpose of this lab is to learn about reading numbers from a text file, allocating memory, and printing numbers with nicely aligned columns. Below ''the book'' refers to the course book printed in 2020, and in particular Appendix B which you can find as a pdf in the Tresorit directory you got a link to in an email.*

1. The first task is to create a hello-world program. Start an editor and a terminal window.

2. Write a hello world program, such as on in Section 1.1 in the book, (if you don't have the book, take e.g. the one from the slide in the first lecture), and save it in a file called e.g. **intopt.c**.

3. Try to locate the file and **cd** to the same directory in the terminal (see e.g. Appendix A in the book).

4. Type the following command in the terminal window:
gcc intopt.c

csharp
Copy code

5. Run it with:
./a.out

6. Next change your program to read two integer numbers from stdin. See first lecture.
The numbers should be stored in two variables called **m** and **n**.
When you run your program, you need to hit the key called "return" or "enter" after the two numbers.

7. Print their values using the formatting string **"m = %d, n = %d\n"**.

8. To avoid typing so much, put your input in a file, called **i** and run with this command:
./a.out < i

9. Now it is time to allocate memory for a linear program. 
The easiest way to create the matrix is to use a double** pointer. 
See book or first few slides of F03. You can also get inspiration from the **check** function in **main.c** in the intopt directory from Tresorit (type **tar xvf intopt.tar.bz2** to expand it).
It is also a good idea to look at the video clips for Lecture 2.
But for lab 1, you don't have to understand anything about linear programs. It is only about reading the input and printing it.
See Appendix B, and use the following format for the input, where m is the number of constraints and n the number of decision variables.

As you can see in the youtube clips, there is a vector with *n* c-coefficients, a matrix with *m* rows and *n* columns, and a vector with *m* b-values. The matrix and vectors should have elements of type **double**.
m n
c_0 c_1 ... c_{n-1}
a_00 a_01 ... a_{0,n-1}
a_10 a_11 ... a_{1,n-1}
...
a_{m-1,0} a_{m_1,1} ... a_{m-1,n-1}
b_0 b_1 ... b_{m-1}

Use the following input (from Appendix B):
2 2
1 2
-0.5 1
3 1
4 18

10. Print out the linear program to check that it is what you expect.

11. Try to print your linear program with nice columns. Hint: if you instead of only using %lf instead use %10.3lf with printf, it will print the numbers 10 characters wide and with 3 decimal digits.

 Compile and run the following program to get ideas about making nice columns!
 ```c
 #include <stdio.h>
 #include <math.h>

 int main()
 {
         double  pi = 4 * atan(1.0);

         printf("%lf\n", pi);
         printf("%+lf\n", pi);
         printf("%-lf\n", -pi);
         printf("% lf\n", pi);
 }
 ```
 Also add ''max z = '' and ≤ and + between columns.
 You can use `<=` or Unicode in the C string such as `"a \u2264 b"` for
 **a ≤ b**.

12. To make the output even neater, you can experiment with using some additional flags. See the course book or give the command:
 ```
 man printf
 ```

13. Now change your program to allocate too little data. Allocate memory for only one instead of two coefficients in the objective function. Run your program and see if anything strange happens.

14. Next compile with -g to tell gcc to add debug information to a.out:
 ```
 gcc -g intopt.c
 ```
 Use Valgrind to let it find the array-index-out-of-bounds error:
 ```
 valgrind ./a.out < i
 ```
 What does it say?

15. Next try the Google sanitizer:
 ```
 gcc -g -fsanitize=address intopt.c
 ./a.out
 ```
 It will say some obscure information but also some very useful things.

16. If you download the intopt program with the input files for your platform from Tresorit, you can try them as follows:
 ```
 ./intopt -f simplex.in
 ```
 and
 ```
 ./intopt -p -a intopt.in
 ```

*Thu Nov  5 10:27:21 CET 2020*