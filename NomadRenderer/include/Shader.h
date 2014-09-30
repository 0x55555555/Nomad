#pragma once
#include "Renderer.h"
#include "NAsset.h"
#include "XShader.h"
#include "shift/Properties/sbaseproperties.h"
#include "ShaderVertexComponent.h"

namespace Nomad
{

class NOMAD_RENDERER_EXPORT Shader : public Asset
  {
  S_ENTITY(Shader, Asset)

public:
  Eks::Shader &shader() { return _shader; }
  const Eks::Shader &shader() const { return _shader; }

  ShaderVertexComponent *vertexComponent() { return _vertexShader.pointed<ShaderVertexComponent>(); }
  const ShaderVertexComponent *vertexComponent() const { return _vertexShader.pointed<ShaderVertexComponent>(); }
  void setVertexComponent(ShaderVertexComponent *l) { _vertexShader.setPointed(l); }

private:
  Eks::Shader _shader;
  Shift::ExternalUuidPointer _vertexShader;
  };

}
