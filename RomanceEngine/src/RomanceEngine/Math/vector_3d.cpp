#include <RomanceEngine/Math/vector_3d.h>

#include <sstream>
#include <string>

namespace RomanceEngine {
namespace Math {

const std::string Vector3D::asString(const int precision) const
{
  std::ostringstream oss;
  oss.precision(precision);
  oss.setf(std::ios::fixed, std::ios::floatfield);
  oss << "(" << p_[0] << ", " << p_[1] << ", " << p_[2] << ")";
  return oss.str();
}
} // namespace Math
} // namespace RomanceEngine
