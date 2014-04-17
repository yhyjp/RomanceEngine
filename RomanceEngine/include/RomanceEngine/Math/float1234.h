// íPèÉÇ» float èWçá.

#ifndef __RM_MATH_FLOAT1234__F130A00EA025470AB09EA266E327391B__
#define __RM_MATH_FLOAT1234__F130A00EA025470AB09EA266E327391B__

#include <cassert>

namespace RomanceEngine {
namespace Math {
  
  struct Float1
  {
    Float1(const float x=0)
      : x_(x)
    {}
    
    Float1(const float* src/*[1]*/)
      : x_(src[0])
    {}

    const float& operator[](const int index) const
    {
      if (index == 0) return x_;
      assert(0);
    }

    float& operator[](const int index)
    {
      if (index == 0) return x_;
      assert(0);
    }

    float x_;
  };
  
  struct Float2
  {
    Float2(const float x=0, const float y=0)
      : x_(x), y_(y)
    {}

    explicit Float2(const Float1& f1, const float z=0)
      : x_(f1.x_), y_(0)
    {}
    
    Float2(const float* src/*[2]*/)
      : x_(src[0]), y_(src[1])
    {}

    const float& operator[](const int index) const
    {
      if (index == 0) return x_;
      if (index == 1) return y_;
      assert(0);
    }

    float& operator[](const int index)
    {
      if (index == 0) return x_;
      if (index == 1) return y_;
      assert(0);
    }

    float x_, y_;
  };
  
  struct Float3
  {
    Float3(const float x=0, const float y=0, const float z=0)
      : x_(x), y_(y), z_(z)
    {}

    explicit Float3(const Float2& f2, const float z=0)
      : x_(f2.x_), y_(f2.y_), z_(0)
    {}
    
    Float3(const float* src/*[3]*/)
      : x_(src[0]), y_(src[1]), z_(src[2])
    {}

    const float& operator[](const int index) const
    {
      if (index == 0) return x_;
      if (index == 1) return y_;
      if (index == 2) return z_;
      assert(0);
    }

    float& operator[](const int index)
    {
      if (index == 0) return x_;
      if (index == 1) return y_;
      if (index == 2) return z_;
      assert(0);
    }

    float x_, y_, z_;
  };

  struct Float4
  {
    Float4(const float x=0, const float y=0, const float z=0, const float w=0)
      : x_(x), y_(y), z_(z), w_(w)
    {}

    explicit Float4(const Float3& f3, const float w=0)
      : x_(f3.x_), y_(f3.y_), z_(f3.z_), w_(w)
    {}

    Float4(const float* src/*[4]*/)
      : x_(src[0]), y_(src[1]), z_(src[2]), w_(src[3])
    {}

    const float& operator[](const int index) const
    {
      if (index == 0) return x_;
      if (index == 1) return y_;
      if (index == 2) return z_;
      if (index == 3) return w_;
      assert(0);
    }

    float& operator[](const int index)
    {
      if (index == 0) return x_;
      if (index == 1) return y_;
      if (index == 2) return z_;
      if (index == 3) return w_;
      assert(0);
    }

    float x_, y_, z_, w_;
  };

} // namespace Math
} // namespace RomanceEngine

#endif //__RM_MATH_FLOAT1234__F130A00EA025470AB09EA266E327391B__