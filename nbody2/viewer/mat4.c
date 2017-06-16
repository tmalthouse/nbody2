//
//  mat4.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/31/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "mat4.h"
#include <tgmath.h>

mat4 mat4_mult(mat4 a, mat4 b) {
  mat4 result = {};
  for (uint i=0; i<4; i++) {
    for (uint j=0; j<4; j++) {
      result.elem[i][j] = vec4_sum(mat4_col(a, j) * mat4_col(b, i));
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

mat4 new_mat4(float *elems) {
  return (mat4){.elem = {
    (vec4){elems[0], elems[4], elems[8], elems[12]},
    (vec4){elems[1], elems[5], elems[9], elems[13]},
    (vec4){elems[2], elems[6], elems[10], elems[14]},
    (vec4){elems[3], elems[7], elems[11], elems[15]}}
  };
}

mat4 rot_matrix(float x, float y, float z) {
  
  mat4 x_mat = new_mat4((float[]){
    1.0f, 0.0f,   0.0f,    0.0f,
    0.0f, cos(x), -sin(x), 0.0f,
    0.0f, sin(x), cos(x),  0.0f,
    0.0f, 0.0f,   0.0f,    1.0f
  });
  
  mat4 y_mat = new_mat4((float[]){
    cos(y),  0.0f, sin(y), 0.0f,
    0.0f,    1.0f, 0.0f,   0.0f,
    -sin(y), 0.0f, cos(x), 0.0f,
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
