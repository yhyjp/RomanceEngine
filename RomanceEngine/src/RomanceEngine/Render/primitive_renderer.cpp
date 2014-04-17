#include <RomanceEngine/Render/primitive_renderer.h>

namespace RomanceEngine {
namespace Render {

PrimitiveRenderer::PrimitiveRenderer()
  : context_()
{
}
PrimitiveRenderer::PrimitiveRenderer(const RenderContextPtr& context)
  : context_(context)
{
}

PrimitiveRenderer::~PrimitiveRenderer()
{
}

void PrimitiveRenderer::drawRect(
    const Math::Float2& pos,
    const Math::Float2& size,
    const Math::Float4& color,
    const bool flipV)
{
  assert(context_);
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
    0, (float)flipV,
    0, (float)!flipV,
    1, (float)!flipV,
    1, (float)flipV,
  };

  const uint32_t idx[4] = { 0, 1, 2, 3 };
  
  context_->setVertexPointer(4, kRM_FLOAT, sizeof(float)*4, p);
  context_->setColorPointer(4, kRM_FLOAT, sizeof(float)*4, c);
  context_->setTexCoordPointer(2, kRM_FLOAT, sizeof(float)*2, t);
  context_->drawElements(kRM_QUADS, 4, kRM_UNSIGNED_INT, idx);
};

void PrimitiveRenderer::drawRect(
    const Math::Rect& rect,
    const Math::Float4& color,
    const bool flipV)
{
  drawRect(rect.pos_, rect.size_, color, flipV);
}

void PrimitiveRenderer::drawLine(
  const Math::Float2& from,
  const Math::Float2& to,
  const Math::Float4& color)
{
  assert(context_);
  const float p[4*2] = {
    from.x_ , from.y_ , 1, 1,
    to.x_   , to.y_   , 1, 1,
  };

  const float c[4*2] = {
    color.x_, color.y_, color.z_, color.w_,
    color.x_, color.y_, color.z_, color.w_
  };

  const uint32_t idx[2] = { 0, 1 };
  
  context_->setVertexPointer(4, kRM_FLOAT, sizeof(float)*4, p);
  context_->setColorPointer(4, kRM_FLOAT, sizeof(float)*4, c);
  context_->drawElements(kRM_LINES, 2, kRM_UNSIGNED_INT, idx);
}

} // RomanceEngine
} // Render