#include "Application.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Serialisation/sjsonio.h"
#include "QFile"
#include "QDebug"
#include "QMessageBox"

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
  block.add(&ApplicationDatabase::projectUserData, "projectUserData");

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
  }

Application::~Application()
  {
  }

void Application::create(File *f)
  {
  f->setSaved();
  }

File *Application::load(const Eks::String &path, Shift::Set *parent)
  {
  QFile toLoad(path.data());
  if(toLoad.open(QFile::ReadOnly) == false)
    {
    qWarning() << "failed to open asset" << path.data();
    return nullptr;
    }

  auto ent = loadSource(toLoad.readAll(), parent);
  if(!ent || !ent->uncheckedCastTo<File>())
    {
    return nullptr;
    }

  auto file = ent->castTo<File>();
  file->setPath(path);
  file->setSaved();

  return file;
  }

Shift::Entity *Application::loadSource(const QByteArray &source, Shift::Set *parent)
  {
  Eks::String toLoadStr = source.data();

  Shift::LoadBuilder builder;
  Eks::TemporaryAllocator alloc(parent->temporaryAllocator());
  auto loading = builder.beginLoading(parent, &alloc);

  Shift::JSONLoader loader;
  loader.load(&toLoadStr, &builder);

  if(loading->loadedData().size() != 1)
    {
    return nullptr;
    }

  return loading->loadedData()[0]->castTo<Nomad::File>();
  }

bool Application::save(File *file)
  {
  QString path(file->path().data());
  QFile toSave(path);
  if(toSave.open(QFile::WriteOnly) == false)
    {
    qWarning() << "failed to save asset" << path;
    return false;
    }

  toSave.write(toSource(file));
  file->setSaved();
  return true;
  }


QByteArray Application::toSource(Shift::Entity *file)
  {
  Shift::SaveBuilder builder;

  Shift::JSONSaver writer;
  writer.setAutoWhitespace(true);

  Eks::String toSaveStr;
    {
    auto block = writer.beginWriting(&toSaveStr);

    builder.save(file, true, &writer);
    }

  return toSaveStr.data();
  }

Application::FileResult Application::offerToSave(Nomad::File *file)
  {
  if (!file->hasChangedFromFile())
    {
    return Success;
    }

  auto result = QMessageBox::question(
        nullptr,
        "Save changes",
        tr("Would you like to save changes to %1").arg(file->path().data()),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
  if (result == QMessageBox::Cancel)
    {
    return Cancel;
    }
  else if (result == QMessageBox::No)
    {
    return Success;
    }

  return save(file) == true ? Success : Failure;
  }

}

}

