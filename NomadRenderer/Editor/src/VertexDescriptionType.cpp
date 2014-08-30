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

QByteArray VertexDescriptionType::defaultSource() const
  {
  return QByteArray();
  }

Asset *VertexDescriptionType::process(Shift::Set *a, const QByteArray &s)
  {
  setSource(s);

  auto desc = a->add<VertexDescription>();

  ... load s ...
  }

}

}
