#pragma once
#include "NObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "RCRenderable.h"

namespace Nomad
{

class RenderableInstance : public RCRenderable
  {
  S_ENTITY(RenderableInstance, RCRenderable)

public:
  TransformProperty transform;

  void render(Eks::Renderer *r, const RenderState &state) const X_OVERRIDE;
  };

class Renderable : public Interface
  {
  S_ENTITY(Renderable, Interface)

public:
  enum
    {
    StaticChildMode = Shift::Container::StaticChildMode | Shift::NamedChildren
    };

  Shift::ExternalUuidPointer mesh;
  Shift::ExternalUuidPointer shader;

  void onInstance(ObjectInstance *) X_OVERRIDE;
  };

}
