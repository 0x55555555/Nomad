#ifndef ASSETBROWSER_H
#define ASSETBROWSER_H

#include <QDockWidget>
#include "UI/UIGlobal.h"
#include "Utilities/XProperty.h"
#include "shift/Utilities/sentityweakpointer.h"
#include "shift/Properties/sdata.h"

namespace Ui {
class AssetBrowser;
}

class QFileSystemModel;

namespace Nomad
{
namespace Editor
{

class AssetBrowserData : public Shift::Entity
  {
  S_ENTITY(AssetBrowserData, Entity);

public:
  };

class AssetBrowser : public QDockWidget
  {
  Q_OBJECT

XProperties:
  XROProperty(ProjectInterface *, project);

public:
  explicit AssetBrowser(ProjectInterface *ifc, QWidget *parent = 0);
  ~AssetBrowser();

private slots:
  void setupProject();

private:
  void rebuildUI();

  Ui::AssetBrowser *_ui;
  Shift::EntityWeakPointer<AssetBrowserData> _browser;

  QFileSystemModel *_model;
  };

}
}


#endif // ASSETBROWSER_H
