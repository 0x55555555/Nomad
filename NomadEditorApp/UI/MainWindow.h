#pragma once
#include <QMainWindow>
#include "UI/UIGlobal.h"
#include "ProjectInterface.h"
#include "XGlobal.h"
#include "NProject.h"
#include "shift/Properties/sdata.inl"
#include <Containers/XUnorderedMap.h>

namespace Ui {
class MainWindow;
}

namespace Nomad
{
namespace Editor
{

class AssetBrowser;
class AssetEditor;
class ApplicationDatabase;

class MainWindow : public QMainWindow, ProjectInterface
  {
  Q_OBJECT

public:
  explicit MainWindow(Shift::TypeRegistry *reg, Nomad::Editor::ApplicationDatabase *db, QWidget *parent = 0);
  ~MainWindow();

  Shift::Array *getScratchParent() X_OVERRIDE;
  Nomad::Project *getCurrentProject() X_OVERRIDE;
  Nomad::Editor::ProjectUserData *getCurrentProjectUserData() X_OVERRIDE;
  void addProjectChanged(QObject *obj, const char *slot) X_OVERRIDE;
  void addProjectAboutToChange(QObject *obj, const char *slot) X_OVERRIDE;
  void openAssetEditor(AssetType *a) X_OVERRIDE;
  const Eks::UnorderedMap<AssetType *, AssetEditor *>& openEditors() X_OVERRIDE;
  void reloadLibraries() X_OVERRIDE;

  void closeEvent(QCloseEvent *event) X_OVERRIDE;

signals:
  void projectChanged();
  void projectAboutToChange();

public slots:
  bool closeProject();
  void newProject();
  bool openProject();
  bool openProject(const QString &path);
  void openRecentProject();
  void saveProject();
  void saveFile();
  void updateFileMenu();

private:
  void addRecent(const QString &recent);
  QStringList getRecents() const;
  void focusEditor(AssetEditor *editor);
  AssetEditor *findActiveEditor();

  void newProjectUserData(const QString &path);
  QString userDataPath(const QString &path) const;

  Ui::MainWindow *_ui;
  Nomad::Editor::ApplicationDatabase *_db;
  Shift::TypeRegistry *_registry;
  Eks::UnorderedMap<AssetType *, AssetEditor *> _editors;
  AssetBrowser *_browser;

  struct ModuleWrapper;
  std::vector<std::unique_ptr<ModuleWrapper>> _libraries;
  };

}
}
