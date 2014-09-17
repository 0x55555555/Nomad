#pragma once
#include "NomadEditor.h"
#include "QtWidgets/QComboBox"
#include "ProjectInterface.h"
#include "ui/spropertydefaultui.h"
#include "sexternalpointer.h"
#include "Assets/AssetType.h"

namespace Nomad
{
namespace Editor
{

class NOMAD_EDITOR_EXPORT AssetSelector : public Shift::PropertyDefaultUI::SUIBase<QComboBox, Shift::ExternalUuidPointer>
  {
  Q_OBJECT

XProperties:
  XROProperty(ProjectInterface *, projectInterface);
  XROProperty(AssetType::CreateInterface *, createInterface);
  XROProperty(const Shift::PropertyInformation *, type);

public:
  AssetSelector(
      ProjectInterface *ifc,
      AssetType::CreateInterface *cre,
      Shift::ExternalUuidPointer *ptr,
      const Shift::PropertyInformation *assetTypeType,
      QWidget *parent);

  void syncGUI() X_OVERRIDE;

  void updateCombo();

signals:
  void assetChanged();

private slots:
  void comboChanged(int);
  };

}

}
