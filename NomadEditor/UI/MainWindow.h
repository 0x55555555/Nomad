#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace Nomad
{
namespace Editor
{
class ApplicationDatabase;

class MainWindow : public QMainWindow
  {
  Q_OBJECT

public:
  explicit MainWindow(Nomad::Editor::ApplicationDatabase *db, QWidget *parent = 0);
  ~MainWindow();

public slots:
  void closeProject();
  void newProject();
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
