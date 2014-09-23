#pragma once
#include "QWidget"
#include "Utilities/XProperty.h"
#include "Assets/AssetType.h"

class QMainWindow;

namespace Nomad
{

namespace Editor
{

class AssetEditor : public QWidget
  {
  Q_OBJECT

XProperties:
  XROProperty(AssetType *, asset);

public:
  static AssetEditor *build(AssetType *t, ProjectInterface *ifc, AssetType::CreateInterface *c, UIInterface *ui);

  void onReloadAvailable();

  void makeDockable(QMainWindow *mw);
  void showEditor();

protected:
  AssetEditor(AssetType *t, ProjectInterface *ifc, AssetType::CreateInterface *c, UIInterface *ui);

  QWidget *getTopLevel();

  QAction *_reload;
  };

}

}
