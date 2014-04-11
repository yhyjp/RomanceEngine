#ifndef __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__
#define __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__

#include <algorithm>
#include <string>
#include <cmath>
#include "vector_3d.h"

namespace RomanceEngine {
namespace Math {

class Matrix4x4
{
public:
  Matrix4x4();
  Matrix4x4(const float* src/*[16]*/);
  Matrix4x4(
    const float a0, const float a1, const float a2, const float a3, 
    const float b0, const float b1, const float b2, const float b3, 
    const float c0, const float c1, const float c2, const float c3, 
    const float d0, const float d1, const float d2, const float d3);

  Matrix4x4::Matrix4x4(
    const float a0, const float a1, const float a2,
    const float b0, const float b1, const float b2,
    const float c0, const float c1, const float c2);

  const std::string asString(const int precision=5) const;
  const bool isDiagonal() const;
  const Matrix4x4 transpose() const;

  const Matrix4x4 operator*(const float rhs) const;
  const Matrix4x4 operator/(const float rhs) const;
  const Matrix4x4& operator*=(const float rhs);
  const Matrix4x4& operator/=(const float rhs);

  const Matrix4x4 multiply(const Matrix4x4& rhs) const;
  const Vector3D multiply(const Vector3D& rhs) const;
  
  const float determinant() const;
  const float determinant3x3() const;

  const Matrix4x4 inverse() const;

public:
  static const Matrix4x4 identity() { return identity_; }
  
  static const Matrix4x4 rotateX(const float radian);
  static const Matrix4x4 rotateY(const float radian);
  static const Matrix4x4 rotateZ(const float radian);
  static const Matrix4x4 rotateByVector(const Vector3D& n, const float radian);
  
  static const Matrix4x4 scale(const float sx, const float sy, const float sz);
  static const Matrix4x4 scale(const Vector3D& s) { return scale(s.p_[0], s.p_[1], s.p_[2]); }

  static const Matrix4x4 translate(const float dx, const float dy, const float dz);
  static const Matrix4x4 translate(const Vector3D& d) { return translate(d.p_[0], d.p_[1], d.p_[2]); }

public:
  float p_[16];

private:
  static Matrix4x4 identity_;
};

inline Matrix4x4::Matrix4x4()
{
  p_[ 0]=1; p_[ 1]=0; p_[ 2]=0; p_[ 3]=0;
  p_[ 4]=0; p_[ 5]=1; p_[ 6]=0; p_[ 7]=0;
  p_[ 8]=0; p_[ 9]=0; p_[10]=1; p_[11]=0;
  p_[12]=0; p_[13]=0; p_[14]=0; p_[15]=1;
}

inline Matrix4x4::Matrix4x4(const float* src/*[16]*/)
{
  std::copy(src, src+16, p_);
}

inline Matrix4x4::Matrix4x4(
  const float a0, const float a1, const float a2, const float a3,
  const float b0, const float b1, const float b2, const float b3,
  const float c0, const float c1, const float c2, const float c3,
  const float d0, const float d1, const float d2, const float d3)
{
  p_[ 0]=a0; p_[ 1]=a1; p_[ 2]=a2; p_[ 3]=a3;
  p_[ 4]=b0; p_[ 5]=b1; p_[ 6]=b2; p_[ 7]=b3;
  p_[ 8]=c0; p_[ 9]=c1; p_[10]=c2; p_[11]=c3;
  p_[12]=d0; p_[13]=d1; p_[14]=d2; p_[15]=d3;
}

inline Matrix4x4::Matrix4x4(
  const float a0, const float a1, const float a2,
  const float b0, const float b1, const float b2,
  const float c0, const float c1, const float c2)
{
  p_[ 0]=a0; p_[ 1]=a1; p_[ 2]=a2; p_[ 3]=0;
  p_[ 4]=b0; p_[ 5]=b1; p_[ 6]=b2; p_[ 7]=0;
  p_[ 8]=c0; p_[ 9]=c1; p_[10]=c2; p_[11]=0;
  p_[12]=0; p_[13]=0; p_[14]=0; p_[15]=1;
}

inline const bool Matrix4x4::isDiagonal() const
{
  return (
    p_[ 1]==0 && p_[ 2]==0 && p_[ 3]==0 &&
    p_[ 4]==0 && p_[ 6]==0 && p_[ 7]==0 &&
    p_[ 8]==0 && p_[ 9]==0 && p_[11]==0 &&
    p_[12]==0 && p_[13]==0 && p_[14]==0);
}

inline const Matrix4x4 Matrix4x4::transpose() const
{
  return Matrix4x4(
    p_[ 0], p_[ 4], p_[ 8], p_[12],
    p_[ 1], p_[ 5], p_[ 9], p_[13],
    p_[ 2], p_[ 6], p_[10], p_[14],
    p_[ 3], p_[ 7], p_[11], p_[15]
  );
}

inline const Matrix4x4 Matrix4x4::operator*(const float rhs) const
{
  Matrix4x4 res = *this;
  res *= rhs;
  return res;
}

inline const Matrix4x4 Matrix4x4::operator/(const float rhs) const
{
  Matrix4x4 res = *this;
  res /= rhs;
  return res;
}

inline const Matrix4x4& Matrix4x4::operator*=(const float rhs)
{
  for (int i=0; i < 16; ++i)
  {
    p_[i] *= rhs;
  }
  return *this;
}

inline const Matrix4x4& Matrix4x4::operator/=(const float rhs)
{
  for (int i=0; i < 16; ++i)
  {
    p_[i] /= rhs;
  }
  return *this;
}

inline const float Matrix4x4::determinant3x3() const
{
  const Vector3D a(p_[0], p_[1], p_[2]);
  const Vector3D b(p_[4], p_[5], p_[6]);
  const Vector3D c(p_[8], p_[9], p_[10]);
  return a.cross(b).dot(c);
}

//========================================================


inline const Matrix4x4 Matrix4x4::rotateX(const float radian)
{
  const float s = sin(radian);
  const float c = cos(radian);
  return Matrix4x4(
    1, 0, 0, 0,
    0, c,-s, 0,
    0, s, c, 0,
    0, 0, 0, 1);
}

inline const Matrix4x4 Matrix4x4::rotateY(const float radian)
{
  const float s = sin(radian);
  const float c = cos(radian);
  return Matrix4x4(
    c, 0, s, 0,
    0, 1, 0, 0,
   -s, 0, c, 0,
    0, 0, 0, 1);
}

inline const Matrix4x4 Matrix4x4::rotateZ(const float radian)
{
  const float s = sin(radian);
  const float c = cos(radian);
  return Matrix4x4(
    c,-s, 0, 0,
    s, c, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);
}

inline const Matrix4x4 Matrix4x4::rotateByVector(const Vector3D& n, const float radian)
{
  const float s = sin(radian);
  const float c = cos(radian);
  const float ic = 1.0f-c;
  const float nx = n.p_[0];
  const float ny = n.p_[1];
  const float nz = n.p_[2];
  const float nxx = nx * nx;
  const float nyy = ny * ny;
  const float nzz = nz * nz;
  const float nxy = nx * ny;
  const float nxz = nx * nz;
  const float nyz = ny * nz;

  return Matrix4x4(
       nxx*ic+c   , nxy*ic+nz*s, nxz*ic-ny*s, 0,
       nxy*ic-nz*s, nyy*ic+c   , nyz*ic+nx*s, 0,
       nxz*ic+ny*s, nyz*ic-nx*s, nzz*ic+c   , 0,
       0, 0, 0, 1).transpose();
}

inline const Matrix4x4 Matrix4x4::scale(const float sx, const float sy, const float sz)
{
  return Matrix4x4(
    sx, 0, 0, 0,
    0, sy, 0, 0,
    0, 0, sz, 0,
    0, 0,  0, 1);
}

inline const Matrix4x4 Matrix4x4::translate(const float dx, const float dy, const float dz)
{
  return Matrix4x4(
    1, 0, 0, dx,
    0, 1, 0, dy,
    0, 0, 1, dz,
    0, 0, 0, 1);
}


} // namespace Math
} // namespace RomanceEngine

#endif // __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__