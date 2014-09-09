#include "QUuid"
#include "NAssetManager.h"
#include "NAsset.h"
#include "NReloadableAsset.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/scontaineriterators.h"

namespace Nomad
{

AssetManagerInterface::~AssetManagerInterface()
  {
  if (_manager)
    {
    _manager->reset(nullptr);
    }
  }

S_IMPLEMENT_PROPERTY(AssetManager, Nomad)

void AssetManager::createTypeInformation(
    Shift::PropertyInformationTyped<AssetManager> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  childBlock.add(&AssetManager::assets, "assets");
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

AssetManager::~AssetManager()
  {
  reset(nullptr);
  }

void AssetManager::clear()
  {
  assets.clear();
  _assets.clear();
  }

void AssetManager::reset(Interface *ifc)
  {
  clear();
  if (_assetInterface)
    {
    _assetInterface->_manager = nullptr;
    }

  _assetInterface = ifc;
	
  if (ifc)
    {
    ifc->_manager = this;
    }
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

    auto newAsset = _assetInterface->load(&assets, id);
    if(!newAsset)
      {
      continue;
      }

    asset->postReload(asset);

    _assets[id] = newAsset;
    assets.remove(asset);
    }
  }

Asset *AssetManager::resolveAsset(
    const QUuid &id,
    Shift::ExternalPointerInstanceInformation::ResolveResult *res)
  {
  xAssert(_assetInterface);

  auto as = _assets.find(id);
  if (as != _assets.end())
    {
    return as->second;
    }

  auto asset = _assetInterface->load(&assets, id);

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

Asset *AssetManager::createAsset(const Shift::PropertyInformation *info)
  {
  Asset *t = assets.add(info)->uncheckedCastTo<Asset>();
  registerAsset(t);
  return t;
  }

void AssetManager::registerAsset(Asset *asset)
  {
  xAssert(asset->parent() == &assets);
  asset->_manager = this;
  _assets[asset->uuid()] = asset;
  }

}
