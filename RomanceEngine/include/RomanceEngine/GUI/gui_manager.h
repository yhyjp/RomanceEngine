#ifndef __RM_GUI_MANAGER__2E24725EFD7A485EA1BCFA53B08B0DCA__
#define __RM_GUI_MANAGER__2E24725EFD7A485EA1BCFA53B08B0DCA__

#include "gui_object.h"
#include <RomanceEngine/Render/render_context.h>
#include <RomanceEngine/Math/float1234.h>

namespace RomanceEngine {
namespace GUI {

class GUIManager
{
public:
  GUIManager() {}
  ~GUIManager() {}

  void update();
  void render(const Render::RenderContextPtr& context);
  void add(const GUIObjectPtr& obj);
  void click(const Math::Float2& p);

private:
  std::vector<GUIObjectPtr> objects_;
};


} // GUI
} // RomanceEngine

#endif // __RM_GUI_MANAGER__2E24725EFD7A485EA1BCFA53B08B0DCA__
