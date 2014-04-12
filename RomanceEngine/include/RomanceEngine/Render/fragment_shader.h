#ifndef __RM_RENDER_FRAGMENT_SHADER__FEF80B8D3DB54230B99E472AC359557A__
#define __RM_RENDER_FRAGMENT_SHADER__FEF80B8D3DB54230B99E472AC359557A__

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <map>
#include <string>
#include <RomanceEngine/Memory/shared_ptr.h>
#include <RomanceEngine/Math/matrix_4x4.h>
#include <RomanceEngine/Math/vector_3d.h>


namespace RomanceEngine {
namespace Render {

class FragmentShader
{
public:
  FragmentShader();

  bool load(const CGcontext context, const std::string& fileName, const std::string& programName);
  void bind();
  void unbind();
  void update();

  void registParameter(const std::string& name);
  void setParameterMatrix(const std::string& name, const Math::Matrix4x4& value);
  void setParameterFloat3(const std::string& name, const Math::Vector3D& value);
  void setParameterFloat3(const std::string& name, const float* value/*[3]*/);
  void setParameterFloat3(const std::string& name, const float value0, const float value1, const float value2);
  void setParameterFloat1(const std::string& name, const float value);

private:
  bool checkCgError(const CGcontext context, const std::string& situation);

private:
  bool hasLoaded_;
  std::string programName_;
  std::string fileName_;
  CGcontext context_;
  CGprofile profile_;
  CGprogram program_;
  std::map<std::string, CGparameter> parameters_;
};
typedef Memory::SharedPtr<FragmentShader> FragmentShaderPtr;



} // RomanceEngine
} // Render


#endif // __RM_RENDER_FRAGMENT_SHADER__FEF80B8D3DB54230B99E472AC359557A__