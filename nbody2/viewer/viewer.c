//
//  viewer.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "viewer.h"
#include "../vec3.h"

matrix3 matrix_mult(matrix3 a, matrix3 b) {
  matrix3 c = {};
  for (uint i=0; i<3; i++) {
    for (uint j=0; j<3; j++) {
      float e = 0;
      for (uint k = 0; k<3; k++) {
        e += a.elem[i][k]*b.elem[k][j];
      }
      c.elem[i][j] = e;
    }
  }
  return c;
}

vec3 matrix_vmult(matrix3 a, vec3 v) {
  vec3 result = vec3_0;
  for (uint i=0; i<3; i++) {
    result += (vec3){v.x*a.elem[i][0], v.y*a.elem[i][1], v.z*a.elem[i][2]};
  }
  return result;
}

