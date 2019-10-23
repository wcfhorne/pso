#include "prng.h"
#include "pso.h"
#include <bits/stdint-uintn.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Sphere Function */
double Sphere_Fitness(double input[DIM]) {

  double sum = 0;

  /* calculate the fitness using the input vars */
  for (int i = 0; i < DIM; i++) {
    sum = sum + (input[i] * input[i]);
  }
  return sum;
}

/* Rastigan Function */
double Rastrigin_Fitness(double input[DIM]) {

  double sum = 0;

  for (int i = 0; i < DIM; i++) {
    sum = sum + ((input[i] * input[i]) - (10.0 * cos(2 * M_PI * input[i])));
  }
  return (10.0 * 2.0) + sum;
}

/* Map a 3 dimensional function with a passed function  */
void Map_Function_3D(char * filename, double (*fitness_func)(double input[2]), double start, double end, double step){

  FILE *fp;
  fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "Can't open file, %s %d\n", __FILE__, __LINE__);
    exit(-1);
  }

  for (double i = start; i < end; i = i + step) {
    for (double j = start; j < end; j = j + step) {

      double input[DIM] = {i, j};
      double output = (*fitness_func)(input);
      
      double vars[DIM + 1] = {input[0], input[1], output};
      fprintf(fp, "%lf %lf %lf\n", vars[0], vars[1], vars[2]);
    }
  }
  fclose(fp);
}

/* Map a 2 dimensional function with a passed function. TODO: Test*/
void Map_Function_2D(char * filename, double (*fitness_func)(double input[1]), double start, double end, double step){

  FILE *fp;
  fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "Can't open file, %s %d\n", __FILE__, __LINE__);
    exit(-1);
  }

  for (double i = start; i < end; i = i + step) {
    double input[DIM] = {i};
    double output = (*fitness_func)(input);
      
    double vars[DIM + 1] = {input[0], output};
    fprintf(fp, "%lf %lf\n", vars[0], vars[1]);
  }
  fclose(fp);
}

void Sphere() {
  Map_Function_3D("sphere.dat", Sphere_Fitness, -5.0, 5.0, 0.1);

  Prng prng = Prng_Default();
  Swarm swarm;
  Swarm_Init(&swarm, &Sphere_Fitness, -5.0, 5.0, &prng);

  Swarm_Print(&swarm);
  Swarm_Run_Map(&swarm, &prng, 1000, "sphere-opt.dat");
  Swarm_Print(&swarm);
}

void Rastrigin() {
  Map_Function_3D("rastigan.dat", Rastrigin_Fitness, -5.2, 5.2, 0.1);

  Prng prng = Prng_Default();
  Swarm swarm;
  Swarm_Init(&swarm, &Rastrigin_Fitness, -5.2, 5.2, &prng);

  Swarm_Print(&swarm);
  Swarm_Run_Map(&swarm, &prng, 1000, "rastigan-opt.dat");
  Swarm_Print(&swarm);
}

int main() {

  Sphere();
  Rastrigin();

  return 0;
}
