//
//  test_vec3.c
//  nbody2
//
//  Created by Thomas Malthouse on 6/6/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "test_vec3.h"

extern const char *current_test_func;

#define FLOAT_TOL (0.05)
#define INIT_TEST current_test_func = __FUNCTION__


bool test_vec3_eq() {
  INIT_TEST;
  int status = 0;
  
  status += !vec3_eq(vec3_0, vec3_0);
  
  status += vec3_eq(vec3_0, vec3_I);
  
  return !status;
}

bool test_vabs() {
  INIT_TEST;
  int status = 0;
  
  status += !(vabs(vec3_0) < FLOAT_TOL);
  
  status += !(vabs(vec3_I) - 1 < FLOAT_TOL);
  
  status += !(vabs((vec3){3.0,4.0,0.0}) - 5 < FLOAT_TOL);
    
  return !status;
}

bool test_inv_vabs() {
  INIT_TEST;
  int status = 0;
  
  status += inv_vabs((vec3){4,0,0}) - 0.5 > FLOAT_TOL;
  
  status += inv_vabs((vec3) {3,4,0}) - 0.2 > FLOAT_TOL;
  
  return !status;
}
