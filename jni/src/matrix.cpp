#include "matrix.h"
#include <cstring>


static float sTemp[32];

/**
 * Multiplies two 4x4 matrices together and stores the result in a third 4x4
 * matrix. In matrix notation: result = lhs x rhs. Due to the way
 * matrix multiplication works, the result matrix will have the same
 * effect as first multiplying by the rhs matrix, then multiplying by
 * the lhs matrix. This is the opposite of what you might expect.
 * <p>
 * The same float array may be passed for result, lhs, and/or rhs. However,
 * the result element values are undefined if the result elements overlap
 * either the lhs or rhs elements.
 *
 * @param result The float array that holds the result.
 * @param lhs The float array that holds the left-hand-side matrix.
 * @param rhs The float array that holds the right-hand-side matrix.
 *
 * @throws IllegalArgumentException if result, lhs, or rhs are null, or if
 * resultOffset + 16 > result.length or lhsOffset + 16 > lhs.length or
 * rhsOffset + 16 > rhs.length.
 */
#define I(_i, _j) ((_j)+ 4*(_i))
void Matrix::multiplyMM(float result[], const float lhs[], const float rhs[])
{
    for (int i=0 ; i<4 ; i++)
    {
        register const float rhs_i0 = rhs[ I(i,0) ];
        register float ri0 = lhs[ I(0,0) ] * rhs_i0;
        register float ri1 = lhs[ I(0,1) ] * rhs_i0;
        register float ri2 = lhs[ I(0,2) ] * rhs_i0;
        register float ri3 = lhs[ I(0,3) ] * rhs_i0;
        for (int j=1 ; j<4 ; j++) {
            register const float rhs_ij = rhs[ I(i,j) ];
            ri0 += lhs[ I(j,0) ] * rhs_ij;
            ri1 += lhs[ I(j,1) ] * rhs_ij;
            ri2 += lhs[ I(j,2) ] * rhs_ij;
            ri3 += lhs[ I(j,3) ] * rhs_ij;
        }
        result[ I(i,0) ] = ri0;
        result[ I(i,1) ] = ri1;
        result[ I(i,2) ] = ri2;
        result[ I(i,3) ] = ri3;
    }
}

/**
 * Multiplies a 4 element vector by a 4x4 matrix and stores the result in a
 * 4-element column vector. In matrix notation: result = lhs x rhs
 * <p>
 * The same float array may be passed for resultVec, lhsMat, and/or rhsVec.
 * However, the resultVec element values are undefined if the resultVec
 * elements overlap either the lhsMat or rhsVec elements.
 *
 * @param resultVec The float array that holds the result vector.
 * @param lhsMat The float array that holds the left-hand-side matrix.
 * @param rhsVec The float array that holds the right-hand-side vector.
 *
 * @throws IllegalArgumentException if resultVec, lhsMat,
 * or rhsVec are null, or if resultVecOffset + 4 > resultVec.length
 * or lhsMatOffset + 16 > lhsMat.length or
 * rhsVecOffset + 4 > rhsVec.length.
 */
static inline
void mx4transform(float x, float y, float z, float w, const float* pM, float* pDest)
{
    pDest[0] = pM[0 + 4 * 0] * x + pM[0 + 4 * 1] * y + pM[0 + 4 * 2] * z + pM[0 + 4 * 3] * w;
    pDest[1] = pM[1 + 4 * 0] * x + pM[1 + 4 * 1] * y + pM[1 + 4 * 2] * z + pM[1 + 4 * 3] * w;
    pDest[2] = pM[2 + 4 * 0] * x + pM[2 + 4 * 1] * y + pM[2 + 4 * 2] * z + pM[2 + 4 * 3] * w;
    pDest[3] = pM[3 + 4 * 0] * x + pM[3 + 4 * 1] * y + pM[3 + 4 * 2] * z + pM[3 + 4 * 3] * w;
}

void Matrix::multiplyMV(float resultVec[], const float lhsMat[], const float rhsVec[])
{
    mx4transform(rhsVec[0], rhsVec[1], rhsVec[2], rhsVec[3], lhsMat, resultVec);
}

/**
 * Transposes a 4 x 4 matrix.
 * <p>
 * mTrans and m must not overlap.
 *
 * @param mTrans the array that holds the output transposed matrix
 * @param m the input array
 */
void Matrix::transposeM(float mTrans[], const float m[])
{
    for (int i = 0; i < 4; i++) {
        int mBase = i * 4;
        mTrans[i +  0] = m[mBase + 0];
        mTrans[i +  4] = m[mBase + 1];
        mTrans[i +  8] = m[mBase + 2];
        mTrans[i + 12] = m[mBase + 3];
    }
}


/**
 * Inverts a 4 x 4 matrix.
 * <p>
 * mInv and m must not overlap.
 *
 * @param mInv the array that holds the output inverted matrix
 * @param m the input array
 * @return true if the matrix could be inverted, false if it could not.
 */
bool Matrix::invertM(float mInv[], const float m[])
{
    // Invert a 4 x 4 matrix using Cramer's Rule

    // transpose matrix
    const float src0  = m[ 0];
    const float src4  = m[ 1];
    const float src8  = m[ 2];
    const float src12 = m[ 3];

    const float src1  = m[ 4];
    const float src5  = m[ 5];
    const float src9  = m[ 6];
    const float src13 = m[ 7];

    const float src2  = m[ 8];
    const float src6  = m[ 9];
    const float src10 = m[10];
    const float src14 = m[11];

    const float src3  = m[12];
    const float src7  = m[13];
    const float src11 = m[14];
    const float src15 = m[15];

    // calculate pairs for first 8 elements (cofactors)
    const float atmp0  = src10 * src15;
    const float atmp1  = src11 * src14;
    const float atmp2  = src9  * src15;
    const float atmp3  = src11 * src13;
    const float atmp4  = src9  * src14;
    const float atmp5  = src10 * src13;
    const float atmp6  = src8  * src15;
    const float atmp7  = src11 * src12;
    const float atmp8  = src8  * src14;
    const float atmp9  = src10 * src12;
    const float atmp10 = src8  * src13;
    const float atmp11 = src9  * src12;

    // calculate first 8 elements (cofactors)
    const float dst0  = (atmp0 * src5 + atmp3 * src6 + atmp4  * src7)
        - (atmp1 * src5 + atmp2 * src6 + atmp5  * src7);
    const float dst1  = (atmp1 * src4 + atmp6 * src6 + atmp9  * src7)
        - (atmp0 * src4 + atmp7 * src6 + atmp8  * src7);
    const float dst2  = (atmp2 * src4 + atmp7 * src5 + atmp10 * src7)
        - (atmp3 * src4 + atmp6 * src5 + atmp11 * src7);
    const float dst3  = (atmp5 * src4 + atmp8 * src5 + atmp11 * src6)
        - (atmp4 * src4 + atmp9 * src5 + atmp10 * src6);
    const float dst4  = (atmp1 * src1 + atmp2 * src2 + atmp5  * src3)
        - (atmp0 * src1 + atmp3 * src2 + atmp4  * src3);
    const float dst5  = (atmp0 * src0 + atmp7 * src2 + atmp8  * src3)
        - (atmp1 * src0 + atmp6 * src2 + atmp9  * src3);
    const float dst6  = (atmp3 * src0 + atmp6 * src1 + atmp11 * src3)
        - (atmp2 * src0 + atmp7 * src1 + atmp10 * src3);
    const float dst7  = (atmp4 * src0 + atmp9 * src1 + atmp10 * src2)
        - (atmp5 * src0 + atmp8 * src1 + atmp11 * src2);

    // calculate pairs for second 8 elements (cofactors)
    const float btmp0  = src2 * src7;
    const float btmp1  = src3 * src6;
    const float btmp2  = src1 * src7;
    const float btmp3  = src3 * src5;
    const float btmp4  = src1 * src6;
    const float btmp5  = src2 * src5;
    const float btmp6  = src0 * src7;
    const float btmp7  = src3 * src4;
    const float btmp8  = src0 * src6;
    const float btmp9  = src2 * src4;
    const float btmp10 = src0 * src5;
    const float btmp11 = src1 * src4;

    // calculate second 8 elements (cofactors)
    const float dst8  = (btmp0  * src13 + btmp3  * src14 + btmp4  * src15)
        - (btmp1  * src13 + btmp2  * src14 + btmp5  * src15);
    const float dst9  = (btmp1  * src12 + btmp6  * src14 + btmp9  * src15)
        - (btmp0  * src12 + btmp7  * src14 + btmp8  * src15);
    const float dst10 = (btmp2  * src12 + btmp7  * src13 + btmp10 * src15)
        - (btmp3  * src12 + btmp6  * src13 + btmp11 * src15);
    const float dst11 = (btmp5  * src12 + btmp8  * src13 + btmp11 * src14)
        - (btmp4  * src12 + btmp9  * src13 + btmp10 * src14);
    const float dst12 = (btmp2  * src10 + btmp5  * src11 + btmp1  * src9 )
        - (btmp4  * src11 + btmp0  * src9  + btmp3  * src10);
    const float dst13 = (btmp8  * src11 + btmp0  * src8  + btmp7  * src10)
        - (btmp6  * src10 + btmp9  * src11 + btmp1  * src8 );
    const float dst14 = (btmp6  * src9  + btmp11 * src11 + btmp3  * src8 )
        - (btmp10 * src11 + btmp2  * src8  + btmp7  * src9 );
    const float dst15 = (btmp10 * src10 + btmp4  * src8  + btmp9  * src9 )
        - (btmp8  * src9  + btmp11 * src10 + btmp5  * src8 );

    // calculate determinant
    const float det =
        src0 * dst0 + src1 * dst1 + src2 * dst2 + src3 * dst3;

    if (det == 0.0f) {
        return false;
    }

    // calculate matrix inverse
    const float invdet = 1.0f / det;
    mInv[ 0] = dst0  * invdet;
    mInv[ 1] = dst1  * invdet;
    mInv[ 2] = dst2  * invdet;
    mInv[ 3] = dst3  * invdet;

    mInv[ 4] = dst4  * invdet;
    mInv[ 5] = dst5  * invdet;
    mInv[ 6] = dst6  * invdet;
    mInv[ 7] = dst7  * invdet;

    mInv[ 8] = dst8  * invdet;
    mInv[ 9] = dst9  * invdet;
    mInv[10] = dst10 * invdet;
    mInv[11] = dst11 * invdet;

    mInv[12] = dst12 * invdet;
    mInv[13] = dst13 * invdet;
    mInv[14] = dst14 * invdet;
    mInv[15] = dst15 * invdet;

    return true;
}


/**
 * Computes an orthographic projection matrix.
 *
 * @param m returns the result
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 */
void Matrix::orthoM(float m[], float left, float right, float bottom, float top, float near, float far)
{
    if (left == right) {
        //throw new IllegalArgumentException("left == right");
        return;
    }
    if (bottom == top) {
        //throw new IllegalArgumentException("bottom == top");
        return;
    }
    if (near == far) {
        //throw new IllegalArgumentException("near == far");
        return;
    }

    const float r_width  = 1.0f / (right - left);
    const float r_height = 1.0f / (top - bottom);
    const float r_depth  = 1.0f / (far - near);
    const float x =  2.0f * (r_width);
    const float y =  2.0f * (r_height);
    const float z = -2.0f * (r_depth);
    const float tx = -(right + left) * r_width;
    const float ty = -(top + bottom) * r_height;
    const float tz = -(far + near) * r_depth;
    m[ 0] = x;
    m[ 5] = y;
    m[10] = z;
    m[12] = tx;
    m[13] = ty;
    m[14] = tz;
    m[15] = 1.0f;
    m[ 1] = 0.0f;
    m[ 2] = 0.0f;
    m[ 3] = 0.0f;
    m[ 4] = 0.0f;
    m[ 6] = 0.0f;
    m[ 7] = 0.0f;
    m[ 8] = 0.0f;
    m[ 9] = 0.0f;
    m[11] = 0.0f;
}


/**
 * Defines a projection matrix in terms of six clip planes.
 *
 * @param m the float array that holds the output perspective matrix
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 */
void Matrix::frustumM(float m[], float left, float right, float bottom, float top, float near, float far)
{
    if (left == right) {
        //throw new IllegalArgumentException("left == right");
        return;
    }
    if (top == bottom) {
        //throw new IllegalArgumentException("top == bottom");
        return;
    }
    if (near == far) {
        //throw new IllegalArgumentException("near == far");
        return;
    }
    if (near <= 0.0f) {
        //throw new IllegalArgumentException("near <= 0.0f");
        return;
    }
    if (far <= 0.0f) {
        //throw new IllegalArgumentException("far <= 0.0f");
        return;
    }
    const float r_width  = 1.0f / (right - left);
    const float r_height = 1.0f / (top - bottom);
    const float r_depth  = 1.0f / (near - far);
    const float x = 2.0f * (near * r_width);
    const float y = 2.0f * (near * r_height);
    const float A = (right + left) * r_width;
    const float B = (top + bottom) * r_height;
    const float C = (far + near) * r_depth;
    const float D = 2.0f * (far * near * r_depth);
    m[ 0] = x;
    m[ 5] = y;
    m[ 8] = A;
    m[ 9] = B;
    m[10] = C;
    m[14] = D;
    m[11] = -1.0f;
    m[ 1] = 0.0f;
    m[ 2] = 0.0f;
    m[ 3] = 0.0f;
    m[ 4] = 0.0f;
    m[ 6] = 0.0f;
    m[ 7] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[15] = 0.0f;
}

/**
 * Defines a projection matrix in terms of a field of view angle, an
 * aspect ratio, and z clip planes.
 *
 * @param m the float array that holds the perspective matrix
 * @param fovy field of view in y direction, in degrees
 * @param aspect width to height aspect ratio of the viewport
 * @param zNear
 * @param zFar
 */
void Matrix::perspectiveM(float m[], float fovy, float aspect, float zNear, float zFar)
{
    float f = 1.0f / (float) tan(fovy * (M_PI / 360.0));
    float rangeReciprocal = 1.0f / (zNear - zFar);

    m[ 0] = f / aspect;
    m[ 1] = 0.0f;
    m[ 2] = 0.0f;
    m[ 3] = 0.0f;

    m[ 4] = 0.0f;
    m[ 5] = f;
    m[ 6] = 0.0f;
    m[ 7] = 0.0f;

    m[ 8] = 0.0f;
    m[ 9] = 0.0f;
    m[10] = (zFar + zNear) * rangeReciprocal;
    m[11] = -1.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 2.0f * zFar * zNear * rangeReciprocal;
    m[15] = 0.0f;
}

/**
 * Computes the length of a vector.
 *
 * @param x x coordinate of a vector
 * @param y y coordinate of a vector
 * @param z z coordinate of a vector
 * @return the length of a vector
 */
float Matrix::length(float x, float y, float z)
{
    return (float) sqrt(x * x + y * y + z * z);
}

/**
 * Sets matrix m to the identity matrix.
 *
 * @param sm returns the result
 */
void Matrix::setIdentityM(float sm[])
{
    int i;

    for (i=0 ; i<16 ; i++) {
        sm[i] = 0;
    }

    for(i = 0; i < 16; i += 5) {
        sm[i] = 1.0f;
    }
}

/**
 * Scales matrix m by x, y, and z, putting the result in sm.
 * <p>
 * m and sm must not overlap.
 *
 * @param sm returns the result
 * @param m source matrix
 * @param x scale factor x
 * @param y scale factor y
 * @param z scale factor z
 */
void Matrix::scaleM(float sm[], const float m[], float x, float y, float z)
{
    for (int i=0 ; i<4 ; i++) {
        sm[     i] = m[     i] * x;
        sm[ 4 + i] = m[ 4 + i] * y;
        sm[ 8 + i] = m[ 8 + i] * z;
        sm[12 + i] = m[12 + i];
    }
}

/**
 * Scales matrix m in place by sx, sy, and sz.
 *
 * @param m matrix to scale
 * @param x scale factor x
 * @param y scale factor y
 * @param z scale factor z
 */
void Matrix::scaleM(float m[], float x, float y, float z)
{
    for (int i=0 ; i<4 ; i++) {
        m[     i] *= x;
        m[ 4 + i] *= y;
        m[ 8 + i] *= z;
    }
}

/**
 * Translates matrix m by x, y, and z, putting the result in tm.
 * <p>
 * m and tm must not overlap.
 *
 * @param tm returns the result
 * @param m source matrix
 * @param x translation factor x
 * @param y translation factor y
 * @param z translation factor z
 */
void Matrix::translateM(float tm[], const float m[], float x, float y, float z)
{
    for (int i=0 ; i<12 ; i++) {
        tm[i] = m[i];
    }

    for (int i=0 ; i<4 ; i++) {
        tm[12 + i] = m[i] * x + m[4 + i] * y + m[8 + i] * z + m[12 + i];
    }
}

/**
 * Translates matrix m by x, y, and z in place.
 *
 * @param m matrix
 * @param x translation factor x
 * @param y translation factor y
 * @param z translation factor z
 */
void Matrix::translateM(float m[], float x, float y, float z)
{
    for (int i=0 ; i<4 ; i++) {
        m[12 + i] += m[i] * x + m[4 + i] * y + m[8 + i] * z;
    }
}

/**
 * Rotates matrix m by angle a (in degrees) around the axis (x, y, z).
 * <p>
 * m and rm must not overlap.
 *
 * @param rm returns the result
 * @param m source matrix
 * @param a angle to rotate in degrees
 * @param x X axis component
 * @param y Y axis component
 * @param z Z axis component
 */
void Matrix::rotateM(float rm[], const float m[], float a, float x, float y, float z)
{
    Matrix::setRotateM(sTemp, a, x, y, z);
    Matrix::multiplyMM(rm, m, sTemp);
}

/**
 * Rotates matrix m in place by angle a (in degrees)
 * around the axis (x, y, z).
 *
 * @param m source matrix
 * @param a angle to rotate in degrees
 * @param x X axis component
 * @param y Y axis component
 * @param z Z axis component
 */
void Matrix::rotateM(float m[], float a, float x, float y, float z)
{
    Matrix::setRotateM(sTemp, a, x, y, z);
    Matrix::multiplyMM(sTemp+16, m, sTemp);
    memcpy(m, sTemp+16, 16);
}

/**
 * Creates a matrix for rotation by angle a (in degrees)
 * around the axis (x, y, z).
 * <p>
 * An optimized path will be used for rotation about a major axis
 * (e.g. x=1.0f y=0.0f z=0.0f).
 *
 * @param rm returns the result
 * @param a angle to rotate in degrees
 * @param x X axis component
 * @param y Y axis component
 * @param z Z axis component
 */
void Matrix::setRotateM(float rm[], float a, float x, float y, float z)
{
    rm[ 3] = 0;
    rm[ 7] = 0;
    rm[11]= 0;
    rm[12]= 0;
    rm[13]= 0;
    rm[14]= 0;
    rm[15]= 1;
    a *= (float) (M_PI / 180.0f);
    float s = (float) sin(a);
    float c = (float) cos(a);
    if (1.0f == x && 0.0f == y && 0.0f == z) {
        rm[ 5] = c;   rm[10]= c;
        rm[ 6] = s;   rm[ 9] = -s;
        rm[ 1] = 0;   rm[ 2] = 0;
        rm[ 4] = 0;   rm[ 8] = 0;
        rm[ 0] = 1;
    } else if (0.0f == x && 1.0f == y && 0.0f == z) {
        rm[ 0] = c;   rm[10]= c;
        rm[ 8] = s;   rm[ 2] = -s;
        rm[ 1] = 0;   rm[ 4] = 0;
        rm[ 6] = 0;   rm[ 9] = 0;
        rm[ 5] = 1;
    } else if (0.0f == x && 0.0f == y && 1.0f == z) {
        rm[ 0] = c;   rm[ 5] = c;
        rm[ 1] = s;   rm[ 4] = -s;
        rm[ 2] = 0;   rm[ 6] = 0;
        rm[ 8] = 0;   rm[ 9] = 0;
        rm[10]= 1;
    } else {
        float len = length(x, y, z);
        if (1.0f != len) {
            float recipLen = 1.0f / len;
            x *= recipLen;
            y *= recipLen;
            z *= recipLen;
        }
        float nc = 1.0f - c;
        float xy = x * y;
        float yz = y * z;
        float zx = z * x;
        float xs = x * s;
        float ys = y * s;
        float zs = z * s;
        rm[ 0] = x*x*nc +  c;
        rm[ 4] =  xy*nc - zs;
        rm[ 8] =  zx*nc + ys;
        rm[ 1] =  xy*nc + zs;
        rm[ 5] = y*y*nc +  c;
        rm[ 9] =  yz*nc - xs;
        rm[ 2] =  zx*nc - ys;
        rm[ 6] =  yz*nc + xs;
        rm[10] = z*z*nc +  c;
    }
}

/**
 * Converts Euler angles to a rotation matrix.
 *
 * @param rm returns the result
 * @param x angle of rotation, in degrees
 * @param y angle of rotation, in degrees
 * @param z angle of rotation, in degrees
 */
void Matrix::setRotateEulerM(float rm[], float x, float y, float z)
{
    x *= (float) (M_PI / 180.0f);
    y *= (float) (M_PI / 180.0f);
    z *= (float) (M_PI / 180.0f);
    float cx = (float) cos(x);
    float sx = (float) sin(x);
    float cy = (float) cos(y);
    float sy = (float) sin(y);
    float cz = (float) cos(z);
    float sz = (float) sin(z);
    float cxsy = cx * sy;
    float sxsy = sx * sy;

    rm[ 0]  =   cy * cz;
    rm[ 1]  =  -cy * sz;
    rm[ 2]  =   sy;
    rm[ 3]  =  0.0f;

    rm[ 4]  =  cxsy * cz + cx * sz;
    rm[ 5]  = -cxsy * sz + cx * cz;
    rm[ 6]  =  -sx * cy;
    rm[ 7]  =  0.0f;

    rm[ 8]  = -sxsy * cz + sx * sz;
    rm[ 9]  =  sxsy * sz + sx * cz;
    rm[10] =  cx * cy;
    rm[11] =  0.0f;

    rm[12] =  0.0f;
    rm[13] =  0.0f;
    rm[14] =  0.0f;
    rm[15] =  1.0f;
}

/**
 * Defines a viewing transformation in terms of an eye point, a center of
 * view, and an up vector.
 *
 * @param rm returns the result
 * @param eyeX eye point X
 * @param eyeY eye point Y
 * @param eyeZ eye point Z
 * @param centerX center of view X
 * @param centerY center of view Y
 * @param centerZ center of view Z
 * @param upX up vector X
 * @param upY up vector Y
 * @param upZ up vector Z
 */
void Matrix::setLookAtM(float rm[],
        float eyeX, float eyeY, float eyeZ,
        float centerX, float centerY, float centerZ,
        float upX, float upY, float upZ)
{
    // See the OpenGL GLUT documentation for gluLookAt for a description
    // of the algorithm. We implement it in a straightforward way:

    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;

    // Normalize f
    float rlf = 1.0f / Matrix::length(fx, fy, fz);
    fx *= rlf;
    fy *= rlf;
    fz *= rlf;

    // compute s = f x up (x means "cross product")
    float sx = fy * upZ - fz * upY;
    float sy = fz * upX - fx * upZ;
    float sz = fx * upY - fy * upX;

    // and normalize s
    float rls = 1.0f / Matrix::length(sx, sy, sz);
    sx *= rls;
    sy *= rls;
    sz *= rls;

    // compute u = s x f
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;

    rm[ 0] = sx;
    rm[ 1] = ux;
    rm[ 2] = -fx;
    rm[ 3] = 0.0f;

    rm[ 4] = sy;
    rm[ 5] = uy;
    rm[ 6] = -fy;
    rm[ 7] = 0.0f;

    rm[ 8] = sz;
    rm[ 9] = uz;
    rm[10] = -fz;
    rm[11] = 0.0f;

    rm[12] = 0.0f;
    rm[13] = 0.0f;
    rm[14] = 0.0f;
    rm[15] = 1.0f;

    Matrix::translateM(rm, -eyeX, -eyeY, -eyeZ);
}
