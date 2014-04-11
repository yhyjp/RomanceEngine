#include <RomanceEngine/Math/matrix_4x4.h>

#include <sstream>
#include <string>

#include <RomanceEngine/Math/constant.h>

namespace RomanceEngine {
namespace Math {

Matrix4x4 Matrix4x4::identity_;

const std::string Matrix4x4::asString(const int precision) const
{
  std::ostringstream oss;
  oss.precision(precision);
  oss.setf(std::ios::fixed, std::ios::floatfield);
  oss << "|" << p_[ 0] << ", " << p_[ 1] << ", " << p_[ 2] << ", " << p_[ 3] << "|" << std::endl
      << "|" << p_[ 4] << ", " << p_[ 5] << ", " << p_[ 6] << ", " << p_[ 7] << "|" << std::endl
      << "|" << p_[ 8] << ", " << p_[ 9] << ", " << p_[10] << ", " << p_[11] << "|" << std::endl
      << "|" << p_[12] << ", " << p_[13] << ", " << p_[14] << ", " << p_[15] << "|" << std::endl;
  return oss.str();
}

const float Matrix4x4::determinant() const
{
  const Matrix4x4 a(
    p_[1*4+1], p_[1*4+2], p_[1*4+3],
    p_[2*4+1], p_[2*4+2], p_[2*4+3],
    p_[3*4+1], p_[3*4+2], p_[3*4+3]
    );
  const Matrix4x4 b(
    p_[1*4+0], p_[1*4+2], p_[1*4+3],
    p_[2*4+0], p_[2*4+2], p_[2*4+3],
    p_[3*4+0], p_[3*4+2], p_[3*4+3]
    );
  const Matrix4x4 c(
    p_[1*4+0], p_[1*4+1], p_[1*4+3],
    p_[2*4+0], p_[2*4+1], p_[2*4+3],
    p_[3*4+0], p_[3*4+1], p_[3*4+3]
    );
  const Matrix4x4 d(
    p_[1*4+0], p_[1*4+1], p_[1*4+2],
    p_[2*4+0], p_[2*4+1], p_[2*4+2],
    p_[3*4+0], p_[3*4+1], p_[3*4+2]
    );

  return p_[0*4+0]*a.determinant3x3()
    - p_[0*4+1]*b.determinant3x3()
    + p_[0*4+2]*c.determinant3x3()
    - p_[0*4+3]*d.determinant3x3();
}

const Matrix4x4 Matrix4x4::multiply(const Matrix4x4& rhs) const
{
  Matrix4x4 res;
  for (int i=0; i < 4; ++i)
  {
    for (int j=0; j < 4; ++j)
    {
      res.p_[i*4+j] = 0;
      for (int k=0; k < 4; ++k)
      {
        res.p_[i*4+j] += p_[i*4+k] * rhs.p_[k*4+j];
      }
    }
  }
  return res;
}

const Vector3D Matrix4x4::multiply(const Vector3D& rhs) const
{
  Vector3D res;
  for (int i=0; i < 4; ++i)
  {
    res.p_[i] = 0;
    for (int j=0; j < 4; ++j)
    {
      res.p_[i] += p_[i*4+j] * rhs.p_[j];
    }
  }
  return res;
}

const Matrix4x4 Matrix4x4::inverse() const
{
  // “f‚«o‚µ–@‚Å‹‚ß‚é.
  Matrix4x4 a = *this;
  Matrix4x4 inv = Matrix4x4::buildIdentity();

  for(int i=0; i < 4; i++)
  {
    const float x = 1/a.p_[i*4+i];
    for(int j=0; j < 4; j++)
    {
      a.p_[i*4+j] *= x;
      inv.p_[i*4+j] *= x;
    }

    for(int j=0; j < 4; j++)
    {
      if(i != j)
      {
        const float y = a.p_[j*4+i];
        for(int k=0; k < 4 ;k++)
        {
          a.p_[j*4+k] -= a.p_[i*4+k] * y;
          inv.p_[j*4+k] -= inv.p_[i*4+k] * y;
        }
      }
    }
  }
  return inv;
}

const Matrix4x4 Matrix4x4::buildPerspective(const float fovRadian, const float aspectRatio, const float zNear, const float zFar)
{
  const float fovHalf = fovRadian / 2.0f;
  const float deltaZ = zFar - zNear;
  const float sine = sin(fovHalf);
  const float cotan = cos(fovHalf) / sine;

  return Matrix4x4(
    cotan/aspectRatio, 0, 0, 0,
    0, cotan, 0, 0,
    0, 0, -(zFar + zNear)/deltaZ, -2*zNear*zFar/deltaZ,
    0, 0, -1, 0);
}

const Matrix4x4 Matrix4x4::buildLookAt(const float eyeX, const float eyeY, const float eyeZ,
                       const float centerX, const float centerY, const float centerZ,
                       const float upX, const float upY, const float upZ)
{
  return buildLookAt(
    Vector3D(eyeX, eyeY, eyeZ),
    Vector3D(centerX, centerY, centerZ),
    Vector3D(upX, upY, upZ));
}

const Matrix4x4 Matrix4x4::buildLookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up)
{
  const Vector3D z = eye - center;
  const Vector3D zn = z.normal();
  const Vector3D x = up.cross(zn);
  const Vector3D y = zn.cross(x);
  const Vector3D xn = x.normal();
  const Vector3D yn = y.normal();

  return Matrix4x4(
    xn[0], xn[1], xn[2], -xn.dot(eye),
    yn[0], yn[1], yn[2], -yn.dot(eye),
    zn[0], zn[1], zn[2], -zn.dot(eye),
    0, 0, 0, 1);
}

} // namespace Math
} // namespace RomanceEngine
