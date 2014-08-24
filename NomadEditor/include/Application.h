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

class ProjectUserData;

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

  Shift::TypedPointer<Project> project;
  Shift::TypedPointer<ProjectUserData> projectUserData;
  Shift::Array scratch;
  };


class Application : public QApplication
  {
public:
  Application(int argc, char **argv);
  ~Application();

  enum FileResult
    {
    Cancel,
    Success,
    Failure
    };

  static void create(File *);
  static File *load(const Eks::String &path, Shift::Set *parent);
  static Shift::Entity *loadSource(const QByteArray &contents, Shift::Set *parent);
  static bool save(Nomad::File *file);
  static QByteArray toSource(Shift::Entity *file);
  static FileResult offerToSave(Nomad::File *file);

  ApplicationDatabase *database() { return &_database; }

private:
  Eks::Core _core;
  Shift::TypeRegistry _registry;
  ModuleHelper _modules;
  ApplicationDatabase _database;
  };

}

}

S_MODULE(, NomadEditor)
