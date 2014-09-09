#include "VertexDescription.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(VertexDescription, NomadRenderer)

void VertexDescription::createTypeInformation(
    Shift::PropertyInformationTyped<VertexDescription> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

VertexDescription::VertexDescription()
    : _layout(Shift::TypeRegistry::generalPurposeAllocator())
  {
  }

void VertexDescription::addElement(
    Eks::ShaderVertexLayoutDescription::Semantic s,
    Eks::ShaderVertexLayoutDescription::Format f)
  {
  _layout.emplaceBack(s, f);
  }

void VertexDescription::bakeSemantics(Eks::Vector<Eks::ShaderVertexLayoutDescription::Semantic> *s)
  {
  xForeach(auto l, _layout)
    {
    s->pushBack(l.semantic);
    }
  }
}
