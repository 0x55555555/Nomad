#include "Assets/ExternalAssetType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
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

void ExternalAssetType::initialise(Shift::Array *p, const QString &l)
  {
  Asset *a = initialise(p);

  _asset = a;
  saveContents(l);
  }

Asset *ExternalAssetType::asset(Shift::Array *p, const QString &l)
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

  QString data = f.readAll();

  Asset *a = process(p, data);
  _asset = a;
  return a;
  }

void ExternalAssetType::save(const QString &location)
  {
  AssetType::save(location);
  saveContents(location);
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
