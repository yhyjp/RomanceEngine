#ifndef __RM_GUI_GUI_OBJECT__D98FE9FDA99E41DB84B4F03C03E0DBBA__
#define __RM_GUI_GUI_OBJECT__D98FE9FDA99E41DB84B4F03C03E0DBBA__

#include <boost/signals2/signal.hpp>
#include <RomanceEngine/Math/float1234.h>
#include <RomanceEngine/Math/rect.h>
#include <RomanceEngine/Render/render_context.h>

namespace RomanceEngine {
namespace GUI {

class GUIObject
{
public:

  GUIObject() {}
  virtual ~GUIObject() {};

  virtual Math::Rect getRegion() const { return region_; }
  virtual void setRegion(const Math::Rect& region) { region_ = region; }

  virtual void render(const Render::RenderContextPtr& context) {}
  virtual void update() {};

  virtual void click(const Math::Float2& p);

public:
  boost::signals2::signal<void(const Math::Float2&)> clickEvent_;

protected:
  Math::Rect region_;
};
typedef Memory::SharedPtr<GUIObject> GUIObjectPtr;

} // GUI
} // RomanceEngine

#endif // __RM_GUI_GUI_OBJECT__D98FE9FDA99E41DB84B4F03C03E0DBBA__
