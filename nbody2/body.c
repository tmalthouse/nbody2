//
//  body.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/17/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "body.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

static inline uint64_t t_ideal(Body *b) {
  if (vec3_eq(b->acc, vec3_0)) {
    return 1;
  }
  
  //return sqrt(TOL_PARAM * EPSILON / vabs(b->acc));
  return 1000;
}

uint64_t update_timestep (Body *b, uint64_t cur_time) {
  uint64_t t_i = t_ideal(b);
  
  //Muldavin p60
  if (b->tstep == 0) {
    b->tstep = t_i;
  } else if (t_i < b->tstep) {
    b->tstep /= 2;
  } else if (t_i > 2*b->tstep && (cur_time/b->tstep)%2 == 0) {
    b->tstep *= 2;
  }
  
  return b->tstep;
}

typedef struct {
  TreeNode **nodes;
  size_t cap;
  size_t len;
} NodeList;

static void NodeList_append(NodeList *l, TreeNode *n) {
  if (l->len >= l->cap) {
    l->nodes = realloc(l->nodes, 2*l->cap*sizeof(TreeNode*));
    l->cap *= 2;
  }
  l->nodes[l->len] = n;
  l->len++;
}


void node_finder(NodeList *l, vec3 pos, TreeNode *tree) {
  TreeNode *nodes = tree->nodes;
  
  for (uint i=0; i<8; i++) {
    if (nodes == NULL || nodes[i].nbodies == 0) {continue;}
    
    else if (nodes[i].nbodies == 1 || !should_open_node(&nodes[i], pos)) {
      NodeList_append(l, &tree->nodes[i]);
    }
    
    else node_finder(l, pos, &nodes[i]);
  }
}

vec3 body_acc(TreeNode **nodes, size_t node_count, Body *b);

void update_body (Body *b, TreeNode *tree) {
  static _Thread_local NodeList nodes = {};
  
  if (nodes.nodes==NULL) {
    nodes.nodes = calloc(INITIAL_NODE_BUFFER, sizeof(TreeNode*));
    nodes.cap = INITIAL_NODE_BUFFER;
  } else {
    nodes.len = 0;
    memset(nodes.nodes, 0x0, nodes.len * sizeof(TreeNode*));
  }
  
  Body tmp_body = *b;
  
  
  //Get the list of nodes that need to be calculated
  node_finder(&nodes, tmp_body.pos, tree);
  vec3 a_1 = body_acc(nodes.nodes, nodes.len, &tmp_body);
  vec3 v_1 = b->vel;
  
  tmp_body.pos+=v_1*((double)tmp_body.tstep/2);
  node_finder(&nodes, tmp_body.pos, tree);
  vec3 a_2 = body_acc(nodes.nodes, nodes.len, &tmp_body);
  vec3 v_2 = tmp_body.vel+=a_1*((double)tmp_body.tstep/2);
  
  tmp_body.pos = b->pos+v_2*((double)tmp_body.tstep/2);
  node_finder(&nodes, tmp_body.pos, tree);
  vec3 a_3 = body_acc(nodes.nodes, nodes.len, &tmp_body);
  vec3 v_3 = b->vel += a_2*((double)tmp_body.tstep/2);
  
  tmp_body.pos = b->pos+v_3*(double)tmp_body.tstep;
  node_finder(&nodes, tmp_body.pos, tree);
  vec3 a_4 = body_acc(nodes.nodes, nodes.len, &tmp_body);
  vec3 v_4 = b->vel += a_3*(double)tmp_body.tstep;
  
  vec3 a_fin = (a_1 + 2*a_2 + 2*a_3 + a_4)/6;
  vec3 v_fin = (v_1 + 2*v_2 + 2*v_3 + v_4)/6;
  
  //if (!vec3_eq(v_fin, vec3_0)) printf("Nonzero velocity on body %d\n", b->id);
  
  b->acc = a_fin;
  b->vel += a_fin*b->tstep;
  b->pos += v_fin*b->tstep;
}

uint total_force_calcs;
vec3 g_acc(vec3 pos1, vec3 pos2, double m2) {
  total_force_calcs++;
  vec3 r = pos2 - pos1;
  
  //The bottom term of this equation is (abs(v))^2==(sqrt(...))^2, which can be written as follows and avoids the expensive sqrt call.
  //The EPSILON softening factor improves accuracy for short-distance interactions (since these aren't actually bodies, but stat. objs.
  double g_mag = (BIG_G * (m2) / (r.x*r.x+r.y*r.y+r.z*r.z + EPSILON * EPSILON));
  vec3 g = g_mag * vec3_normalize(r);
  return g;
}
  
  

vec3 body_acc(TreeNode **nodes, size_t node_count, Body *b) {
  vec3 net_acc = vec3_0;
  for (uint i=0; i<node_count; i++) {
    //If the node has just one body:
    switch (nodes[i]->nbodies) {
      case 0: break;

      case 1: {
        Body *cur = nodes[i]->bodies[0];
        //Don't calculate the force between the given body and itself---there lies div/0
        if (cur->id == b->id) break;
        
#ifdef UNIT_MASS
        net_acc += g_acc(b->pos, cur->pos, 1);
#else
        net_acc += g_acc(b->pos, cur->pos, cur->mass);
#endif
        break;
      }
      
      default: {
#ifdef UNIT_MASS
        net_acc += g_acc(b->pos, nodes[i]->ctr_mass, nodes[i]->mass);
#else
        net_acc += g_acc(b->pos, nodes[i]->ctr_mass, nodes[i]->mass);
#endif
        break;
      }
      
    }
  }
  return net_acc;
}
