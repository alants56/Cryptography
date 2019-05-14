#include "bigint.h"

const bigint ZERO = {"\0", 1, 1};
const bigint ONE = {"\1", 1, 1};
const bigint TWO = {"\2", 1, 1};

static inline int check_s(int a, int b)
{
    return (a*2 + b);
}

static inline int char_to_int(char a)
{
    if (a>= '0' && a <= '9') {
        return a - '0';
    } else if (a>= 'a' && a <= 'f') {
        return (a - 'a' + 10);
    } else if (a>= 'A' && a <= 'F'){
        return (a - 'A' + 10);
    }
    return 0;
}


static inline char int_to_char(int a)
{
    if (a>=0 && a <= 9) {
        return (a+'0');
    } else {
        return (a-10+'A');
    }
}


static inline int __is_equal(bigint l, bigint r)
{
    if (l.flag > r.flag) {
        return 1;
    } else if (l.flag  < r.flag) {
        return -1;
    }

    if (l.flag) {
        if (l.len > r.len) {
            return 1;
        } else if (l.len < r.len) {
            return -1;
        }

        for (int i = l.len - 1; i >= 0; --i) {
            if (l.data[i] > r.data[i]) {
                return 1;
            } else if (l.data[i] < r.data[i]) {
                return -1;
            }
        }

    } else {
        if (l.len < r.len) {
            return 1;
        } else if (l.len > r.len) {
            return -1;
        }

        for (int i = l.len - 1; i >= 0; --i) {
            if (l.data[i] < r.data[i]) {
                return 1;
            } else if (l.data[i] > r.data[i]) {
                return -1;
            }
        }
    }
    return 0;
}
bigint string_to_bigint(char *s)
{
    bigint b;
    memset(b.data, 0, MAX);
    int i = 0;
    int j = 0;

    b.len = strlen(s);
    for (i = b.len-1; i>0; --i) {
        b.data[j++] = char_to_int(s[i]);
    }
    if (s[0] == '-') {
        b.flag = NEGATIVE;
        b.len -= 1;
    } else {
        b.data[j] = char_to_int(s[0]);
        b.flag = POSITIVE;
    }
    return b;
}
void bigint_to_string(bigint b, char *s)
{
    int i = 0;
    if (b.flag) {
        while(i < b.len) {
            s[i] = int_to_char(b.data[b.len-i-1]);
            ++i;
        }
    } else {
        s[i++] = '-';
        while(i <= b.len) {
            s[i] = int_to_char(b.data[b.len-i]);
            ++i;
        }
    }
    s[i] = '\0';
}

static inline int max(int l, int r)
{
    return (l>r ? l : r);
}

static inline int min(int l, int r)
{
    return (l<r ? l : r);
}

static int get_bigint_len(char *s, int len)
{
    while ( (len-1)>0 && s[len-1] == 0) {
        --len;
    }
    if (len > 0) {
        return len;
    } else {
        return 1;
    }
}

static inline bigint  __add_bigint(bigint l, bigint r)
{
    bigint b;
    b.len = min(l.len, r.len);
    base_type flag = 0;
    int i = 0;
    for ( ; i < b.len; ++i) {
        b.data[i] = (l.data[i] + r.data[i] + flag)%BASE;
        flag = (l.data[i] + r.data[i] + flag)/BASE;
    }

    if (l.len > r.len) {
        for( ; i < l.len; ++i) {
            b.data[i] = (l.data[i]  + flag)%BASE;
            flag = (l.data[i] + flag)/BASE;
            ++b.len;
        }
    } else if (l.len < r.len) {
        for( ; i < r.len; ++i) {
            b.data[i] = (r.data[i]  + flag)%BASE;
            flag = (r.data[i] + flag)/BASE;
            ++b.len;
        }
    }
    if (flag) {
        b.data[b.len++] = flag;
    }
    return b;
}

static inline bigint __sub_bigint_n(bigint l, bigint r)
{
    base_type flag = 0;
    int i = 0;
    bigint b;
    b.len = min(l.len, r.len);
    for ( ; i < b.len; ++i) {
        if ((l.data[i] + flag) >= r.data[i]) {
            b.data[i] = (l.data[i] + flag - r.data[i]);
            flag = 0;
        } else {
            b.data[i] = (BASE + l.data[i] + flag - r.data[i]);
            flag = -1;
        }
    }

    if (l.len > r.len) {
        for( ; i < l.len; ++i) {
            if ((l.data[i] + flag) >= 0) {
                b.data[i] = (l.data[i] + flag);
                flag = 0;
            } else {
                b.data[i] = (BASE + l.data[i] + flag);
                flag = -1;
            }
            ++b.len;
        }
    }
    b.len = get_bigint_len(b.data, b.len);
    return b;
}

static inline bigint  __sub_bigint(bigint l, bigint r)
{
    bigint b;
    if (is_less(l,r)) {
        b =  __sub_bigint_n(r, l);
        b.flag = NEGATIVE;
    } else {
        b =  __sub_bigint_n(l, r);
    }
    return b;
}


static inline bigint  __mut_bigint(bigint l, bigint r)
{
    bigint b;
    base_type flag = 0;
    int i,j;
    memset(b.data, 0, MAX);
    b.len = l.len + r.len;
    for (i = 0; i < r.len; ++i) {
        int tmp = 0;
        for (j = 0; j < l.len; ++j) {
            tmp = b.data[i+j] + flag + r.data[i] * l.data[j];
            flag = (tmp/BASE);
            b.data[i+j] = (tmp)%BASE;
        }
        tmp = b.data[i+j] + flag;
        b.data[i+j] = (tmp%BASE);
        flag = (tmp/BASE);
    }

    b.len = get_bigint_len(b.data, b.len);
    return b;
}






bigint  pow_bigint(bigint x, int n)
{
    bigint d = ONE;
    while (n != 0) {
        if (n%2 == 1) {
            d = mut_bigint(d,x);
        }
        x = mut_bigint(x,x);
        n = n/2;
    }
    return d;
}

void add_bigint_1(bigint * b)
{
    base_type flag = 1;
    base_type tmp = 0;
    for (int i = 0; i < b->len; ++i) {
        tmp = flag + b->data[i];
        flag = (tmp/BASE);
        b->data[i] = tmp%BASE;
    }

    if (flag == 1) {
        b->data[b->len++] = flag;
    }
}


bigint  add_bigint(bigint l, bigint r)
{
    bigint b;
    int ch = check_s(l.flag, r.flag);

    switch (ch) {
    case 0:
        b = __add_bigint(l, r);
        b.flag = NEGATIVE;
        break;
    case 1:
        l.flag = POSITIVE;
        b = __sub_bigint(r, l);
        break;
    case 2:
        r.flag = POSITIVE;
        b = __sub_bigint(l, r);
        break;
    case 3:
    default:
        b = __add_bigint(l, r);
        b.flag = POSITIVE;
        break;
    }
    return b;
}

bigint  sub_bigint(bigint l, bigint r)
{
    bigint b;
    int ch = check_s(l.flag, r.flag);

    switch (ch) {
    case 0:
        r.flag = POSITIVE;
        l.flag = POSITIVE;
        b = __sub_bigint(r, l);
        break;
    case 1:
        l.flag = POSITIVE;
        r.flag = POSITIVE;
        b = __add_bigint(l, r);
        b.flag = NEGATIVE;
        break;
    case 2:
        r.flag = POSITIVE;
        l.flag = POSITIVE;
        b = __add_bigint(l, r);
        b.flag = POSITIVE;
        break;
    case 3:
    default:
        b = __sub_bigint(l, r);
        break;
    }
    return b;
}

bigint  mut_bigint(bigint l, bigint r)
{
    bigint b;
    b = __mut_bigint(l, r);
    b.flag = !(l.flag^r.flag);
    return b;
}

bigint mut_bigint_int(bigint a, int n)
{
    bigint b = ZERO;
    while (n-- > 0) {
        b = add_bigint(b,a);
    }
    return b;
}

static inline bigint __get_ONE_N(int n)
{
    bigint a;
    memset(a.data, 0, MAX);
    a.flag = 1;
    a.len = n+1;
    a.data[n] = 1;
    return a;
}

void  div_mod_bigint(bigint *a, bigint *r, bigint *b, bigint *l)
{
    *b = ZERO;
    *l = *a;
    while (!is_less(*l,*r)) {
        if (l->len - r->len > 1) {
            bigint base = __get_ONE_N(l->len-r->len-1);
            bigint c = *r;
            s_left(&c, l->len - r->len - 1);
            while (!is_less(*l, c)) {
                *b = add_bigint(*b, base);
                *l = sub_bigint(*l,c);
            }

        } else {
            *l = sub_bigint(*l,*r);
            *b = add_bigint(*b, ONE);
        }

    }
}

static inline int get_bitlen_basetype(base_type b)
{
    int len = 0;
    while (b != 0) {
        b = b>>1;
        ++len;
    }
    return len;
}

int get_bitlen(bigint* d)
{
    return (get_bitlen_basetype(d->data[d->len - 1]) + (d->len - 1) * BASE_BITLEN);
}


void lshift(bigint* d, int n)
{
    if (!n) {
        return;
    }

    int len = n/BASE_BITLEN;
    int bitlen = n%BASE_BITLEN;
    int tmp = 0;
    int flag = 0;

    for (int i = 0; i < d->len; ++i) {
        tmp = (d->data[d->len - i - 1] << bitlen);
        d->data[d->len + len - i] = tmp / BASE + flag;
        flag =  tmp % BASE;
    }

    d->data[len] = flag;

    for (int i = 0; i < len; ++i) {
        d->data[i] = 0;
    }
    d->len += len+1;
    d->len = get_bigint_len(d->data, d->len);
}


void s_left(bigint *a, int n)
{
    for (int i = 0; i < a->len; ++i) {
        a->data[a->len+n-i-1] = a->data[a->len-i-1];
    }
    for (int i = 0; i < n; ++i) {
        a->data[i] = 0;
    }
    a->len += n;
}

bigint  div_bigint(bigint l, bigint r)
{
    bigint b = ZERO;
    while (!is_less(l,r)) {
        if (l.len - r.len > 1) {
            bigint base = __get_ONE_N(l.len-r.len-1);
            bigint c = r;
            s_left(&c, l.len - r.len - 1);
            while (!is_less(l, c)) {
                l = sub_bigint(l,c);
                b = add_bigint(b,base);
            }
        } else {
            l = sub_bigint(l,r);
            b = add_bigint(b, ONE);
        }

    }
    return b;
}


bigint  mod_bigint(bigint l, bigint r)
{
    while (!is_less(l,r)) {
        int n = get_bitlen(&l) - get_bitlen(&r) - 1;
        if (n > 0) {
            bigint c = r;
            lshift(&c, n);
            l = sub_bigint(l,c);
        } else {
            l = sub_bigint(l,r);
        }
    }
    return l;
}


bigint  pow_m_bigint(bigint x, bigint n, bigint m)
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





int is_equal(bigint l, bigint r)
{
    if (__is_equal(l, r) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int is_less(bigint l, bigint r)
{
    if (__is_equal(l, r) == -1) {
        return 1;
    } else {
        return 0;
    }
}

int is_greater(bigint l, bigint r)
{
    if (__is_equal(l, r) == 1) {
        return 1;
    } else {
        return 0;
    }
}

void output_bigint(bigint t)
{
    char buff[MAX+1];
    bigint_to_string(t, buff);
    printf("0x%s\n", buff);
}



bigint random_bigint_n(int n)
{
    bigint b;
    b.flag = POSITIVE;
    b.len = n;

    for (int i = 0; i < b.len; ++i) {
        b.data[i] = rand()%BASE;
    }

    b.len = get_bigint_len(b.data, b.len);
    return b;
}

bigint random_bigint(bigint l, bigint r)
{
    if (is_less(r, l) || is_equal(l, r)) {
        return ZERO;
    }
    bigint d = sub_bigint(r, l);
    bigint b = random_bigint_n(d.len+10);
    bigint c = mod_bigint(b, d);
    return (add_bigint(c, l));
}

void div_2(bigint * h)
{
    base_type flag = 0;
    for (int i = h->len-1; i>= 0; --i) {
        base_type tmp = flag * BASE;
        flag = (h->data[i] + tmp)%2;
        h->data[i] = (h->data[i] + tmp)/2;
    }
    h->len = get_bigint_len(h->data, h->len);
}
