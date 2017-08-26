//
//  CUMathBase.h
//  CUGL
//
//  Created by Walker White on 5/30/16.
//  Copyright © 2016 Game Design Initiative at Cornell. All rights reserved.
//

// TODO: COMMENT THIS FILE
#ifndef __CU_MATH_BASE_H__
#define __CU_MATH_BASE_H__
#include <cugl/base/CUBase.h>
#include <string>
#include <math.h>

#if defined (__WINDOWS__)
    #define M_PI_2     1.57079632679489661923f   // pi/2
    #define M_PI_4     0.785398163397448309616f  // pi/4
#endif


/**Util macro for conversion from degrees to radians.*/
#define CU_MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
/**Util macro for conversion from radians to degrees.*/
#define CU_MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
/** Compare two values for approximate equality */
#define CU_MATH_APPROX(x,y,t)          ( -(t) < (x)-(y) && (x)-(y) < (t) )

/** Small epsilon for high precision */
#define CU_MATH_FLOAT_SMALL            1.0e-37f
/** Normal epsilon for testing and other applications */
#define CU_MATH_EPSILON                1.0e-7f

// Define the vectorization support
#if defined (__ANDROID__) && defined (__arm64__)
    #define CU_MATH_VECTOR_NEON64
#elif defined (__ANDROID__)
    #define CU_MATH_VECTOR_ANDROID
#elif defined (__MACOSX__)
    // vecLib is essentially useless on iOS
    #define CU_MATH_VECTOR_APPLE
#elif defined (__IPHONE__)
    #define CU_MATH_VECTOR_IOS
#elif defined (__SSE__)
	// TODO: There is apparently an alignment error here
	// I do not know enough SSE to debug this right now
    // #define CU_MATH_VECTOR_SSE
#endif

/**
 * Returns value, clamped to the range [min,max]
 *
 * This function only works on floats
 *
 * @param value	The original value
 * @param min	The range minimum
 * @param max	The range maximum
 *
 * @return value, clamped to the range [min,max]
 */
inline float clampf(float value, float min, float max) {
    return value < min ? min : value < max? value : max;
}

/**
* Returns value, clamped to the range [min,max]
*
* This function only works on bytes
*
* @param value	The original value
* @param min	The range minimum
* @param max	The range maximum
*
* @return value, clamped to the range [min,max]
*/
inline GLubyte clampb(GLuint value, GLubyte min, GLubyte max) {
    return static_cast<GLubyte>(value < min ? min : value < max? value : max);
}

/**
 * Returns the power of two greater than or equal to x
 *
 * @param x		The original integer
 *
 * @return the power of two greater than or equal to x
 */
int nextPOT(int x);

#endif /* CU_MATH_BASE_H */
