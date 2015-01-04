#include "NObjectInstance.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAssetManager.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(ObjectInstance, Nomad)

void ObjectInstance::createTypeInformation(
    Shift::PropertyInformationTyped<ObjectInstance> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  auto type = childBlock.add(&ObjectInstance::type, "type");
  type->setResolveFunction(AssetManager::resolveAssetPointer);
  }
}
