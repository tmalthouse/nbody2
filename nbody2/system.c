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
#include "3rdparty/pcg/pcg_basic.h"
#include <inttypes.h>
#include <stdlib.h>

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
      #define vec3_to_triple(v) v.x, v.y, v.z
      printf("Created body %d (%f, %f, %f)\n", i, vec3_to_triple(pos));
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
