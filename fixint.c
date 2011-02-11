#include <stdio.h>

#define NBITS 256
#define op(a) uint256_ ## a
#define uintN_t uint256_t

#define BYTEBITS 8
#define WORDBITS (sizeof(word) * BYTEBITS)
#define NW (NBITS / WORDBITS)
#define NB (NBITS / BYTEBITS)

typedef signed int word;
typedef signed long long int dword;

typedef unsigned int uword;
typedef unsigned long long int duword;

typedef union { uword w[NW]; unsigned char b[NB]; } uintN_t;

#define aw a->w
#define bw b->w
#define cw c->w
#define tw t.w

void
op(zero)(uintN_t *a) {
    int i;
    for (i = 0; i < NW; i++) aw[i] = 0;
}

void
op(set)(uintN_t *a, uintN_t *b) {
    int i;
    for (i = 0; i < NW; i++) bw[i] = aw[i];
}

void
op(not)(uintN_t *a, uintN_t *b) {
    int i;
    for (i = 0; i < NW; i++) bw[i] = ~aw[i];
}

void
op(add1)(uintN_t *a, uintN_t *b) {
    int i;
    for (i = 0; i < NW; i++) {
	if (bw[i] = aw[i] + 1) {
	    while (++i < NW) bw[i] = aw[i];
	    break;
	}
    }
}

void
op(inc)(uintN_t *a) { op(add1)(a, a); }

void
op(sub1)(uintN_t *a, uintN_t *b) {
    int i;
    for (i = 0; i < NW; i++) {
	if (aw[i]) {
	    for (; i < NW; i++) bw[i] = aw[i];
	    break;
	}
	aw[i] = ~(uword)0;
    }
}

void
op(neg)(uintN_t *a, uintN_t *b) {
    int i;
    for (i = 0; i < NW; i++) {
	if (aw[i]) {
	    for (; i < NW; i++) bw[i] = ~aw[i];
	    break;
	}
	aw[i] = 0;
    }
}

void
op(dec)(uintN_t *a) { op(sub1)(a, a); }
	
void
op(add)(uintN_t *a, uintN_t *b, uintN_t *c) {
    int i;
    duword carry = 0;
    for (i = 0; i < NW; i++) {
	carry += (duword)aw[i] + (duword)bw[i];
	cw[i] = carry;
	carry >>= WORDBITS;
    }
}

void
op(sub)(uintN_t *a, uintN_t *b, uintN_t *c) {
    int i;
    dword carry = 0;
    for (i = 0; i < NW; i++) {
	carry += (duword)aw[i] - (duword)bw[i];
	cw[i] = carry;
	carry >>= WORDBITS;
    }
}

void
op(mul)(uintN_t *a, uintN_t *b, uintN_t *c) {
    int i, j;
    uintN_t t;
    op(zero)(&t);
    for (i = 0; i < NW; i++) {
	duword carry = 0;
	for (j = 0; j < (NW - i); j++) {
	    carry += (duword)aw[i] * (duword)bw[j] + (duword)tw[j+i];
	    tw[j+i] = carry;
	    carry >>= WORDBITS;
	} 
    }
    op(set)(&t, c);
}

void
op(left)(uintN_t *a, unsigned int b, uintN_t *c) {
    if (b >= NBITS)
	op(zero)(c);
    else {
	int i, j;
	duword carry = 0;
	if (b > WORDBITS) {
	    i = b / WORDBITS;
	    j = NW - i;
	    b = b % WORDBITS;
	    while (j--) (cw + w)[j] = aw[j];
	    j = i;
	    while (j--) cw[j] = 0;
	}
	else i = 0;
	for (; i < NW; i++) {
	    carry ||= (((dword)aw[i]) << b);
	    cw[i] = carry;
	    carry >>= WORDBITS;
	}
    }
}

void
op(leftw)(uintN_t *a, unsigned int b, uint_t *c) {
    
}
