#include "testgame.h"
#include "QtWidgets/QApplication"
#include "shift/TypeInformation/spropertygroup.h"
#include "shift/Serialisation/sloader.h"
#include "shift/Serialisation/sjsonio.h"
#include "NReloadableAsset.h"
#include "X3DCanvas.h"
#include "QFile"

int main(int argc, char **argv)
  {
  QApplication app(argc, argv);

  Game::TestGame game;

  return app.exec();
  }

namespace Game
{
Shift::PropertyGroup &propertyGroup()
  {
  static Shift::PropertyGroup grp;
  return grp;
  }


Canvas::Canvas()
  {
  Eks::AbstractCanvas *canvas = nullptr;
  auto widget = Eks::Canvas3D::createBest(nullptr, &canvas);
  xAssert(widget);

  connect(widget, SIGNAL(initialise3D(Eks::Renderer *)), this, SLOT(initialise3D(Eks::Renderer*)));
  connect(widget, SIGNAL(paint3D(Eks::Renderer*,Eks::FrameBuffer*)), this, SLOT(paint3D(Eks::Renderer*,Eks::FrameBuffer*)));
  connect(widget, SIGNAL(resize3D(Eks::Renderer*,xuint32,xuint32)), this, SLOT(resize3D(Eks::Renderer*,xuint32,xuint32)));

  widget->resize(640, 480);
  }

Canvas::~Canvas()
  {
  }

void Canvas::initialise3D(Eks::Renderer *r)
  {
  }

void Canvas::paint3D(Eks::Renderer *r, Eks::FrameBuffer *buffer)
  {
  }

void Canvas::resize3D(Eks::Renderer *r, xuint32 w, xuint32 h)
  {
  }


AssetInterface::AssetInterface()
  {
  }


Nomad::Asset *AssetInterface::loadPrimaryAsset(const char *str)
  {
  xAssert(manager());
  return load(str, manager()->assetContainer());
  }

Nomad::Asset *AssetInterface::load(const char *name, Shift::Array *parent)
  {
  QFile toLoad(name);
  if(!toLoad.open(QFile::ReadOnly))
    {
    xAssertFail();
    return nullptr;
    }

  Shift::LoadBuilder builder;
  Eks::TemporaryAllocator alloc(parent->temporaryAllocator());
  auto loading = builder.beginLoading(parent, &alloc);

  Shift::JSONLoader loader;
  loader.load(&toLoad, &builder);

  xAssert(loading->loadedData().size() == 1);
  if (loading->loadedData().size() != 1)
    {
    return nullptr;
    }

  auto asset = loading->loadedData().front()->castTo<Nomad::Asset>();
  manager()->registerAsset(asset);

  return asset;
  }

void AssetInterface::triggerReload(const QUuid &uuid)
  {
  _toReload.insert(uuid);
  }

Nomad::Asset *AssetInterface::load(const QUuid &name, Shift::Array *parent) X_OVERRIDE
  {
  _toReload.erase(name);

  QString path = name.toString();

  path = ":/resources/" + path + ".json";

  return load(path.toUtf8().data(), parent);
  }

bool AssetInterface::requiresReload(const QUuid &id) X_OVERRIDE
  {
  return _toReload.find(id) != _toReload.end();
  }


}
