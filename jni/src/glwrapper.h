#ifndef GLWRAPPER_H_
#define GLWRAPPER_H_
#include <GLES2/gl2.h>

#ifndef M_PI
# define M_PI           3.14159265358979323846  /* pi */
#endif

#define ARRAYSIZE(a) ( (sizeof(a) / sizeof(*(a)) )

#define  CHECK_GL(...)   _checkGLError(__FILE__, __LINE__)
GLuint _checkGLError(const char * file, int line);

void log_matrix(const float * m, int n, int column);

GLuint load_shader(const char *FS, const char *VS);

GLuint load_texture_from_png(const char * png_data, int data_size);

#define D2R(d) (d * M_PI / 180.0)
#define R2D(r) (r * 180.0 / M_PI)

static __inline__ float Degrees2Radians(float degrees) { return degrees * M_PI / 180.0; };
static __inline__ float Radians2Degrees(float radians) { return radians * 180.0 / M_PI; };


#endif /* GLWRAPPER_H_ */
