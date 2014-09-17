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
    : _needsSave(false)
  {
  }

void ExternalAssetType::createNewAsset(CreateInterface *c)
  {
  Asset *a = defaultCreate(c);

  _uuid = a->uuid();
  _asset = a;
  _needsSave = false;
  save();
  }

Asset *ExternalAssetType::initialiseFromSource(const QByteArray &src, CreateInterface *c)
  {
  Asset *a = process(src, c);
  _uuid = a->uuid();
  _asset = a;
  _needsSave = false;
  setSaved();

  return a;
  }

Asset *ExternalAssetType::asset(CreateInterface *c)
  {
  if(auto p = _asset())
    {
    return p;
    }

  QFile f(contentsPath(path().data()));
  if (!f.open(QFile::ReadOnly))
    {
    return nullptr;
    }

  QByteArray data = f.readAll();
  return initialiseFromSource(data, c);
  }

bool ExternalAssetType::save()
  {
  return AssetType::save() && saveContents(path().data());
  }

bool ExternalAssetType::needsSave()
  {
  return AssetType::needsSave() || _needsSave;
  }

void ExternalAssetType::clear()
  {
  }

Asset *ExternalAssetType::cachedAsset()
  {
  return _asset();
  }

void ExternalAssetType::setNeedsSave()
  {
  _needsSave = true;
  }

bool ExternalAssetType::saveContents(const QString &assFile)
  {
  auto ass = _asset();
  if (!ass)
    {
    _needsSave = false;
    return true;
    }

  QString path = contentsPath(assFile);

  QFile f(path);
  if (!f.open(QFile::WriteOnly))
    {
    return false;
    }

  QString data = unprocess(ass);

  f.write(data.toUtf8());
  _needsSave = false;
  return true;
  }

QString ExternalAssetType::contentsPath(const QString &assFile)
  {
  QFileInfo f(assFile);
  return f.path() + "/"+ f.baseName() + "." + extension();
  }

}

}
