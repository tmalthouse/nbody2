//
//  system.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "system.h"
#include "tree.h"
#include <inttypes.h>

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
