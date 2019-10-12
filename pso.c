#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include "prng.h"
#include "pso.h"
#include <stdlib.h>

void Particle_Init(Particle *particle, double (*fitness_func)(double input[DIM]), Prng * prng, double lower_bounds, double upper_bounds) {

  double position[DIM] = {Prng_Bounded(prng, lower_bounds, upper_bounds), Prng_Bounded(prng, lower_bounds, upper_bounds)};

  double velocity[DIM] = {0, 0};
  
  memcpy(&particle->position, &position, sizeof(particle->position));
  memcpy(&particle->velocity, &velocity, sizeof(particle->velocity));
  memcpy(&particle->best, &position, sizeof(particle->best));

  particle->current_fitness = (*fitness_func)(particle->best);
  particle->best_fitness = (*fitness_func)(particle->best);
}

void Particle_Restart(Particle *particle, Prng * prng, double lower_bounds, double upper_bounds) {
  
  double position[DIM] = {Prng_Bounded(prng, lower_bounds, upper_bounds), Prng_Bounded(prng, lower_bounds, upper_bounds)};

  double velocity[DIM] = {0, 0};
  
  memcpy(&particle->position, &position, sizeof(particle->position));
  memcpy(&particle->velocity, &velocity, sizeof(particle->velocity));
}

void Particle_UpdateVelocity(Particle *particle, Prng * prng, double c1, double c2, double global_best[DIM]) {
  double new_velocity[DIM];

  for (int i = 0; i < DIM; i++) {
    double random1 = Prng_Uniform(prng);
    double random2 = Prng_Uniform(prng);
    
    new_velocity[i] = particle->velocity[i] +
        (random1 * c1 * (particle->best[i] - particle->position[i])) +
        (random2 * c2 * (global_best[i] - particle->position[i]));
  }
  
  memcpy(&particle->velocity, &new_velocity, sizeof(particle->velocity));
}

/* check if the velocity bounds have been violated. TODO: make bounds an array */
bool Particle_CheckPositionBounds(Particle *particle, double lower, double upper) {

  for (int i = 0; i < DIM; i++) {
    if ((particle->position[i] < lower) || (particle->position[i] > upper)){
      return false;
    }
  }
  return true;
}

/* check if the velocity bounds have been violated. TODO: make bounds an array */
bool Particle_CheckVelocityBounds(Particle *particle, double lower, double upper) {
  
  for (int i = 0; i < DIM; i++) {
    if ((particle->velocity[i] < lower) || (particle->velocity[i] > upper)){
      return false;
    }
  }
  return true;
}

/*  */
void Particle_UpdatePosition(Particle *particle) {
  double new_position[DIM];

  for (int i = 0; i < DIM; i++) {
    new_position[i] = particle->position[i] + particle->velocity[i];
  }
  memcpy(&particle->position, &new_position, sizeof(particle->position));
}

/* Rename */
void Particle_FindBest(Particle *particle, double (*fitness_func)(double input[DIM])) {

  double fitness = (*fitness_func)(particle->position);
  particle->current_fitness = fitness;
  
  if( fitness < particle->best_fitness){
    memcpy(&particle->best, &particle->position, sizeof(particle->best));
    particle->best_fitness = fitness;
  }
}

void Particle_Print(Particle *particle) {

  printf("%s: {", "position");
  for (int i = 0; i < DIM; i++) {
    printf("%lf, ", particle->position[i]);
  }
  printf("}\n");
  printf("%s: {", "current_fitness");
  printf("%lf, ", particle->current_fitness);  
  printf("}\n");
  printf("%s: {", "velocity");
  for (int i = 0; i < DIM; i++) {
    printf("%lf, ", particle->velocity[i]);
  }
  printf("}\n");
  printf("%s: {", "best");
  for (int i = 0; i < DIM; i++) {
    printf("%lf, ", particle->best[i]);
  }
  printf("}\n");
  printf("%s: {", "best_fitness");
  printf("%lf, ", particle->best_fitness);
  printf("}\n");

}

void Swarm_Print(Swarm *swarm) {

  printf("%s: {", "particles");
  for (int i = 0; i < NP; i++) {
    Particle_Print(&swarm->particles[i]);
  }
  printf("}\n");
  printf("%s: {", "global_best");
  printf("%s: {", "best");
  for (int i = 0; i < DIM; i++) {
    printf("%lf, ", swarm->global_best[i]);
  }
  printf("}\n");
  printf("%s: {", "global_fitness");
  printf("%lf, ", swarm->fitness_func(swarm->global_best));
  printf("}\n"); 
}

/*  */
void Swarm_Init(Swarm * swarm, double (*fitness_func)(double input[DIM]), double lower_bounds, double upper_bounds, Prng * prng) {

  swarm->fitness_func = fitness_func;
  memcpy(&swarm->lower_bounds, &lower_bounds, sizeof(swarm->lower_bounds));
  memcpy(&swarm->upper_bounds, &upper_bounds, sizeof(swarm->upper_bounds));

  swarm->c1 = 1.0;
  swarm->c2 = 1.0;
  
  /* Initialize each particle */
  for (int i = 0; i < NP; i++) {
    Particle_Init(&swarm->particles[i], (swarm->fitness_func), prng, swarm->lower_bounds, swarm->upper_bounds);
  }

  double best_fitness = DBL_MAX;
  for (int i = 0; i < NP; i++) {
      if( swarm->particles[i].best_fitness < best_fitness){
	memcpy(&swarm->global_best, &swarm->particles[i].best, sizeof(swarm->global_best));
	best_fitness = swarm->particles[i].best_fitness;
    }
  }
}

/*  */
void Swarm_Run(Swarm * swarm, Prng * prng, uint64_t iterations) {

  #ifdef PLOT
  FILE *fp;
  fp = fopen("swarm.dat", "w");
  if(fp == NULL){
    fprintf(stderr, "Can't open file, %s %d\n", __FILE__, __LINE__);
    exit(-1);
  }
  #endif

  
  /* run until stopping condition */
  while (iterations > 0) {

    /* update each particle's velocity */
    for (int i = 0; i < NP; i++) {
      Particle_UpdateVelocity(&swarm->particles[i], prng, swarm->c1, swarm->c2, swarm->global_best); /* need to check for bounds and restart*/
    }

    /* update each particle's position */
    for (int i = 0; i < NP; i++) {
      Particle_UpdatePosition(&swarm->particles[i]); /* need to check for bounds and restart*/
      bool inbounds = Particle_CheckPositionBounds(&swarm->particles[i], swarm->lower_bounds, swarm->upper_bounds);

      if (inbounds != true) {
	Particle_Restart(&swarm->particles[i], prng, swarm->lower_bounds, swarm->upper_bounds);
      }
    }

    /* update each particle's best known position */
    for (int i = 0; i < NP; i++) {
      Particle_FindBest(&swarm->particles[i], swarm->fitness_func);
    } 
    
    /* update the swarm's best known position */
    double best_fitness = swarm->fitness_func(swarm->global_best);
    for (int i = 0; i < NP; i++) {
      if( swarm->particles[i].best_fitness < best_fitness){
	memcpy(&swarm->global_best, &swarm->particles[i].best, sizeof(swarm->global_best));
	best_fitness = swarm->particles[i].best_fitness;
      }
    }

    #ifdef PLOT
    /* Plot the particles */
    fprintf(fp, "%lu %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", iterations, swarm->particles[0].position[0], swarm->particles[0].position[1], swarm->particles[0].current_fitness, swarm->particles[1].position[0], swarm->particles[1].position[1], swarm->particles[1].current_fitness, swarm->particles[2].position[0], swarm->particles[2].position[1], swarm->particles[2].current_fitness, swarm->global_best[0], swarm->global_best[1], best_fitness);
    #endif
    
    
    iterations--;
  }

  #ifdef PLOT
  fclose(fp);
  #endif
}
