#ifndef common_h
#define common_h

#ifdef WIN32
#include <windows.h>
#endif

#include <cmath>

#ifdef WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment (lib,"SDL")
#pragma comment (lib,"SDLmain")
#pragma comment (lib,"SDL_image")
#pragma comment (lib,"SDL_ttf")
#endif


#include <GL/glu.h>
#include <GL/gl.h>

#include "SDL/SDL.h"

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include "play.h"
#include "ball.h"

// global values
SDL_Surface* surface;
SDL_Event event;
Uint8* keys;


const int width = 1024;
const int height = 768;
int pal = 32;

int fullscreen = 0;

int done = 0;

// textures
GLuint back, sprite;




#endif
