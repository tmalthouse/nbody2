//
//  tree.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/17/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <tgmath.h>
#include "tree.h"
#include "body.h"

//Turns coord info into an index.
inline uint8_t coord_to_index(vec3 coord, vec3 divs) {
  return (coord.x>divs.x) + 2*(coord.y>divs.y) + 4*(coord.z>divs.z);
}

void sum_mass(TreeNode *node) {
#ifndef UNIT_MASS
  double mass = 0;
  for (uint i=0; i<node->nbodies; i++) {
    mass += node->bodies[i]->mass;
  }
  node->mass = mass;
#else
  node->mass = node->nbodies;
#endif
}

static vec3 max_point(Body *bodies, uint count) {
  double max = 0;
  for (uint i=0; i<count; i++) {
    vec3 pos = bodies[i].pos;
    
    if (fabs(pos.x)>max) max = pos.x;
    if (fabs(pos.y)>max) max = pos.y;
    if (fabs(pos.z)>max) max = pos.z;
  }
  
  return (vec3){max,max,max};
}

static inline vec3 node_min_point(vec3 min, vec3 div, uint8_t index) {
  bool x=0, y=0, z=0;
  
  if (index%2) x=1;
  
  if (index%4 > 1) y=1;
  
  if (index%8 > 3) z=1;
  
  return (vec3){
    x?div.x:min.x,
    y?div.y:min.y,
    z?div.z:min.z
  };
}


TreeNode build_tree(Body *bodies, uint count) {
  TreeNode node = {};
  node.bodies = calloc(count, sizeof(Body));
  memcpy(node.bodies, bodies, count*sizeof(Body));
  node.nbodies = count;
  node.divs = vec3_0;
  node.max = max_point(bodies, count);
  node.min = -1*node.max;
  
  return node;
}

static void update_node(TreeNode *node) {
  if (node->nodes == NULL) {
    node->nodes = calloc(8, sizeof(TreeNode));
  }
  
  for (uint i=0; i<node->nbodies; i++) {
    uint8_t index = coord_to_index(node->bodies[i].pos, node->divs);
    
    if (!node->nodes[i].initialized) {
      
    }
  }
}








