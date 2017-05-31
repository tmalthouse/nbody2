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
#include <OpenGL/gl3.h>
#include "vec2.h"

typedef struct {
  SDL_DisplayMode mode;
  SDL_Window *win;
  SDL_GLContext renderer;
  vec2i screensize;
  
  GLuint shader_prog;
  GLuint vao;
  GLuint vbo;
} SDL2Context;

void testDrawTri();



#endif /* viewer_h */
