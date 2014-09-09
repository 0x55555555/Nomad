#pragma once
#include "Renderer.h"
#include "NAsset.h"
#include "XGeometry.h"

namespace Nomad
{

class NOMAD_RENDERER_EXPORT Mesh : public Asset
  {
  S_ENTITY(Mesh, Asset)

public:
  Eks::Geometry &geometry() { return _geo; }

  const Eks::Geometry &geometry() const { return _geo; }

private:
  Eks::Geometry _geo;
  };

}
