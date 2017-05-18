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
static inline uint8_t coord_to_index(vec3 coord, vec3 divs) {
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



vec3 center_of_mass(TreeNode *node) {
  vec3 cmass = vec3_0;
  
  for (uint i=0; i<node->nbodies; i++) {
#ifdef UNIT_MASS
    double multiplier = 1/node->mass;
#else
    double multiplier = node->bodies[i]->mass/node->mass;
#endif
    cmass += multiplier * node->bodies[i]->pos;
  }
  
  return cmass;
}

static void update_node(TreeNode *node) {
  if (node->nodes == NULL) {
    node->nodes = calloc(8, sizeof(TreeNode));
  }
  
  //Clear the body buffers
  for (uint i=0; i<8; i++) {
    if (node->nodes[i].initialized) {
      memset(node->nodes[i].bodies, 0x0, sizeof(Body*)*node->nodes[i].capacity);
      node->nodes[i].nbodies = 0;
    }
  }
  
  for (uint i=0; i<node->nbodies; i++) {
    uint8_t index = coord_to_index(node->bodies[i]->pos, node->divs);
    TreeNode *child_node = &node->nodes[index];
    
    //If a node is new (freshly allocated), set it up and add parameters.
    if (!child_node->initialized) {
      child_node->min = node_min_point(node->min, node->divs, index);
      child_node->max = node->nodes[i].min + node->divs;
      child_node->divs = (child_node->max + child_node->min)*0.5;
      child_node->bodies = calloc(sizeof(Body*), node->nbodies);
      child_node->capacity = node->nbodies;
      child_node->initialized = true;
    }
    
    //If the body ptr buffer is at capacity, double its size
    if (child_node->nbodies == child_node->capacity) {
      child_node->bodies = realloc(child_node->bodies, (2*sizeof(Body*)*child_node->capacity));
      child_node->capacity *= 2;
    }
    
    child_node->bodies[child_node->nbodies] = node->bodies[i];
    child_node->nbodies++;
#ifdef UNIT_MASS
    child_node->mass++;
#else
    child_node->mass += node->bodies[i]->mass;
#endif
  }
  

  
  //Then recurse, and repeat the procedure for all nodes with >1 particle
  for (uint i=0; i<8; i++) {
    //But first, calculate the centers of mass
    node->nodes[i].ctr_mass = center_of_mass(&node->nodes[i]);
    
    if (node->nodes[i].nbodies > 1) {
      update_node(&node->nodes[i]);
    }
  }
}


TreeNode build_tree(Body *bodies, uint count) {
  TreeNode node = {};
  node.bodies = calloc(count, sizeof(Body*));
  for (uint i=0; i<count; i++) {
    node.bodies[i] = &bodies[i];
  }
  node.nbodies = count;
  node.divs = vec3_0;
  node.max = max_point(bodies, count);
  node.min = -1*node.max;
  node.initialized = true;
  
  update_node(&node);
  
  return node;
}

void free_node(TreeNode *node) {
  for (uint i=0; i<8; i++) {
    if (node->nodes[i].initialized) {
      free_node(&node->nodes[i]);
    }
  }
  
  free(node->bodies);
  free(node->nodes);
}




