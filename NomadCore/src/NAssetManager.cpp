#include "QUuid"
#include "NAssetManager.h"
#include "NAsset.h"
#include "NReloadableAsset.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/scontaineriterators.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(AssetManager, Nomad)

void AssetManager::createTypeInformation(
    Shift::PropertyInformationTyped<AssetManager> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto childBlock = info->createChildrenBlock(data);

    childBlock.add(&AssetManager::assets, "assets");
    }
  }

AssetManagerInterface::AssetManagerInterface()
    : _manager(nullptr)
  {
  }

AssetManager::AssetManager()
    : _assetInterface(nullptr),
      _assets(Shift::TypeRegistry::generalPurposeAllocator())
  {
  }

void AssetManager::reloadRequiredAssets()
  {
  xAssert(_assetInterface);

  Eks::TemporaryAllocator alloc(temporaryAllocator());
  Eks::Vector<ReloadableAsset*> toReload(&alloc);

  xForeach(auto asset, assets.walker<ReloadableAsset>())
    {
    QUuid id = asset->uuid();

    if(_assetInterface->requiresReload(id))
      {
      toReload.pushBack(asset);
      }
    }

  xForeach(auto asset, toReload)
    {
    QUuid id = asset->uuid();

    bool canReload = asset->preReload();
    if(!canReload)
      {
      continue;
      }

    auto newAsset = _assetInterface->load(id, &assets);
    if(!newAsset)
      {
      continue;
      }

    asset->postReload(asset);

    _assets[id] = newAsset;
    assets.remove(asset);
    }
  }

void AssetManager::setInterface(Interface *ifc)
  {
  ifc->_manager = this;
  _assetInterface = ifc;
  }

Shift::Array *AssetManager::assetContainer()
  {
  return &assets;
  }

Asset *AssetManager::resolveAsset(const QUuid &id, Shift::ExternalPointerInstanceInformation::ResolveResult *res)
  {
  xAssert(_assetInterface);

  auto as = _assets.find(id);
  if (as != _assets.end())
    {
    return as.value();
    }

  auto asset = _assetInterface->load(id, &assets);

  if(res)
    {
    if(asset)
      {
      *res = Shift::ExternalPointerInstanceInformation::Success;
      }
    else
      {
      *res = Shift::ExternalPointerInstanceInformation::DataDoesntExist;
      }
    }

  return asset;
  }

void AssetManager::registerAsset(Asset *asset)
  {
  asset->_manager = this;
  _assets[asset->uuid()] = asset;
  }


}
