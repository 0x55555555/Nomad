#pragma once
#include "NGlobal.h"
#include "XCore.h"
#include "shift/TypeInformation/styperegistry.h"
#include "NAssetManager.h"
#include "NAsset.h"
#include <set>


namespace Nomad
{

namespace Editor
{

class AssetInterface : public Nomad::AssetManager::Interface
  {
public:
  AssetInterface();

  template <typename T> T *loadPrimaryAsset(const char *str)
    {
    auto ass = loadPrimaryAsset(str);
    xAssert(ass);
    return ass ? ass->castTo<T>() : nullptr;
    }

  Nomad::Asset *loadPrimaryAsset(const char *str);
  Nomad::Asset *load(const char *name, Shift::Array *parent);

  void triggerReload(const QUuid &uuid);

private:
  Nomad::Asset *load(const QUuid &name, Shift::Array *parent) X_OVERRIDE;
  bool requiresReload(const QUuid &id) X_OVERRIDE;

  std::set<QUuid> _toReload;
  };

}

}
