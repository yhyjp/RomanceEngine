#ifndef __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__
#define __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__

#include <algorithm>
#include <string>

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
	
	const std::string asString(const int precision=5) const;
	const bool isDiagonal() const;
	const Matrix4x4 transpose() const;
	
	const Matrix4x4 operator*(const float rhs) const;
	const Matrix4x4 operator/(const float rhs) const;
	const Matrix4x4& operator*=(const float rhs);
	const Matrix4x4& operator/=(const float rhs);
	
	const Matrix4x4 multiply(const Matrix4x4& rhs) const;

public:
	static Matrix4x4 identity() { return identity_; }

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

inline const Matrix4x4 Matrix4x4::multiply(const Matrix4x4& rhs) const
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

} // namespace Math
} // namespace RomanceEngine

#endif // __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__