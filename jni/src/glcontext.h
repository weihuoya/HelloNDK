#ifndef __hellosdl__glcontext__
#define __hellosdl__glcontext__

#include "glwrapper.h"
#include "glcube.h"
#include "gltransform.h"


class GLContext
{
public:
    ~GLContext();

    static GLContext * instance();

    void surfaceCreated();
    void surfaceChanged(int width, int height);
    void drawFrame(long long delta);

    void loadMatrix();
    void loadShader();
    void loadTexture();

    void incRotate(float rotateX, float rotateY, float rotateZ);
    void incTranslate(float x, float y);
    void incScale(float scale);

private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(GLContext);

    GLCube * cube_;
    GLTransform * transform_;

    GLuint shaderProgram_;
    GLuint textureBuffer_;
};


#endif /* defined(__HelloSDL__glcontext__) */
