#pragma once
#include <QDockWidget>
#include <QFileSystemWatcher>
#include "UI/UIGlobal.h"
#include "Utilities/XProperty.h"
#include "shift/Utilities/sentityweakpointer.h"
#include "shift/Properties/sdata.h"
#include "NAssetManager.h"
#include "Assets/AssetType.h"
#include "ProjectInterface.h"
#include "Containers/XUnorderedMap.h"

namespace Ui {
class AssetBrowser;
}

#define ASSET_EXT "ass"
#define ASSET_FILTER "*." ASSET_EXT

class QFileSystemModel;

namespace Nomad
{
namespace Editor
{

class AssetBrowser;
class AssetType;
class AssetBrowserData;

class AssetBrowser : public QDockWidget
  {
  Q_OBJECT

XProperties:
  XROProperty(ProjectInterface *, project);

public:
  explicit AssetBrowser(ProjectInterface *ifc, AssetType::CreateInterface *ctx, QWidget *parent = 0);
  ~AssetBrowser();

private slots:
  void tearDownProject(bool *abort);
  void setupProject();
  void openAsset(const QModelIndex &);
  void openAsset(const QString &);
  void addAsset();
  void removeAsset();
  void fileAdded(const QModelIndex &paret, int first, int last);

private:
  void rebuildUI();
  void loadAllAssets(const QString &dir);

  Ui::AssetBrowser *_ui;
  Shift::EntityWeakPointer<AssetBrowserData> _browser;

  QFileSystemModel *_model;
  };

}
}
