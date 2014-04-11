#ifndef __RE_MATH_VECTOR_3D__45AB48C4149A4D8B9835BB55D26A51F9__
#define __RE_MATH_VECTOR_3D__45AB48C4149A4D8B9835BB55D26A51F9__

#include <string>
#include <cmath>

namespace RomanceEngine {
namespace Math {
	
const float kRM_PI = 3.14159265358979323846f;
const float kRM_PI2 = kRM_PI * kRM_PI; // PI^2
const float kRM_PI_HALF = kRM_PI / 2.0f; // PI/2

inline const float degreeToRadian(const float degree)
{
  return degree * kRM_PI / 180.0f;
}

inline const float radianToDegree(const float radian)
{
  return radian * 180.0f / kRM_PI;
}


} // namespace Math
} // namespace RomanceEngine

#endif // __RE_MATH_VECTOR_3D__45AB48C4149A4D8B9835BB55D26A51F9__