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
#include "mat4.h"
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

  printf("Attempting to load shader at location: %s\n", path);

  *shader = glCreateShader(shader_type);
  puts("Created shader object");
  char *shader_path = path;
  FILE *sh = fopen(shader_path, "r");
  if (sh == NULL) {
    goto err;
  }
  size_t size = fsize(path);
  rewind(sh);
  char *shader_buf = NULL;
  if (size > 0) {
    shader_buf = malloc(size+1);
  } else {
    goto err;
  }
  fread(shader_buf, size, 1, sh);
  shader_buf[size] = '\0';
  fclose(sh);

  
  glShaderSource(*shader, 1, (const char* const*)&shader_buf, NULL);
  free(shader_buf);
  check_gl_error();

  glCompileShader(*shader);
  check_gl_error();

  GLint status;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
  check_gl_error();
  if (status == GL_FALSE) {
    ret++;
  }

  char log[512];
  glGetShaderInfoLog(*shader, 512, NULL, log);
  check_gl_error();

  if (strlen(log) == 0) strcpy(log, "No errors. Compilation successful.\n");
  
  printf("Compiling shader %s\n%s\n", path, log);
  

  return ret;

err:
  printf("Couldn't load file or compile shader!\n");
  return -1;
}

int init_shaders(SDL2Context *con) {
#ifdef __linux__
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    perror("Could not initialize GLEW! Quitting");
    exit(-1);
  }
#endif

  printf("Using OpenGL version %s\n", glGetString(GL_VERSION));

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

  char log[512] = {0};
  glGetProgramInfoLog(con->shader_prog, 512, NULL, log);

  if (strlen(log) == 0) strcpy(log, "No errors. Shader linking successful.\n");
  printf("Linking shader program\n%s\n", log);
  
  
  glUseProgram(con->shader_prog);
  check_gl_error();

  GLint posAttrib = glGetAttribLocation(con->shader_prog, "position");

  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);
  check_gl_error();

  con->MVP_uni = glGetUniformLocation(con->shader_prog, "MVPmatrix");
  
  return -1;

}



float randf() {
  return ((float)rand())/((float)RAND_MAX);
}


void draw_bodies (SDL2Context c, CameraState *cam, Body *bodies, uint count);


void event_handler(System *s, CameraState *cam, SDL2Context c) {
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
              print_tree(&s->tree, f);
              fclose(f);
              puts("Writing tree dump");
              SDL_Delay(1000);
              break;
            }

            case SDLK_e:
              printf("Total system kinetic energy: %f\n", system_total_e(s));
              break;

            case SDLK_TAB:
              s = random_disk(1e6, 100);
              s->tree = build_tree(s->bodies, s->count);
              break;

            case SDLK_BACKSPACE:
              prune_tree(&s->tree);
              break;
            
            case SDLK_UP:
              cam->altitude += CONTROL_SENSITIVITY;
              if (cam->altitude > M_PI_2) cam->altitude = M_PI_2;
              break;
            
            case SDLK_DOWN:
              cam->altitude -= CONTROL_SENSITIVITY;
              if (cam->altitude < -M_PI_2) cam->altitude = -M_PI_2;
              break;
            
            case SDLK_RIGHT:
              cam->azimuth += CONTROL_SENSITIVITY;
              cam->azimuth = fmod(cam->azimuth, 2*M_PI);
              break;
            
            case SDLK_LEFT:
              cam->azimuth -= CONTROL_SENSITIVITY;
              cam->azimuth = fmod(cam->azimuth, 2*M_PI);
              break;
          }
        }

        default:
          break;
      }
    }
}

void run_simulation() {
  srand((int)time(NULL));
  SDL2Context c = new_SDL2Context(SDL_INIT_VIDEO);
  System *s = random_disk(1e4, 1000);
  //System s = load_tispy("/Users/Thomas/Downloads/IsolatedCollapse.000000");
  s->tree = build_tree(s->bodies, s->count);

  
  init_shaders(&c);
  
  CameraState cam;
  cam.aspectRatio = c.screensize.x/c.screensize.y;
  
  time_t begin, end;
  

  while(1) {
    begin = clock();
    update_system(s);
    //float r = randf()/2, g = randf()/2, b = randf()/2;
    float r=0,g=0,b=0;
    glClearColor(r, g, b, 0);
    
    glClear(GL_COLOR_BUFFER_BIT);
    draw_bodies(c, &cam, s->bodies, s->count);
    
    
    SDL_GL_SwapWindow(c.win);
    //SDL_Delay(5);
    
    total_force_calcs = 0;
    
    event_handler(s, &cam, c);
    end = clock();
    //printf("Time for update: %ld ms\n", (end-begin)*1000/CLOCKS_PER_SEC);
    printf("%f, %f, %f\n", vec3_to_triple(node_cmass(&s->tree)));
    
    
    //print_tree(&s.tree);
  }
}


float *serialize_positions(Body *bodies, uint count) {
  static float *vector_buffer = NULL;
  static uint cap = 0;

  static double max_pt = 0;
  if (max_pt==0) {max_pt = 2*max_point(bodies, count);};

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

mat4 create_mvp_matrix(CameraState cam, double max_dist);

void draw_bodies (SDL2Context c, CameraState *cam, Body *bodies, uint count) {
  check_gl_error();

  mat4 mvp = create_mvp_matrix(*cam, 1/*max_point(bodies, count)*/);
  //print_mat4(mvp);
  glUniformMatrix4fv(c.MVP_uni, 1, GL_FALSE, (float*)&mvp);

  glBufferData(GL_ARRAY_BUFFER, count*3*sizeof(float), serialize_positions(bodies, count), GL_STREAM_DRAW);check_gl_error();

  glDrawArrays(GL_POINTS, 0, count*3);check_gl_error();

}



void _check_gl_error(const char *file, int line) {
  GLenum err = glGetError();
  bool isError = false;

  while(err!=GL_NO_ERROR) {
    char *error;
    isError = true;

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
  if (isError) exit(-1);
}

mat4 create_mvp_matrix(CameraState cam, double max_dist) {

  
  mat4 model = rotation_matrix(cam.altitude, 0,cam.azimuth);
  mat4 view = translation_matrix(vec3_to_triple(-cam.camera_pos));
  mat4 pres = scale_matrix(cam.aspectRatio, 1, 1);
  
  mat4 cumulative = mat4_mult(pres, mat4_mult(view, model));
  
  return cumulative;
}

