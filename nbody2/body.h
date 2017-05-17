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
#include "vec3.h"
#include "config.h"


typedef struct {
  uint32_t id;
  vec3 pos;
  vec3 vel;
  
#ifndef UNIT_MASS
  double mass;
#endif
} Body;

#endif /* body_h */
