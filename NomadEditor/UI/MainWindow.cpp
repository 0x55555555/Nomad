#include "UI/MainWindow.h"
#include "ui_MainWindow.h"
#include "shift/sdatabase.h"
#include "Application.h"
#include "QFileDialog"
#include "QSettings"
#include "AssetBrowser.h"

namespace Nomad
{

namespace Editor
{

MainWindow::MainWindow(Nomad::Editor::ApplicationDatabase *db, QWidget *parent) :
  QMainWindow(parent),
  _ui(new Ui::MainWindow),
  _db(db)
  {
  _ui->setupUi(this);

  addDockWidget(Qt::LeftDockWidgetArea, new AssetBrowser(this));

  connect(_ui->actionNew_Project, SIGNAL(triggered()), this, SLOT(newProject()));
  connect(_ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));
  connect(_ui->actionClose_Project, SIGNAL(triggered()), this, SLOT(closeProject()));
  connect(_ui->actionSave_Project, SIGNAL(triggered()), this, SLOT(saveProject()));

  connect(_ui->menuFile, &QMenu::aboutToShow,
    [this, db]()
    {
    _ui->actionSave_Project->setEnabled(db->project() != nullptr);
    });

  auto recentMenu = new QMenu();
  _ui->actionRecent_Projects->setMenu(recentMenu);
  connect(recentMenu, &QMenu::aboutToShow,
    [recentMenu, this]()
    {
    recentMenu->clear();

    QFont font;
    QFontMetrics met(font);

    auto recents = getRecents();

    xForeach(auto &r, recents)
      {
      auto newText = met.elidedText(r, Qt::ElideLeft, 300);
      auto action = recentMenu->addAction(newText, this, SLOT(openRecentProject()));

      action->setData(r);
      }
    });

  auto recents = getRecents();
  if(recents.size())
    {
    openProject(recents[0]);
    }
  }

MainWindow::~MainWindow()
  {
  delete _ui;
  }

void MainWindow::closeProject()
  {
  auto current = _db->project();
  if(current)
    {
    _db->children.remove(current);
    }

  emit projectChanged();
  }

Shift::Array *MainWindow::getScratchParent()
  {
  return &_db->scratch;
  }

Nomad::Project *MainWindow::getCurrentProject()
  {
  return _db->project.pointed<Nomad::Project>();
  }

void MainWindow::addProjectChanged(QObject *obj, const char *slot)
  {
  connect(this, SIGNAL(projectChanged()), obj, slot);
  }

void MainWindow::newProject()
  {
  closeProject();

  auto newPro = _db->children.add<Nomad::Project>();


  QString path = QFileDialog::getSaveFileName(this, "Open Project", QString(), "*.npro");

  addRecent(path);

  newPro->setPath(path);

  _db->project.setPointed(newPro);
  emit projectChanged();

  if(!Application::save(newPro))
    {
    qWarning() << "failed to save new project";
    }
  }

void MainWindow::openProject()
  {
  openProject(QString());
  }

void MainWindow::openProject(const QString &path)
  {
  closeProject();

  QString toLoad = path;
  if(toLoad.isEmpty())
    {
    toLoad = QFileDialog::getOpenFileName(this, "Open Project", QString(), "*.npro");
    }

  addRecent(toLoad);

  auto file = Application::load(toLoad, _db);
  _db->project.setPointed(file);
  emit projectChanged();
  }

void MainWindow::openRecentProject()
  {
  QAction *a = qobject_cast<QAction*>(sender());
  if(!a)
    {
    return;
    }

  QString path = a->data().toString();
  openProject(path);
  }

void MainWindow::saveProject()
  {
  auto pro = getCurrentProject();
  if(!pro)
    {
    qWarning() << "no project to save";
    return;
    }

  addRecent(pro->path().toQString());

  if(!Application::save(pro))
    {
    qWarning() << "failed to save project";
    }
  }

#define RECENTS_KEY "editor/recentProjects"
QStringList MainWindow::getRecents() const
  {
  QSettings settings;

  QStringList result;
  int size = settings.beginReadArray(RECENTS_KEY);
  for (int i = 0; i < size; ++i)
    {
    settings.setArrayIndex(i);
    result << settings.value("file").toString();
    }
  return result;
  }

void MainWindow::addRecent(const QString &recent)
  {
  QList<QString> recents = getRecents();

  recents.removeAll(recent);
  if(QFile::exists(recent))
    {
    QFileInfo f(recent);
    QString canocialised = f.canonicalFilePath();
    recents.prepend(canocialised);
    }

  QSettings settings;

  settings.beginWriteArray(RECENTS_KEY);
  for (int i = 0; i < recents.size(); ++i)
    {
    settings.setArrayIndex(i);
    settings.setValue("file", recents.at(i));
    }
  settings.endArray();
  }
}

}
