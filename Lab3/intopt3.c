#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 10e-6

struct simplex_t{
    int         m;
    int         n;
    int*        var;
    double**    a;
    double*     b;
    double*     x;
    double*     c;
    double      y;
};

void printInput(double** a, double* b, double*c, int m, int n);

void prepare (struct simplex_t* s, int k);

double simplex(int m, int n, double** a, double* b, double* c, double* x, double y);

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);

void pivot(struct simplex_t* s, int row, int col);

int initial(struct simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var);

int select_nonbasic(struct simplex_t* s);

int init(struct simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var);

int testarr[2];

int main() {
            
    int m; /* Constraints */
    int n; /* Decision Varaibles */

    double* c;
    double** a; /* Matrix */
    double* b;

    double* x;
    double y;


    scanf("%10d %10d", &m, &n);
    
    int	local_array[10];
    testarr[4] = 3;

    
    
    
    /* ------Allocating------ */
    
    /* Allocating c*/
    c = calloc(n, sizeof(double));

    /* Allcocating a - matrix*/
    a = calloc(m, sizeof(double*));
    for(int i=0; i< m; i++){
        a[i] = calloc(n, sizeof(double));
    }

    /* Allocating b*/
    b = calloc(m, sizeof(double));


    x = calloc(n+1, sizeof(double));

    //y = malloc(sizeof(double));
    /* ----------------------------*/
    

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
    printf("\nresult: %0.3lf\n",simplex(m, n, a, b, c, x, y));
    /* ---------------------------- */

    /* Deallocating variables */
    for(int i=0; i< m; i++){
        free(a[i]); 
    }
    free(a);
    free(b);
    free(c);
    free(x);
    
    return 0;
}

void printInput(double** a, double* b, double*c, int m, int n){
    
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
}

void prepare (struct simplex_t* s, int k) {
    int m = s->m;
    int n = s->n;
    int i;
    // make room for xm+n at s.var[n] by moving s.var[n..n+m-1] one
    // step to the right.
    for (i = m + n; i > n; i--) {
        s->var[i] = s->var[i-1];
    }
    s->var[n]= m + n;
    // add xm+n to each constraint
    n = n+1;
    for (i = 0; i < m; i = i +1) {
        s->a[i][n-1]  = -1;
    }

    s->x = calloc(m+n, sizeof(double)); // new double [m+ n];
    s->c = calloc(n, sizeof(double)); //new double [n];

    
    s -> c[n-1]= -1;
    s -> n = n;
    pivot(s, k, n-1);
}


double simplex(int m, int n, double** a, double* b, double* c, double* x, double y) {
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h) {
    struct simplex_t   s;
    int                 i, row, col;

    

    if (!initial(&s, m, n, a, b, c, x, y, var)) {
        free(s.var);
        return NAN; //not a number.
    }

    while ((col = select_nonbasic(&s)) >= 0) {
        row = -1;
        for (i = 0; i < m; ++i) {
            if (a[i][col] > EPSILON &&
            (row < 0 || b[i] / a[i][col] < b[row] / a[row][col])) {
                row = i;
            }
        }

        if (row < 0) {
            free(s.var);
            return INFINITY; //unbounded.
        }

        pivot(&s, row, col);
    }

    if (h == 0) {
        for (i = 0; i < n; ++i) {
            if (s.var[i] < n) {
                x[s.var[i]] = 0;
            }
        }
        for (i = 0; i < m; ++i) {
            if (s.var[n + i] < n) {
                x[s.var[n + i]] = s.b[i];
            }
        }
        free(s.var);
    } else {
        for (i = 0; i < n; ++i) {
            x[i] = 0;
        }
        for (i = n; i < n + m; ++i) {
            x[i] = s.b[i - n];
        }
    }

    double result = s.y;

    return result;
}

void pivot(struct simplex_t* s, int row, int col) {
    double**        a = s->a;
    double*         b = s->b;
    double*         c = s->c;
    int             m = s->m;
    int             n = s->n;
    int             i, j, t;

    t = s->var[col];
    s->var[col] = s->var[n + row];
    s->var[n + row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];

    for (i = 0; i < n; ++i) {
        if (i != col) {
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
        }
    }
    c[col] = -c[col] / a[row][col];

    for (i = 0; i < m; ++i) {
        if (i != row) {
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }

    for (i = 0; i < m; ++i) {
        if (i != row) {
            for (j = 0; j < n; ++j) {
                if (j != col) {
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }  
        }
    }

    for (i = 0; i < m; ++i) {
        if (i != row) {
            a[i][col] = -a[i][col] / a[row][col];
        }
    }

    for (i = 0; i < n; ++i) {
        if (i != col) {
            a[row][i] = a[row][i] / a[row][col];
        }
    }

    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];
   //printf("pivot:\n");
    //printf("row: %d\n", row);
    //printf("col: %d\n", col);
}

int initial(struct simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
    int             i, j, k;
    double          w;

    k = init(s, m, n, a, b, c, x, y, var);

    //assume b[k] >= 0.
    return 1; //feasible.

    if (b[k] >= 0) {
        return 1; //feasible.
    }

    prepare(s,k);
    n = s->n;
    s->y = xsimplex(m,n, s->a, s->b, s->c, s->x, 0, s->var, 1);

    for (i = 0; i < m+n; ++i) {
        if (s->var[i] == m+n+1) {
            if (fabs(s->x[i]) > EPSILON) {
                free(s->x);
                free(s->c);
                return 0; //infeasible.
            } else {
                break;
            }
        }
    }

    if(i >= n){ 
        // xn+m is basic. ﬁnd good nonbasic.
        for (j = k = 0; k < n; k = k + 1)
            if (fabs(s->a[i - n][k]) > fabs(s->a[i - n][ j])) {
                j=k;
            }
        pivot(s,i-n,j);
        i=j;
    }
        
    if(i < n-1) {
        // xn+m is nonbasic and not last. swap columns i and n-1
        k = s -> var[i]; s->var[i] = s->var[n-1]; s -> var[n-1] = k;
        for (k = 0; k < m; k = k + 1) {
            w = s->a[k][n-1]; s -> a[k][n-1] = s -> a[k][i]; s -> a[k][i] = w;
        }
    }
    else {} // xn+m is nonbasic and last. forget it.
        
    free(c); //delete s.c
    s->c = c;
    s->y = y;
    for (k = n-1; k < n+m-1; k = k + 1){
        s->var[k] = s->var[k+1];
    }
    n = s->n = s->n - 1;

    double* t; //= new double [n]
    t = calloc(n, sizeof(double));
    //double t[n];

    for (k = 0; k < n; k = k + 1) {
        for (j = 0; j < n; j = j + 1)
            if (k = s->var[j]){ 
                // xk is nonbasic. add ck
                t[j] = t[j] + s->c[k];
                goto next_k;
            }


        // xk is basic.
        for (j = 0; j < m; j = j + 1) {
            if (s->var[n+j] = k) {
                // xk is at row j
                break;
            }
        }
        s->y = s->y + s->c[k] * s->b[j];
        for (i = 0; i < n; i = i + 1) {
            t[i] = t[i] - s->c[k] * s->a[j][i];
        }

    next_k:
    }



    for (i = 0; i < n; i = i + 1) {
        s->c[i] = t[i];
    }
    //delete t and s.x
    free(t);
    free(x);

    return 1;
}

int select_nonbasic(struct simplex_t* s) {
    int i;
    for (i = 0; i < s->n; ++i) {
        if (s->c[i] > EPSILON) {
            return i;
        }
    }
    return -1;
}

int init(struct simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
    int i, k;

    //s = (struct simplex_t*)malloc(sizeof(struct simplex_t));

   // *s = (simplex_t){ m, n, var, a, b, c, x, y };

    
    s->m = m;
    s->n = n;
    s->a = a;
    s->b = b;
    s->c = c;
    s->x = x;
    s->y = y;
    s->var = var;
    
    if (s->var == NULL) {
        s->var = (int*)calloc(m + n + 1, sizeof(int));
        //s->var = malloc(sizeof(int) * (m+n+1));

        
        for (i = 0; i < m+n; ++i) {
            s->var[i] = i;
        }
        
    }

    for (k = 0, i = 1; i < m; ++i) {
        if (b[i] < b[k]) {
            k = i;
        }
    }

    return k;
}