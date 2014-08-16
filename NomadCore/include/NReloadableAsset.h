#pragma once
#include "NAsset.h"

namespace Nomad
{

class NOMAD_EXPORT ReloadableAsset : public Asset
  {
  S_ABSTRACT_ENTITY(ReloadableAsset, Asset)

public:
  virtual bool preReload();
  virtual bool postReload(ReloadableAsset *oldAsset);
  };

}
