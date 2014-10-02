#include "AssetBrowser.h"
#include "ui_AssetBrowser.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sdata.inl"
#include "Application.h"
#include "NewAsset.h"
#include "AssetEditor.h"
#include "NAsset.h"
#include "UI/ProjectUserData.h"
#include "QFileSystemModel"
#include "QMessageBox"

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

  Asset *load(Shift::Set *parent, const QUuid &name) X_OVERRIDE;
  bool requiresReload(const QUuid &id) X_OVERRIDE;
  void markForReload(const QUuid &a) X_OVERRIDE;

  Nomad::Editor::AssetType *loadHandle(const QString &file);
  Editor::AssetType *createAsset(
    const Shift::PropertyInformation *info,
    const QString &location);
  void saveAsset(Editor::AssetType *a);
  Editor::AssetType *findHandle(const QString &file);
  Editor::AssetType *findHandle(const QUuid &file);
  void clear();

  AssetManager manager;
  Shift::Set assetHandles;
  ProjectInterface *interface;
  AssetType::CreateInterface *createContext;

private:
  void addHandle(const QString &, AssetType *t);

  Eks::UnorderedMap<QUuid, QString> _uuids;
  QHash<QString, AssetType *> _paths;
  };

S_IMPLEMENT_PROPERTY(AssetBrowserData, NomadEditor)

void AssetBrowserData::createTypeInformation(
    Shift::PropertyInformationTyped<AssetBrowserData> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto block = info->createChildrenBlock(data);

  block.add(&AssetBrowserData::manager, "manager");
  block.add(&AssetBrowserData::assetHandles, "assetHandles");
  }

AssetBrowserData::AssetBrowserData()
    : _uuids(Eks::Core::defaultAllocator())
  {
  }

Nomad::Editor::AssetType *AssetBrowserData::loadHandle(const QString &name)
  {
  if (auto handle = findHandle(name))
    {
    return handle;
    }

  AssetType *asset = AssetType::load(name, &assetHandles, manager.assetParent(), interface, createContext);
  if (!asset)
    {
    return nullptr;
    }

  addHandle(name, asset);

  return asset;
  }

Asset *AssetBrowserData::load(Shift::Set *parent, const QUuid &name)
  {
  if(name.isNull())
    {
    return nullptr;
    }

  QString path = name.toString();
  auto it = _uuids.find(name);
  if(it == _uuids.end())
    {
    qWarning() << "Can't find asset for " << name;
    return nullptr;
    }

  AssetType* type = loadHandle(it->second);

  xAssert(manager.assetParent() == parent);
  return type->asset(createContext);
  }

bool AssetBrowserData::requiresReload(const QUuid &name)
  {
  auto it = _uuids.find(name);
  if (it == _uuids.end())
    {
    return false;
    }

  AssetType* type = loadHandle(it->second);
  if (!type)
    {
    return false;
    }

  return type->requiresReload();
  }

void AssetBrowserData::markForReload(const QUuid  &a)
  {
  auto it = _uuids.find(a);
  if (it == _uuids.end())
    {
    xAssertFail();
    return;
    }

  AssetType* type = loadHandle(it->second);
  if (!type)
    {
    xAssertFail();
    return;
    }

  type->setRequiresReload(true);
  }

AssetType *AssetBrowserData::createAsset(
    const Shift::PropertyInformation *info,
    const QString &location)
  {
  AssetType *a = AssetType::create(info, location, &assetHandles, manager.assetParent(), interface, createContext);
  addHandle(location, a);
  return a;
  }

void AssetBrowserData::saveAsset(AssetType *a)
  {
  xAssert(_uuids.contains(a->uuid()), a->uuid().toByteArray().data());

  a->save();
  }

Editor::AssetType *AssetBrowserData::findHandle(const QString &file)
  {
  QFileInfo f(file);
  return _paths.value(f.canonicalFilePath());
  }

Editor::AssetType *AssetBrowserData::findHandle(const QUuid &file)
  {
  return findHandle(_uuids[file]);
  }

void AssetBrowserData::clear()
  {
  Shift::Block b(database());
  assetHandles.clear();
  manager.clear();
  _uuids.clear();
  _paths.clear();
  }

void AssetBrowserData::addHandle(const QString &file, AssetType *t)
  {
  QFileInfo f(file);
  QString path = f.canonicalFilePath();
  _paths[path] = t;
  _uuids[t->uuid()] = path;
  t->setPath(path, interface);
  }

AssetBrowser::AssetBrowser(ProjectInterface *ifc, AssetType::CreateInterface *ctx, QWidget *parent) :
  QDockWidget(parent),
  _project(ifc),
  _ui(new Ui::AssetBrowser)
  {
  _ui->setupUi(this);

  _ui->add->setIcon(QIcon::fromTheme("list-add"));
  _ui->remove->setIcon(QIcon::fromTheme("list-remove"));

  _project->addProjectChanged(this, SLOT(setupProject()));
  _project->addProjectAboutToChange(this, SLOT(tearDownProject(bool*)));

  auto scratch = _project->getScratchParent();
  _browser = scratch->add<AssetBrowserData>();
  _browser->interface = ifc;
  _browser->createContext = ctx;

  _model = new QFileSystemModel;

  setupProject();
  }

AssetBrowser::~AssetBrowser()
  {
  delete _ui;
  }

AssetManager *AssetBrowser::getManager()
  {
  return &_browser->manager;
  }

Shift::Set *AssetBrowser::getHandleParent()
  {
  return &_browser->assetHandles;
  }

AssetType *AssetBrowser::getAssetHandle(const QUuid &u)
  {
  return _browser->findHandle(u);
  }

void AssetBrowser::tearDownProject(bool *abort)
  {
  auto currentUser = _project->getCurrentProjectUserData();
  currentUser->openFiles.clear();
  xForeach(AssetType *a, _browser->assetHandles.walker<AssetType>())
    {
    if (a->offerToSave() != Application::Success)
      {
      *abort = true;
      }
    }

  xForeach(auto &a, _project->openEditors())
    {
    if (!a.second->isVisible())
      {
      continue;
      }

    auto prop = currentUser->openFiles.add<Shift::StringProperty>();
    prop->assign(a.first->path());
    }

  _browser->clear();
  }

void AssetBrowser::setupProject()
  {
  auto currentProject = _project->getCurrentProject();
  _ui->dockWidgetContents->setEnabled(currentProject != nullptr);
  _ui->treeView->setModel(nullptr);

  if(!currentProject)
    {
    return;
    }

  _browser->manager.reset(_browser);

  QFileInfo project = currentProject->path().toQString();
  auto root = project.dir().absolutePath();
  _model->setRootPath(root);
  _model->setNameFilters(QStringList() << ASSET_FILTER);
  connect(_model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(fileAdded(QModelIndex,int,int)));

  _ui->treeView->setModel(_model);
  _ui->treeView->setRootIndex(_model->index(root));

  loadAllAssets(root);
  rebuildUI();

  connect(_ui->treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(openAsset(const QModelIndex &)));
  connect(_ui->add, SIGNAL(clicked()), this, SLOT(addAsset()));
  connect(_ui->remove, SIGNAL(clicked()), this, SLOT(removeAsset()));

  auto currentUser = _project->getCurrentProjectUserData();
  xForeach(auto file, currentUser->openFiles.walker<Shift::StringProperty>())
    {
    auto &toOpen = file->value();
    openAsset(toOpen.toQString());
    }
  }

void AssetBrowser::rebuildUI()
  {
  }

void AssetBrowser::openAsset(const QModelIndex &i)
  {
  openAsset(_model->filePath(i));
  }

void AssetBrowser::openAsset(const QString &path)
  {
  auto handle = _browser->loadHandle(path);
  if (handle)
    {
    _project->openAssetEditor(handle);
    }
  }

void AssetBrowser::addAsset()
  {
  auto f = new NewAsset(this);

  QFileInfo info(_project->getCurrentProject()->path().data());
  QString parentPath = info.dir().absolutePath();

  auto rows = _ui->treeView->selectionModel()->selectedRows();
  if (rows.size() > 0 && _model->isDir(rows[0]))
    {
    parentPath = _model->filePath(rows[0]);
    }

  if (f->show(parentPath) != QDialog::Accepted)
    {
    return;
    }

  QString path = f->location();

  _browser->createAsset(f->type(), path);
  openAsset(path);
  }

void AssetBrowser::removeAsset()
  {
  auto rows = _ui->treeView->selectionModel()->selectedRows();

  if (QMessageBox::question(
        nullptr,
        "Delete Assets",
        tr("Are you sure you want to delete %n asset(s)?", 0, rows.size()),
        QMessageBox::Yes,
        QMessageBox::No) != QMessageBox::Yes)
    {
    return;
    }

  xForeach (auto r, rows)
    {
    QFile f(_model->filePath(r));

    f.remove();
    }
  }

void AssetBrowser::fileAdded(const QModelIndex &parent, int first, int last)
  {
  for (int i = first; i <= last; ++i)
    {
    QModelIndex index = parent.child(i, 0);
    QString path = _model->filePath(index);

    QFileInfo info(path);
    if (info.suffix() != ASSET_EXT)
      {
      continue;
      }

    _browser->loadHandle(path);
    }
  }

void AssetBrowser::loadAllAssets(const QString &dir)
  {
  xForeach(auto file, QDir(dir).entryList(
      QStringList() << ASSET_FILTER,
      QDir::Files|QDir::AllDirs|QDir::NoDotAndDotDot))
    {
    QString path = dir + "/" + file;
    if (QFileInfo(path).isDir())
      {
      loadAllAssets(path);
      continue;
      }

    _browser->loadHandle(path);
    }
  }
}

}
