#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"


void test_add()
{
    printf("add : 11111111111 + 98979695949\n");
    bigint a = string_to_bigint("11111111111");
    bigint b = string_to_bigint("98979695949");
    bigint c = add_bigint(a,b);
    output_bigint(c);
    printf("\n\n");
}


void test_mut()
{
    printf("mut : FFFFFFFFFFFFFFFFFFFFFFFF * FFFFFFFFFFFFFFFFFFFFFFFF\n");
    bigint a = string_to_bigint("FFFFFFFFFFFFFFFFFFFFFFFF");
    bigint b = string_to_bigint("FFFFFFFFFFFFFFFFFFFFFFFF");
    bigint c = mut_bigint(a,b);
    output_bigint(c);
    printf("\n\n");
}


void test_sub()
{
    printf("sub : 2222222222222222222 - 222222222322222222222\n");
    bigint a = string_to_bigint("2222222222222222222");
    bigint b = string_to_bigint("222222222322222222222");
    bigint c = sub_bigint(a,b);
    output_bigint(c);
    printf("\n\n");
}


void test_bigint()
{
    bigint b;
    memset(b.data, 0, MAX);
    for (int i = 0; i < MAX; ++i) {
        printf("%d", b.data[i]);
    }
    printf("\n\n");
}

void test_const()
{
    printf("zero, one, two \n");
    output_bigint(ZERO);
    output_bigint(ONE);
    output_bigint(TWO);
    printf("\n\n");
}


void test_pow()
{
    printf("pow : 2^1024\n");
    bigint a = string_to_bigint("2");
    bigint c = pow_bigint(a,1024);
    output_bigint(c);
    printf("\n\n");
}


void test_div()
{
    printf("div : 517608181126615635485045030823374602335/2\n");
    bigint a = string_to_bigint("517608181126615635485045030823374602335");
    bigint b = string_to_bigint("2");
    bigint c = div_bigint(a,b);
    output_bigint(c);
    printf("\n\n");
}


void test_mod()
{
    printf("mod : 517608181126615635485045030823374602335 mod 2\n");
    bigint a = string_to_bigint("517608181126615635485045030823374602335");
    bigint b = string_to_bigint("2");
    bigint c = mod_bigint(a,b);
    char s[MAX+1];
    bigint_to_string(c, s);
    printf("%s\n", s);
    printf("\n\n");
}


/*
快速幂(Python) ：
def pow_m_fast(x, n, m) :
    d = 1
    while n > 0:
        if n % 2 == 1 :
            d = (d * x)%m
        x = (x**2)%m
        n = n//2
    return d

*/

bigint  pow_m_fast(bigint x, bigint n, bigint m)
{
    bigint d = ONE;
    while (is_greater(n, ZERO)) {
        if (n.data[0]&1) {
            d = mod_bigint(mut_bigint(d,x), m);
        }
        x = mod_bigint(mut_bigint(x,x), m);
        div_2(&n);
    }
    return d;
}

/*
扩展欧几里得(Python) :
def Exgcd(a, b) :
    x0,y0,x1,y1=1,0,0,1
    while b != 0 :
        r,q = a%b, a//b
        x,y = x0-q*x1,y0-q*y1
        x0,y0,x1,y1 = x1,y1,x,y
        a,b = b,r
    return a,x0,y0

def Inverse(a, m) :
    return (Exgcd(a, m)[1])

*/

bigint Exgcd(bigint a, bigint b, bigint *x0, bigint *y0)
{
    *x0 = ONE;
    *y0 = ZERO;
    bigint x1 = ZERO;
    bigint y1 = ONE;
    bigint r;
    bigint q;
    while (!is_equal(b, ZERO)) {
        div_mod_bigint(&a,&b,&q,&r);
        bigint x = sub_bigint(*x0, mut_bigint(q, x1));
        bigint y = sub_bigint(*y0, mut_bigint(q, y1));
        *x0 = x1; *y0 = y1; x1 = x; y1 = y;
        a = b; b = r;
    }
    return a;
}

bigint Inverse(bigint a, bigint m)
{
    bigint x0, y0;
    Exgcd(a,m, &x0, &y0);
    return x0;
}

/*
CRT(Python) :
def Multiply_all(X) :
    r = 1
    for x in X :
        r *= x
    return r

def CRT(a, m, k) :
    M = Multiply_all(m)
    T = [(M//m[i]) * Inverse(M//m[i], m[i]) * a[i] for i in range(k)]
    x = sum(T)%M
    return x

*/

bigint Multiply_all(bigint *array, int k)
{
    bigint r = ONE;
    for (int i = 0; i < k; ++i) {
        r = mut_bigint(r, array[i]);
    }
    return r;
}


bigint CRT(bigint *a, bigint *m, int k)
{
    bigint x = ZERO;
    bigint M = Multiply_all(m, k);

    for (int i = 0; i < k; ++i) {
        bigint t = mut_bigint(mut_bigint(div_bigint(M,m[i]), Inverse(div_bigint(M,m[i]), m[i])), a[i]);
        x = add_bigint(x, t);
    }

    return (mod_bigint(x,M));
}


/*
素性检验(Python from qaqmander)：miller_rabin

def _v2(n):    # get k, q for n - 1 = q * 2 ^ k where q is odd
    k, q = 0, n
    while not (q & 1):
        k, q = k + 1, q // 2
    return k, q

def miller_rabin(n, t):    # judge n, for t times
    if n <= 1:
        return False
    k, q = _v2(n - 1)    # get k, q
    while t:    # for t times
        a = randint(2, n - 2)    # a <- random num
        a = pow_fast_m(a, q, n)
        flag = False
        if a == 1:    # whether a ^ q == 1 ( mod n )
            flag = True
        for j in range(k):    # whether a ^ {2 ^ j * q} == 1 ( mod n )
            if a == n - 1:
                flag = True
                break
            a = a ** 2 % n
        if not flag:
            return False
        t -= 1
    return True


*/




void _v2(bigint n, bigint *k, bigint *q)
{
    *k = ZERO;
    *q = n;

    while (!(q->data[0] & 1)) {
        *k = add_bigint(*k, ONE);
        div_2(q);
    }
}

int miller_rabin(bigint n, int t)
{
    if (is_less(n, ONE) || !(n.data[0] & 1)) {
        return 0;
    }
    bigint k,q;
    bigint n_1 = sub_bigint(n, ONE);
    _v2(n_1, &k, &q);
    while (t-- > 0) {
        srand((unsigned)time(0));
        bigint a = random_bigint(TWO, n_1);
        a = pow_m_fast(a, q, n);
        int flag = 0;
        if (is_equal(a, ONE)) {
            flag = 1;
        }
        for (bigint j = ZERO; is_less(j, k); add_bigint_1(&j)) {
            if (is_equal(a, n_1)) {
                flag = 1;
                break;
            }
            a = mod_bigint(mut_bigint(a,a),n);
        }
        if (!flag) {
            return 0;
        }
    }
    return 1;
}

void test_MR()
{
    printf("MR: 2^521-1\n");
    bigint c = sub_bigint(pow_bigint(TWO,521), ONE);
    output_bigint(c);
    printf("is_prime : %d\n", miller_rabin(c, 10));

    printf("MR: 2^127-1\n");
    bigint d = sub_bigint(pow_bigint(TWO,127), ONE);
    output_bigint(d);
    printf("is_prime : %d\n", miller_rabin(d, 10));
    printf("\n\n");
}


void test_random_n()
{
    srand((unsigned)time(0));
    int t = 5;
    for (int i = 0; i < t; ++i) {
        bigint b = random_bigint_n(10);
        output_bigint(b);
    }
    printf("\n\n");
}

void test_random()
{
    printf("random: [2, 517608181126615635485045030823374602335)\n");
    srand((unsigned)time(0));
    bigint b = random_bigint(TWO, string_to_bigint("517608181126615635485045030823374602335"));
    output_bigint(b);
    printf("\n\n");
}

void test_Exgcd()
{
    printf("exgcd: 1160718174, 316258250\n");
    bigint x0, y0;
    bigint r = Exgcd(string_to_bigint("1160718174"),string_to_bigint("316258250"), &x0, &y0);
    output_bigint(r);
    output_bigint(x0);
    output_bigint(y0);
    printf("\n\n");
}

void test_pow_fast()
{
    printf("pow_m_fast : (517608181126615635485045030823374602335^17608181126615635485045030823374602) mod 7608181126615635485045030823374602335\n");
    bigint c = pow_m_fast(string_to_bigint("517608181126615635485045030823374602335"), string_to_bigint("17608181126615635485045030823374602"), string_to_bigint("7608181126615635485045030823374602335"));
    output_bigint(c);
    printf("\n\n");
}

void test_CRT()
{

    printf("CRT: x = 2(mod 3), x = 3(mod 5), x = 2 (mod 7)\n");
    bigint a[] = {
        {"\2",1,1},
        {"\3",1,1},
        {"\2",1,1},
    };

    bigint m[] = {
        {"\3",1,1},
        {"\5",1,1},
        {"\7",1,1},
    };

    bigint r = CRT(a,m,3);
    output_bigint(r);
    printf("\n\n");
}

void test_pow_fast_1024()
{
    printf("pow_fast_1024\n");
    bigint a = TWO;
    bigint c = sub_bigint(pow_bigint(a,1024), ONE);
    output_bigint(c);
    bigint b = sub_bigint(pow_bigint(a,513), ONE);
    output_bigint(b);
    bigint d = sub_bigint(pow_bigint(a,512), ONE);
    output_bigint(d);

    output_bigint(pow_m_fast(b,d,c));
    printf("\n\n");
}

void test_div2()
{
    printf("test div_2\n");
    bigint n = string_to_bigint("FFFFFFFFFFFFF");
    output_bigint(n);
    div_2(&n);
    output_bigint(n);
    printf("\n\n");
}

void test_add_1()
{
    bigint n = string_to_bigint("FFF");
    for (bigint j = ONE; is_less(j,n); add_bigint_1(&j) ) {
        output_bigint(j);
    }
}

void test_lshift()
{
    for (int i = 0; i < 16; ++i) {
        bigint n = string_to_bigint("FFF");
        lshift(&n, i);
        printf("%d :", get_bitlen(&n));
        output_bigint(n);

    }

}

int main()
{
    clock_t start, end;
    start = clock();
    //test_lshift();

    test_const();
    test_add();
    test_mut();
    test_sub();
    test_div();
    test_mod();
    test_pow();
    test_Exgcd();
    test_CRT();
    test_random();
    test_pow_fast();
    test_pow_fast_1024();
    test_div2();
    test_MR();

    end = clock();
    double all = (double)(end - start)/CLOCKS_PER_SEC;
    printf("%f seconds/n", all);
    return 0;
}
