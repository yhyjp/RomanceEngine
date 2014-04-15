#ifndef __RM_RENDER_PRIMITIVE_RENDERER__6C5B90FAD627469C9A81736EA5563216__
#define __RM_RENDER_PRIMITIVE_RENDERER__6C5B90FAD627469C9A81736EA5563216__

#include <RomanceEngine/Math/float1234.h>
#include "render_context.h"
#include "constant.h"

namespace RomanceEngine {
namespace Render {

class PrimitiveRenderer
{
public:
  void drawRect(
    RenderContext& rctx,
    const Math::Float2& pos,
    const Math::Float2& size,
    const Math::Float4& color);

  void drawLine(
    RenderContext& rctx,
    const Math::Float2& from,
    const Math::Float2& to,
    const Math::Float4& color);
};

} // RomanceEngine
} // Render


#endif //__RM_RENDER_PRIMITIVE_RENDERER__6C5B90FAD627469C9A81736EA5563216__
