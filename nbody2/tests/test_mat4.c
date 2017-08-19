//
//  test_mat4.c
//  nbody2
//
//  Created by Thomas Malthouse on 7/26/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "test_mat4.h"

extern const char *current_test_func;

#define FLOAT_TOL (0.05)
#define INIT_TEST current_test_func = __FUNCTION__; printf("Testing %s:\n", current_test_func);

static bool mat4_approx_eq(mat4 a, mat4 b) {
  vec4 diff = {vec4_sum(a.elem[0]-b.elem[0]),
               vec4_sum(a.elem[1]-b.elem[1]),
               vec4_sum(a.elem[2]-b.elem[2]),
               vec4_sum(a.elem[3]-b.elem[3])};
  return vec4_sum(diff) < FLOAT_TOL*8;
}

mat4 base_matrix;

void init_tests() { base_matrix = new_mat4((float[]){
  1.0f, 2.0f, 3.0f, 4.0f,
  5.0f, 6.0f, 7.0f, 8.0f,
  9.0f, 10.0f,11.0f,12.0f,
  13.0f,14.0f,15.0f,16.0f
}); }

bool test_mat4_eq () {
  INIT_TEST;
  
  bool status = true;
  
  status *= mat4_eq(base_matrix, base_matrix);
  
  status *= !mat4_eq(base_matrix, mat4_id);
  
  return status;
}

bool test_mat4_row () {
  INIT_TEST;
  
  typedef int b4v __attribute((ext_vector_type(4)));
  b4v eqs = mat4_row(base_matrix, 2) == (vec4){5.0f, 6.0f, 7.0f, 8.0f};
  
  return eqs.x && eqs.y && eqs.z && eqs.w;
}

bool test_mat4_col () {
  INIT_TEST;
  
  typedef int b4v __attribute((ext_vector_type(4)));
  b4v eqs = mat4_col(base_matrix, 2) == (vec4){3.0f, 7.0f, 11.0f, 15.0f};
  
  return eqs.x && eqs.y && eqs.z && eqs.w;
}

bool test_mat4_mult () {
  INIT_TEST;
  
  mat4 matrix2 = new_mat4((float[]){
    -13.0f, 9.0f, 73.0f, -42.0f,
    11.0f, 4.0f, -19.0f, 15.0f,
    26.0f, 31.0f, 18.0f, 31.0f,
    -21.0f, -54.0f, 17.0f, 9.0f
  });
  
  bool status = true;
  
  status *= mat4_approx_eq(
    mat4_mult(base_matrix, matrix2),
    
  new_mat4((float[])
   {
     3,   -106,  157,   157,
     15,  -146,  513,   249,
     27,  -186,  869,   341,
     39,  -226,  1225,  433
   }));
  
  status *= mat4_approx_eq(
   mat4_mult(matrix2, base_matrix),
   
   new_mat4((float[])
            {
              143,  170,  197,  224,
              55,   66,   77,    88,
              746,  852,  958,  1064,
              109,  70,   31,    -8
            }));
  
  return status;
}
