#ifndef GLWRAPPER_H_
#define GLWRAPPER_H_

#include <GLES2/gl2.h>
#include <math.h>

//#ifdef __cplusplus
//extern "C" {
//#endif

#ifndef M_PI
# define M_PI           3.14159265358979323846  /* pi */
#endif

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

// The arraysize(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.  If you use arraysize on
// a pointer by mistake, you will get a compile-time error.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

// That gcc wants both of these prototypes seems mysterious. VC, for
// its part, can't decide which to use (another mystery). Matching of
// template overloads: the final frontier.
#ifndef COMPILER_MSVC
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// ARRAYSIZE performs essentially the same calculation as arraysize,
// but can be used on anonymous types or types defined inside
// functions.  It's less safe than arraysize as it accepts some
// (although not all) pointers.  Therefore, you should use arraysize
// whenever possible.
// Starting with Visual C++ 2005, WinNT.h includes ARRAYSIZE.
#if !defined(COMPILER_MSVC) || (defined(_MSC_VER) && _MSC_VER < 1400)
#define ARRAYSIZE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
#endif

void logWrite(const char *fmt, ...);

#define  CHECK_GL(...)   _checkGLError(__FILE__, __LINE__)
GLuint _checkGLError(const char * file, int line);

void logMatrix(const float * m, int n, int column);

GLuint loadShader(const char *FS, const char *VS);

GLuint loadTextureFromPng(const char * png_data, int data_size);

#define D2R(d) (d * M_PI / 180.0)
#define R2D(r) (r * 180.0 / M_PI)

static __inline__ float Degrees2Radians(float degrees) { return degrees * M_PI / 180.0; };
static __inline__ float Radians2Degrees(float radians) { return radians * 180.0 / M_PI; };

//#ifdef __cplusplus
//}
//#endif

#endif /* GLWRAPPER_H_ */
