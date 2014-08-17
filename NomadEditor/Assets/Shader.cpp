#include "Assets/Shader.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(Shader, Nomad)

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

Asset *Shader::process(Shift::Array *, const QString &)
  {
  return nullptr;
  }

QString Shader::unprocess(Asset *)
  {
  return "pork";
  }

Asset *Shader::initialise(Shift::Array *a)
  {
  return a->add<Asset>();
  }

}

}
