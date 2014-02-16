#ifndef UI_GLBOAL_H
#define UI_GLBOAL_H

namespace Shift
{
class Array;
}

namespace Nomad
{
class Project;

namespace Editor
{

class ProjectInterface
  {
public:
  virtual Nomad::Project *getCurrentProject() = 0;

  virtual Shift::Array *getScratchParent() = 0;

  virtual void addProjectChanged(QObject *obj, const char *slot) = 0;
  };

}
}

#endif // UI_GLBOAL_H
