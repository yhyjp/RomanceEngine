#include <RomanceEngine/Render/vertex_shader.h>

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <map>
#include <string>
#include <cassert>

namespace RomanceEngine {
namespace Render {

VertexShader::VertexShader()
  : hasLoaded_(false)
{
}

bool VertexShader::load(const CGcontext context, const std::string& fileName, const std::string& programName)
{
  assert(hasLoaded_ == false);
  context_ = context;
  fileName_ = fileName;
  programName_ = programName;

  profile_ = cgGLGetLatestProfile(CG_GL_VERTEX);
  cgGLSetOptimalOptions(profile_);
  if (checkCgError(context_, "selecting vertex profile") == false) return false;

  program_ =
    cgCreateProgramFromFile(
    context_,                /* Cg runtime context */
    CG_SOURCE,               /* Program in human-readable form */
    fileName_.c_str(),       /* Name of file containing program */
    profile_,                /* Profile: OpenGL ARB vertex program */
    programName_.c_str(),    /* Entry function name */
    NULL);                   /* No extra compiler options */
  if (checkCgError(context_, "creating vertex program from file") == false) return false;

  cgGLLoadProgram(program_);
  if (checkCgError(context_, "loading vertex program") == false) return false;

  hasLoaded_ = true;

  return true;
}

void VertexShader::registParameter(const std::string& name)
{
  parameters_[name] = cgGetNamedParameter(program_, name.c_str());
  checkCgError(context_, "could not get " + name + " parameter");
}

void VertexShader::bind()
{
  cgGLBindProgram(program_);
  checkCgError(context_, "binding vertex program");

  cgGLEnableProfile(profile_);
  checkCgError(context_, "enabling vertex profile");
}

void VertexShader::unbind()
{
  cgGLDisableProfile(profile_);
  checkCgError(context_, "disabling vertex profile");
}

void VertexShader::setMatrixParameter(const std::string& name, const Math::Matrix4x4& value)
{
  std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
  if (it == parameters_.end())
  {
    printf("Error: parameter not registed > %s\n", name.c_str());
    assert(false);
  }
  cgSetMatrixParameterfr(parameters_[name], value.p_);
}

void VertexShader::update()
{
  cgUpdateProgramParameters(program_);
}

bool VertexShader::checkCgError(const CGcontext context, const std::string& situation)
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

