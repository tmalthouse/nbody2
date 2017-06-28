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
#include <inttypes.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include <gsl/gsl_rng.h>
#ifndef __APPLE__
#include <bsd/stdlib.h>
#endif

void update_system(System *sys) {
  if (!sys->tree.initialized) {
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
    
    if (sys->time == 0 || (sys->bodies[i].tstep)%(sys->time) == 0) {
      update_body(&sys->bodies[i], &sys->tree);
    }
  }
  
  sys->time += min_tstep?sys->time%min_tstep:1000;
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
  
  s.count = count;
  s.time = 0;
  
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
  
  vec3 pos = (vec3){x, y, 0.0};
  
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

System random_disk(double max, uint count) {
  System s = {};
  s.count = count;
  s.bodies = calloc(sizeof(Body), count);
  
  gsl_rng_default_seed = time(NULL);
  gsl_rng_env_setup();
  gsl_rng *r = gsl_rng_alloc(gsl_rng_default);
  
  for (uint i=0; i<count; i++) {
    s.bodies[i] = random_body(r, max);
  }
  
  gsl_rng_free(r);
  return s;
}

