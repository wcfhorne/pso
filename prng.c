#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include "prng.h"

/* a test point */
uint64_t state = {0x653DF3914661D71A};

/* xoshift64* for a better performing prng */
uint64_t xorshift64star(uint64_t *state) {
  uint64_t x = *state;
  x ^= x >> 12;
  x ^= x << 25;
  x ^= x >> 27;
  *state = x;
  return x * UINT64_C(0x2545f4914f6cdd1d);
}

Prng Prng_Fake() {

  Prng def = {state, &xorshift64star};  
  return def;
}

/* Initialize the basic default PRNG */
Prng Prng_Default() {

  /* can seed it with the /dev/random setup but later */
  uint64_t seed = (uint64_t)time(NULL);
  
  Prng def = {seed, &xorshift64star};  
  return def;
}

/* Generate a uniform distribution from the prng */
double Prng_Uniform(Prng * prng){

  uint64_t rnum = (prng->func)(&prng->state);
  double result = (double)rnum / (double)UINT64_MAX;
  
  return result;  
}

/* Generate a uniform distrubtion between a range */
double Prng_Bounded(Prng * prng, double lower, double upper){
  
  double uniform_random = Prng_Uniform(prng);

  /* will need a specialized check for default bounds*/
  double space = upper - lower;
  double distance = uniform_random * space;

  return (lower + distance);
}
