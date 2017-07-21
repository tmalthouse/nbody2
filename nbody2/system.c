//
//  system.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "system.h"
#include "tree.h"
#include "body.h"
#include "vec3.h"
#include "calculationthread.h"
#include <inttypes.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include <gsl/gsl_rng.h>
#ifndef __APPLE__
#include <bsd/stdlib.h>
#endif


void update_system(System *sys) {
  if (sys->tree.nodes == NULL) {
    sys->tree = build_tree(sys->bodies, sys->count);
  } else {
    update_tree(&sys->tree);
  }
  
  for (uint64_t i=0; i<sys->count; i++) {
    update_timestep(&sys->bodies[i], sys->time);
  }
  
  uint64_t min_tstep = UINT64_MAX;
  for (uint i=0; i<sys->count; i++) {
    if (sys->bodies[i].tstep<min_tstep) {
      min_tstep = sys->bodies[i].tstep;
    }
  }
  
  //Set up the threads, and start them
  Body *body_index = sys->bodies;
  for (uint j=0; j<THREAD_COUNT; j++) {
    CalculationThread *t = &sys->threads[j];
    t->bodies = body_index;
    t->done = false;
    body_index += t->count;
  }
  
  //Wait for threads to finish
  int sum;
  do {
    sum = 1;
    for (uint j=0; j<THREAD_COUNT; j++) {
      sum*=sys->threads[j].done;
    }
  } while(!sum);
  
  sys->time += min_tstep?sys->time%min_tstep:1000;
}

void initialize_threads(System *s) {
  s->threads = calloc(sizeof(CalculationThread), THREAD_COUNT);
  uint basecount = s->count/THREAD_COUNT;
  uint remainder = s->count%THREAD_COUNT;
  for (uint i=0; i<THREAD_COUNT; i++) {
    int status = thrd_create(&s->threads[i].t_id, thread_loop, &s->threads[i]);
    if (status != thrd_success) {
      printf("Error creating thread num. %d\n", i);
      exit(-1);
    }
    //The first couple thread gets the normal amount of bodies, plus the remainder.
    s->threads[i].count = (i<remainder)?basecount+1:basecount;
    s->threads[i].done = false;
    s->threads[i].time = &s->time;
    s->threads[i].tree = &s->tree;
  }
}

double system_total_e(System *s) {
  double e = 0;
  
  for (uint i=0; i<s->count; i++) {
#ifdef UNIT_MASS
    double mass = 1;
#else
    double mass = s->bodies[i].mass;
#endif
    
    e += 0.5 * mass * vabs(s->bodies[i].vel);
  }
  
  return e;
}

#define pcg32_random rand

static double randd() {
  return (double)(arc4random_uniform(2<<12));
}

static double randds() {
  int8_t sign = (arc4random()>(UINT32_MAX/2))?1:-1;
  return randd()*sign;
}

System random_sys(uint max_pos, uint count) {
  //Seed it with the address of this function
  srand((uint)(&random_sys));
  System s;
  s.bodies = calloc(sizeof(Body), count);
  s.count = count;
  s.time = 0;
  initialize_threads(&s);
  
  uint i=0;
  while (i<count) {
    vec3 pos = (vec3){randds(), randds(), randds()};
    if (vabs(pos)<max_pos) {
      s.bodies[i].pos = pos;
      //printf("Created body %d (%f, %f, %f)\n", i, vec3_to_triple(pos));
#ifndef UNIT_MASS
      s.bodies[i].mass = 1e31;
#endif
      i++;
    }
  }
  
  
  return s;
}

static double rand_dist(gsl_rng *r, double max) {
  double dist = max * pow(gsl_rng_uniform(r), 2);
  return dist;
}

static double rand_theta(gsl_rng *r) {
  return 2 * M_PI * gsl_rng_uniform(r);
}

static Body random_body(gsl_rng *r, double max) {
  static uint id = 0;
  double theta = rand_theta(r);
  double dist = rand_dist(r, max);
  
  double x = dist * sin(theta);
  double y = dist * cos(theta);
  double z = 0.0;
  
  vec3 pos = (vec3){x, y, z};
  
  double base_vel = sqrt(dist/max);
  
  double x_vel = -base_vel * cos(theta);
  double y_vel = base_vel * sin(theta);
  
  vec3 vel = (vec3){x_vel, y_vel, 0.0};
  
  Body b = {};
  
  b.pos = pos;
  b.vel = vel;
#ifndef UNIT_MASS
  b.mass = 1e25;
#endif
  b.id = id++;
  
  return b;
}

System *random_disk(double max, uint count) {
  System *s = calloc(sizeof(System), 1);
  s->count = count;
  s->bodies = calloc(sizeof(Body), count);
  
  gsl_rng_default_seed = time(NULL);
  gsl_rng_env_setup();
  gsl_rng *r = gsl_rng_alloc(gsl_rng_default);
  
  for (uint i=0; i<count; i++) {
    s->bodies[i] = random_body(r, max);
  }
  
  gsl_rng_free(r);
  initialize_threads(s);
  return s;
}


