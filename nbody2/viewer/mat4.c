//
//  mat4.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/31/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "mat4.h"
#include <tgmath.h>
#include <stdlib.h>

mat4 mat4_mult(mat4 a, mat4 b) {
  mat4 result = {};
  for (uint i=0; i<4; i++) {
    for (uint j=0; j<4; j++) {
      result.elem[i][j] = vec4_sum(mat4_row(a, j) * mat4_col(b, i));
    }
  }
  
  return result;
}

mat4 mat4_transpose(mat4 a) {
  mat4 res = {};
  for (uint i=0; i<4; i++) {
    for (uint j=0; j<4; j++) {
      res.elem[i][j] = a.elem[j][i];
    }
  }
  return res;
}

static inline mat4 new_mat4(float *elems) {
  if (elems==NULL) return (mat4){};
  else             return (mat4){.elem = {
    (vec4){elems[0], elems[4], elems[8], elems[12]},
    (vec4){elems[1], elems[5], elems[9], elems[13]},
    (vec4){elems[2], elems[6], elems[10], elems[14]},
    (vec4){elems[3], elems[7], elems[11], elems[15]}}
  };
}

mat4 rotation_matrix(float x, float y, float z) {
  
  mat4 x_mat = new_mat4((float[]){
    1.0f, 0.0f,   0.0f,    0.0f,
    0.0f, cos(x), -sin(x), 0.0f,
    0.0f, sin(x), cos(x),  0.0f,
    0.0f, 0.0f,   0.0f,    1.0f
  });
  
  mat4 y_mat = new_mat4((float[]){
    cos(y),  0.0f, sin(y), 0.0f,
    0.0f,    1.0f, 0.0f,   0.0f,
    -sin(y), 0.0f, cos(y), 0.0f,
    0.0f,    0.0f, 0.0f,   1.0f
  });
  
  mat4 z_mat = new_mat4((float[]){
    cos(z), -sin(z), 0.0f, 0.0f,
    sin(z), cos(z),  0.0f, 0.0f,
    0.0f,   0.0f,    1.0f, 0.0f,
    0.0f,   0.0f,    0.0f, 1.0f
  });
  
  return mat4_mult(x_mat, mat4_mult(y_mat, z_mat));
}

mat4 translation_matrix(float x, float y, float z) {
  return new_mat4((float[]){
    1, 0, 0, x,
    0, 1, 0, y,
    0, 1, 0, z,
    0, 0, 0, 1
  });
}

mat4 scale_matrix(float x, float y, float z) {
  return new_mat4((float[]){
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1
  });
}

mat4 uniform_scale_matrix(float f) {
  return scale_matrix(f, f, f);
}

char *mat4_string(mat4 m, char *buf) {
  if (buf == NULL) buf = calloc(1, 1024);
  vec4 *e = m.elem;
  sprintf(buf,
         "[%f,\t%f,\t%f,\t%f]\n"
         "[%f,\t%f,\t%f,\t%f]\n"
         "[%f,\t%f,\t%f,\t%f]\n"
         "[%f,\t%f,\t%f,\t%f]\n",
         e[0][0], e[0][1], e[0][2], e[0][3],
         e[1][0], e[1][1], e[1][2], e[1][3],
         e[2][0], e[2][1], e[2][2], e[2][3],
         e[3][0], e[3][1], e[3][2], e[3][3]);
  
  return buf;
}

void print_mat4(mat4 m) {
  char *matstr = mat4_string(m, NULL);
  printf("%s\n", matstr);
  free(matstr);
}
