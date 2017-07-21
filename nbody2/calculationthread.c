//
//  calculationthread.c
//  nbody2
//
//  Created by Thomas Malthouse on 7/13/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "calculationthread.h"
#include "body.h"
#include <signal.h>

int thread_loop(void *info) {
  CalculationThread *self = (CalculationThread*)info;
  
  do {
    if (!self->done && self->bodies != NULL) {
      for (uint i=0; i<self->count; i++) {
        if (*(self->time) == 0 || (self->bodies[i].tstep) % *(self->time) == 0) {
          update_body(&self->bodies[i], self->tree);
        }
      }
      self->done = true;
    }
  } while (true);
  
  //Should never reach here? Ever?
  return -1;
}
