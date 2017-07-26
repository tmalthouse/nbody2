//
//  body.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/17/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef body_h
#define body_h

#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include "vec3.h"
#include "config.h"
#include "types.h"

extern uint total_force_calcs;


uint64_t update_timestep (Body *b, uint64_t cur_time);

void update_body (Body *b, TreeNode *tree);

#define check_dbl(d) (!(isnan(d) || isinf(d)))

static inline void verify_body(Body b) {
  int status = 1*
  check_dbl(b.pos.x)*
  check_dbl(b.pos.y)*
  check_dbl(b.pos.z)*
  check_dbl(b.vel.x)*
  check_dbl(b.vel.y)*
  check_dbl(b.vel.z);
  
  if (!status) {
    printf("Body corrupted! (id %d)\n", b.id);
    raise(SIGINT);
  }
}

#undef check_dbl


#endif /* body_h */
