#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "error.h"
#include "poly.h"

void combine_like_terms(poly_t* poly);

struct poly_t {
    int *coeffs; // Array of coefficients
    int *exps;   // Array of exponents
    int length;  // Number of terms
};

poly_t* new_poly_from_string(const char* string) {
    poly_t* p = malloc(sizeof(poly_t));
    int current_char, coef, expo, sign, length = 1;

    p->length = 0;
    p->coeffs = malloc(sizeof(int) * length);
    p->exps = malloc(sizeof(int) * length);

    current_char = *(string++);
    while(current_char != '\0'){
        coef = 1;
        expo = 0;
        sign = 1;

        // check sign
        if(current_char == '-'){
            sign = -1;
            current_char = *(string++);
            if(current_char == ' '){
                current_char = *(string++); // skip space
            }
        }else if(current_char == '+'){
            current_char = *(string++);
            if(current_char == ' '){
                current_char = *(string++); // skip space
            }
        }

        // check coef
        if(isdigit(current_char)){
            coef = (current_char - '0');
            current_char = *(string++);
            while(isdigit(current_char)){
                coef = coef * 10 + (current_char - '0');
                current_char = *(string++);
            }
            coef *= sign;
        }

        // check expo
        if(current_char == 'x'){
            expo = 1;
            current_char = *(string++);
            if(current_char == '^'){
                current_char = *(string++);
                expo = (current_char - '0');
                current_char = *(string++);
                while(isdigit(current_char)){
                    expo = expo * 10 + (current_char - '0');
                    current_char = *(string++);
                }
            }
        }

        p->coeffs[length - 1] = coef;
        p->exps[length - 1] = expo;

        if(current_char != '\0'){
            length++;
            p->coeffs = realloc(p->coeffs, sizeof(int) * length);
            p->exps = realloc(p->exps, sizeof(int) * length);

            current_char = *(string++);
        }
    }

    p->length = length;

    return p;
}


void free_poly(poly_t* p){
    free(p->coeffs);
    free(p->exps);
    free(p);
}

poly_t* mul(poly_t* p, poly_t* q) {
    // Allocate memory for the result polynomial
    poly_t* result = malloc(sizeof(poly_t));
    int max_terms = p->length * q->length;
    result->coeffs = malloc(sizeof(int) * max_terms);
    result->exps = malloc(sizeof(int) * max_terms);
    

    int term_index = 0;
    for (size_t i = 0; i < p->length; i++) {
        for (size_t j = 0; j < q->length; j++) {
            result->coeffs[term_index] = p->coeffs[i] * q->coeffs[j];
            result->exps[term_index] = p->exps[i] + q->exps[j];
            term_index++;
        }
    }

    // Update the length of the result polynomial
    result->length = term_index;

    // Combine like terms in the result polynomial
    combine_like_terms(result);

    return result;
}

void combine_like_terms(poly_t* poly) {
    for (size_t i = 0; i < poly->length; i++) {
        for (size_t j = i + 1; j < poly->length; j++) {
            if (poly->exps[i] == poly->exps[j]) {
                // Combine terms
                poly->coeffs[i] += poly->coeffs[j];
                
                // Shift the remaining terms
                for (size_t k = j; k < poly->length - 1; k++) {
                    poly->coeffs[k] = poly->coeffs[k + 1];
                    poly->exps[k] = poly->exps[k + 1];
                }
                poly->length--;
                j--; // Adjust the index since we just removed a term
            }
        }
    }
}

void print_poly(poly_t* p) {
    for (size_t i = 0; i < p->length; ++i) {
        int coef = p->coeffs[i];
        int expo = p->exps[i];

        // Handle the sign of the coefficient
        if (coef < 0) {
            if (i != 0) printf(" - "); // Print minus for non-first terms
            coef = -coef; // Make the coefficient positive for printing
        } else if (i != 0) {
            printf(" + "); // Print plus for non-first positive terms
        }

        // Print the coefficient if it's not 1 or if the exponent is 0 (constant term)
        if (coef != 1 || expo == 0) {
            printf("%d", coef);
        } else if (expo != 0 && coef == -1) {
            printf("-"); // Handle case of -1 coefficient with non-zero exponent
        }

        // Print the variable and exponent if necessary
        if (expo > 0) {
            printf("x");
            if (expo > 1) {
                printf("^%d", expo);
            }
        }
    }
    
    printf("\n"); // Newline at the end
}
