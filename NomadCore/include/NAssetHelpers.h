#pragma once
#include "NAsset.h"

namespace Nomad
{

namespace detail
{

template <typename T, typename PropType>
  static Shift::PropertyInstanceInformationTyped<PropType, Nomad::AssetPointer> *addAssetPointer(
    T &block,
    Nomad::AssetPointer PropType::* ptr,
    const Shift::NameArg &name)
  {
  auto inst = block.add(ptr, name);

  inst->setResolveFunction(Asset::resolve);

  return inst;
  }

}

}
