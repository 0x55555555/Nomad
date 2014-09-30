#pragma once
#include "Renderer.h"
#include "NAsset.h"
#include "XShader.h"
#include "VertexDescription.h"

namespace Nomad
{

class NOMAD_RENDERER_EXPORT ShaderVertexComponent : public Asset
  {
  S_ENTITY(ShaderVertexComponent, Asset)

public:
  Eks::ShaderVertexComponent &component() { return _comp; }
  const Eks::ShaderVertexComponent &component() const { return _comp; }

  Eks::ShaderVertexLayout &layout() { return _layout; }
  const Eks::ShaderVertexLayout &layout() const { return _layout; }

  VertexDescription *layoutDescription() { return _layoutDescription.pointed<VertexDescription>(); }
  const VertexDescription *layoutDescription() const { return _layoutDescription.pointed<VertexDescription>(); }
  void setLayoutDescription(VertexDescription *l) { _layoutDescription.setPointed(l); }

private:
  Eks::ShaderVertexComponent _comp;
  Eks::ShaderVertexLayout _layout;
  Shift::ExternalUuidPointer _layoutDescription;
  };

}
