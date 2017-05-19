//
//  tipsy.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/19/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#include "tipsy.h"
#include "body.h"
#include "config.h"
#include "system.h"
#include <stdlib.h>

struct tipsy_head {
  double time ;
  int nbodies ;
  int ndim ;
  int nsph ;
  int ndark ;
  int nstar ;
  int pad;
};

struct tipsy_gas {
  float mass;
  float pos[3];
  float vel[3];
  float rho;
  float temp;
  float eps;
  float metals ;
  float phi ;
};

struct tipsy_dark {
  float mass;
  float pos[3];
  float vel[3];
  float eps;
  float phi;
};

struct tipsy_star {
  float mass;
  float pos[3];
  float vel[3];
  float metals ;
  float tform ;
  float eps;
  float phi;
};

typedef uint8_t byte;

static inline vec3 float3_to_vec3 (float *float3) {
  if (float3==NULL) {
    printf("NULL float triple passed to float3_to_vec3. Returning 0-vec, but watch out!\n");
    return vec3_0;
  }
  
  return (vec3){(double)float3[0], (double)float3[1], (double)float3[2]};
}

System load_tispy(char *file) {
  FILE *tipsy = fopen(file, "r");
  if (tipsy==NULL) {
    goto FILE_OPEN_ERROR;
  }
  
  fseek(tipsy, 0L, SEEK_END);
  size_t filesize = ftell(tipsy);
  rewind(tipsy);
  byte *filebuf = malloc(filesize);
  fread(filebuf, filesize, 1, tipsy);
  fclose(tipsy);
  
  byte *file_ptr = filebuf;
  
  System sys = {};
  
  struct tipsy_head h = *(struct tipsy_head*)file_ptr;
  sys.count = h.nbodies;
  sys.time = (uint64_t)(h.time);
  sys.bodies = calloc(sizeof(Body), sys.count);
  uint body_index = 0;
  
  file_ptr += sizeof(struct tipsy_head);
  for (uint i=0; i<h.nsph; i++) {
    Body b;
    struct tipsy_gas g = *(struct tipsy_gas*)(file_ptr+i*sizeof(struct tipsy_gas));
    b.pos = float3_to_vec3(g.pos);
    b.vel = float3_to_vec3(g.vel);
    b.id = body_index;
    b.type = GAS_BODY;
    
    sys.bodies[body_index] = b;
    body_index++;
  }
  file_ptr += h.nsph*sizeof(struct tipsy_gas);
  
  for (uint i=0; i<h.ndark; i++) {
    Body b;
    struct tipsy_dark d = *(struct tipsy_dark*)(file_ptr+i*sizeof(struct tipsy_dark));
    b.pos = float3_to_vec3(d.pos);
    b.vel = float3_to_vec3(d.vel);
    b.id = body_index;
    b.type = DARK_BODY;
    
    sys.bodies[body_index] = b;
    body_index++;
  }
  file_ptr += h.ndark*sizeof(struct tipsy_dark);
  
  for (uint i=0; i<h.nstar; i++) {
    Body b;
    struct tipsy_star s = *(struct tipsy_star*)(file_ptr+i*sizeof(struct tipsy_star));
    b.pos = float3_to_vec3(s.pos);
    b.vel = float3_to_vec3(s.vel);
    b.id = body_index;
    b.type = STAR_BODY;
    
    sys.bodies[body_index] = b;
    body_index++;
  }
  
  free(filebuf);
  
  return sys;


FILE_OPEN_ERROR:
  perror("File error: ");
  return (System){};
}
