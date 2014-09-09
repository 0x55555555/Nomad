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

void ExternalAssetType::createNewAsset(const QString &l, CreateInterface *c)
  {
  Asset *a = defaultCreate(c);

  _uuid = a->uuid();
  _asset = a;
  saveContents(l);
  }

Asset *ExternalAssetType::initialiseFromSource(const QByteArray &src, CreateInterface *c)
  {
  Asset *a = process(src, c);
  _uuid = a->uuid();
  _asset = a;

  return a;
  }

Asset *ExternalAssetType::asset(const QString &l, CreateInterface *c)
  {
  if(auto p = _asset())
    {
    return p;
    }

  QFile f(contentsPath(l));
  if (!f.open(QFile::ReadOnly))
    {
    return nullptr;
    }

  QByteArray data = f.readAll();
  return initialiseFromSource(data, c);
  }

void ExternalAssetType::save()
  {
  AssetType::save();
  saveContents(path().data());
  }

void ExternalAssetType::clear()
  {
  }

Asset *ExternalAssetType::cachedAsset()
  {
  return _asset();
  }

void ExternalAssetType::saveContents(const QString &assFile)
  {
  auto ass = _asset();
  if (!ass)
    {
    return;
    }

  QString path = contentsPath(assFile);

  QFile f(path);
  if (!f.open(QFile::WriteOnly))
    {
    return;
    }

  QString data = unprocess(ass);

  f.write(data.toUtf8());
  }

QString ExternalAssetType::contentsPath(const QString &assFile)
  {
  QFileInfo f(assFile);
  return f.path() + "/"+ f.baseName() + "." + extension();
  }

}

}
