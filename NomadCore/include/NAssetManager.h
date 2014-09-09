#pragma once
#include "shift/sentity.h"
#include "shift/Properties/sarray.h"
#include "sexternalpointer.h"
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
  ~AssetManagerInterface();

  virtual Asset *load(Shift::Set *parent, const QUuid &name) = 0;
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
  ~AssetManager();

  void clear();
  void reset(Interface *ifc);
  void reloadRequiredAssets();

  Asset *resolveAsset(
    const QUuid &id,
    Shift::ExternalPointerInstanceInformation::ResolveResult *res);

  Asset *createAsset(const Shift::PropertyInformation *info);
  template <typename T> T *createAsset()
    {
    return createAsset(T::staticTypeInformation())->template uncheckedCastTo<T>();
    }

  Shift::Set *assetParent() { return &assets; }
  void registerAsset(Asset *asset);

private:
  Shift::Set assets;
  Eks::UnorderedMap<QUuid, Asset *> _assets;
  };

}
