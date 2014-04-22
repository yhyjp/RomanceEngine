#ifndef __RM_TRIANGLE__2B027E8A34DB4DD282636CA342BB7D10__
#define __RM_TRIANGLE__2B027E8A34DB4DD282636CA342BB7D10__

#include "vector_3d.h"

namespace RomanceEngine {
namespace Math {

class Triangle
{
public:
  Triangle();
  Triangle(const Vector3D* v/* [3] ccw */)
  {
    set(v);
  }

  Triangle(const Vector3D& v0 , const Vector3D& v1, const Vector3D& v2/* ccw */)
  {
    set(v0, v1, v2);
  }

  void set(const Vector3D& v0 , const Vector3D& v1, const Vector3D& v2/* ccw */)
  {
    v_[0] = v0;
    v_[1] = v1;
    v_[2] = v2;
  }

  void set(const Vector3D* v/* [3] ccw */)
  {
    set(v[0], v[1], v[2]);
  }


  Vector3D normal() const
  {
    const Vector3D v01 = v_[1] - v_[0];
    const Vector3D v02 = v_[2] - v_[0];
    return v02.cross(v01).normal();
  }

  float area() const
  {
    const Vector3D v01 = v_[1] - v_[0];
    const Vector3D v02 = v_[2] - v_[0];
    return v01.cross(v02).abs() / 2.0f;
  }

  Vector3D v_[3]; // ccw.
};

} // Math
} // RomanceEngine

#endif // __RM_TRIANGLE__2B027E8A34DB4DD282636CA342BB7D10__
