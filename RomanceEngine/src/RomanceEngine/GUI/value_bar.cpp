#include <RomanceEngine/GUI/value_bar.h>
#include <RomanceEngine/Render/primitive_renderer.h>

namespace RomanceEngine {
namespace GUI {

void ValueBar::click(const Math::Float2& p)
{
  GUIObject::click(p);
  float w = getRegion().width();
  float pos = p.x_ - getRegion().left();
  value_ = pos / w;
}

void ValueBar::render(const Render::RenderContextPtr& context)
{
  Render::PrimitiveRenderer pr(context);
  pr.drawRect(region_, Math::Float4(0.5f, 0.5f, 0.5f, 1.0f));
  
  Math::Rect valueRect = region_;
  valueRect.size_.x_ *= value_;
  pr.drawRect(valueRect, Math::Float4(0.3f, 0.3f, 0.8f, 1.0f));
}
  

} // GUI
} // RomanceEngine