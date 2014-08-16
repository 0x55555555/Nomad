#include "NReloadableAsset.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(ReloadableAsset, Nomad)

void ReloadableAsset::createTypeInformation(
    Shift::PropertyInformationTyped<ReloadableAsset> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

bool ReloadableAsset::preReload()
  {
  return true;
  }

bool ReloadableAsset::postReload(ReloadableAsset *)
  {
  return true;
  }

}
