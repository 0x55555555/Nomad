#include "ShaderType.h"
#include "Shader.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(Shader, NomadRendererEditor)

void Shader::createTypeInformation(
    Shift::PropertyInformationTyped<Shader> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

const char *Shader::extension()
  {
  return "shader";
  }

Asset *Shader::initialise(Shift::Set *a)
  {
  return a->add<Asset>();
  }

}

}
