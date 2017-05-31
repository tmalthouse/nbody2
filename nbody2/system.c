//
//  system.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "system.h"
#include "tree.h"
#include "3rdparty/pcg/pcg_basic.h"
#include <inttypes.h>
#include <stdlib.h>

void update_system(System *sys) {
  if (!sys->tree.initialized) {
    sys->tree = build_tree(sys->bodies, sys->count);
  } else {
    update_tree(&sys->tree);
  }
  
  uint64_t min_tstep = UINT64_MAX;
  for (uint i=0; i<sys->count; i++) {
    if (sys->bodies[i].tstep<min_tstep) {
      min_tstep = sys->bodies[i].tstep;
    }
    
    if ((sys->bodies[i].tstep)%(sys->time) == 0) {
      update_body(&sys->bodies[i], &sys->tree);
    }
  }
  
  sys->time += sys->time%min_tstep;
}

static double randd() {
  int main_part = pcg32_random();
  int sign = pcg32_random();
  return (sign%2 ? 1 : -1) * (pow((double)main_part/(double)UINT32_MAX,2));
}

System random_sys(uint max_pos, uint count) {
  //Seed it with the address of this function
  srand((uint)(&random_sys));
  System s;
  s.bodies = calloc(sizeof(Body), count);
  
  uint i=0;
  while (i<count) {
    vec3 pos = (vec3){randd(), randd(), randd()};
    if (vabs(pos)<max_pos) {
      s.bodies[i].pos = pos;
      i++;
    }
  }
  
  s.count = count;
  
  return s;
}
