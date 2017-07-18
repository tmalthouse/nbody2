//
//  viewer.h
//  nbody2
//
//  Created by Thomas Malthouse on 5/23/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//

#ifndef viewer_h
#define viewer_h

#include <stdio.h>
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GLES3/gl3.h>
#endif
#include "vec2.h"

typedef struct {
  SDL_DisplayMode mode;
  SDL_Window *win;
  SDL_GLContext renderer;
  vec2i screensize;
  
  GLuint shader_prog;
  GLuint vao;
  GLuint vbo;
  GLuint MVP_uni;
} SDL2Context;

typedef struct {
  vec3 camera_pos;
  // The angle (in rads) between the current camera longitude and the +x direction. Range between -pi and pi.
  double azimuth;
  // The angle between the camera view and the x/y plane. Range between -pi and pi.
  double altitude;
  
  double aspectRatio;
} CameraState;

void testDrawTri();



#endif /* viewer_h */
