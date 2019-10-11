#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include "prng.h"
#include "pso.h"

double Sphere_Fitness(double input[DIM]){

  double sum = 0;
  
  /* calculate the fitness using the input vars */
  for (int i = 0; i < DIM; i++){
    sum = sum + (input[i] * input[i]);
  }
  return sum;
}

void Map_Sphere(){

  FILE *fp;

  fp = fopen("sphere.dat", "w");
  if(fp == NULL){
    fprintf(stderr, "Can't open file, %s %d\n", __FILE__, __LINE__);
    exit(-1);
  }

  for (double i = -5.0; i < 5.0; i = i + 0.1){

    for (double j = -5.0; j < 5.0; j = j + 0.1){

      double input[DIM] = {i, j};

      double output = Sphere_Fitness(input);

      double vars[DIM+1] = {input[0], input[1], output};

      fprintf(fp, "%lf %lf %lf\n", vars[0], vars[1], vars[2]);
      
    }
  }

  fclose(fp);
}

/* rename to harness */
void Sphere_Function() {
  //Prng prng = Prng_Fake();
  Prng prng = Prng_Default();

  Swarm swarm;
  Swarm_Init(&swarm, &Sphere_Fitness, -5.0, 5.0, 1000, &prng);

  Swarm_Print(&swarm);
  Swarm_Run(&swarm, &prng);
  Swarm_Print(&swarm);
}

void test(){
  Sphere_Function();
}

int main() {

  test();
  //Map_Sphere();
  
  return 0;
}
