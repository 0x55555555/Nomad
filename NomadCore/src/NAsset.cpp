#include "NAsset.h"
#include "NAssetManager.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(Asset, Nomad)

void Asset::createTypeInformation(
    Shift::PropertyInformationTyped<Asset> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

Asset::Asset()
    : _manager(nullptr)
  {
  }

const Shift::Property *Asset::resolve(
     const Shift::ExternalPointer *ptr,
     const Shift::ExternalPointerInstanceInformation *,
     Shift::ExternalPointerInstanceInformation::ResolveResult *res)
  {
  const Asset *ptrOwner = ptr->parent()->castTo<Asset>();

  AssetManager *manager = ptrOwner->manager();

  return manager->resolveAsset(ptr->uncheckedCastTo<Shift::ExternalUuidPointer>()->uuid(), res);
  }

}
