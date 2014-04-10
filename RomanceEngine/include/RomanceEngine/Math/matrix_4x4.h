#ifndef __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__
#define __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__

#include <algorithm>

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
	
public:
	static Matrix4x4 identity() { return identity_; }

public:
	float v_[16];

private:
	static Matrix4x4 identity_;
};

inline Matrix4x4::Matrix4x4()
{
	v_[ 0]=1; v_[ 1]=0; v_[ 2]=0; v_[ 3]=0;
	v_[ 4]=0; v_[ 5]=1; v_[ 6]=0; v_[ 7]=0;
	v_[ 8]=0; v_[ 9]=0; v_[10]=1; v_[11]=0;
	v_[12]=0; v_[13]=0; v_[14]=0; v_[15]=1;
}

inline Matrix4x4::Matrix4x4(const float* src/*[16]*/)
{
	std::copy(src, src+16, v_);
}

inline Matrix4x4::Matrix4x4(
	const float a0, const float a1, const float a2, const float a3,
	const float b0, const float b1, const float b2, const float b3,
	const float c0, const float c1, const float c2, const float c3,
	const float d0, const float d1, const float d2, const float d3)
{
	v_[ 0]=a0; v_[ 1]=a1; v_[ 2]=a2; v_[ 3]=a3;
	v_[ 4]=b0; v_[ 5]=b1; v_[ 6]=b2; v_[ 7]=b3;
	v_[ 8]=c0; v_[ 9]=c1; v_[10]=c2; v_[11]=c3;
	v_[12]=d0; v_[13]=d1; v_[14]=d2; v_[15]=d3;
}



} // namespace Math
} // namespace RomanceEngine

#endif __RM_MATH_MATRIX_4X4__7838A348_ACD7_4731_A2E9_33D0E5EF831B__