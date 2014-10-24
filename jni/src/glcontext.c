#include "glcontext.h"
#include "glwrapper.h"
#include "asset.h"
#include "log.h"


typedef struct GLContext {
    GLuint shader;
    GLuint texture;
} GLContext;



static GLContext context = {.shader= 0, texture= 0};



void glcontext_initialize()
{
    glEnable(GL_BLEND);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}


void glcontext_load_shader()
{
    int size = 0;
    char * FS = NULL, * VS = NULL;

    load_asset("fragment.glsl", &FS, &size);
    load_asset("vertex.glsl", &VS, &size);

    context.shader = load_shader(FS, VS);
    glUseProgram(context.shader);

    free(FS);
    free(VS);
}


void glcontext_load_texture()
{
    int size = 0;
    char * data = NULL;

    load_asset("floor.png", &data, &size);
    context.texture = load_texture_from_png(data, size);


    GLint textureSlot = glGetUniformLocation(shader, "u_texture");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, context.texture);
    glUniform1i(textureSlot, 0);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void glcontext_surface_created()
{
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
}


void glcontext_surface_changed(int width, int height)
{
    Log("[surface] size: %d, %d", width, height);
}


void glcontext_draw_frame()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
