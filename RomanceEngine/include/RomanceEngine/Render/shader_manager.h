#ifndef __RM_RENDER_SHADER_MANAGER__2B859CA0A5154F5BA84B20C1471448F8__
#define __RM_RENDER_SHADER_MANAGER__2B859CA0A5154F5BA84B20C1471448F8__

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <string>
#include "fragment_shader.h"
#include "vertex_shader.h"

namespace RomanceEngine {
namespace Render {

class ShaderManager
{
public:
  ShaderManager();

  void init();

  VertexShaderPtr createVertexShader(const std::string& fileName, const std::string& programName);
  FragmentShaderPtr createFragmentShader(const std::string& fileName, const std::string& programName);

  bool checkCgError(const CGcontext context, const std::string& situation);

private:
  CGcontext context_;
  bool hasInit_;
};



} // RomanceEngine
} // Render


#endif // __RM_RENDER_SHADER_MANAGER__2B859CA0A5154F5BA84B20C1471448F8__