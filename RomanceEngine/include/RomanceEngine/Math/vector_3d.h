#ifndef __RE_MATH_VECTOR_3D__C27819453D9C436C88E574DBFE9B9099__
#define __RE_MATH_VECTOR_3D__C27819453D9C436C88E574DBFE9B9099__

#include <string>
#include <cmath>
#include <cassert>
#include "float1234.h"

namespace RomanceEngine {
namespace Math {

class Vector3D
{
public:
	Vector3D()
	{
		p_[0] = p_[1] = p_[2] = 0;
    p_[3] = 1;
	}

	Vector3D(const float x, const float y, const float z)
	{
		p_[0] = x;
		p_[1] = y;
		p_[2] = z;
    p_[3] = 1;
	}

	Vector3D(const float* src/*[3]*/)
	{
		p_[0] = src[0];
		p_[1] = src[1];
		p_[2] = src[2];
    p_[3] = src[3];
	}

  Vector3D(const Float3& src)
  {
    p_[0] = src.x_;
    p_[1] = src.y_;
    p_[2] = src.z_;
  }

  Float3 asFloat3() const { return Float3(p_); }

  float& operator[](const int index)
  {
    assert(0<=index && index < 4);
    return p_[index];
  }

  const float& operator[](const int index) const
  {
    assert(0<=index && index < 4);
    return p_[index];
  }
	
	bool operator==(const Vector3D& rhs) const
	{
		return (
			p_[0] == rhs.p_[0] &&
			p_[1] == rhs.p_[1] &&
			p_[2] == rhs.p_[2]);
	}

	const Vector3D operator*(const float rhs) const
	{
		return Vector3D(p_[0]*rhs, p_[1]*rhs, p_[2]*rhs);
	}

	const Vector3D operator/(const float rhs) const
	{
		return Vector3D(p_[0]/rhs, p_[1]/rhs, p_[2]/rhs);
	}

	const Vector3D operator+(const Vector3D& rhs) const
	{
		return Vector3D(p_[0]+rhs.p_[0], p_[1]+rhs.p_[1], p_[2]+rhs.p_[2]);
	}

	const Vector3D operator-(const Vector3D& rhs) const
	{
		return Vector3D(p_[0]-rhs.p_[0], p_[1]-rhs.p_[1], p_[2]-rhs.p_[2]);
	}

	const Vector3D& operator*=(const float rhs)
	{
		p_[0] *= rhs;
		p_[1] *= rhs;
		p_[2] *= rhs;
		return *this;
	}

	const Vector3D& operator/=(const float rhs)
	{
		Vector3D(p_[0]/rhs, p_[1]/rhs, p_[2]/rhs);
		return *this;
	}
	
	const Vector3D& operator+=(const Vector3D& rhs)
	{
		p_[0] += rhs.p_[0];
		p_[1] += rhs.p_[1];
		p_[2] += rhs.p_[2];
		return *this;
	}
	
	const Vector3D& operator-=(const Vector3D& rhs)
	{
		p_[0] -= rhs.p_[0];
		p_[1] -= rhs.p_[1];
		p_[2] -= rhs.p_[2];
		return *this;
	}

	const float abs() const
	{
		return sqrt(abs2());
	}

	// abs^2.
	const float abs2() const
	{
		return dot(*this);
	}

	const float dot(const Vector3D& rhs) const
	{
		return p_[0]*rhs.p_[0] + p_[1]*rhs.p_[1] + p_[2]*rhs.p_[2];
	}

	const Vector3D cross(const Vector3D& rhs) const
	{
		return Vector3D(
			p_[1]*rhs.p_[2] - p_[2]*rhs.p_[1],
			p_[2]*rhs.p_[0] - p_[0]*rhs.p_[2],
			p_[0]*rhs.p_[1] - p_[1]*rhs.p_[0]);
	}
	
	float distance(const Vector3D& rhs) const
	{
		return (rhs-*this).abs();
	}

	// distance^2.
	float distance2(const Vector3D& rhs) const
	{
		return (rhs-*this).abs2();
	}

	const Vector3D normal() const
	{
		const float len = abs();
		return *this / len;
	}

	const Vector3D& normalize()
	{
		*this = normal();
		return *this;
	}
	
	// ２ベクトルがなす角のコサイン.
	float calcCos(const Vector3D& rhs) const
	{
		return dot(rhs)/sqrt(abs2()*rhs.abs2());
	}
	
	// ２ベクトルがなす角のサイン.
	float calcSin(const Vector3D& rhs) const
	{
		return cross(rhs).abs()/sqrt(abs2()*rhs.abs2());
	}

	// ２ベクトルがなす角.
	float calcAngle(const Vector3D& rhs) const
	{
		return acos(calcCos(rhs));
	}

	// 投影.
	const Vector3D projection(const Vector3D& n) const
	{
		return n*(dot(n)/n.abs2());
	}
	
	const std::string asString(const int precision=5) const;

public:
	float p_[4];
};

inline const Vector3D operator*(const float lhs, const Vector3D& rhs)
{
	return rhs * lhs;
}

} // namespace Math
} // namespace RomanceEngine


#endif // __RE_VECTOR_3D__C27819453D9C436C88E574DBFE9B9099__