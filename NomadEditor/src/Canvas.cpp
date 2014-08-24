#include "Canvas.h"
#include "X3DCanvas.h"

namespace Nomad
{

namespace Editor
{

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

void Canvas::initialise3D(Eks::Renderer *)
  {
  xAssertFail();
  }

void Canvas::paint3D(Eks::Renderer *, Eks::FrameBuffer *)
  {
  xAssertFail();
  }

void Canvas::resize3D(Eks::Renderer *, xuint32, xuint32)
  {
  xAssertFail();
  }

}

}
