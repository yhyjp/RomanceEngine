#ifndef __RM_RENDER_VERTEX_SHADER__6FB63F2000CF491E84CA4296E62B6572__
#define __RM_RENDER_VERTEX_SHADER__6FB63F2000CF491E84CA4296E62B6572__

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <map>
#include <string>
#include <RomanceEngine/Memory/shared_ptr.h>
#include <RomanceEngine/Math/matrix_4x4.h>

namespace RomanceEngine {
namespace Render {

class VertexShader
{
public:
  VertexShader();

  bool load(const CGcontext context, const std::string& fileName, const std::string& programName);
  void registParameter(const std::string& name);
  void bind();
  void unbind();
  void update();

  void setMatrixParameter(const std::string& name, const Math::Matrix4x4& value);

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
typedef Memory::SharedPtr<VertexShader> VertexShaderPtr;



} // RomanceEngine
} // Render


#endif // __RM_RENDER_VERTEX_SHADER__6FB63F2000CF491E84CA4296E62B6572__