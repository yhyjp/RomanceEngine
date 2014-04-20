#include <RomanceEngine/Render/camera.h>

namespace RomanceEngine {
namespace Render {

Camera::Camera()
  : location_(-5, -5, -5)
  , target_(0, 0, 0)
  , upVector_(0, 1, 0)
{

}

Camera::~Camera()
{
}

void Camera::setLocation(Math::Vector3D& location)
{
  location_ = location;
}

void Camera::setTarget(Math::Vector3D& target)
{
  target_ = target;
}

void Camera::setUpVector(Math::Vector3D& upVector)
{
  upVector_ = upVector;
}

// ƒLƒƒƒbƒVƒ…‚É‚·‚é‚©‚à.
Math::Matrix4x4 Camera::getViewMatrix() const
{
  return Math::Matrix4x4::buildLookAt(location_, target_, upVector_);
}

} // Render
} // RomanceEngine
