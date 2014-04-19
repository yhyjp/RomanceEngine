#include <RomanceEngine/GUI/button.h>
#include <RomanceEngine/Render/primitive_renderer.h>

namespace RomanceEngine {
namespace GUI {

void Button::render(const Render::RenderContextPtr& context)
{
  Render::PrimitiveRenderer pr(context);
  pr.drawRect(region_, Math::Float4(1, 1, 0, 0.5));
}

} // GUI
} // RomanceEngine