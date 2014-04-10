#ifndef RE_MATH_VECTOR_3D_C2781945_3D9C_436C_88E5_74DBFE9B9099
#define RE_MATH_VECTOR_3D_C2781945_3D9C_436C_88E5_74DBFE9B9099

#include <string>

namespace RomanceEngine {
namespace Math {

class Vector3D
{
public:
	Vector3D()
		: x_(0), y_(0), z_(0)
	{}

	Vector3D(const float x, const float y, const float z)
		: x_(x), y_(y), z_(z)
	{}

	Vector3D(const float* p/*[3]*/)
		: x_(p[0]), y_(p[1]), z_(p[2])
	{}
	
	bool operator==(const Vector3D& lhs) const
	{
		return (
			x_ == lhs.x_ &&
			y_ == lhs.y_ &&
			z_ == lhs.z_);
	}
	
	const Vector3D operator*(const float lhs) const
	{
		return Vector3D(x_*lhs, y_*lhs, z_*lhs);
	}

	const Vector3D operator/(const float lhs) const
	{
		return Vector3D(x_/lhs, y_/lhs, z_/lhs);
	}

	const Vector3D operator+(const Vector3D& lhs) const
	{
		return Vector3D(x_+lhs.x_, y_+lhs.y_, z_+lhs.z_);
	}

	const Vector3D operator-(const Vector3D& lhs) const
	{
		return Vector3D(x_-lhs.x_, y_-lhs.y_, z_-lhs.z_);
	}

	const Vector3D& operator*=(const float lhs)
	{
		x_ *= lhs;
		y_ *= lhs;
		z_ *= lhs;
		return *this;
	}

	const Vector3D& operator/=(const float lhs)
	{
		Vector3D(x_/lhs, y_/lhs, z_/lhs);
		return *this;
	}
	
	const Vector3D& operator+=(const Vector3D& lhs)
	{
		x_ += lhs.x_;
		y_ += lhs.y_;
		z_ += lhs.z_;
		return *this;
	}
	
	const Vector3D& operator-=(const Vector3D& lhs)
	{
		x_ -= lhs.x_;
		y_ -= lhs.y_;
		z_ -= lhs.z_;
		return *this;
	}

	const float dot(const Vector3D& lhs) const
	{
		return x_*lhs.x_ + y_*lhs.y_ + z_*lhs.z_;
	}

	const Vector3D cross(const Vector3D& lhs) const
	{
		return Vector3D(
			y_*lhs.z_ - z_*lhs.y_,
			z_*lhs.x_ - x_*lhs.z_,
			x_*lhs.y_ - y_*lhs.x_);
	}
	
	const std::string asString(const int precision=5) const;

public:
	float x_, y_, z_;
};

} // namespace Math
} // namespace RomanceEngine

#endif // RE_VECTOR_3D_C2781945_3D9C_436C_88E5_74DBFE9B9099