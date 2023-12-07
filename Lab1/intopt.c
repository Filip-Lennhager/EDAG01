#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int printInput(double** a, double* b, double* c, int m, int n);

int main(int argc, char** argv)
{
    int m; /* Constraints */
    int n; /* Decision Varaibles */

    double* c;
    double** a; /* Matrix */
    double* b;

    scanf("%10d %10d", &m, &n);
    
    /* ------Allocating------ */
    //m = 1;
    /* Allocating c*/
    c = calloc(n, sizeof(double));

    /* Allcocating a - matrix*/
    a = calloc(m, sizeof(double*));
    for(int i=0; i< m; i++){
        a[i] = calloc(n, sizeof(double));
    }

    /* Allocating b*/
    b = calloc(m, sizeof(double));
    /* ----------------------------*/
    //m = 2;
    

    /* -----Scanning------*/
    /* Scan c */
    for(int i=0; i<n; i++){ 
        scanf("%lf", &c[i]);
    } 

    /* Scan a - matrix*/
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            scanf("%lf", &a[i][j]);
        }
    }

    /* Scan b */
    for(int i=0; i<m; i++){ 
        scanf("%lf", &b[i]);
    } 
    /* ----------------------------*/



    /*------------Printing-------------*/
    printInput(a,b,c,m,n);
    /* ---------------------------- */

    /* Deallocating variables */
    //free(m);
    //free(n);
    //a = calloc(m, sizeof(double*));
    for(int i=0; i< m; i++){
        free(a[i]); 
    }
    free(a);
    free(b);
    free(c);
        
    return 0;
}

int printInput(double** a, double* b, double*c, int m, int n){


    
    printf("Max Z = ");
    for(int i=0; i<n; i++){
        printf("%+10.3lf\t", c[i]);
    }
    
    for(int i=0; i<m; i++){
        printf("\n");
        for(int j=0; j<n; j++){
            printf("\t%+10.3lf", a[i][j]);
        }
        printf("\t\u2264%+10.3lf", b[i]);
    }
    printf("\n");

    return 0;
}


