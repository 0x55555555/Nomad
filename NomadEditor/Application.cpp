#include "Application.h"
#include "UI/MainWindow.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Serialisation/sjsonio.h"
#include "QFile"
#include "QDebug"

S_IMPLEMENT_MODULE(NomadEditor)

namespace Nomad
{

namespace Editor
{

ModuleHelper::ModuleHelper(Shift::TypeRegistry *reg)
  {
  reg->installModule(Nomad::shiftModule());
  reg->installModule(NomadEditor::shiftModule());
  }

S_IMPLEMENT_PROPERTY(ApplicationDatabase, NomadEditor)

void ApplicationDatabase::createTypeInformation(
      Shift::PropertyInformationTyped<ApplicationDatabase> *info,
      const Shift::PropertyInformationCreateData &data)
  {
  auto block = info->createChildrenBlock(data);

  block.add(&ApplicationDatabase::project, "project");

  block.add(&ApplicationDatabase::scratch, "scratch");
  }

Application::Application(int argc, char **argv)
    : QApplication(argc, argv),
      _registry(_core.defaultAllocator()),
      _modules(&_registry)
  {
  QCoreApplication::setOrganizationName("NomadSoftware");
  QCoreApplication::setOrganizationDomain("github.com/jorj1988/Nomad");
  QCoreApplication::setApplicationName("NomadEditor");

  _window = new MainWindow(&_database);
  _window->show();
  }

Application::~Application()
  {
  delete _window;
  }

File *Application::load(const Eks::String &path, Shift::Entity *parent)
  {
  QFile toLoad(path.data());
  if(toLoad.open(QFile::ReadOnly) == false)
    {
    qWarning() << "failed to open project" << path.data();
    return nullptr;
    }

  Eks::String toLoadStr = toLoad.readAll().data();

  Shift::LoadBuilder builder;
  Eks::TemporaryAllocator alloc(parent->children.temporaryAllocator());
  auto loading = builder.beginLoading(&parent->children, &alloc);

  Shift::JSONLoader loader;
  loader.load(&toLoadStr, &builder);

  if(loading->loadedData().size() != 1)
    {
    return nullptr;
    }
  auto file = loading->loadedData()[0]->castTo<Nomad::Project>();
  if(!file)
    {
    return nullptr;
    }

  file->setPath(path);

  return file;
  }

bool Application::save(File *file)
  {
  QString path(file->path().data());
  QFile toSave(path);
  if(toSave.open(QFile::WriteOnly) == false)
    {
    qWarning() << "failed to save project" << path;
    return false;
    }

  Shift::SaveBuilder builder;

  Shift::JSONSaver writer;
  writer.setAutoWhitespace(true);

  Eks::String toSaveStr;
    {
    auto block = writer.beginWriting(&toSaveStr);

    builder.save(file, true, &writer);
    }

  toSave.write(toSaveStr.data());
  return true;
  }

}

}

int main(int argc, char **argv)
  {
  Nomad::Editor::Application app(argc, argv);

  return app.exec();
  }
