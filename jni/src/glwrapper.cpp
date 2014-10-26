#include "glwrapper.h"
#include "image.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <android/log.h>


#define MAX_LOG_MESSAGE 2048


static const char * category_prefixes[] = {
    "APP",
    "ERROR",
    "SYSTEM",
    "AUDIO",
    "VIDEO",
    "RENDER",
    "INPUT"
};


static int android_priority[] = {
    ANDROID_LOG_UNKNOWN,
    ANDROID_LOG_VERBOSE,
    ANDROID_LOG_DEBUG,
    ANDROID_LOG_INFO,
    ANDROID_LOG_WARN,
    ANDROID_LOG_ERROR,
    ANDROID_LOG_FATAL
};


void logWrite(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    char tag[32];
    char * message = (char*) malloc(MAX_LOG_MESSAGE);

    snprintf(tag, arraysize(tag), "NDK/%s", category_prefixes[0]);
    vsnprintf(message, MAX_LOG_MESSAGE, fmt, ap);

    __android_log_write(android_priority[3], tag, message);

    va_end(ap);
    free(message);
}



GLuint _checkGLError(const char * file, int line)
{
    GLuint err = glGetError();
    if (err > 0 )  {
        logWrite("GL ERROR: file:%s line:%d error:%d", file, line, err);
    }
    return err;
}


void logMatrix(const float * m, int n, int column)
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
    logWrite("matrix:%s", buff);
}


static GLuint compileShader(const char * source, int type)
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

        logWrite("compile failed:%s\nsource:\n %s\n", buf, source);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static void linkShader(GLuint prog)
{
    GLint status;
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        logWrite("Can't link program");
    }
}

GLuint loadShader(const char *FS, const char *VS)
{
    // Create shader program.
    GLuint shader = glCreateProgram();

    // fragment shader
    GLuint fs = compileShader(FS, GL_FRAGMENT_SHADER);
    if (fs == 0) {
        logWrite("Can't compile fragment shader");
    } else {
        glAttachShader(shader, fs);
    }

    // vertex shader
    GLuint vs = compileShader(VS, GL_VERTEX_SHADER);
    if (vs == 0) {
        logWrite("Can't compile vertex shader");
    } else {
        glAttachShader(shader, vs);
    }

    linkShader(shader);

    glDetachShader(shader, fs);
    glDeleteShader(fs);

    glDetachShader(shader, vs);
    glDeleteShader(vs);

    return shader;
}




GLuint loadTexture(const GLsizei width, const GLsizei height, const GLenum type, const GLvoid* pixels)
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


GLuint loadTextureFromPng(const char * png_data, int data_size)
{
    RawImageData * rawdata = load_raw_image_data_from_png(png_data, data_size);
    GLuint texture_id = loadTexture(rawdata->width, rawdata->height, rawdata->format, rawdata->data);
    release_raw_image_data(rawdata);
    return texture_id;
}

