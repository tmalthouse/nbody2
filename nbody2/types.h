//
//  types.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef types_h
#define types_h


typedef enum {
  GAS_BODY,
  STAR_BODY,
  DARK_BODY
} BodyType;

typedef struct {
  uint32_t id;
  uint64_t time;
  vec3 pos;
  vec3 vel;
  BodyType type;
  
#ifndef UNIT_MASS
  double mass;
#endif
  
  //Quantities past here are 'volatile' and liable to be changed at any time. They typically shouldn't be accessed directly.
  vec3 acc;
  uint64_t tstep;
} Body;



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

typedef struct {
  Body *bodies;
  uint count;
  uint64_t time;
  
  TreeNode tree;
} System;



#endif /* types_h */
