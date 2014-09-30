#include "Mesh.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAssetManager.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(Mesh, NomadRenderer)

void Mesh::createTypeInformation(
    Shift::PropertyInformationTyped<Mesh> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  auto layout = childBlock.add(&Mesh::_layoutDescription, "layoutDescription");
  layout->setResolveFunction(AssetManager::resolveAssetPointer);
  }

}
