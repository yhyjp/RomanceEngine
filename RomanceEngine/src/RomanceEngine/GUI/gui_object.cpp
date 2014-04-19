#include <RomanceEngine/GUI/gui_object.h>

namespace RomanceEngine {
namespace GUI {

void GUIObject::click(const Math::Float2& p)
{
  clickEvent_(p);
};

} // GUI
} // RomanceEngine