#pragma once
#include <QDockWidget>
#include <QFileSystemWatcher>
#include "UI/UIGlobal.h"
#include "Utilities/XProperty.h"
#include "shift/Utilities/sentityweakpointer.h"
#include "shift/Properties/sdata.h"
#include "NAssetManager.h"
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

class AssetBrowserData
    : public Shift::Entity,
      public AssetManagerInterface
  {
  S_ENTITY(AssetBrowserData, Entity);

public:
  AssetBrowserData();

  Asset *load(Shift::Set *parent, const QUuid &name) X_OVERRIDE;
  bool requiresReload(const QUuid &id) X_OVERRIDE;

  Nomad::Editor::AssetType *loadHandle(const QString &file);
  Editor::AssetType *createAsset(
    const Shift::PropertyInformation *info,
    const QString &location);
  void saveAsset(Editor::AssetType *a);
  Editor::AssetType *findHandle(const QString &file);

  AssetManager manager;
  ProjectInterface *interface;

private:
  void addHandle(const QString &, AssetType *t);

  Shift::Set assetHandles;
  Eks::UnorderedMap<QUuid, QString> _uuids;
  QHash<QString, AssetType *> _paths;
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
  void tearDownProject();
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
