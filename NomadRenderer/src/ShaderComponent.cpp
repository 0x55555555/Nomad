#include "ShaderComponent.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(ShaderComponent, NomadRenderer)

void ShaderComponent::createTypeInformation(
    Shift::PropertyInformationTyped<ShaderComponent> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

}
