#include <RomanceEngine/Render/primitive_renderer.h>

namespace RomanceEngine {
namespace Render {

void PrimitiveRenderer::drawRect(
    RenderContext& rctx,
    const Math::Float2& pos,
    const Math::Float2& size,
    const Math::Float4& color)
{
  const float p[4*4] = {
    pos.x_        , pos.y_        , 1, 1,
    pos.x_        , pos.y_+size.y_, 1, 1,
    pos.x_+size.x_, pos.y_+size.y_, 1, 1,
    pos.x_+size.x_, pos.y_        , 1, 1,
  };

  const float c[4*4] = {
    color.x_, color.y_, color.z_, color.w_,
    color.x_, color.y_, color.z_, color.w_,
    color.x_, color.y_, color.z_, color.w_,
    color.x_, color.y_, color.z_, color.w_,
  };

  const float t[4*2] = {
    0, 1,
    0, 0,
    1, 0,
    1, 1,
  };

  const uint32_t idx[4] = { 0, 1, 2, 3 };
  
  rctx.setVertexPointer(4, kRM_FLOAT, sizeof(float)*4, p);
  rctx.setColorPointer(4, kRM_FLOAT, sizeof(float)*4, c);
  rctx.setTexCoordPointer(2, kRM_FLOAT, sizeof(float)*2, t);
  rctx.drawElements(GL_QUADS, 4, kRM_UNSIGNED_INT, idx);
};

} // RomanceEngine
} // Render