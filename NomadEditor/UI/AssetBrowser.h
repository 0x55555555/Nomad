#ifndef ASSETBROWSER_H
#define ASSETBROWSER_H

#include <QDockWidget>
#include <QFileSystemWatcher>
#include "UI/UIGlobal.h"
#include "Utilities/XProperty.h"
#include "shift/Utilities/sentityweakpointer.h"
#include "shift/Properties/sdata.h"
#include "NAssetManager.h"
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

class AssetBrowserData
    : public Shift::Entity,
      public AssetManagerInterface
  {
  S_ENTITY(AssetBrowserData, Entity);

public:
  AssetBrowserData();

  Nomad::Asset *load(const char *name, Shift::Array *parent);
  Asset *load(const QUuid &name, Shift::Array *parent) X_OVERRIDE;
  bool requiresReload(const QUuid &id) X_OVERRIDE;

  Asset *createAsset(const Shift::PropertyInformation *info, const QString &location);
  void saveAsset(Asset *a);
  void loadAsset(const QString &d);

  bool hasLoaded(const QString &d) const;

  AssetManager manager;

private:
  Eks::UnorderedMap<QUuid, QString> _paths;
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
  void openAsset(const QModelIndex &);
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


#endif // ASSETBROWSER_H
