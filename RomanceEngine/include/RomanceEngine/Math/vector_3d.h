#ifndef __RE_MATH_VECTOR_3D__C2781945_3D9C_436C_88E5_74DBFE9B9099__
#define __RE_MATH_VECTOR_3D__C2781945_3D9C_436C_88E5_74DBFE9B9099__

#include <string>

namespace RomanceEngine {
namespace Math {

class Vector3D
{
public:
	Vector3D()
	{
		p_[0] = p_[1] = p_[2] = 0;
	}

	Vector3D(const float x, const float y, const float z)
	{
		p_[0] = x;
		p_[1] = y;
		p_[2] = z;
	}

	Vector3D(const float* src/*[3]*/)
	{
		p_[0] = src[0];
		p_[1] = src[1];
		p_[2] = src[2];
	}
	
	bool operator==(const Vector3D& lhs) const
	{
		return (
			p_[0] == lhs.p_[0] &&
			p_[1] == lhs.p_[1] &&
			p_[2] == lhs.p_[2]);
	}
	
	const Vector3D operator*(const float lhs) const
	{
		return Vector3D(p_[0]*lhs, p_[1]*lhs, p_[2]*lhs);
	}

	const Vector3D operator/(const float lhs) const
	{
		return Vector3D(p_[0]/lhs, p_[1]/lhs, p_[2]/lhs);
	}

	const Vector3D operator+(const Vector3D& lhs) const
	{
		return Vector3D(p_[0]+lhs.p_[0], p_[1]+lhs.p_[1], p_[2]+lhs.p_[2]);
	}

	const Vector3D operator-(const Vector3D& lhs) const
	{
		return Vector3D(p_[0]-lhs.p_[0], p_[1]-lhs.p_[1], p_[2]-lhs.p_[2]);
	}

	const Vector3D& operator*=(const float lhs)
	{
		p_[0] *= lhs;
		p_[1] *= lhs;
		p_[2] *= lhs;
		return *this;
	}

	const Vector3D& operator/=(const float lhs)
	{
		Vector3D(p_[0]/lhs, p_[1]/lhs, p_[2]/lhs);
		return *this;
	}
	
	const Vector3D& operator+=(const Vector3D& lhs)
	{
		p_[0] += lhs.p_[0];
		p_[1] += lhs.p_[1];
		p_[2] += lhs.p_[2];
		return *this;
	}
	
	const Vector3D& operator-=(const Vector3D& lhs)
	{
		p_[0] -= lhs.p_[0];
		p_[1] -= lhs.p_[1];
		p_[2] -= lhs.p_[2];
		return *this;
	}

	const float dot(const Vector3D& lhs) const
	{
		return p_[0]*lhs.p_[0] + p_[1]*lhs.p_[1] + p_[2]*lhs.p_[2];
	}

	const Vector3D cross(const Vector3D& lhs) const
	{
		return Vector3D(
			p_[1]*lhs.p_[2] - p_[2]*lhs.p_[1],
			p_[2]*lhs.p_[0] - p_[0]*lhs.p_[2],
			p_[0]*lhs.p_[1] - p_[1]*lhs.p_[0]);
	}
	
	const std::string asString(const int precision=5) const;

public:
	float p_[3];
};

} // namespace Math
} // namespace RomanceEngine

#endif // __RE_VECTOR_3D__C2781945_3D9C_436C_88E5_74DBFE9B9099__