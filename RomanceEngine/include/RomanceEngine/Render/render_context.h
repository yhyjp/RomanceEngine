// RenderContext.
// Še RenderSystem ‚É‚Ä template method.

#ifndef __RM_RENDER_RENDER_CONTEXT__20F91890EDC44EC8A6E3544AF06C5BDC__
#define __RM_RENDER_RENDER_CONTEXT__20F91890EDC44EC8A6E3544AF06C5BDC__

#include "shader_manager.h"
#include "fragment_shader.h"
#include "vertex_shader.h"

namespace RomanceEngine {
namespace Render {
  
class RenderContext
{
public:
  virtual void init();

  virtual ShaderManagerPtr getShaderManager();

  virtual FragmentShaderPtr getFragmentShader() = 0;
  virtual VertexShaderPtr getVertexShader() = 0;
  virtual void setFragmentShader() = 0;
  virtual void setVertexShader() = 0;

  virtual void renderBegin() = 0;
  virtual void setVertexArray() = 0;
  virtual void setColorArray() = 0;
  virtual void setTexcoordArray() = 0;
  virtual void drawElements() = 0;
  virtual void renderEnd() = 0;
};

} // RomanceEngine
} // Render


#endif //__RM_RENDER_RENDER_CONTEXT__20F91890EDC44EC8A6E3544AF06C5BDC__