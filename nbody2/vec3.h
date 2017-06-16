//
//  vec3.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/17/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef vec3_h
#define vec3_h

#include <stdio.h>
#include <tgmath.h>
#include <stdbool.h>

typedef double vec3 __attribute__((ext_vector_type(3)));

#define vec3_0 ((vec3){0,0,0})
#define vec3_I ((vec3){1,0,0})
#define vec3_J ((vec3){0,1,0})
#define vec3_K ((vec3){0,0,1})

#define vec3_to_triple(v) (double)v.x, (double)v.y, (double)v.z

#define vec3_condense_bool(v) (v.x && v.y && v.z)

static inline double vabs (vec3 v) {return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);}

static inline bool vec3_eq (vec3 v, vec3 w) {
  return v.x==w.x && v.y==w.y && v.z==w.z;
}

static inline vec3 vec3_unit (vec3 v) {return v/vabs(v);}




#endif /* vec3_h */
