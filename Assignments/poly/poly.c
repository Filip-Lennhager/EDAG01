#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "error.h"
#include "poly.h"

struct poly_t {
    int* coeffs;
    int* exps;
};

poly_t*     new_poly_from_string(const char* string){
    poly_t* p = malloc(sizeof(poly_t));
    return p;
}

void        free_poly(poly_t* p){
    free(p->coeffs);
    free(p->exps);
}

poly_t*     mul(poly_t* p, poly_t* q){
    poly_t* result = malloc(sizeof(poly_t));
    return result;
}

void        print_poly(poly_t* p){
    printf("test\n");
}
