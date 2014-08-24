#pragma once
#include "NGlobal.h"
#include "XCore.h"
#include "QObject"

namespace Eks
{
class Renderer;
class FrameBuffer;
}

namespace Nomad
{

namespace Editor
{

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

}

}
