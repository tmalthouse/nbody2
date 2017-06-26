//
//  tests.c
//  nbody2
//
//  Created by Thomas Malthouse on 6/6/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "tests.h"

const char *current_test_func;

bool (*funclist[])() = {test_vabs, test_vec3_eq, test_inv_vabs, test_fastinvsqrt};
char success[] = "\u2705 Function %s passed!\n";
char failure[] = "\u274C Function %s failed!\n";

void runtests(bool benchmark) {
  uint count = sizeof(funclist)/sizeof(void *);
  uint net = 0;
  for (uint i=0; i<count; i++) {
    bool result = funclist[i]();
    printf(result?success:failure, current_test_func);
    net += !result;
  }
  printf("%d test(s) failed.\n", net);
  
  if (benchmark) {
    benchmark_vec3_normalize();
  }
}
