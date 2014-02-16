#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UI/UIGlobal.h"
#include "XGlobal.h"

namespace Ui {
class MainWindow;
}

namespace Nomad
{
namespace Editor
{
class ApplicationDatabase;

class MainWindow : public QMainWindow, ProjectInterface
  {
  Q_OBJECT

public:
  explicit MainWindow(Nomad::Editor::ApplicationDatabase *db, QWidget *parent = 0);
  ~MainWindow();

  Shift::Array *getScratchParent() X_OVERRIDE;
  Nomad::Project *getCurrentProject() X_OVERRIDE;
  void addProjectChanged(QObject *obj, const char *slot) X_OVERRIDE;

signals:
  void projectChanged();

public slots:
  void closeProject();
  void newProject();
  void openProject();
  void openProject(const QString &path);
  void openRecentProject();
  void saveProject();

private:
  void addRecent(const QString &recent);
  QStringList getRecents() const;

  Ui::MainWindow *_ui;
  Nomad::Editor::ApplicationDatabase *_db;
  };

}
}

#endif // MAINWINDOW_H
