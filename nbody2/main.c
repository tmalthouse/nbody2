//
//  main.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/17/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include "system.h"
#include "types.h"

int main(int argc, const char * argv[]) {
  System s = {};
  while (1) {
    update_system(&s);
  }
  return 0;
}
