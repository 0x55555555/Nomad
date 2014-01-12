#pragma once
#include "NGlobal.h"
#include "XCore.h"
#include "QApplication"
#include "QMainWindow"
#include "shift/sdatabase.h"
#include <set>


namespace Nomad
{

namespace Editor
{

class ApplicationDatabase : public Shift::Database
  {
public:
  ApplicationDatabase()
    {
    initiateInheritedDatabaseType(staticTypeInformation());
    }
  };


class Application : public QApplication
  {
public:
  Application(int argc, char **argv);

private:
  Eks::Core _core;
  QMainWindow _window;
  Shift::TypeRegistry _registry;
  ApplicationDatabase _database;
  };

}

}
