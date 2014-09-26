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

#define DEFAULT_SHADER "Default"
#define COLOUR_DEBUG "Colour Debug"
#define NORMAL_DEBUG "Normal Debug"
#define TEXTURE_DEBUG "Texture Debug"

const char *defaultShaderSrc =
    "#if X_GLSL_VERSION >= 130 || defined(X_GLES)\n"
    "precision mediump float;\n"
    "#endif\n"
    "in vec2 textureCoordinateOut;"
    "in vec3 normalOut;"
    "out vec4 outColour;"
    "void main(void)"
    "  {"
    "  vec3 col = vec3(1.0, 1.0, 1.0);"
    "  vec3 dir = normalize(vec3(1.0, 1.0, 1.0));"
    "  outColour = vec4(col * clamp(dot(normalOut, dir), 0.0, 1.0), 1.0);"
    "  }";

const char *normalShaderSrc =
    "#if X_GLSL_VERSION >= 130 || defined(X_GLES)\n"
    "precision mediump float;\n"
    "#endif\n"
    "in vec2 textureCoordinateOut;"
    "in vec3 normalOut;"
    "out vec4 outColour;"
    "void main(void)"
    "  {"
    "  outColour = vec4(abs(normalOut), 1.0);"
    "  }";

const char *textureShaderSrc =
    "#if X_GLSL_VERSION >= 130 || defined(X_GLES)\n"
    "precision mediump float;\n"
    "#endif\n"
    "in vec3 normalOut;"
    "in vec2 textureCoordinateOut;"
    "out vec4 outColour;"
    "void main(void)"
    "  {"
    "  outColour = vec4(abs(textureCoordinateOut), 0.0, 1.0);"
    "  }";

const char *colourShaderSrc =
    "#if X_GLSL_VERSION >= 130 || defined(X_GLES)\n"
    "precision mediump float;\n"
    "#endif\n"
    "out vec4 outColour;"
    "void main(void)"
    "  {"
    "  outColour = vec4(1.0, 1.0, 1.0, 1.0);"
    "  }";

const char *vsrc =
    "layout (std140) uniform cb0 { mat4 model; mat4 modelView; mat4 modelViewProj; };"
    "layout (std140) uniform cb1 { mat4 view; mat4 proj; };"
    "in vec3 position;"
    "in vec3 normal;"
    "in vec2 textureCoordinate;"
    "out vec3 normalOut;"
    "out vec2 textureCoordinateOut;"
    "void main(void)"
    "  {"
    "  normalOut = normal;"
    "  textureCoordinateOut = textureCoordinate;"
    "  gl_Position = modelViewProj * vec4(position, 1.0);"
    "  }";

PreviewViewport::PreviewViewport(UIInterface *r)
    : _geometryInitialised(false),
      _shaderInitialised(false),
      _gridInitialised(false),
      _defaultLayout(Eks::Core::defaultAllocator()),
      _t(0.0f),
      _distance(5.0f),
      _spin(true),
      _grid(true)
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

  auto grid = t->addAction("Grid");
  grid->setCheckable(true);
  grid->setChecked(_grid);
  connect(grid, &QAction::toggled, [this](bool t)
    {
    _grid = t;
    }
  );

  _modes = new QComboBox(this);
  t->addWidget(_modes);
  setShaderModes(QStringList() << DEFAULT_SHADER << COLOUR_DEBUG << NORMAL_DEBUG << TEXTURE_DEBUG);

  Eks::AbstractCanvas* canvas = nullptr;
  _widget = r->createViewport(this, &canvas);
  xAssert(canvas);
  setCanvas(canvas);
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

void PreviewViewport::bindShader(Eks::Renderer *r, const QString &mode)
  {
  auto* desc = vertexLayout();
  if (!desc || !desc->size())
    {
    return;
    }

  if((!_shaderInitialised && desc) || desc != _oldLayout)
    {
    _shaderInitialised = true;

    auto buildShader = [desc, r](const char *vsrc, const char *fsrc, Shader *s)
      {
      s->vert.~ShaderVertexComponent();
      s->frag.~ShaderComponent();
      s->shader.~Shader();
      s->layout.~ShaderVertexLayout();
      new(&s->vert) Eks::ShaderVertexComponent(r, vsrc, strlen(vsrc), desc->data(), desc->size(), &s->layout);
      new(&s->frag) Eks::ShaderComponent(r, Eks::ShaderComponent::Fragment, fsrc, strlen(fsrc));

      Eks::ShaderComponent* comps[] = { &s->vert, &s->frag };
      const char *outputs[] = { "outColour" };
      new(&s->shader) Eks::Shader(r, comps, X_ARRAY_COUNT(comps), outputs, X_ARRAY_COUNT(outputs));
      };


    buildShader(vsrc, normalShaderSrc, &_normal);
    buildShader(vsrc, textureShaderSrc, &_texture);
    buildShader(vsrc, colourShaderSrc, &_colour);
    buildShader(vsrc, defaultShaderSrc, &_default);
    }

  if(mode == NORMAL_DEBUG)
    {
    r->setShader(&_normal.shader, &_normal.layout);
    }
  else if(mode == TEXTURE_DEBUG)
    {
    r->setShader(&_texture.shader, &_texture.layout);
    }
  else if(mode == COLOUR_DEBUG)
    {
    r->setShader(&_colour.shader, &_colour.layout);
    }
  else if(mode == DEFAULT_SHADER)
    {
    r->setShader(&_default.shader, &_default.layout);
    }
  }

void PreviewViewport::renderGeometry(Eks::Renderer *r)
  {
  auto* layout = vertexLayout();

  if(!_geometryInitialised || layout != _oldLayout)
    {
    _geometryInitialised = true;

    Eks::Modeller m(Eks::Core::defaultAllocator());
    m.drawCube();
    m.drawSphere(0.7f, 36, 36);

    Eks::Vector<Eks::ShaderVertexLayoutDescription::Semantic, 8> desc;
    if(auto layout = vertexLayout())
      {
      xForeach(auto &d, *layout)
        {
        desc << d.semantic;
        }
      }

    _igeo.~IndexGeometry();
    _geo.~Geometry();
    new(&_igeo) Eks::IndexGeometry();
    new(&_geo) Eks::Geometry();
    m.bakeTriangles(r, desc.data(), desc.size(), &_igeo, &_geo);
    }
  r->drawTriangles(&_igeo, &_geo);
  }

const Eks::Vector<Eks::ShaderVertexLayoutDescription, 8> *PreviewViewport::vertexLayout() const
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
  return &_defaultLayout;
  }

void PreviewViewport::paint3D(Eks::Renderer *r, Eks::FrameBuffer *buffer)
  {
  Eks::FrameBufferRenderFrame fr(r, buffer);

  r->setProjectionTransform(_proj);
  r->setViewTransform(_view);

  r->setTransform(Eks::Transform::Identity());

  r->setDepthStencilState(&_state);

  bindShader(r, _modes->currentText());
  renderGeometry(r);

  if (_grid)
    {
    renderGrid(r);
    }

  _oldLayout = vertexLayout();
  }

Eks::Transform PreviewViewport::spinTransform(float t)
  {
  return Eks::TransformUtilities::lookAt(
    Eks::Vector3D(sinf(t) * _distance, 3, cosf(t) * _distance),
    Eks::Vector3D(0, 0, 0),
    Eks::Vector3D(0, 1, 0));
  }

void PreviewViewport::setShaderModes(const QStringList &l)
  {
  _shaders = l;
  _modes->clear();
  _modes->addItems(_shaders);
  }

void PreviewViewport::renderGrid(Eks::Renderer *r)
  {
  if(!_gridInitialised)
    {
    const Eks::ShaderVertexLayoutDescription desc[] = {
      { Eks::ShaderVertexLayoutDescription::Position, Eks::ShaderVertexLayoutDescription::FormatFloat3 }
    };
    const Eks::ShaderVertexLayoutDescription::Semantic descSem[] = {
      Eks::ShaderVertexLayoutDescription::Position
    };
    _gridInitialised = true;
    Eks::ShaderVertexComponent::delayedCreate(_gridShader.vert, r, vsrc, strlen(vsrc), desc, X_ARRAY_COUNT(desc), &_gridShader.layout);
    Eks::ShaderComponent::delayedCreate(_gridShader.frag, r, Eks::ShaderComponent::Fragment, colourShaderSrc, strlen(colourShaderSrc));

    Eks::ShaderComponent* comps[] = { &_gridShader.vert, &_gridShader.frag };
    const char *outputs[] = { "outColour" };
    Eks::Shader::delayedCreate(_gridShader.shader, r, comps, X_ARRAY_COUNT(comps), outputs, X_ARRAY_COUNT(outputs));

    Eks::Modeller m(Eks::Core::defaultAllocator());

    m.begin(Eks::Modeller::Lines);
      for (int i = -5; i <= 5; ++i)
        {
        m.vertex(-5, 0, i);
        m.vertex(5, 0, i);
        }
      for (int i = -5; i <= 5; ++i)
        {
        m.vertex(i, 0, -5);
        m.vertex(i, 0, 5);
        }
    m.end();

    m.bakeLines(r, descSem, X_ARRAY_COUNT(descSem), &_gridIGeo, &_gridGeo);
    }

  r->setShader(&_gridShader.shader, &_gridShader.layout);
  r->drawLines(&_gridIGeo, &_gridGeo);
  }

void PreviewViewport::zoom(float, float, float)
  {
  }

void PreviewViewport::track(float, float)
  {
  }

void PreviewViewport::dolly(float, float)
  {
  }

void PreviewViewport::pan(float, float)
  {
  }

}

}
