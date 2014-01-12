#include "Application.h"

namespace Nomad
{

namespace Editor
{

Application::Application(int argc, char **argv)
    : QApplication(argc, argv),
      _registry(_core.defaultAllocator())
  {
  _window.show();
  }

}

}

int main(int argc, char **argv)
  {
  Nomad::Editor::Application app(argc, argv);

  return app.exec();
  }
