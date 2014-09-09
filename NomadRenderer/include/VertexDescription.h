#pragma once
#include "Renderer.h"
#include "NAsset.h"
#include "XShader.h"

namespace Nomad
{

class NOMAD_RENDERER_EXPORT VertexDescription : public Asset
  {
  S_ENTITY(VertexDescription, Asset)

public:
  typedef Eks::Vector<Eks::ShaderVertexLayoutDescription, 8> Layout;

  VertexDescription();

  const Layout& layout() const { return _layout; }

  void addElement(
    Eks::ShaderVertexLayoutDescription::Semantic s,
    Eks::ShaderVertexLayoutDescription::Format f);

  void bakeSemantics(Eks::Vector<Eks::ShaderVertexLayoutDescription::Semantic> *s);

private:
  Layout _layout;
  };

}
