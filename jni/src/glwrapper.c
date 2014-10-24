#include "glwrapper.h"
#include "image.h"
#include "log.h"


#include <stdlib.h>


GLuint _checkGLError(const char * file, int line)
{
    GLuint err = glGetError();
    if (err > 0 )  {
        Log("GL ERROR: file:%s line:%d error:%d", file, line, err);
    }
    return err;
}


void log_matrix(const float * m, int n, int column)
{
    const int size = 2048;
    int i = 0, k = 0;
    char buff[size];

    for(i=0; i < n; ++i)
    {
        if (i % column == 0)
        {
            buff[k++] = '\n';
        }

        k += snprintf(buff + k, size-k, "%.02f", m[i]);
        buff[k++] = ',';
        buff[k++] = ' ';
    }

    buff[k] = 0;
    Log("matrix:%s", buff);
}


static GLuint compile_shader(const char * source, int type)
{
    GLint status;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        char buf[1024];
        GLint len;
        glGetShaderInfoLog(shader, 1024, &len, buf);

        SDL_Log("compile failed:%s\nsource:\n %s\n", buf, source);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static void link_shader(GLuint prog)
{
    GLint status;
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        SDL_Log("Can't link program");
    }
}

GLuint load_shader(const char *FS, const char *VS)
{
    // Create shader program.
    GLuint shader = glCreateProgram();

    // fragment shader
    GLuint fs = compile_shader(FS, GL_FRAGMENT_SHADER);
    if (fs == 0) {
        Log("Can't compile fragment shader");
    } else {
        glAttachShader(shader, fs);
    }

    // vertex shader
    GLuint vs = compile_shader(VS, GL_VERTEX_SHADER);
    if (vs == 0) {
        Log("Can't compile vertex shader");
    } else {
        glAttachShader(shader, vs);
    }

    link_shader(shader);

    glDetachShader(shader, fs);
    glDeleteShader(fs);

    glDetachShader(shader, vs);
    glDeleteShader(vs);

    return shader;
}




GLuint load_texture(const GLsizei width, const GLsizei height, const GLenum type, const GLvoid* pixels)
{
    GLuint texture_object_id;
    glGenTextures(1, &texture_object_id);
    glBindTexture(GL_TEXTURE_2D, texture_object_id);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);

    //glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture_object_id;
}


GLuint load_texture_from_png(const char * png_data, int data_size)
{
    RawImageData * rawdata = load_raw_image_data_from_png(png_data, data_size);
    GLuint texture_id = load_texture(rawdata->width, rawdata->height, rawdata->format, rawdata->data);
    release_raw_image_data(rawdata);
    return texture_id;
}

