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

typedef double vec3 __attribute__((ext_vector_type(3)));

#define vec3_0 ((vec3){0,0,0})
#define vec3_I ((vec3){1,0,0})
#define vec3_J ((vec3){0,1,0})
#define vec3_K ((vec3){0,0,1})

#endif /* vec3_h */
