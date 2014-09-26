#include "ShaderVertexComponent.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(ShaderVertexComponent, NomadRenderer)

void ShaderVertexComponent::createTypeInformation(
    Shift::PropertyInformationTyped<ShaderVertexComponent> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

}
