#include <RomanceEngine/Math/vector_3d.h>

#include <sstream>
#include <string>

namespace RomanceEngine {
namespace Math {

const std::string Vector3D::asString(const int precision) const
{
	std::ostringstream oss;
	oss.precision(precision);
	oss << "(" << x_ << ", " << y_ << ", " << z_ << ")";
	return oss.str();
}

} // namespace Math
} // namespace RomanceEngine
