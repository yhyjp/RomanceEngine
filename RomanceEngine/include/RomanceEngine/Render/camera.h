#ifndef __RM_CAMERA__2B666ACA863747DBBA2150C6B6742156__
#define __RM_CAMERA__2B666ACA863747DBBA2150C6B6742156__

#include <RomanceEngine/Math/vector_3d.h>
#include <RomanceEngine/Math/matrix_4x4.h>

namespace RomanceEngine {
namespace Render {

class Camera
{
public:
  Camera();
  ~Camera();
  
  void setLocation(Math::Vector3D& location);
  void setTarget(Math::Vector3D& target);
  void setUpVector(Math::Vector3D& upVector);
  
  const Math::Vector3D& getLocation() const { return location_; }
  const Math::Vector3D& getTarget() const { return target_; }
  const Math::Vector3D& getUpVector() const { return upVector_; }

  Math::Matrix4x4 getViewMatrix() const;


private:
  Math::Vector3D location_;
  Math::Vector3D target_;
  Math::Vector3D upVector_;
};

} // Render
} // RomanceEngine

#endif // __RM_CAMERA__2B666ACA863747DBBA2150C6B6742156__
