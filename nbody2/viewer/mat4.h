//
//  mat4.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/31/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef mat4_h
#define mat4_h

#include <stdio.h>

typedef float vec4 __attribute__((ext_vector_type(4)));

typedef struct {
  vec4 elem[4];
} mat4;

static inline vec4 mat4_row(mat4 m, uint row) {
  return (vec4){m.elem[row][0], m.elem[row][1], m.elem[row][2], m.elem[row][3]};
}

static inline vec4 mat4_col(mat4 m, uint col) {
  return m.elem[col];
}

mat4 mat4_mult(mat4 a, mat4 b);

static inline float vec4_sum(vec4 v) {
  return v.x+v.y+v.z+v.w;
}

#endif /* mat4_h */
