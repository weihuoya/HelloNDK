#include "glcube.h"


#define TEX_COORD_MAX   4.0f


typedef struct Vertex {
    float position[3];
    float color[4];
    float texcoord[2];
} Vertex;



typedef struct GLCube {
    GLuint * vboids;
    GLsizei count;
}GLCube;


static GLCube cube = {.vboids = NULL, count= 0};


void glcube_initialize()
{
    glGenBuffers(2, cube.vboids);
}


void glcube_load_cube1()
{
}


void glcube_load_cube2()
{
}


void glcube_draw()
{
}

