#include "ShaderComponentType.h"
#include "ShaderComponent.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sdata.inl"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ShaderComponentType, NomadRendererEditor)

void ShaderComponentType::createTypeInformation(
    Shift::PropertyInformationTyped<ShaderComponentType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  childBlock.add(&ShaderComponentType::type, "type");
  }

const char *ShaderComponentType::extension() const
  {
  return "shac";
  }

QByteArray ShaderComponentType::defaultSource() const
  {
  return
"#version 150\n"
"precision mediump float;\n"
"\n"
"out vec4 outColor;\n"
"\n"
"void main()\n"
"{\n"
"  outColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";
  }

Asset *ShaderComponentType::processSource(const QByteArray &source, CreateInterface *c)
  {
  auto comp = assetParent()->add<ShaderComponent>();

  if(!Eks::ShaderComponent::delayedCreate(comp->component(), c->renderer(), (Eks::ShaderComponent::ShaderType)type(), source.data(), source.length()))
    {
    return comp;
    }

  return comp;
  }

}

}
