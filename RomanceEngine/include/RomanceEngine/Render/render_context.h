// RenderContext.
// Še RenderSystem ‚É‚Ä template method.

#ifndef __RM_RENDER_RENDER_CONTEXT__20F91890EDC44EC8A6E3544AF06C5BDC__
#define __RM_RENDER_RENDER_CONTEXT__20F91890EDC44EC8A6E3544AF06C5BDC__

#include <RomanceEngine/Memory/shared_ptr.h>

#include "shader_manager.h"
#include "fragment_shader.h"
#include "vertex_shader.h"

namespace RomanceEngine {
namespace Render {
  
class RenderContext
{
public:
  virtual ~RenderContext() {}

  virtual void init() = 0;

  virtual ShaderManagerPtr& getShaderManager() = 0;
  virtual const ShaderManagerPtr& getShaderManager() const = 0;

  virtual void setVertexPointer(const int32_t size, const uint32_t type, const uint32_t stride, const void* pointer) = 0;
  virtual void setNormalPointer(const uint32_t type, const uint32_t stride, const void* pointer) = 0;
  virtual void setColorPointer(const int32_t size, const uint32_t type, const uint32_t stride, const void* pointer) = 0;
  virtual void setTexCoordPointer(const int32_t size, const uint32_t type, const uint32_t stride, const void* pointer) = 0;
  virtual void drawElements(const uint32_t mode, const uint32_t count, const uint32_t type, const void* indices) = 0;
  
  virtual void setVertexPointerEnabled(const bool isEnable) {};
  virtual void setNormalPointerEnabled(const bool isEnable) {};
  virtual void setColorPointerEnabled(const bool isEnable) {};
  virtual void setTexCoordPointerEnabled(const bool isEnable) {};
};
typedef Memory::SharedPtr<RenderContext> RenderContextPtr;

} // RomanceEngine
} // Render


#endif //__RM_RENDER_RENDER_CONTEXT__20F91890EDC44EC8A6E3544AF06C5BDC__
