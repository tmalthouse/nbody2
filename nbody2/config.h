//
//  config.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/17/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef config_h
#define config_h

//Comment this out if you want bodies to have a mass != 1
#define UNIT_MASS

//Change this if you want to use a different big G
#ifdef UNIT_MASS
#define BIG_G (1.0)
#else
#define BIG_G (6.67e-11)
#endif

//Softening factor
#define EPSILON 0.05

//Max timestep (in seconds.) Must be an integer, max 2^64 (so not a problem)
#define T_MAX (31500000000u) //1,000 years

#define TOL_PARAM (1.0)

#define NODE_OPEN_PARAM (1.0)

#define INITIAL_NODE_BUFFER (256)

#define SHADER_DIR "/Users/Thomas/Documents/xcode/nbody2/nbody2/viewer/shaders"

#ifndef __APPLE__
typedef unsigned int uint;
#endif

#endif /* config_h */
