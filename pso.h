
#ifndef PSO_H
#define PSO_H

#include <stdbool.h>
#include "prng.h"

#define NP 3
#define DIM 2

typedef struct Particle {
  double position[DIM];
  double current_fitness; 	/* Could be generated but easier */
  double velocity[DIM];
  double best[DIM];
  double best_fitness; 		/* COuld be generated but easier to store */
} Particle;


typedef struct Swarm {
  Particle particles[NP];
  double (*fitness_func)(double input[DIM]);
  double lower_bounds;
  double upper_bounds;
  uint64_t iterations;
  double global_best[DIM];
  double c1; 
  double c2;
} Swarm;

void Swarm_Init(Swarm * swarm, double (*fitness_func)(double input[DIM]), double lower_bounds, double upper_bounds, uint64_t iterations, Prng * prng);

void Swarm_Print(Swarm *swarm);

void Swarm_Run(Swarm * swarm, Prng * prng);

#endif
