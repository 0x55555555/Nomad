#pragma once
#include "NomadEditor.h"
#include "QtWidgets/QComboBox"
#include "ProjectInterface.h"
#include "ui/spropertydefaultui.h"
#include "sexternalpointer.h"

namespace Nomad
{
namespace Editor
{

class NOMAD_EDITOR_EXPORT AssetSelector : public Shift::PropertyDefaultUI::SUIBase<QComboBox, Shift::ExternalUuidPointer>
  {
XProperties:
  XROProperty(ProjectInterface *, interface);
  XROProperty(const Shift::PropertyInformation *, type);

public:
  AssetSelector(ProjectInterface *ifc, Shift::ExternalUuidPointer *ptr, const Shift::PropertyInformation *assetTypeType, QWidget *parent);

  void syncGUI() X_OVERRIDE;
  };

}

}
