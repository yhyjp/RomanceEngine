#include <RomanceEngine/Render/shader_manager.h>


namespace RomanceEngine {
namespace Render {

ShaderManager::ShaderManager()
  : hasInit_(false)
{}

void ShaderManager::init()
{
  context_ = cgCreateContext();
  checkCgError(context_, "creating context");

  cgGLSetDebugMode(CG_FALSE);
  cgSetParameterSettingMode(context_, CG_DEFERRED_PARAMETER_SETTING);

  hasInit_ = true;
}

VertexShaderPtr ShaderManager::createVertexShader(const std::string& fileName, const std::string& programName)
{
  assert(hasInit_);
  VertexShaderPtr res(new VertexShader);
  if (res->load(context_, fileName, programName) == false)
  {
    checkCgError(context_, "load: " + fileName + " > " + programName);
    return VertexShaderPtr();
  }
  return res;
}

FragmentShaderPtr ShaderManager::createFragmentShader(const std::string& fileName, const std::string& programName)
{
  assert(hasInit_);
  FragmentShaderPtr res(new FragmentShader);
  if (res->load(context_, fileName, programName) == false)
  {
    checkCgError(context_, "load: " + fileName + " > " + programName);
    return FragmentShaderPtr();
  }
  return res;
}

bool ShaderManager::checkCgError(const CGcontext context, const std::string& situation)
{
  CGerror error;
  const char *str = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) {
    printf("Error: %s: %s\n", situation.c_str(), str);
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(context));
    }
    return false;
  }
  return true;
}

} // RomanceEngine
} // Render
