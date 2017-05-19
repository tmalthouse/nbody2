//
//  viewer.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef viewer_h
#define viewer_h

#include <stdio.h>

#define new_matrix3(elems) ((matrix3){.elem=elems})
#define matrix3_ident (matrix3){.elem={{1,0,0},{0,1,0},{0,0,1}}}

typedef struct {
  float elem[3][3];
} matrix3;

matrix3 i = matrix3_ident;

#endif /* viewer_h */
