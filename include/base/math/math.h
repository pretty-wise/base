/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
#pragma once

#include "base/core/types.h"
#include "base/core/assert.h"

#include <stdlib.h> // rand
#include <math.h>

namespace Base {
namespace Math {

//! Constants.
static const float PI = float(3.14159265358979323846f);
static const float TWO_PI = 2 * PI;

//! Bit Operations.

#define ROUND_UP_32(value, alignment)                                          \
  ((((s32)(value) + (alignment - 1)) & ~(alignment - 1)))
#define ROUND_DOWN_32(value, alignment) ((s32)(value) &= (s32)(alignment - 1))

#define BIT_SET_ON(val, bit) ((val) |= (1 << (bit)))

#define BIT_SET_OFF(val, bit) ((val) ^= (1 << (bit)))

#define BIT_IS_ON(val, bit) ((val) & (1 << (bit)))

#define BIT32(x) (1 << (x))
#define BIT64(x) (1ULL << (x))

// marks mask bit in value as set.
#define BIT_MASK_ON(val, mask) ((val) |= (mask))

/// marks mask bit in value as not set.
#define BIT_MASK_OFF(val, mask) ((val) &= ~(mask))

/// toggle mask bit.
#define BIT_MASK_TOGGLE(val, mask) ((val) ^= (mask))

/// checks whether all bits of mask are set.
#define BIT_MASK_IS_ON(val, mask) (((val) & (mask)) == (mask))

/// checks whether any bit of mask is set.
#define BIT_MASK_IS_ANY_ON(val, mask) ((val) & (mask))

//---------------------------------------------------------------------------------------

void SRand();

u32 Rand(u32 uMax);

//! returns random floating point number from <0, 1>
float RandFloat();

//! returns a random floating point number from <0, fMax>
float RandFloat(float fMax);

//! returns a random floating point number from <fMin, fMax>
float RandFloatRange(float fMin, float fMax);

template <class T> inline T Pow(T base, T exp);

template <class T> inline T Log(T val);

} // namespace Math
} // namespace Base

#include "base/math/math.inl"
