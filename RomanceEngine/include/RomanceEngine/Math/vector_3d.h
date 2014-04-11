#ifndef __RE_MATH_VECTOR_3D__C27819453D9C436C88E574DBFE9B9099__
#define __RE_MATH_VECTOR_3D__C27819453D9C436C88E574DBFE9B9099__

#include <string>
#include <cmath>

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
	
	bool operator==(const Vector3D& rhs) const
	{
		return (
			p_[0] == rhs.p_[0] &&
			p_[1] == rhs.p_[1] &&
			p_[2] == rhs.p_[2]);
	}

	const Vector3D Vector3D::operator*(const float rhs) const
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
	
	// ‚QƒxƒNƒgƒ‹‚ª‚È‚·Šp‚ÌƒRƒTƒCƒ“.
	float calcCos(const Vector3D& rhs) const
	{
		return dot(rhs)/sqrt(abs2()*rhs.abs2());
	}
	
	// ‚QƒxƒNƒgƒ‹‚ª‚È‚·Šp‚ÌƒTƒCƒ“.
	float calcSin(const Vector3D& rhs) const
	{
		return cross(rhs).abs()/sqrt(abs2()*rhs.abs2());
	}

	// ‚QƒxƒNƒgƒ‹‚ª‚È‚·Šp.
	float calcAngle(const Vector3D& rhs) const
	{
		return acos(calcCos(rhs));
	}

	// “Š‰e.
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