//
//  types.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef types_h
#define types_h
#include <stdint.h>
#include "vec3.h"
#include "c11threads.h"
#include <stdbool.h>
#include <sys/types.h>

typedef enum {
  GAS_BODY,
  STAR_BODY,
  DARK_BODY
} BodyType;


typedef struct {
  uint32_t id;
  vec3 pos;
  vec3 vel;
  BodyType type;
  
#ifndef UNIT_MASS
  double mass;
#endif
  
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
  bool resized;
};

typedef struct {
  thrd_t t_id;
  //These will not be the same for each thread.
  Body *bodies;
  uint count;
  volatile bool done;
  
  //Whereas these will, and are updated by the root.
  TreeNode *tree;
  uint64_t *time;
} CalculationThread;

typedef struct {
  Body *bodies;
  uint count;
  uint64_t time;
  
  TreeNode tree;
  
  CalculationThread *threads;
  uint thread_count;
  uint base_count;
  uint high_count;
} System;





#endif /* types_h */
