#pragma once
#include "Renderer.h"
#include "NAsset.h"
#include "XShader.h"

namespace Nomad
{

class NOMAD_RENDERER_EXPORT ShaderComponent : public Asset
  {
  S_ENTITY(ShaderComponent, Asset)

public:
  Eks::ShaderComponent &component() { return _comp; }
  const Eks::ShaderComponent &component() const { return _comp; }

private:
  Eks::ShaderComponent _comp;
  };

}
