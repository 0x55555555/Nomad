#pragma once
#include "RendererEditor.h"
#include "Assets/ExternalDbAsset.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_RENDERER_EDITOR_EXPORT VertexDescriptionType : public ExternalDbAsset
  {
  S_ENTITY(VertexDescriptionType, ExternalDbAsset)

public:
  virtual const char *extension() X_OVERRIDE;
  Asset *initialise(Shift::Set *a) X_OVERRIDE;
  };

}

}
