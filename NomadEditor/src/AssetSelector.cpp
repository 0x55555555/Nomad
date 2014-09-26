#include "AssetSelector.h"
#include "NAssetManager.h"
#include "shift/Properties/scontaineriterators.h"
#include "NAsset.h"
#include "Utilities/XScopedValue.h"

namespace Nomad
{

namespace Editor
{

AssetSelector::AssetSelector(
  ProjectInterface *ifc,
  AssetType::CreateInterface *cre,
  Shift::ExternalUuidPointer *ptr,
  const Shift::PropertyInformation *assetTypeType,
  QWidget *parent)
    : SUIBase<QComboBox, Shift::ExternalUuidPointer>(parent, ptr),
      _projectInterface(ifc),
      _createInterface(cre),
      _type(assetTypeType),
      _disableChanges(false)
  {
  updateCombo();
  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChanged(int)));
  }

void AssetSelector::syncGUI()
  {
  updateCombo();
  }

void AssetSelector::updateCombo()
  {
  Eks::ScopedValue<bool> s(_disableChanges, true);
  clear();

  addItem("None");
  int item = 0;

  int i = 1;
  xForeach(auto t, _projectInterface->getAssetHandleParent()->walker())
    {
    // this should never fail.
    auto type = t->castTo<AssetType>();
    if(type && type->inheritsFromType(_type))
      {
      addItem(type->relativePath(), QVariant::fromValue((qulonglong)type));

      if (type->asset(_createInterface) == propertyValue()->pointed())
        {
        item = i;
        }

      ++i;
      }
    }

  setCurrentIndex(item);
  }

void AssetSelector::comboChanged(int i)
  {
  if (_disableChanges)
    {
    return;
    }

  Eks::ScopedValue<bool> s(_disableChanges, true);

  Shift::Block h(propertyValue()->handler());
  auto old = propertyValue()->pointed();
  auto v = (AssetType*)itemData(i).toLongLong();
  if (v)
    {
    Asset *asset = v->asset(_createInterface);
    if (!asset)
      {
      xAssertFail();
      }

    if(propertyValue()->pointed() != asset)
      {
      propertyValue()->setPointed(asset);
      }
    }
  else if(old != nullptr)
    {
    propertyValue()->setPointed(nullptr);
    }

  if (old != propertyValue()->pointed())
    {
    emit assetChanged();
    }
  }

}

}
