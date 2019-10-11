
#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

typedef struct Prng {
  uint64_t state;
  uint64_t (*func)(uint64_t * state);
} Prng;


Prng Prng_Fake();
Prng Prng_Default();
double Prng_Uniform(Prng * prng);
double Prng_Bounded(Prng * prng, double lower, double upper);

#endif
