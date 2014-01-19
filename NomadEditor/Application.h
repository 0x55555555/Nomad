#pragma once
#include "NGlobal.h"
#include "XCore.h"
#include "QApplication"
#include "QMainWindow"
#include "shift/sdatabase.h"
#include "shift/Properties/sbasepointerproperties.h"
#include "NProject.h"
#include <set>

namespace Nomad
{

namespace Editor
{

class MainWindow;

class ModuleHelper
  {
public:
  ModuleHelper(Shift::TypeRegistry *reg);
  };

class ApplicationDatabase : public Shift::Database
  {
  S_ENTITY(ApplicationDatabase, Database)

public:
  ApplicationDatabase()
    {
    initiateInheritedDatabaseType(staticTypeInformation());
    }

  Shift::Pointer project;
  };


class Application : public QApplication
  {
public:
  Application(int argc, char **argv);
  ~Application();

  static File *load(const Eks::String &path, Shift::Entity *parent);
  static bool save(Nomad::File *file);

private:
  Eks::Core _core;
  MainWindow *_window;
  Shift::TypeRegistry _registry;
  ModuleHelper _modules;
  ApplicationDatabase _database;
  };

}

}

S_MODULE(, NomadEditor)
