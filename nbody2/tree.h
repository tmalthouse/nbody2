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
#include "types.h"




void sum_mass(TreeNode *node);

bool should_open_node(TreeNode *node, vec3 pos);

TreeNode build_tree(Body *bodies, uint count);

void update_tree(TreeNode *node);

void free_node(TreeNode *node);

double max_point(Body *bodies, uint count);

void print_tree(TreeNode *node, FILE *f);

void prune_tree(TreeNode *tree);

vec3 node_cmass(TreeNode *node);

#endif /* tree_h */
