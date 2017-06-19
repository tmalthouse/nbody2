//
//  viewer.c
//  nbody2
//
//  Created by Thomas Malthouse on 5/23/17.
//  Copyright © 2017 Thomas Malthouse. All rights reserved.
//
//  Largely based off of http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/

#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "../vec3.h"
#include "../body.h"
#include "../tree.h"
#include "../config.h"
#include "../system.h"
#include "../tipsy.h"
#include "viewer.h"
//#include <GL/glew.h>
#include <OpenGL/gl3.h>
#include <SDL2/SDL_opengl.h>

static off_t fsize(char *f) {
  struct stat st;
  
  if (stat(f, &st) == 0) {
    return st.st_size;
  }
  
  printf("Can't determine size of %s: %s\n", f, strerror(errno));
  return -1;
}

vec3 colors[] = {(vec3){0.5,0,0}, (vec3){0,0.5,0}, (vec3){0,0,0.5}, (vec3){0.5,0.5,0}, (vec3){0,0.5,0.5}, (vec3){0.5,0,0.5}};
void _check_gl_error(const char *file, int line);
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

GLuint test_vert_shader;
GLuint test_frag_shader;

SDL2Context new_SDL2Context(uint32_t init_flags) {
  SDL2Context c = {};
  
  SDL_Init(init_flags);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
  
  SDL_GetCurrentDisplayMode(0, &c.mode);
  
  c.win = SDL_CreateWindow("nbody2 test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           c.mode.w, c.mode.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  c.renderer = SDL_GL_CreateContext(c.win);
  
  //glewExperimental = GL_TRUE;
  //glewInit();
  
  int x,y;
  SDL_GetWindowSize(c.win, &x, &y);
  
  c.screensize = (vec2i){x,y};
  
  return c;
}

int load_shader (GLuint *shader, char *path, GLenum shader_type) {
  int ret = 0;
  
  *shader = glCreateShader(shader_type);
  char *shader_path = path;
  FILE *sh = fopen(shader_path, "r");
  if (sh == NULL) {
    goto err;
  }
  size_t size = fsize(path);
  rewind(sh);
  char *shader_buf = NULL;
  if (size > 0) {
    shader_buf = calloc(1, size+1);
  } else {
    goto err;
  }
  fread(shader_buf, size, 1, sh);
  shader_buf[size] = '\0';
  fclose(sh);
  
  printf("%s\n", shader_buf);
  glShaderSource(*shader, 1, (const char* const*)&shader_buf, NULL);
  free(shader_buf);
  check_gl_error();
  
  glCompileShader(*shader);
  check_gl_error();
  
  GLint status;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    ret++;
  }
  
  char log[512];
  glGetShaderInfoLog(*shader, 512, NULL, log);
  printf("Log for shader %s:\n%s\n", path, log);
  
  return ret;
  
err:
  printf("Couldn't load file or compile shader!\n");
  return -1;
}

int init_shaders(SDL2Context *con) {
  if (load_shader(&test_vert_shader, SHADER_DIR"/body_vertex_shader.glsl", GL_VERTEX_SHADER) ||
      load_shader(&test_frag_shader, SHADER_DIR"/simple_frag_shader.glsl", GL_FRAGMENT_SHADER)) {
    printf("Err loading shaders!!!!\n");
  } check_gl_error();
  
  
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  check_gl_error();
  
  GLuint vbo;
  glGenBuffers(1, &vbo);check_gl_error();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  check_gl_error();
  
  
  con->shader_prog = glCreateProgram();
  glAttachShader(con->shader_prog, test_vert_shader);
  glAttachShader(con->shader_prog, test_frag_shader);
  glBindFragDataLocation(con->shader_prog, 0, "outColor");
  check_gl_error();
  
  glLinkProgram(con->shader_prog);
  check_gl_error();
  
  char log[512];
  glGetProgramInfoLog(con->shader_prog, 512, NULL, log);
  printf("Shader program info: %s\n", log);
  
  glUseProgram(con->shader_prog);
  check_gl_error();
  
  GLint posAttrib = glGetAttribLocation(con->shader_prog, "position");
  
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);
  check_gl_error();
  
  
  
  
  
  return -1;
  
}



float randf() {
  return ((float)rand())/((float)RAND_MAX);
}


void draw_bodies (SDL2Context c, Body *bodies, uint count);


void testDrawTri() {
  srand((int)time(NULL));
  SDL2Context c = new_SDL2Context(SDL_INIT_VIDEO);
  System s = random_sys(1e10, 1000);
  //System s = load_tispy("/Users/Thomas/Downloads/IsolatedCollapse.000000");
  s.tree = build_tree(s.bodies, s.count);
  
  init_shaders(&c);
  
  
  while(1) {
    update_system(&s);
    //float r = randf()/2, g = randf()/2, b = randf()/2;
    float r=0,g=0,b=0;
    glClearColor(r, g, b, 0);
    
    glClear(GL_COLOR_BUFFER_BIT);
    draw_bodies(c, s.bodies, s.count);
    
    
    SDL_GL_SwapWindow(c.win);
    SDL_Delay(5);
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          //case SDL_KEYDOWN:
          SDL_GL_DeleteContext(c.renderer);
          SDL_DestroyWindow(c.win);
          exit(0);
          break;
        
        case SDL_KEYDOWN: {
          SDL_KeyboardEvent key = e.key;
          switch (key.keysym.sym) {
            case SDLK_d: {
              FILE *f = fopen("/Users/Thomas/treedump.txt", "w");
              print_tree(&s.tree, f);
              fclose(f);
              puts("Writing tree dump");
              SDL_Delay(1000);
              break;
            }
              
            case SDLK_e:
              printf("Total system kinetic energy: %f\n", system_total_e(&s));
              break;
            
            case SDLK_TAB:
              s = random_sys(1e10, 1000);
              s.tree = build_tree(s.bodies, 1000);
              break;
              
          }
        }
          
        default:
          break;
      }
    }
    
    
    //print_tree(&s.tree);
  }
}


float *serialize_positions(Body *bodies, uint count) {
  static float *vector_buffer = NULL;
  static uint cap = 0;
  
  double max_pt = max_point(bodies, count);
  
  if (vector_buffer == NULL && count > 0) {
    vector_buffer = calloc(4*count, sizeof(float));
    cap = count;
  } else if (cap<count) {
    vector_buffer = realloc(vector_buffer, 3*count*sizeof(float));
  }
  
  for (uint i=0; i<count; i++) {
    vector_buffer[3*i]   = (float)bodies[i].pos.x/max_pt;
    vector_buffer[3*i+1] = (float)bodies[i].pos.y/max_pt;
    vector_buffer[3*i+2] = (float)bodies[i].pos.z/max_pt;
  }
  
  return vector_buffer;
}

void draw_bodies (SDL2Context c, Body *bodies, uint count) {
  check_gl_error();
  
  
  glBufferData(GL_ARRAY_BUFFER, count*3*sizeof(float), serialize_positions(bodies, count), GL_STREAM_DRAW);check_gl_error();
  
  glDrawArrays(GL_POINTS, 0, count*3);check_gl_error();

}



void _check_gl_error(const char *file, int line) {
  GLenum err = glGetError();
  
  while(err!=GL_NO_ERROR) {
    char *error;
    
    switch(err) {
      case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
      case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
      case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
      case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
      default: error="Unknown error!"; break;
    }

    printf("GL_%s - %s:%d\n", error, file, line);
    err=glGetError();
  }
}



