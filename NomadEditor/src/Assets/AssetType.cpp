#include "Assets/AssetType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sarray.h"
#include "shift/Serialisation/sloader.h"
#include "shift/Serialisation/sjsonio.h"
#include "Utilities/XParseException.h"
#include "NProject.h"
#include "NAsset.h"
#include "NAssetManager.h"
#include "Application.h"
#include "ProjectInterface.h"
#include "QFile"
#include "QDir"
#include "QDebug"
#include "QMessageBox"

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

AssetType::AssetType()
    : _requiresReload(false),
      _messages(Shift::TypeRegistry::generalPurposeAllocator())
  {
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

QWidget *AssetType::createPreview(UIInterface *, CreateInterface *)
  {
  return nullptr;
  }

bool AssetType::save()
  {
  return Application::save(this);
  }

bool AssetType::needsSave()
  {
  return hasChangedFromFile();
  }

void AssetType::clear()
  {
  _messages.clear();
  emit messagesChanged();
  }

Application::FileResult AssetType::offerToSave()
  {
  if (!needsSave())
    {
    return Application::Success;
    }

  auto result = QMessageBox::question(
        nullptr,
        "Save changes",
        QObject::tr("Would you like to save changes to asset %1").arg(path().data()),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
  if (result == QMessageBox::Cancel)
    {
    return Application::Cancel;
    }
  else if (result == QMessageBox::No)
    {
    return Application::Success;
    }

  return save() == true ? Application::Success : Application::Failure;
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
  a->createNewAsset(c);

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
  asset->setSaved();
  return asset;
  }

void AssetType::setRequiresReload(bool r)
  {
  _requiresReload = r;
  emit requiresReloadChanged();
  }

void AssetType::markDependantsForReload()
  {
  Shift::Property *ass = asset();
  if (!ass)
    {
    return;
    }

  for(auto op = ass->output(); op; op = op->nextOutput())
    {
    Shift::Property* owner = op;
    while(owner)
      {
      QUuid id;
      Asset *a = owner->castTo<Asset>();
      if(a)
        {
        id = a->uuid();
        }
      else if(auto t = owner->castTo<AssetType>())
        {
        id = t->uuid();
        }

      if(!id.isNull())
        {
        if (id != uuid())
          {
          project()->getAssetManager()->markForReload(id);
          }
        break;
        }

      owner = owner->parent();
      }
    }
  }

void AssetType::addError(const Eks::DetailedCodeLocation &d, const Eks::String &m)
  {
  auto &b = _messages.createBack();
  b.type = Message::Error;
  b.location = d;
  b.message = m;
  emit messagesChanged();
  }

void AssetType::addError(const Eks::ParseError &e)
  {
  auto &b = _messages.createBack();
  b.type = Message::Error;
  b.location = e.location();
  b.context = e.fileLocation();
  b.message = e.message();
  emit messagesChanged();
  }

void AssetType::addWarning(const Eks::ParseError &e)
  {
  auto &b = _messages.createBack();
  b.type = Message::Warning;
  b.location = e.location();
  b.context = e.fileLocation();
  b.message = e.message();
  emit messagesChanged();
  }
}

}
