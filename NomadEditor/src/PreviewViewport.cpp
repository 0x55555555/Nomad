#include "PreviewViewport.h"
#include "X3DCanvas.h"
#include "QVBoxLayout"
#include "QToolBar"
#include "QAction"
#include "XModeller.h"
#include "XCore.h"
#include "XFramebuffer.h"

namespace Nomad
{

namespace Editor
{

PreviewViewport::PreviewViewport(UIInterface *r)
    : _geometryInitialised(false),
      _shaderInitialised(false),
      _defaultLayout(Eks::Core::defaultAllocator()),
      _t(0.0f),
      _distance(5.0f),
      _spin(true)	
  {
  QVBoxLayout *l = new QVBoxLayout();
  l->setContentsMargins(0, 0, 0, 0);
  l->setSpacing(0);
  setLayout(l);

  auto t = new QToolBar(this);
  l->addWidget(t);

  auto spin = t->addAction("Spin");
  spin->setCheckable(true);
  spin->setChecked(_spin);
  connect(spin, &QAction::toggled, [this](bool t)
    {
    _spin = t;
    }
  );

  _widget = r->createViewport(this);
  l->addWidget(_widget);

  connect(&_refresh, SIGNAL(timeout()), this, SLOT(tick()));
  _refresh.start(1000.0/60.0);

  connect(_widget, SIGNAL(initialise3D(Eks::Renderer*)), this, SLOT(initialise3D(Eks::Renderer*)));
  connect(_widget, SIGNAL(paint3D(Eks::Renderer*,Eks::FrameBuffer*)), this, SLOT(paint3D(Eks::Renderer*,Eks::FrameBuffer*)));
  connect(_widget, SIGNAL(resize3D(Eks::Renderer*,xuint32,xuint32)), this, SLOT(resize3D(Eks::Renderer*,xuint32,xuint32)));

  _view = spinTransform(0.0f);
  }

void PreviewViewport::initialise3D(Eks::Renderer *r)
  {
  Eks::DepthStencilState::delayedCreate(
    _state,
    r,
    Eks::DepthStencilState::MaskAll,
    Eks::DepthStencilState::DepthTest);
  }

void PreviewViewport::resize3D(Eks::Renderer *, xuint32 w, xuint32 h)
  {
  float aspect = (float)w / (float)h;

  _proj = Eks::TransformUtilities::perspective(Eks::degreesToRadians(45.0f), aspect, 0.1f, 100.0f);
  }

void PreviewViewport::tick()
  {
  if(_spin)
    {
    _t += 0.02f;

    _view = spinTransform(_t);
    }

  _widget->update();
  }

void PreviewViewport::bindShader(Eks::Renderer *r)
  {
  if (!_shaderInitialised)
    {
    _shaderInitialised = true;
    const char *fsrc =
        "#if X_GLSL_VERSION >= 130 || defined(X_GLES)\n"
        "precision mediump float;\n"
        "#endif\n"
        "in vec3 colOut;"
        "out vec4 outColour;"
        "void main(void)"
        "  {"
        "  outColour = vec4(abs(colOut), 1.0);"
        "  }";

    const char *vsrc =
        "layout (std140) uniform cb0 { mat4 model; mat4 modelView; mat4 modelViewProj; };"
        "layout (std140) uniform cb1 { mat4 view; mat4 proj; };"
        "in vec3 position;"
        "in vec3 normal;"
        "out vec3 colOut;"
        "void main(void)"
        "  {"
        "  colOut = normal;"
        "  gl_Position = modelViewProj * vec4(position, 1.0);"
        "  }";

    auto& desc = vertexLayout();

    Eks::ShaderVertexComponent::delayedCreate(_v, r, vsrc, strlen(vsrc), desc.data(), desc.size(), &_layout);
    Eks::ShaderComponent::delayedCreate(_f, r, Eks::ShaderComponent::Fragment, fsrc, strlen(fsrc));

    Eks::ShaderComponent* comps[] = { &_v, &_f };
    const char *outputs[] = { "outColour" };
    Eks::Shader::delayedCreate(_shader, r, comps, X_ARRAY_COUNT(comps), outputs, X_ARRAY_COUNT(outputs));
    }

  r->setShader(&_shader, &_layout);
  }

void PreviewViewport::renderGeometry(Eks::Renderer *r)
  {
  if (!_geometryInitialised)
    {
    _geometryInitialised = true;

    Eks::Modeller m(Eks::Core::defaultAllocator());
    m.drawCube();

    Eks::Vector<Eks::ShaderVertexLayoutDescription::Semantic, 8> desc;
    xForeach(auto &d, vertexLayout())
      {
      desc << d.semantic;
      }

    m.bakeTriangles(r, desc.data(), desc.size(), &_igeo, &_geo);
    }
  r->drawTriangles(&_igeo, &_geo);
  }

const Eks::Vector<Eks::ShaderVertexLayoutDescription> &PreviewViewport::vertexLayout() const
  {
  if (_defaultLayout.isEmpty())
    {
    _defaultLayout <<
      Eks::ShaderVertexLayoutDescription(Eks::ShaderVertexLayoutDescription::Position,
        Eks::ShaderVertexLayoutDescription::FormatFloat3);
    _defaultLayout <<
      Eks::ShaderVertexLayoutDescription(Eks::ShaderVertexLayoutDescription::Normal,
        Eks::ShaderVertexLayoutDescription::FormatFloat3);
    _defaultLayout <<
      Eks::ShaderVertexLayoutDescription(Eks::ShaderVertexLayoutDescription::TextureCoordinate,
        Eks::ShaderVertexLayoutDescription::FormatFloat2);
    }
  return _defaultLayout;
  }

void PreviewViewport::paint3D(Eks::Renderer *r, Eks::FrameBuffer *buffer)
  {
  Eks::FrameBufferRenderFrame fr(r, buffer);

  r->setProjectionTransform(_proj);
  r->setViewTransform(_view);

  r->setTransform(Eks::Transform::Identity());

  r->setDepthStencilState(&_state);

  bindShader(r);
  renderGeometry(r);
  }

Eks::Transform PreviewViewport::spinTransform(float t)
  {
  return Eks::TransformUtilities::lookAt(
    Eks::Vector3D(sinf(t) * _distance, 3, cosf(t) * _distance),
    Eks::Vector3D(0, 0, 0),
    Eks::Vector3D(0, 1, 0));
  }

}

}
