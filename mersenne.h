#ifndef MERSENNE_HEADER
#define MERSENNE_HEADER

typedef unsigned long uint32;

void seedMT(uint32 seed);
uint32 reloadMT(void);
uint32 randomMT(void);

#endif