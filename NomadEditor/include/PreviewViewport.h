#pragma once
#include "NomadEditor.h"
#include "QtWidgets/QWidget"
#include "UIInterface.h"
#include "XShader.h"
#include "QTimer"
#include "XGeometry.h"
#include "XRasteriserState.h"
#include "XDepthStencilState.h"
#include "XTransform.h"
#include "Containers/XVector.h"


namespace Nomad
{
namespace Editor
{

class NOMAD_EDITOR_EXPORT PreviewViewport : public QWidget
  {
  Q_OBJECT

public:
  PreviewViewport(UIInterface *r);

protected slots:
  void initialise3D(Eks::Renderer *r);
  void resize3D(Eks::Renderer *r, xuint32 w, xuint32 h);
  void paint3D(Eks::Renderer *r, Eks::FrameBuffer *buffer);
  void tick();

protected:
  virtual void bindShader(Eks::Renderer *r);
  virtual void renderGeometry(Eks::Renderer *r);
  virtual const Eks::Vector<Eks::ShaderVertexLayoutDescription> &vertexLayout() const;

  Eks::Transform spinTransform(float t);

  QWidget *_widget;

  Eks::ComplexTransform _proj;
  Eks::Transform _view;

  bool _geometryInitialised;
  Eks::Geometry _geo;
  Eks::IndexGeometry _igeo;

  bool _shaderInitialised;
  Eks::ShaderVertexLayout _layout;
  Eks::Shader _shader;
  Eks::ShaderComponent _f;
  Eks::ShaderVertexComponent _v;
  Eks::DepthStencilState _state;

  mutable Eks::Vector<Eks::ShaderVertexLayoutDescription> _defaultLayout;

  float _t;
  float _distance;
  bool _spin;
  QTimer _refresh;
  };

}

}
