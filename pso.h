
#ifndef PSO_H
#define PSO_H

#include "prng.h"
#include <bits/stdint-uintn.h>

//#define PLOT

#define NP 3
#define DIM 2

typedef struct Particle {
  double position[DIM];
  double current_fitness; /* Could be generated but easier */
  double velocity[DIM];
  double best[DIM];
  double best_fitness; /* Could be generated but easier to store */
} Particle;

typedef struct Swarm {
  Particle particles[NP];
  double (*fitness_func)(double input[DIM]);
  double lower_bounds;
  double upper_bounds;
  double global_best[DIM];
  double c1;
  double c2;
} Swarm;

/* Create a new swarm */
void Swarm_Init(Swarm *swarm, double (*fitness_func)(double input[DIM]),
                double lower_bounds, double upper_bounds, Prng *prng);

/*  Print the contents of the swarm */
void Swarm_Print(Swarm *swarm);

/*
TODO: Should have a backend choice for hardware, threading, etc
Should automatically check for heardware options, probably during compilation,
Or the user can choose the underlying option and tuning parameters
*/

/* Run the swarm for a number of iterations */
void Swarm_Run(Swarm *swarm, Prng *prng, uint64_t iterations);

/*  */
void Swarm_Run_Map(Swarm *swarm, Prng *prng, uint64_t iterations, char * file);

/* Run the swarm predicated on a callback condition */
/* void Swarm_Run(Swarm *swarm, Prng *prng, ); */

#endif
