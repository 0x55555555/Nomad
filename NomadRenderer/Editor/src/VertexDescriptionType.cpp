#include "VertexDescriptionType.h"
#include "VertexDescription.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(VertexDescriptionType, NomadRendererEditor)

void VertexDescriptionType::createTypeInformation(
    Shift::PropertyInformationTyped<VertexDescriptionType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

const char *VertexDescriptionType::extension()
  {
  return "vtxd";
  }

Asset *VertexDescriptionType::initialise(Shift::Set *a)
  {
  return a->add<Asset>();
  }

}

}
