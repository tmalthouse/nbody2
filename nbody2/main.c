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
#include "viewer/viewer.h"
#include "tests/tests.h"

#define TESTS 0

int main(int argc, const char * argv[]) {
  
#if TESTS
  runtests(true);
#else
  run_simulation();
#endif
}
