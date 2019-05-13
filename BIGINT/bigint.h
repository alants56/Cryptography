#ifndef AL_BIGINT_H
#define AL_BIGINT_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>


#define MAX 2048
#define POSITIVE 1
#define NEGATIVE 0

#define BASE 16

typedef char  base_type;

typedef struct __bigint{
    base_type data[MAX];
    int flag;
    int len;
}bigint;

const bigint ZERO;
const bigint ONE;
const bigint TWO;

bigint  add_bigint(bigint, bigint);
bigint  sub_bigint(bigint, bigint);
bigint  mut_bigint(bigint, bigint);
bigint  div_bigint(bigint, bigint);
bigint  mod_bigint(bigint, bigint);
bigint  pow_bigint(bigint, int);
bigint  pow_m_bigint(bigint l, bigint r, bigint m);

bigint  random_bigint_n(int n);
bigint  random_bigint(bigint l, bigint r);

void  div_mod_bigint(bigint *a, bigint *r, bigint *b, bigint *l);
void s_left(bigint *, int );
void div_2(bigint *);

int is_equal(bigint, bigint);
int is_less(bigint, bigint);
int is_greater(bigint, bigint);

bigint  string_to_bigint(char *);
void bigint_to_string(bigint, char *);
void output_bigint(bigint t);


#endif
