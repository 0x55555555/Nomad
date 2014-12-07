#include "Assets/ExternalAssetType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sdata.inl"
#include "NAsset.h"
#include "QFile"
#include "QFileInfo"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ExternalAssetType, Nomad)

void ExternalAssetType::createTypeInformation(
    Shift::PropertyInformationTyped<ExternalAssetType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  auto asset = childBlock.add(&ExternalAssetType::_asset, "asset");
  asset->setNeverSave(true);
  }

ExternalAssetType::ExternalAssetType()
    : _needsSave(false),
      _failedLoad(false)
  {
  }

void ExternalAssetType::createNewAsset(CreateInterface *c)
  {
  Asset *a = defaultCreate(c);

  _failedLoad = a == nullptr;
  if (a)
    {
    _assetUuid = a->uuid();
    _asset = a;
    }

  _needsSave = false;
  save();
  }
  
void ExternalAssetType::reload(CreateInterface *c)
  {
  reinitialise(source(), c);
  }

Asset *ExternalAssetType::reinitialise(const QByteArray &src, CreateInterface *c)
  {
  setRequiresReload(false);
  markDependantsForReload();
  clear();
  auto a = initialiseFromSource(src, c);
  setNeedsSave();
  return a;
  }

Asset *ExternalAssetType::initialiseFromSource(const QByteArray &src, CreateInterface *c)
  {
  xAssert(!_asset());
  Asset *a = process(src, c);
  if(!a)
    {
    _failedLoad = true;
    return nullptr;
    }

  _failedLoad = false;
  _assetUuid = a->uuid();
  _asset = a;
  _needsSave = false;
  setSaved();


  return a;
  }

Asset *ExternalAssetType::asset(CreateInterface *c)
  {
  if (_failedLoad)
    {
    xAssert(!_asset());
    return nullptr;
    }

  if(auto p = _asset())
    {
    return p;
    }

  if (!c)
    {
    return nullptr;
    }

  QByteArray src = source();
  auto asset = initialiseFromSource(src, c);
  xAssert(!asset || asset->uuid() == assetUuid());

  return asset;
  }

QByteArray ExternalAssetType::getDiskSource() const
  {
  QFile f(contentsPath(path().data()));
  if (!f.open(QFile::ReadOnly))
    {
    return nullptr;
    }

  return f.readAll();
  }

QByteArray ExternalAssetType::source() const
  {
  return getDiskSource();
  }

bool ExternalAssetType::save()
  {
  return AssetType::save() && saveContents(path().data());
  }

bool ExternalAssetType::needsSave()
  {
  return AssetType::needsSave() || _needsSave;
  }

Asset *ExternalAssetType::cachedAsset()
  {
  return _asset();
  }

const Asset *ExternalAssetType::cachedAsset() const
  {
  return _asset();
  }

void ExternalAssetType::setNeedsSave()
  {
  _needsSave = true;
  }

bool ExternalAssetType::saveContents(const QString &assFile)
  {
  QString path = contentsPath(assFile);

  QFile f(path);
  if (!f.open(QFile::WriteOnly))
    {
    return false;
    }

  QString data = unprocess();

  f.write(data.toUtf8());
  _needsSave = false;
  return true;
  }

QString ExternalAssetType::contentsPath(const QString &assFile) const
  {
  QFileInfo f(assFile);
  return f.path() + "/"+ f.baseName() + "." + extension();
  }

}

}
