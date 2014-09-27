#include "UI/MainWindow.h"
#include "ui_MainWindow.h"
#include "shift/sdatabase.h"
#include "Application.h"
#include "QFileDialog"
#include "QCloseEvent"
#include "AssetBrowser.h"
#include "ProjectEditor.h"
#include "Assets/AssetType.h"
#include "UI/AssetEditor.h"
#include "UI/ProjectUserData.h"
#include "QSettings"
#include "QLibrary"
#include "NFile.h"
#include "XGLRenderer.h"
#include "X3DCanvas.h"
#include "Utilities/XOptional.h"

namespace Nomad
{

namespace Editor
{

struct MainWindow::ModuleWrapper
  {
  ModuleWrapper(Shift::TypeRegistry *reg, const QString &path)
      : library(path),
        registry(reg)
    {
    if(!library.load())
      {
      qWarning() << "......" << path << "failed to load:" << library.errorString();
      return;
      }

    auto fn = (Shift::ModuleCSignature)library.resolve(S_MODULE_EXPORT_NAME);
    if(!fn)
      {
      qWarning() << "......" << path << "failed to load: invalid shift module";
      return;
      }

    module = fn();
    if(!module)
      {
      qWarning() << "......" << path << "failed to load: invalid shift module returned";
      return;
      }

    registry->installModule(*module);
    qDebug() << "...... loaded" << (void*)fn;
    }

  ~ModuleWrapper()
    {
    if (module)
      {
      registry->uninstallModule(*module);
      }

    library.unload();
    }

  QLibrary library;
  Shift::Module *module;
  Shift::TypeRegistry *registry;
  };

MainWindow::MainWindow(
    Shift::TypeRegistry *reg,
    Nomad::Editor::ApplicationDatabase *db,
    QWidget *parent) :
  QMainWindow(parent),
  _ui(new Ui::MainWindow),
  _db(db),
  _registry(reg),
  _editors(Eks::Core::defaultAllocator()),
  _primaryContext(Eks::GL3DCanvas::makeFormat()),
  _renderer(nullptr)
  {
  _ui->setupUi(this);
  _ui->centralwidget->hide();

  connect(_ui->menuFile, SIGNAL(aboutToShow()), this, SLOT(updateFileMenu()));

  if(!_primaryContext.isValid())
  {
    qWarning() << "Cannot create GL context";
    return;
  }
  _primaryContext.makeCurrent();
  _renderer = Eks::GLRenderer::createGLRenderer(false, Eks::Core::defaultAllocator());

  _browser = new AssetBrowser(this, this);
  addDockWidget(Qt::LeftDockWidgetArea, _browser);

  addDockWidget(Qt::LeftDockWidgetArea, new ProjectEditor(this));

  connect(_ui->actionNew_Project, SIGNAL(triggered()), this, SLOT(newProject()));
  connect(_ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));
  connect(_ui->actionClose_Project, SIGNAL(triggered()), this, SLOT(closeProject()));
  connect(_ui->actionSave_Project, SIGNAL(triggered()), this, SLOT(saveProject()));
  connect(_ui->actionSave_File, SIGNAL(triggered()), this, SLOT(saveFile()));

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
  xForeach(auto &r, recents)
    {
    if (openProject(r))
      {
      return;
      }
    }
  }

MainWindow::~MainWindow()
  {
  delete _ui;
  if (_renderer)
    {
    Eks::GLRenderer::destroyGLRenderer(_renderer, Eks::Core::defaultAllocator());
    }
  }

void MainWindow::closeEvent(QCloseEvent *event)
  {
  if (closeProject() == false)
    {
    event->ignore();
    return;
    }

  QMainWindow::closeEvent(event);
  }

bool MainWindow::closeProject()
  {
  auto current = _db->project();
  if(!current)
    {
    return false;
    }

  bool abort = false;
  emit projectAboutToChange(&abort);

  if(abort || Application::offerToSave(current) != Application::Success)
    {
    return false;
    }

  xForeach(auto e, _editors.values())
    {
    delete e;
    }

  _editors.clear();

  _db->children.remove(current);
  _db->clearChanges();

  auto currentUser = _db->projectUserData();
  if(currentUser)
    {
    currentUser->geometry.assign(saveGeometry().toBase64().data());
    currentUser->layout.assign(saveState().toBase64().data());
    }

  Application::save(currentUser);
  if(currentUser)
    {
    _db->children.remove(currentUser);
    }


  emit projectChanged();
  return true;
  }

Shift::Array *MainWindow::getScratchParent()
  {
  return &_db->scratch;
  }

Nomad::Project *MainWindow::getCurrentProject()
  {
  return _db->project.pointed();
  }

AssetManager *MainWindow::getAssetManager()
  {
  return _browser->getManager();
  }

Shift::Set *MainWindow::getAssetHandleParent()
  {
  return _browser->getHandleParent();
  }

Nomad::Editor::ProjectUserData *MainWindow::getCurrentProjectUserData()
  {
  return _db->projectUserData.pointed();
  }

void MainWindow::addProjectChanged(QObject *obj, const char *slot)
  {
  connect(this, SIGNAL(projectChanged()), obj, slot);
  }

void MainWindow::addProjectAboutToChange(QObject *obj, const char *slot)
  {
  connect(this, SIGNAL(projectAboutToChange(bool*)), obj, slot);
  }

void MainWindow::openAssetEditor(AssetType *a)
  {
  if (!a)
    {
    xAssertFail();
    return;
    }

  auto found = _editors.value(a, nullptr);
  if (found)
    {
    focusEditor(found);
    return;
    }

  found = AssetEditor::build(a, this, this, this);
  _editors[a] = found;

  focusEditor(found);
  }

const Eks::UnorderedMap<AssetType *, AssetEditor *> &MainWindow::openEditors()
  {
  return _editors;
  }

void MainWindow::reloadLibraries()
  {
  qDebug() << "Reloading libraries";
  _libraries.clear();
  auto project = getCurrentProject();
  if (!project)
    {
    return;
    }

  QString bin = project->binFolder().data();
  qDebug() << "... loading libraries from" << bin;

  xForeach(auto str, project->libraries.walker<Shift::StringProperty>())
    {
    QString name = str->value().data();
    QString path = bin + "/" + name;
    qDebug() << "... loading" << name;

    if(!QFile(path).exists())
      {
      qWarning() << "......" << path << "doesn't exist";
      continue;
      }

    _libraries.emplace_back(std::unique_ptr<ModuleWrapper>(new ModuleWrapper(_registry, path)));
    }
  }

Eks::Renderer *MainWindow::renderer()
  {
  return _renderer;
  }

QWidget *MainWindow::createViewport(QWidget *parent, Eks::AbstractCanvas** canOpt)
  {
  struct Viewport : Eks::GL3DCanvas
    {
    Viewport(MainWindow *m, QWidget *w)
        : Eks::GL3DCanvas(&m->_primaryContext, w),
          _mainWindow(m)
      {
      }

    void initializeGL() X_OVERRIDE
      {
      setRenderer(_mainWindow->renderer(), false);
      Eks::GL3DCanvas::initializeGL();
      }

    MainWindow *_mainWindow;
    };

  auto vp = new Viewport(this, parent);

  Eks::Optional<Eks::AbstractCanvas*> can(canOpt);
  can = vp;

  return vp;
  }

void MainWindow::focusEditor(AssetEditor *editor)
  {
  editor->makeDockable(this);

  editor->showEditor();
  }

AssetEditor *MainWindow::findActiveEditor()
  {
  QWidget *w = QApplication::focusWidget();
  while (w && !qobject_cast<AssetEditor*>(w))
    {
    w = w->parentWidget();
    }
  return qobject_cast<AssetEditor*>(w);
  }

void MainWindow::newProject()
  {
  closeProject();

  auto newPro = _db->children.add<Nomad::Project>();

  QString path = QFileDialog::getSaveFileName(this, "Open Project", QString(), "*.npro");

  addRecent(path);

  newPro->setPath(path);

  _db->project.setPointed(newPro);
  newProjectUserData(path);
  emit projectChanged();

  if(!Application::save(newPro))
    {
    qWarning() << "failed to save new project";
    }
  newPro->setSaved();
  }

void MainWindow::newProjectUserData(const QString &proPath)
  {
  auto newProUd = _db->children.add<Nomad::Editor::ProjectUserData>();
  _db->projectUserData.setPointed(newProUd);

  newProUd->setPath(userDataPath(proPath));

  if(!Application::save(newProUd))
    {
    qWarning() << "failed to save new project user data";
    }
  }

QString MainWindow::userDataPath(const QString &path) const
  {
  return path + ".user";
  }

bool MainWindow::openProject()
  {
  return openProject(QString());
  }

bool MainWindow::openProject(const QString &path)
  {
  closeProject();

  QString toLoad = path;
  if(toLoad.isEmpty())
    {
    toLoad = QFileDialog::getOpenFileName(this, "Open Project", QString(), "*.npro");
    }

  auto file = Application::load(toLoad, &_db->children);
  if (!file || !file->castTo<Project>())
    {
    return false;
    }

  auto ud = Application::load(userDataPath(toLoad), &_db->children);
  if (ud && ud->castTo<ProjectUserData>())
    {
    auto currentUser = ud->uncheckedCastTo<ProjectUserData>();
    _db->projectUserData.setPointed(currentUser);
    }
  else
    {
    newProjectUserData(toLoad);
    }


  addRecent(toLoad);
  _db->project.setPointed(file->uncheckedCastTo<Project>());
  reloadLibraries();
  emit projectChanged();

  auto currentUser = _db->projectUserData();
  auto str = QByteArray::fromBase64(currentUser->geometry().data());
  if (!str.isEmpty())
    {
    if (!restoreGeometry(str))
      {
      qWarning() << "Failed to restore application geometry";
      }
    }

  str = QByteArray::fromBase64(currentUser->layout().data());
  if (!str.isEmpty())
    {
    if (!restoreState(str))
      {
      qWarning() << "Failed to restore application ui state";
      }
    }

  return true;
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

void MainWindow::saveFile()
  {
  auto editor = findActiveEditor();
  if (!editor)
    {
    return;
    }

  auto asset = editor->asset();
  if (!asset)
    {
    xAssertFail();
    }

  asset->save();
  }

void MainWindow::updateFileMenu()
  {
  AssetEditor* activeEditor = findActiveEditor();

  auto save = _ui->actionSave_File;
    
  save->setEnabled(activeEditor != nullptr);
  if (!activeEditor)
    {
    save->setText(tr("Save File"));

    return;
    }

  auto asset = activeEditor->asset();
  xAssert(asset);
  save->setText(tr("Save %1").arg(asset->relativePath()));
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

int main(int argc, char **argv)
  {
  Nomad::Editor::Application app(argc, argv);

  Nomad::Editor::MainWindow w(app.registry(), app.database());
  w.show();

  return app.exec();
  }
