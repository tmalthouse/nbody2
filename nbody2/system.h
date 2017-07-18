//
//  system.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef system_h
#define system_h
#include "body.h"
#include "tree.h"
#include "types.h"

void update_system(System *sys);
System random_sys(uint max_pos, uint count);
double system_total_e(System *s);
System *random_disk(double max, uint count);


#endif /* system_h */
