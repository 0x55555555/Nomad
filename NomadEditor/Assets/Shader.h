#pragma once
#include "ExternalAssetType.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_EXPORT Shader : public ExternalAssetType
  {
  S_ENTITY(Shader, ExternalAssetType)

public:
  virtual const char *extension() X_OVERRIDE;
  virtual Asset *process(Shift::Array *parent, const QString &source) X_OVERRIDE;
  virtual QString unprocess(Asset *a) X_OVERRIDE;
  virtual Asset *initialise(Shift::Array *parent) X_OVERRIDE;
  };

}

}
