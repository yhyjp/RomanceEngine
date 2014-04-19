#ifndef __RM_GUI_VALUE_BAR__7648B2A493964BB180FD829B2C61991A__
#define __RM_GUI_VALUE_BAR__7648B2A493964BB180FD829B2C61991A__

#include "gui_object.h"
#include <RomanceEngine/Render/render_context.h>

namespace RomanceEngine {
namespace GUI {

class ValueBar : public GUIObject
{
public:
  ValueBar(const float value=0.0f)
    : value_(value)
  {}
  virtual ~ValueBar() {}

  virtual void click(const Math::Float2& p);
  virtual void render(const Render::RenderContextPtr& context);
  
  float getValue() const { return value_; }
  void setValue(const float value) { value_ = value; }

private:
  float value_;
};

} // GUI
} // RomanceEngine

#endif // __RM_GUI_VALUE_BAR__7648B2A493964BB180FD829B2C61991A__
