#include <RomanceEngine/Math/matrix_4x4.h>

#include <sstream>
#include <string>

namespace RomanceEngine {
namespace Math {

const std::string Matrix4x4::asString(const int precision) const
{
	std::ostringstream oss;
	oss.precision(precision);
	oss << "|" << p_[ 0] << ", " << p_[ 1] << ", " << p_[ 2] << ", " << p_[ 3] << "|" << std::endl
		<< "|" << p_[ 4] << ", " << p_[ 5] << ", " << p_[ 6] << ", " << p_[ 7] << "|" << std::endl
		<< "|" << p_[ 8] << ", " << p_[ 9] << ", " << p_[10] << ", " << p_[11] << "|" << std::endl
		<< "|" << p_[12] << ", " << p_[13] << ", " << p_[14] << ", " << p_[15] << "|" << std::endl;
	return oss.str();
}

} // namespace Math
} // namespace RomanceEngine
