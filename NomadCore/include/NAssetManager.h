#pragma once
#include "shift/sentity.h"
#include "shift/Properties/sarray.h"
#include "shift/Properties/sexternalpointer.h"
#include "NGlobal.h"

namespace Nomad
{

class Asset;
class AssetManager;

class NOMAD_EXPORT AssetManagerInterface
  {
XProperties:
  XROProperty(AssetManager *, manager);

public:
  virtual Asset *load(const QUuid &name, Shift::Array *parent) = 0;
  virtual bool requiresReload(const QUuid &id) = 0;


protected:
  AssetManagerInterface();

private:
  friend class AssetManager;
  };


class NOMAD_EXPORT AssetManager : public Shift::Entity
  {
  S_ENTITY(AssetManager, Entity)

public:
  typedef AssetManagerInterface Interface;

XProperties:
  XROProperty(Interface *, assetInterface);

public:
  AssetManager();

  void reloadRequiredAssets();

  void setInterface(Interface *ifc);
  Shift::Array *assetContainer();

  Asset *resolveAsset(
    const QUuid &id,
    Shift::ExternalPointerInstanceInformation::ResolveResult *res);

  template <typename T> T *createAsset()
    {
    auto t = assets.add<T>();
    registerAsset(t);
    return t;
    }

  void registerAsset(Asset *asset);

private:
  Shift::Array assets;
  Eks::UnorderedMap<QUuid, Asset *> _assets;
  };

}
