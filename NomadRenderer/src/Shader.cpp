#include "Shader.h"
#include "shift/Properties/sdata.inl"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAssetManager.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(Shader, NomadRenderer)

void Shader::createTypeInformation(
    Shift::PropertyInformationTyped<Shader> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  auto shader = childBlock.add(&Shader::_vertexShader, "vertexShader");
  shader->setResolveFunction(AssetManager::resolveAssetPointer);
  }

}
