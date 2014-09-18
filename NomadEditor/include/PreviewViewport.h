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

#include "QComboBox"

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
  virtual void bindShader(Eks::Renderer *r, const QString &shader);
  virtual void renderGeometry(Eks::Renderer *r);
  virtual const Eks::Vector<Eks::ShaderVertexLayoutDescription> &vertexLayout() const;

  Eks::Transform spinTransform(float t);

  void setShaderModes(const QStringList &l);
  const QStringList &shaderModes() const { return _shaders; }

private:
  QWidget *_widget;

  Eks::ComplexTransform _proj;
  Eks::Transform _view;

  bool _geometryInitialised;
  Eks::Geometry _geo;
  Eks::IndexGeometry _igeo;

  bool _shaderInitialised;
  struct Shader
    {
    Eks::ShaderVertexLayout layout;
    Eks::Shader shader;
    Eks::ShaderComponent frag;
    Eks::ShaderVertexComponent vert;
    };
  Shader _normal;
  Shader _texture;
  Shader _default;
  Eks::DepthStencilState _state;

  QStringList _shaders;
  QComboBox *_modes;

  mutable Eks::Vector<Eks::ShaderVertexLayoutDescription> _defaultLayout;

  float _t;
  float _distance;
  bool _spin;
  QTimer _refresh;
  };

}

}
