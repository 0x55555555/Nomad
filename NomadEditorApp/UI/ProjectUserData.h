#pragma once
#include <QMainWindow>
#include "UI/UIGlobal.h"
#include "XGlobal.h"
#include "NProject.h"
#include "shift/Properties/sdata.inl"
#include <Containers/XUnorderedMap.h>

namespace Ui {
class MainWindow;
}

namespace Nomad
{
namespace Editor
{

class ProjectUserData : public Nomad::File
  {
  S_ENTITY(ProjectUserData, File)

  Shift::StringProperty layout;
  Shift::Array openFiles;
  };

}
}

