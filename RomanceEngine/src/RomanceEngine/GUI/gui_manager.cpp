#include <RomanceEngine/GUI/gui_manager.h>

namespace RomanceEngine {
namespace GUI {

void GUIManager::update()
{
  for (int i=0; i < (int)objects_.size(); ++i)
  {
    objects_[i]->update();
  }
}

void GUIManager::render(const Render::RenderContextPtr& context)
{
  for (int i=0; i < (int)objects_.size(); ++i)
  {
    objects_[i]->render(context);
  }
}

void GUIManager::add(const GUIObjectPtr& obj)
{
  objects_.push_back(obj);
}

void GUIManager::click(const Math::Float2& p)
{
  for (int i=0; i < (int)objects_.size(); ++i)
  {
    if (objects_[i]->getRegion().contains(p))
    {
      objects_[i]->click(p);
      break;
    }
  }
}

} // GUI
} // RomanceEngine

