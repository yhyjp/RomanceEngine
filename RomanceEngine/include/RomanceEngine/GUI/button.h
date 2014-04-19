#ifndef __RM_GUI_BUTTON__D98FE9FDA99E41DB84B4F03C03E0DBBA__
#define __RM_GUI_BUTTON__D98FE9FDA99E41DB84B4F03C03E0DBBA__

#include "gui_object.h"
#include <RomanceEngine/Render/render_context.h>

namespace RomanceEngine {
namespace GUI {

class Button : public GUIObject
{
public:
  virtual ~Button() {}
  virtual void render(const Render::RenderContextPtr& context);
};

} // GUI
} // RomanceEngine

#endif // __RM_GUI_BUTTON__D98FE9FDA99E41DB84B4F03C03E0DBBA__
