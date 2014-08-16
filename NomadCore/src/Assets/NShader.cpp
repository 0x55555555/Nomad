#include "Assets/NShader.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(Shader, Nomad)

void Shader::createTypeInformation(
    Shift::PropertyInformationTyped<Shader> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

}
