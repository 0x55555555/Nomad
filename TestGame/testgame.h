#pragma once
#include "NGlobal.h"
#include "XCore.h"
#include "shift/TypeInformation/styperegistry.h"
#include "NAssetManager.h"
#include <set>

namespace Eks
{
class Renderer;
class FrameBuffer;
}

namespace Game
{

Shift::PropertyGroup &propertyGroup();

class Canvas : public QObject
  {
  Q_OBJECT
public:
  Canvas();
  ~Canvas();

public slots:
  void initialise3D(Eks::Renderer *r);
  void paint3D(Eks::Renderer *r, Eks::FrameBuffer *buffer);
  void resize3D(Eks::Renderer *r, xuint32 w, xuint32 h);

private:
  };

class AssetInterface : public Nomad::AssetManager::Interface
  {
public:
  AssetInterface();

  template <typename T> T *loadPrimaryAsset(const char *str)
    {
    auto ass = loadPrimaryAsset(str);
    xAssert(ass);
    return ass ? ass->castTo<T>() : nullptr;
    }

  Nomad::Asset *loadPrimaryAsset(const char *str);
  Nomad::Asset *load(const char *name, Shift::Array *parent);

  void triggerReload(const QUuid &uuid);

private:
  Nomad::Asset *load(const QUuid &name, Shift::Array *parent) X_OVERRIDE;
  bool requiresReload(const QUuid &id) X_OVERRIDE;

  std::set<QUuid> _toReload;
  };


class TestGame
  {
public:
  TestGame()
    {
    Shift::TypeRegistry::initiate(core.defaultAllocator());

    Nomad::initiate();

    Shift::TypeRegistry::addPropertyGroup(Game::propertyGroup());
    }

  ~TestGame()
    {
    Shift::TypeRegistry::terminate();
    }

private:
  Eks::Core core;
  Canvas _canvas;
  };


}
