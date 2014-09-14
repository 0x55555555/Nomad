#include "AssetSelector.h"

namespace Nomad
{

namespace Editor
{

AssetSelector::AssetSelector(ProjectInterface *ifc, Shift::ExternalUuidPointer *ptr, const Shift::PropertyInformation *assetTypeType, QWidget *parent)
    : SUIBase<QComboBox, Shift::ExternalUuidPointer>(parent, ptr),
      _interface(ifc),
      _type(assetTypeType)
  {
  }

void AssetSelector::syncGUI()
  {
  }
}

}
