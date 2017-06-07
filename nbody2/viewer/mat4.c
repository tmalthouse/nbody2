//
//  mat4.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/31/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "mat4.h"

mat4 mat4_mult(mat4 a, mat4 b) {
  mat4 result = {};
  for (uint i=0; i<4; i++) {
    for (uint j=0; j<4; j++) {
      result.elem[i][j] = vec4_sum(mat4_col(a, j) * mat4_col(b, i));
    }
  }
  
  return result;
}
