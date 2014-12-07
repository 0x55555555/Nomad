#pragma once
#include "NObject.h"
#include "Mesh.h"
#include "Shader.h"

namespace Nomad
{

class Renderable : public Interface
  {
  S_ENTITY(Renderable, Interface)

public:
  enum
    {
    StaticChildMode = Shift::Container::StaticChildMode | Shift::NamedChildren
    };

  Renderable();

  Shift::ExternalUuidPointer mesh;
  Shift::ExternalUuidPointer shader;
  };

}
