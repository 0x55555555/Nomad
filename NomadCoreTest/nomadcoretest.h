#ifndef NOMADTEST_H
#define NOMADTEST_H

#include <QtTest>
#include <functional>
#include "XCore.h"
#include "Memory/XGlobalAllocator.h"
#include "../Shift/ShiftCoreTest/shifttestcommon.h"
#include "shift/sentity.h"
#include "shift/Properties/sarray.h"
#include "shift/Properties/sbaseproperties.h"
#include "NGlobal.h"

namespace Eks
{
class Renderer;
class FrameBuffer;
class AbstractCanvas;
}

class NomadCoreTest : public QObject
  {
  Q_OBJECT

public:
  NomadCoreTest() : registry(core.defaultAllocator())
    {
    registry.installModule(Nomad::shiftModule());
    registry.installModule(Test::shiftModule());
    }

  ~NomadCoreTest()
    {
    }

private Q_SLOTS:
  void basicResource();
  void reloadResource();

private:
  Eks::Core core;
  Shift::TypeRegistry registry;
  };

#endif // RENDERTEST_H
