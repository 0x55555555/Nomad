#include "AssetInterface.h"
#include "shift/TypeInformation/smodule.h"
#include "shift/Serialisation/sloader.h"
#include "shift/Serialisation/sjsonio.h"
#include "NReloadableAsset.h"
#include "QFile"

namespace Nomad
{

namespace Editor
{

Nomad::Asset *AssetInterface::loadPrimaryAsset(const char *str)
  {
  xAssert(manager());
  return load(str, manager()->assetContainer());
  }

Nomad::Asset *AssetInterface::load(const char *name, Shift::Array *parent)
  {
  QFile toLoad(name);
  if(!toLoad.open(QFile::ReadOnly))
    {
    xAssertFail();
    return nullptr;
    }

  Shift::LoadBuilder builder;
  Eks::TemporaryAllocator alloc(parent->temporaryAllocator());
  auto loading = builder.beginLoading(parent, &alloc);

  Shift::JSONLoader loader;
  loader.load(&toLoad, &builder);

  xAssert(loading->loadedData().size() == 1);
  if (loading->loadedData().size() != 1)
    {
    return nullptr;
    }

  auto asset = loading->loadedData().front()->castTo<Nomad::Asset>();
  manager()->registerAsset(asset);

  return asset;
  }

void AssetInterface::triggerReload(const QUuid &uuid)
  {
  _toReload.insert(uuid);
  }

Nomad::Asset *AssetInterface::load(const QUuid &name, Shift::Array *parent)
  {
  _toReload.erase(name);

  QString path = name.toString();

  path = ":/resources/" + path + ".json";

  return load(path.toUtf8().data(), parent);
  }

bool AssetInterface::requiresReload(const QUuid &id)
  {
  return _toReload.find(id) != _toReload.end();
  }

}

}
