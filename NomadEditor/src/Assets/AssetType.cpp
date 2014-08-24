#include "Assets/AssetType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sarray.h"
#include "shift/Serialisation/sloader.h"
#include "shift/Serialisation/sjsonio.h"
#include "NProject.h"
#include "Application.h"
#include "ProjectInterface.h"
#include "QFile"
#include "QDir"
#include "QDebug"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(AssetType, Nomad)

void AssetType::createTypeInformation(
    Shift::PropertyInformationTyped<AssetType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  childBlock.add(&AssetType::_uuid, "uuid");
  }

QString AssetType::relativePath() const
  {
  QDir s(QFileInfo(project()->getCurrentProject()->path().data()).dir());

  return s.relativeFilePath(path().data());
  }

void AssetType::setPath(const Eks::String &s, ProjectInterface *ifc)
  {
  File::setPath(s);
  _project = ifc;
  }

QWidget *AssetType::createPreview()
  {
  return nullptr;
  }

AssetType *AssetType::create(
    const Shift::PropertyInformation *info,
    Shift::Set *handleParent,
    Shift::Set *assetParent,
    const QString &location,
    ProjectInterface *ifc)
  {
  AssetType *a = handleParent->add(info)->uncheckedCastTo<AssetType>();
  if (!a)
    {
    return nullptr;
    }

  a->setPath(location.toUtf8().data(), ifc);
  a->initialise(assetParent, location);

  a->save();
  return a;
  }

void AssetType::save()
  {
  Application::save(this);
  }

AssetType *AssetType::load(const QString &location, Shift::Set *parent)
  {
  auto type = Application::load(location.toUtf8().data(), parent);
  if(!type)
    {
    return nullptr;
    }

  return type->castTo<AssetType>();
  }

}

}
