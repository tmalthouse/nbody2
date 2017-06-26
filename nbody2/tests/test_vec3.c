//
//  test_vec3.c
//  nbody2
//
//  Created by Thomas Malthouse on 6/6/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "test_vec3.h"
#include <time.h>
#include <stdlib.h>

extern const char *current_test_func;

#define FLOAT_TOL (0.05)
#define INIT_TEST current_test_func = __FUNCTION__


bool test_vec3_eq() {
  INIT_TEST;
  int status = 0;
  
  status += !vec3_eq(vec3_0, vec3_0);
  
  status += vec3_eq(vec3_0, vec3_I);
  
  return !status;
}

bool test_vabs() {
  INIT_TEST;
  int status = 0;
  
  status += !(vabs(vec3_I) - 1 < FLOAT_TOL);
  
  status += !(vabs((vec3){3.0,4.0,0.0}) - 5 < FLOAT_TOL);
    
  return !status;
}

bool test_inv_vabs() {
  INIT_TEST;
  int status = 0;
  
  status += inv_vabs((vec3){4,0,0}) - 0.5 > FLOAT_TOL;
  
  status += inv_vabs((vec3) {3,4,0}) - 0.2 > FLOAT_TOL;
  
  return !status;
}

static int rand_sint() {
  int sign = rand()%2?1:-1;
  return sign * arc4random();
}



bool test_fastinvsqrt() {
  INIT_TEST;
  int status = 0;
  
  status += fabs(fastinvsqrt(4)-0.5) > FLOAT_TOL;
  
  status += fabs(fastinvsqrt(1)-1) > FLOAT_TOL;
  
  status += fabs(fastinvsqrt(100) - 0.1) > FLOAT_TOL;
  
  status += fabs(fastinvsqrt(0.25) - 2) > FLOAT_TOL;
  
  double avg_diff = 0;
  for (uint i=0; i<5000; i++) {
    double x = arc4random_uniform(2<<16);
    double diff = fabs(fastsqrt(x) - sqrt(x));
    status += diff > FLOAT_TOL;
    
    avg_diff += diff * (1.0/5000.0);
  }
  printf("Avg error: %.12f\n", avg_diff);
  
  return !status;
}

time_t benchmark_vec3_normalize() {
  int count = 2<<16;
  srand((uint)time(NULL));
  vec3 *vecs = malloc(sizeof(vec3) * count);
  vec3 *nrms = malloc(sizeof(vec3) * count);
  for (uint i=0; i<count; i++) {
    vecs[i] = (vec3){rand_sint(),rand_sint(),rand_sint()};
  }
  
  clock_t start = clock();
  
  for (uint i=0; i<count; i++) {
    nrms[i] = vec3_normalize(vecs[i]);
  }
  
  clock_t diff = clock() - start;
  
  printf("Time to normalize %d vectors: %lu ms\n", count, diff);
  
  int status = 0;
  
  for (uint i=0; i<count; i++) {
    status += fabs(vabs(nrms[i])-1)>FLOAT_TOL;
  }
  
  printf("%d vectors normalized incorrectly.\n", status);
  return diff;
}

void grapherthing() {
  FILE *f = fopen("/Users/Thomas/Desktop/graph.csv", "w");
  for (uint i=0; i<100000; i+=10) {
    fprintf(f,"%d, %f\n\n", i, vabs(vec3_normalize((vec3)(i))));
  }
  fclose(f);
}
