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
#include <string.h>
#include "../types.h"

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

static inline bool mat4_eq(mat4 a, mat4 b) {
  return !memcmp(&a, &b, sizeof(mat4));
}

mat4 mat4_mult(mat4 a, mat4 b);

static inline float vec4_sum(vec4 v) {
  return v.x+v.y+v.z+v.w;
}

mat4 new_mat4(float *elems);
mat4 rotation_matrix(float x, float y, float z);
mat4 translation_matrix(float x, float y, float z);
mat4 scale_matrix(float x, float y, float z);
mat4 uniform_scale_matrix(float f);
void print_mat4(mat4 m);
char *mat4_string(mat4 m, char *buf);

static mat4 mat4_id = {.elem={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}};



#endif /* mat4_h */
