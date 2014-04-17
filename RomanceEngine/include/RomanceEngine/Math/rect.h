#ifndef __RM_MATH_RECT__A89793559B934D00BEEF2169A2713D3D__
#define __RM_MATH_RECT__A89793559B934D00BEEF2169A2713D3D__

#include "float1234.h"

namespace RomanceEngine {
namespace Math {

//
// lb   rb  
//
// ^     
// |
// |
// +----->
// lt   rt
//
class Rect
{
public:
  Rect()
    : pos_(), size_()
  {}

  Rect(const float x, const float y, const float w, const float h)
    : pos_(x, y), size_(w, h)
  {}

  Rect(const Float2 pos, const Float2 size)
    : pos_(pos), size_(size)
  {}

  float left() const { return pos_.x_; }
  float right() const { return pos_.x_ + size_.x_; }
  float top() const { return pos_.y_; }
  float bottom() const { return pos_.y_ + size_.y_; }
  float width() const { return size_.x_; }
  float height() const { return size_.y_; }

  // ある点が矩形に含まれるか.
  bool contains(const float px, const float py) const
  {
    return (
      left() <= px && px <= right() &&
      top() <= py && py <= bottom()
      );
  }

  bool contains(const Float2 p) const
  {
    return contains(p.x_, p.y_);
  }

  // 二つの矩形が重なっているか.
  bool collisioned(const Rect& other) const
  {
    return (
      left()   < other.right()  &&
      right()  > other.left()   &&
      top()    < other.bottom() &&
      bottom() > other.top()
      );
  }

  Float2 pos_;
  Float2 size_;
};

} // Math
} // RomanceEngine

#endif // __RM_MATH_RECT__A89793559B934D00BEEF2169A2713D3D__
