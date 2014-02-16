#ifndef ASSETBROWSER_H
#define ASSETBROWSER_H

#include <QDockWidget>
#include "UI/UIGlobal.h"
#include "Utilities/XProperty.h"
#include "shift/Utilities/sentityweakpointer.h"

namespace Ui {
class AssetBrowser;
}

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

  Ui::AssetBrowser *ui;
  AssetBrowserData *_browser;
  };

}
}


#endif // ASSETBROWSER_H
