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
#include <math.h>
#include <float.h>
#include <assert.h>
#include "vec3.h"
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

double max_point(Body *bodies, uint count) {
  double max = 0;
  for (uint64_t i=0; i<count; i++) {
    vec3 pos = bodies[i].pos;
    
    if (fabs(pos.x)>max) max = fabs(pos.x);
    if (fabs(pos.y)>max) max = fabs(pos.y);
    if (fabs(pos.z)>max) max = fabs(pos.z);
  }
  assert (max >= 0);
  
  return max;
}

vec3 max_point_vec(Body *bodies, uint count) {
  return (vec3)(max_point(bodies, count));
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



static inline vec3 center_of_mass(TreeNode *node) {
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
  if (node->level == 0) {
    double max_pt = max_point(*node->bodies, node->nbodies);
    if (2*max_pt > node->max.x) {
      assert(max_pt > 0);
      node->max = (vec3)(10*max_pt);
      node->min = -node->max;
      node->resized = true;
      printf("Resizing tree! New max is %f\n", max_pt);
    }
  }
  
  
  if (node->nodes == NULL) {
    node->nodes = calloc(8, sizeof(TreeNode));
    for (uint i=0; i<8; i++) {
      TreeNode *child_node = &node->nodes[i];
      //This triggers calculation of bounds
      node->resized = true;
      child_node->bodies = calloc(sizeof(Body*), node->nbodies);
      child_node->nodes = NULL;
      child_node->capacity = node->nbodies;
      child_node->nbodies = 0;
      child_node->level = node->level+1;
      child_node->initialized = true;
    }
  }
  
  //Reset sizes, if neccessary
  if (node->resized) {
    for (uint i=0; i<8; i++) {
      TreeNode *child = &node->nodes[i];
      child->resized = true;
      child->min = node_min_point(node->min, node->divs, i);
      child->max = child->min + (node->max-node->divs);
      child->divs = (child->max + child->min)*0.5;
    }
  }
  
  //Clear the body buffers
  for (uint i=0; i<8; i++) {
    if (node->nodes[i].initialized) {
      node->nodes[i].nbodies = 0;
      node->nodes[i].mass = 0;
    }
  }
  
  for (uint i=0; i<node->nbodies; i++) {
    vec3 tpos = node->bodies[i]->pos;
    vec3 tdivs = node->divs;
    
    
    uint8_t index = coord_to_index(tpos, tdivs);
    TreeNode *child_node = &node->nodes[index];

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

void update_tree(TreeNode *node) {
  update_node(node);
  FILE *f = fopen("/Users/Thomas/Desktop/out.txt", "w");
  print_tree(node, f);
  fclose(f);
}

TreeNode build_tree(Body *bodies, uint count) {
  TreeNode node = {};
  node.bodies = calloc(count, sizeof(Body*));
  for (uint i=0; i<count; i++) {
    node.bodies[i] = &bodies[i];
  }
  node.nbodies = count;
  node.divs = vec3_0;
  node.max = 10*max_point_vec(bodies, count);
  node.min = -node.max;
  node.initialized = true;
  node.nodes = NULL;
  node.level = 0;

  sum_mass(&node);

  
  update_node(&node);
  
  return node;
}

void free_node(TreeNode *node) {
  for (uint i=0; i<8; i++) {
    if (node->nodes && node->nodes[i].initialized) {
      free_node(&node->nodes[i]);
    }
  }
  
  free(node->bodies);
  free(node->nodes);
  
  node->nodes = NULL;
  node->bodies = NULL;
}

//From http://www.cita.utoronto.ca/~dubinski/treecode/node4.html
bool should_open_node(TreeNode *node, vec3 pos) {
  double d = vabs(pos-node->ctr_mass);
  double size = node->max.x - node->min.x;
  double delta = vabs(node->ctr_mass-node->divs);
  
  return d < (size/NODE_OPEN_PARAM) + delta;
}



void prune_tree(TreeNode *tree) {
  if (tree->nbodies == 0) {
    free_node(tree);
  } else if (tree->nodes != NULL) {
    for (uint i=0; i<8; i++) {
      prune_tree(&tree->nodes[i]);
    }
  }
  
  if (tree->level == 0) puts("pruned tree");
}


void _print_tree(TreeNode *node, FILE *f) {
  char levsep[] = "\u2503\t";
  char preamble[512] = {};
  
  //Don't print if the node is empty
  if (node->nbodies == 0) {return;};
  
  for (uint i=0; i<node->level; i++) {
    strcat(preamble, levsep);
  }
  
  
  if (node->nbodies == 1) {
    Body *b = node->bodies[0];
    fprintf(f, "%s\u2523Body: addr: %p loc: (%.2f, %.2f, %.2f) vel: (%.2f, %.2f, %.2f)\n", preamble, b, vec3_to_triple(b->pos), vec3_to_triple(b->vel));
  } else {
    fprintf(f, "%s\u2523Node: addr: %p count: %d mass: %f, CoM: (%.2f, %.2f, %.2f)\n", preamble, node, node->nbodies, node->mass, vec3_to_triple(node->ctr_mass));
    for (uint i=0; i<8; i++) {
      _print_tree(&node->nodes[i], f);
    }
  }
  
}

void print_tree(TreeNode *node, FILE *f) {
  _print_tree(node, f);
}






