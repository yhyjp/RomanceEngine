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

void PrimitiveRenderer::drawCube(
  const float size,
  const Math::Float4& color)
{
  // 0 2.
  // 1 3.
  //
  // 4 7.
  // 5 6.

	float s = size / 2;
	float p[8][4] = {
		{ -s, +s, +s, 1 }, // 0
		{ -s, +s, -s, 1 }, // 1
		{ +s, +s, +s, 1 }, // 2
		{ +s, +s, -s, 1 }, // 3

		{ -s, -s, +s, 1 }, // 4
		{ -s, -s, -s, 1 }, // 5
		{ +s, -s, -s, 1 }, // 6
		{ +s, -s, +s, 1 }, // 7
	};

  float n[6][3] = {
    { 0, 1, 0 }, // up.
    { 0,-1, 0 }, // down.
    { 0, 0, 1 }, // far
    { 0, 0,-1 }, // near
    { 1, 0, 0 }, // right
    {-1, 0, 0 }, // left
  };

  uint32_t idx[6][2][3] = {
    { { 0, 1, 3 }, { 0, 3, 2 } }, // up
    { { 7, 5, 4 }, { 7, 6, 5 } }, // down
    { { 0, 2, 4 }, { 2, 7, 4 } }, // far
    { { 1, 5, 3 }, { 3, 5, 6 } }, // near
    { { 3, 6, 2 }, { 2, 6, 7 } }, // right
    { { 0, 4, 5 }, { 0, 5, 1 } }, // left
  };

  uint32_t iv[6*2*3];
  for (int i=0; i < 6*2*3; ++i) iv[i] = i;

  float tex[36][2] = { 0 };

  float pv[6][2][3][4];
  float nv[6][2][3][3];
  float c[6][2][3][4];
  for (int i=0; i < 6; ++i)
  {
    for (int j=0; j < 2; ++j)
    {
      for (int k=0; k < 3; ++k)
      {
        const uint32_t t = idx[i][j][k];

        // pv
        std::copy(p[t], p[t]+4, pv[i][j][k]);

        // nv
        std::copy(n[i], n[i]+3, nv[i][j][k]);

        // color
        c[i][j][k][0] = color.x_;
        c[i][j][k][1] = color.y_;
        c[i][j][k][2] = color.z_;
        c[i][j][k][3] = color.w_;
      }
    }
  }

  context_->setVertexPointer(4, kRM_FLOAT, sizeof(float)*4, pv);
  context_->setNormalPointer(kRM_FLOAT, sizeof(float)*3, nv);
  context_->setColorPointer(4, kRM_FLOAT, sizeof(float)*4, c);
  context_->setTexCoordPointer(2, kRM_FLOAT, sizeof(float)*2, tex);
  context_->drawElements(kRM_TRIANGLES, 6*2*3, kRM_UNSIGNED_INT, iv);
}

} // RomanceEngine
} // Render