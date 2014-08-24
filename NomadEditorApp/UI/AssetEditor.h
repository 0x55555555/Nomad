#pragma once
#include "QWidget"
#include "Utilities/XProperty.h"

class QMainWindow;

namespace Nomad
{

namespace Editor
{

class AssetType;

class AssetEditor : public QWidget
  {
  Q_OBJECT

XProperties:
  XROProperty(AssetType *, asset);

public:
  static AssetEditor *build(AssetType *t);

  void makeDockable(QMainWindow *mw);
  void showEditor();

protected:
  AssetEditor(AssetType *t);

  QWidget *getTopLevel();
  };

}

}
