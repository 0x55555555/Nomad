#pragma once
#include "Assets/ExternalDbAsset.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_EXPORT Shader : public ExternalDbAsset
  {
  S_ENTITY(Shader, ExternalDbAsset)

public:
  virtual const char *extension() X_OVERRIDE;
  Asset *initialise(Shift::Set *a) X_OVERRIDE;
  };

}

}
