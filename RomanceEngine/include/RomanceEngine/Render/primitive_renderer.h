#ifndef __RM_RENDER_PRIMITIVE_RENDERER__6C5B90FAD627469C9A81736EA5563216__
#define __RM_RENDER_PRIMITIVE_RENDERER__6C5B90FAD627469C9A81736EA5563216__

#include <RomanceEngine/Math/float1234.h>
#include <RomanceEngine/Math/rect.h>
#include "render_context.h"
#include "constant.h"

namespace RomanceEngine {
namespace Render {

class PrimitiveRenderer
{
public:
  PrimitiveRenderer();
  PrimitiveRenderer(const RenderContextPtr& context);
  ~PrimitiveRenderer();
  
  RenderContextPtr& getRenderContext() { return context_; }
  const RenderContextPtr& getRenderContext() const { return context_; }

  void setRenderContext(const RenderContextPtr& context) { context_; }

  void drawRect(
    const Math::Float2& pos,
    const Math::Float2& size,
    const Math::Float4& color);

  void drawRect(
    const Math::Rect& rect,
    const Math::Float4& color);

  void drawLine(
    const Math::Float2& from,
    const Math::Float2& to,
    const Math::Float4& color);

private:
  RenderContextPtr context_;
};

} // RomanceEngine
} // Render


#endif //__RM_RENDER_PRIMITIVE_RENDERER__6C5B90FAD627469C9A81736EA5563216__
