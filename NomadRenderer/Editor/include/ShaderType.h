#pragma once
#include "RendererEditor.h"
#include "Assets/ExternalDbAsset.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_RENDERER_EDITOR_EXPORT Shader : public ExternalDbAsset
  {
  S_ENTITY(Shader, ExternalDbAsset)

public:
  virtual const char *extension() const X_OVERRIDE;
  Asset *defaultCreate(CreateInterface *c) X_OVERRIDE;
  };

}

}
