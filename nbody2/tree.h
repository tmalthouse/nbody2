//
//  tree.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/17/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef tree_h
#define tree_h

#include <stdio.h>
#include "vec3.h"
#include "body.h"
#include "config.h"


typedef struct _tree_node TreeNode;

struct _tree_node {
  struct _tree_node *nodes;
  uint level;
  vec3 divs;
  vec3 max;
  vec3 min;
  
  Body **bodies;
  uint nbodies;
  double mass;
  vec3 ctr_mass;
  
  uint capacity;
  bool initialized;
};

void sum_mass(TreeNode *node);

#endif /* tree_h */
