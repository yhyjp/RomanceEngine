#include <RomanceEngine/Render/fragment_shader.h>

#include <cassert>

namespace RomanceEngine {
namespace Render {

FragmentShader::FragmentShader()
  : hasLoaded_(false)
{
}

bool FragmentShader::load(const CGcontext context, const std::string& fileName, const std::string& programName)
{
  assert(hasLoaded_ == false);
  context_ = context;
  fileName_ = fileName;
  programName_ = programName;

  profile_ = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  cgGLSetOptimalOptions(profile_);
  if (checkCgError(context, "selecting fragment profile") == false) return false;

  program_ =
    cgCreateProgramFromFile(
    context_,                /* Cg runtime context */
    CG_SOURCE,               /* Program in human-readable form */
    fileName_.c_str(),       /* Name of file containing program */
    profile_,                /* Profile: OpenGL ARB vertex program */
    programName_.c_str(),    /* Entry function name */
    NULL);                   /* No extra compiler options */
  if (checkCgError(context_, "creating fragment program from file : "+ fileName_ + ">" + programName_) == false) return false;

  cgGLLoadProgram(program_);
  if (checkCgError(context_, "loading fragment program") == false) return false;

  hasLoaded_ = true;

  return true;
}

void FragmentShader::registParameter(const std::string& name)
{
  parameters_[name] = cgGetNamedParameter(program_, name.c_str());
  checkCgError(context_, "could not get " + name + " parameter");
}

void FragmentShader::bind()
{
  cgGLBindProgram(program_);
  checkCgError(context_, "binding fragment program");

  cgGLEnableProfile(profile_);
  checkCgError(context_, "enabling fragment profile");

  for (int i=0; i < texList_.size(); ++i)
  {
    cgGLEnableTextureParameter(texList_[i]);
    checkCgError(context_, "enabling decal texture");
  }
}

void FragmentShader::unbind()
{
  cgGLDisableProfile(profile_);
  checkCgError(context_, "disabling fragment profile");

  for (int i=0; i < texList_.size(); ++i)
  {
    cgGLDisableTextureParameter(texList_[i]);
    checkCgError(context_, "disabling decal texture");
  }
}

void FragmentShader::setParameterMatrix(const std::string& name, const Math::Matrix4x4& value)
{
  std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
  if (it == parameters_.end())
  {
    printf("Error: parameter not registed > %s\n", name.c_str());
    assert(false);
  }
  cgSetMatrixParameterfr(parameters_[name], value.p_);
  checkCgError(context_, "setting " + name + " parameter");
}

void FragmentShader::setParameterFloat3(const std::string& name, const Math::Vector3D& value)
{
  setParameterFloat3(name, value.p_);
}

void FragmentShader::setParameterFloat3(const std::string& name, const float* value/*[3]*/)
{
  std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
  if (it == parameters_.end())
  {
    printf("Error: parameter not registed > %s\n", name.c_str());
    assert(false);
  }
  cgSetParameter3fv(parameters_[name], value);
  checkCgError(context_, "setting " + name + " parameter");
}

void FragmentShader::setParameterFloat3(const std::string& name, const float value0, const float value1, const float value2)
{
  std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
  if (it == parameters_.end())
  {
    printf("Error: parameter not registed > %s\n", name.c_str());
    assert(false);
  }
  cgSetParameter3f(parameters_[name], value0, value1, value2);
  checkCgError(context_, "setting " + name + " parameter");
}

void FragmentShader::setParameterFloat1(const std::string& name, const float value)
{
  std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
  if (it == parameters_.end())
  {
    printf("Error: parameter not registed > %s\n", name.c_str());
    assert(false);
  }
  cgSetParameter1f(parameters_[name], value);
  checkCgError(context_, "setting " + name + " parameter");
}

void FragmentShader::setParameterTexture(const std::string& name, const uint32_t id)
{
  std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
  if (it == parameters_.end())
  {
    printf("Error: parameter not registed > %s\n", name.c_str());
    assert(false);
  }
  cgGLSetTextureParameter(parameters_[name], id);
  checkCgError(context_, "setting " + name + " parameter");

  texList_.push_back(it->second);
}

void FragmentShader::update()
{
  cgUpdateProgramParameters(program_);
}


bool FragmentShader::checkCgError(const CGcontext context, const std::string& situation)
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
