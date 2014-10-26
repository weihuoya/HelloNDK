#include "glcontext.h"
#include "matrix.h"
#include "asset.h"


// model
static float modelMatrix[16];
// view
static float viewMatrix[16];
// projection
static float projectionMatrix[16];
// projection * view * model
static float mvpMatrix[16];



GLContext::GLContext() :
    cube_(0), transform_(0),
    shaderProgram_(0), textureBuffer_(0)
{
}

GLContext::~GLContext()
{
    if(shaderProgram_) glDeleteProgram(shaderProgram_);
    if(textureBuffer_) glDeleteTextures(1, &textureBuffer_);

    if(cube_) delete cube_;
    if(transform_) delete transform_;
}

GLContext * GLContext::instance()
{
    static GLContext context;
    return &context;
}


void GLContext::surfaceCreated()
{
    cube_ = new GLCube();
    transform_ = new GLTransform();

    glEnable(GL_BLEND);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


    // load
    this->loadShader();
    this->loadTexture();

    cube_->init();
    cube_->load1();

}


void GLContext::surfaceChanged(int width, int height)
{
    // projection
    float aspect = (float)width / (float)height;
    Matrix::setIdentityM(projectionMatrix);
    //Matrix::perspectiveM(projectionMatrix, 45.0f, aspect, 3.0f, 7.0f);
    Matrix::frustumM(projectionMatrix, -1, 1, -1, 1, 2, 10);

    // view
    //Matrix::setIdentityM(viewMatrix);
    Matrix::setLookAtM(viewMatrix, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}


void GLContext::drawFrame(long long delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    loadMatrix();

    cube_->draw(shaderProgram_);
}


void GLContext::loadShader()
{
    int size = 0;
    char *fshader, *vshader;

    // fragment shader
    Asset::instance()->getBuffer("fragment.glsl", (void**)&fshader, &size);

    // vertex shader
    Asset::instance()->getBuffer("vertex.glsl", (void**)&vshader, &size);

    shaderProgram_ = ::loadShader(fshader, vshader);
    glUseProgram(shaderProgram_);

    delete [] fshader;
    delete [] vshader;
}


void GLContext::loadTexture()
{
    //glEnable(GL_TEXTURE_2D);

    int size = 0;
    char * data = NULL;
    Asset::instance()->getBuffer("floor.png", (void**)&data, &size);

    GLuint tid = ::loadTextureFromPng(data, size);
    GLint textureSlot = glGetUniformLocation(shaderProgram_, "u_texture");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tid);
    glUniform1i(textureSlot, 0);

    textureBuffer_ = tid;

    // Set nearest filtering mode for texture minification
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    delete [] data;
}


void GLContext::loadMatrix()
{
    // model
    //Matrix::setIdentityM(modelMatrix);
    //Matrix::translateM(modelMatrix, 0.0f, 0.0f, -4.0f);

    transform_->getModelViewMatrix(modelMatrix);

    // projection * view * model
    //Matrix::multiplyMM(temporaryMatrix_, viewMatrix, modelMatrix);
    Matrix::multiplyMM(mvpMatrix, projectionMatrix, modelMatrix);

    //Matrix::setIdentityM(mvpMatrix);

    // mvp
    GLint mvpSlot = glGetUniformLocation(shaderProgram_, "u_mvpmatrix");
    glUniformMatrix4fv(mvpSlot, 1, 0, mvpMatrix);
}


void GLContext::incRotate(float rotateX, float rotateY, float rotateZ)
{
    logWrite("rotate: (%f, %f)", rotateX, rotateY, rotateZ);
}

void GLContext::incScale(float scale)
{
    logWrite("scale: %f", scale);
}

void GLContext::incTranslate(float x, float y)
{
    logWrite("translate: (%f, %f)", x, y);
}
