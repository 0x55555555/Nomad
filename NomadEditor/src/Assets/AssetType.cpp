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

  auto parent = childBlock.add(&AssetType::_assetParent, "_assetParent");
  parent->setNeverSave(true);
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

QWidget *AssetType::createPreview(UIInterface *)
  {
  return nullptr;
  }

void AssetType::save()
  {
  Application::save(this);
  }

AssetType *AssetType::create(
    const Shift::PropertyInformation *info,
    const QString &location,
    Shift::Set *handleParent,
    Shift::Set *assetParent,
    ProjectInterface *ifc,
    CreateInterface *c)
  {
  AssetType *a = handleParent->add(info)->uncheckedCastTo<AssetType>();
  if (!a)
    {
    return nullptr;
    }

  a->_assetParent.setPointed(assetParent);
  a->setPath(location.toUtf8().data(), ifc);
  a->createNewAsset(location, c);

  a->save();
  return a;
  }

AssetType *AssetType::load(
    const QString &location,
    Shift::Set *parent,
    Shift::Set *assetParent,
    ProjectInterface *ifc,
    CreateInterface *
    )
  {
  auto type = Application::load(location.toUtf8().data(), parent);
  if(!type)
    {
    return nullptr;
    }

  auto asset = type->castTo<AssetType>();
  if(!asset)
    {
    return nullptr;
    }

  asset->setPath(location.toUtf8().data(), ifc);
  asset->_assetParent.setPointed(assetParent);
  return asset;
  }

}

}
