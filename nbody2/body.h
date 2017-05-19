//
//  body.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/17/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef body_h
#define body_h

#include <stdio.h>
#include <stdint.h>
#include "vec3.h"
#include "config.h"
#include "types.h"



uint64_t update_timestep (Body *b, uint64_t cur_time);

void update_body (Body *b, uint64_t cur_time, TreeNode *tree);


#endif /* body_h */
