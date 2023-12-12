#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

struct node_t{
    int m; /*Constrainsts*/
    int n; /* Decision Variables*/
    int k; /* Parent branches i */
    int h;
    double xh;
    double ak;
    double bk;
    double* min; /* [n] */
    double* max; /* [n] */
    double** a; /* [m][n] */
    double* b; /* [m] */
    double* x; /* [b] */
    double* c; /* [n] */
    double z;
};

struct set_t {
    int             count;
    int             alloc;
    struct node_t** nodes;
};

double intopt(int m, int n, double** a, double* b, double* c, double* x);

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
    //printf("\nresult: %0.3lf\n",simplex(m, n, a, b, c, x, y));
    //printf("\nresult: %0.3lf\n",intopt(m, n, a, b, c, x));

    double z = intopt(m, n, a, b, c, x);
    printf("result: %lf\n", z);

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







struct node_t* initial_node (int m, int n, double** a, double* b, double* c)
{
    int i;
   // auto p = allocate memory for a node
    struct node_t *p = malloc(sizeof(struct node_t));

    
     
    //p->a[m+1][n+1]; //new double [m+1][n+1]
    p->a = calloc(m+1, sizeof(double*));
    for(i=0; i<= n; i++){
        p->a[i] = calloc(n, sizeof(double));
    }

    p->b = calloc(m+1, sizeof(double));//;->b[m+1]; // = new double [m+1]
    p->c = calloc(n+1, sizeof(double));; //= new double [n+1]
    p->x = calloc(n+1, sizeof(double));// = new double [n+1]
    p->min = calloc(n, sizeof(double));;//; = new double [n]
    p -> max = calloc(n, sizeof(double));
    p -> m = m;
    p -> n = n;
    //copy a, b, and c parameters to p

    for (i = 0; i < m; i++) {
        memcpy(p->a[i], a[i], n * sizeof(double));
    }
    memcpy(p->b, b, m * sizeof(double));
    memcpy(p->c, c, n * sizeof(double));

    for (i = 0; i < n; i = i + 1) {
        p -> min[i] = -INFINITY;
        p -> max[i] = INFINITY;
    }
    return p;
}

struct node_t* extend (struct node_t* p, int m, int n, double** a, double* b, double* c, int k, double ak, double bk) {

    //auto q = allocate memory for a node
    struct node_t *q = malloc(sizeof(struct node_t));

    int i,j;

    q->k = k;
    q->ak = ak;
    q->bk = bk;

    if (ak > 0 && p->max[k] < INFINITY ){
        q->m = p->m;
    } 

    else if (ak < 0 && p->min[k] > 0 ){
            q->m = p->m;
    }
    else{
        q->m = p->m + 1;
    }

    q->n = p->n;
    q->h = -1;

    //q->a = new double [q.m+1][q.n+1] // note normally q.m > m
    q->a = calloc(q->m+1, sizeof(double*));
    for(i=0; i < q->n+1; i++){
        p->a[i] = calloc(n, sizeof(double));
    }

    q->b = calloc(q->m+1, sizeof(double));//new double [q.m+1]
    q->c = calloc(q->n+1, sizeof(double));//new double [q.n+1]
    q->x = calloc(q->n+1, sizeof(double));//new double [q.n+1]
    q->min = calloc(n, sizeof(double));//new double [n]
    q->max = calloc(n, sizeof(double));//new double [n]


    //copy p.min and p.max to q // each element and not only pointers
     memcpy(q->min, p->min, n* sizeof(double));
     memcpy(q->max, p->max, n* sizeof(double));


    //copy m ﬁrst rows of parameter a to q.a // each element
    for(i=0; i<m; i++){
        memcpy(q->a[i], a[i], m* sizeof(double));
    }


    //copy m ﬁrst elements of parameter b to q.b
    memcpy(p->b, c, m * sizeof(double));


    //copy parameter c to q.c // each element
    memcpy(p->c, c, n * sizeof(double));


    if (ak > 0) {
        if (q->max[k] = INFINITY || bk < q->max[k]) {
        q->max[k] = bk;
        }
    }
    else if (q->min[k] = -INFINITY || -bk > q->min[k]) {
        q->min[k] = -bk;
    }
    for (i = m, j = 0; j < n; j = j + 1) {
        if (q->min[j] > -INFINITY) {
            q->a[i][j] = -1;
            q->b[i] = -q->min[j];
            i += 1;
        }
        if(q->max[j] < INFINITY) {
            q->a[i][j] = 1;
            q->b[i] = q->max[j];
            i += 1;
        }
    }
    return q;
}

int is_integer(double* xp) {
    double x = *xp;
    double r = lround(x);

    if (fabs(r - x) < EPSILON) {
        *xp = r;
        return 1;
    } else {
        return 0;
    }
}

int integer(struct node_t* p){
    int i;
    for (i = 0; i < p->n; i = i + 1){
        if (!is_integer(&(p->x[i]))) {
            return 0;
        }
    }
    
    return 1;
}

void free_node(struct node_t* p) {
    for (int i = 0; i < p->m + 1; i++) {
        free(p->a[i]);
    }
    free(p->a);
    free(p->b);
    free(p->c);
    free(p->x);
    free(p->min);
    free(p->max);
    free(p);
}


void bound (struct node_t* p, struct set_t*  h, double* zp, double* x) {
// zp is a pointer to max z found so far
if (p->z > *zp) {
    *zp = p->z;
    //copy each element of p.x to x // save best x
    memcpy(x, p->x, p->n+1 *sizeof(double));

    

    //remove and delete all nodes q in h with q.z < p.z
    for (int i = 0; i < h->alloc; i++) {
            if (!h->nodes[i] || h->nodes[i]->z >= p->z) {
                continue;
            }

            free_node(h->nodes[i]);
            h->nodes[i] = NULL;
            h->count--;
        }

    }   
}



/*
int isﬁnite (x){

// ISO C function
    if (x is a NaN or |x| =1){
        return 0
    } 
    
    else{
        return 1
    }

}*/

int branch (struct node_t *q, double z) {

    double min,max;
    int h;
    if (q->z < z){
        return 0;
    } 
    
    for (h = 0; h < q->n; h = h + 1){
        if (!is_integer(&q->x[h])){
            if (q->min[h] = -INFINITY){
                min = 0;
            }  
            else{
                min = q->min[h];
            }
            
            max = q->max[h];
            if (floor(q->x[h]) < min || ceil(q->x[h]) > max){
                continue;
            } 
            q->h = h;
            q->xh = q->x[h];
            //delete each of a,b,c,x of q // or recycle in other way
            return 1;
        }   
    }   
    return 0;
}

void add(struct set_t* h, struct node_t* p) {
    int i;

    if (h->count < h->alloc) {
        for (i = 0; i < h->alloc; i++) {
            if ((h->nodes)[i] == NULL) {
                h->nodes[i] = p;
                h->count++;
                return;
            }
        }
    } else {
        h->alloc = h->alloc * 2;
        h->nodes = (struct node_t**)realloc(h->nodes, h->alloc * sizeof(struct node_t*));
        for (i = h->count; i < h->alloc; i++) {
            h->nodes[i] = NULL;
        }
        h->nodes[h->count] = p;
        h->count++;
    }
}

void succ (struct node_t* p, struct set_t* h, int m, int n, double** a, double* b, double* c, int k, double ak, double bk, double* zp, double* x) {
    
    struct node_t* q = extend(p, m, n, a, b, c, k, ak, bk);
    if (q == NULL) {
        return;
    }

    q->z = simplex(q->m, q->n, q->a, q->b, q->c, q->x, 0);

    if (isfinite(q->z)){
        if(integer(q)) {
            bound(q,h,zp,x);
        }
        else if(branch (q, *zp)) {
            //add q to h
            add(h, q);
            return;
        }
    }
        
    //delete q
    free_node(q);
}

struct set_t* create_set() {
    struct set_t* h = (struct set_t*)calloc(1, sizeof(struct set_t));
    h->alloc = 10;
    h->count = 0;
    h->nodes = (struct node_t**)calloc(h->alloc, sizeof(struct node_t*));

    for (int i = 0; i < h->alloc; i++) {
        h->nodes[i] = NULL;
    }

    return h;
}

struct node_t* pop(struct set_t* h) {
    struct node_t* p;
    for (int i = 0; i < h->alloc; i++) {
        if ((h->nodes)[i]) {
            p = h->nodes[i];
            h->nodes[i] = NULL;
            h->count--;
            break;
        }
    }

    return p;
}

double intopt(int m, int n, double** a, double* b, double* c, double* x) {

    struct node_t* p= initial_node(m,n,a,b,c);
    //set h= {p}
    struct set_t* h = create_set();
    add(h, p);

    double z= -INFINITY; // best integer solution found so far

    p->z = simplex(p->m, p->n, p->a, p->b, p->c, p->x, 0);

    if (integer(p) || !isfinite(p->z)){
        z = p->z;
        if (integer(p)){
            memcpy(x, p->x, p->n +1 *  sizeof(double));//copy p.x to x
        } 
        
        free_node(p);//delete p
        return z;
    } 
 
    branch (p,z);
    while (h->count > 0){
        //take p from h
        struct node_t* p = pop(h);
        succ(p, h, m, n, a, b, c, p->h, 1, floor(p->xh), &z, x);
        succ(p, h, m, n, a, b, c, p->h, -1, -ceil(p->xh), &z, x);
        //delete p
        free_node(p);
    }
    
    if (z== -INFINITY) {
        return NAN; // not-a-number
    }
    else {
        return z;
    }
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