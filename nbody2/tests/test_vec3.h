//
//  test_vec3.h
//  nbody2
//
//  Created by Thomas Malthouse on 6/6/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef test_vec3_h
#define test_vec3_h

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "../vec3.h"
#include "../config.h"

bool test_vec3_eq();
bool test_vabs();
bool test_inv_vabs();
bool test_fastinvsqrt();

time_t benchmark_vec3_normalize();


#endif /* test_vec3_h */
