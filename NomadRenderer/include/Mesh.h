#pragma once
#include "Renderer.h"
#include "NAsset.h"
#include "XGeometry.h"
#include "VertexDescription.h"

namespace Nomad
{

class NOMAD_RENDERER_EXPORT Mesh : public Asset
  {
  S_ENTITY(Mesh, Asset)

public:
  Eks::Geometry &geometry() { return _geo; }
  const Eks::Geometry &geometry() const { return _geo; }

  VertexDescription *layoutDescription() { return _layoutDescription.pointed<VertexDescription>(); }
  const VertexDescription *layoutDescription() const { return _layoutDescription.pointed<VertexDescription>(); }
  void setLayoutDescription(VertexDescription *l) { _layoutDescription.setPointed(l); }

private:
  Eks::Geometry _geo;
  Shift::ExternalUuidPointer _layoutDescription;
  };

}
