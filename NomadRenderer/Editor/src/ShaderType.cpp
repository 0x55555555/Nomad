#include "ShaderType.h"
#include "Shader.h"
#include "ShaderVertexComponent.h"
#include "ShaderVertexComponentType.h"
#include "ShaderComponent.h"
#include "ShaderComponentType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "Utilities/XParseException.h"
#include "QVBoxLayout"
#include "QLabel"
#include "QPushButton"
#include "AssetSelector.h"
#include "NAssetManager.h"
#include "NAsset.h"
#include "PreviewViewport.h"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ShaderType, NomadRendererEditor)

void ShaderType::createTypeInformation(
    Shift::PropertyInformationTyped<ShaderType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  auto f = childBlock.add(&ShaderType::_frag, "frag");
  f->setResolveFunction(AssetManager::resolveAssetPointer);
  auto v = childBlock.add(&ShaderType::_vert, "vert");
  v->setResolveFunction(AssetManager::resolveAssetPointer);
  }

const char *ShaderType::extension() const
  {
  return "shader";
  }

QByteArray ShaderType::defaultSource() const
  {
  return "";
  }

Asset *ShaderType::processSource(const QByteArray &, CreateInterface *c)
  {
  auto sha = assetParent()->add<Shader>();

  auto v = _vert.pointed<ShaderVertexComponent>();
  auto f = _frag.pointed<ShaderComponent>();
  if (!v || !f)
    {
    addError(X_CURRENT_CODE_LOCATION_DETAILED, "Bad shader component.");
    return nullptr;
    }

  Eks::ShaderComponent *comps[] = {
    &v->component(),
    &f->component()
  };

  const char *outputs[] = {
    "outColour"
  };

  struct Error : public Eks::ParseErrorInterface
    {
    void error(const Eks::ParseError &e) X_OVERRIDE
      {
      asset->addError(e);
      }

    void warning(const Eks::ParseError &e) X_OVERRIDE
      {
      asset->addWarning(e);
      }

    ShaderType *asset;
    } ifc;
  ifc.asset = this;

  if(!Eks::Shader::delayedCreate(
      sha->shader(),
      c->renderer(),
      comps,
      X_ARRAY_COUNT(comps),
      outputs,
      X_ARRAY_COUNT(outputs),
      &ifc))
    {
    return sha;
    }

  sha->setVertexComponent(v);

  return sha;
  }

QWidget *ShaderType::createEditor(ProjectInterface *ifc, CreateInterface *c)
  {
  QWidget *w = new QWidget();
  QVBoxLayout *l = new QVBoxLayout();
  l->setContentsMargins(0, 0, 0, 0);
  l->setSpacing(0);
  w->setLayout(l);

  QHBoxLayout *h = new QHBoxLayout();
  h->setContentsMargins(0, 0, 0, 0);
  h->setSpacing(8);
  l->addLayout(h);

  QWidget *fWidg = nullptr;
  QWidget *vWidg = nullptr;

  auto fIfc = ifc->createAssetEditor(_frag.uuid(), w, &fWidg);
  auto vIfc = ifc->createAssetEditor(_vert.uuid(), w, &vWidg);

  h->addWidget(new QLabel("Vertex"));
  auto box = new AssetSelector(ifc, c, &_vert, ShaderVertexComponentType::staticTypeInformation(), w);
  box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  QObject::connect(box, &AssetSelector::assetChanged, [this, c, vIfc]()
    {
    reinitialise(source(), c);
    vIfc->setAsset(_vert.uuid());
    });
  h->addWidget(box);

  vWidg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  l->addWidget(vWidg);

  h = new QHBoxLayout();
  h->setContentsMargins(0, 0, 0, 0);
  h->setSpacing(8);
  l->addLayout(h);

  h->addWidget(new QLabel("Fragment"));
  box = new AssetSelector(ifc, c, &_frag, ShaderComponentType::staticTypeInformation(), w);
  box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  QObject::connect(box, &AssetSelector::assetChanged, [this, c, fIfc]()
    {
    reinitialise(source(), c);
    fIfc->setAsset(_frag.uuid());
    });
  h->addWidget(box);

  fWidg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  l->addWidget(fWidg);
  return w;
  }

QWidget *ShaderType::createPreview(UIInterface *ifc, CreateInterface *c)
  {
  struct ShaderViewport : public PreviewViewport
    {
    ShaderViewport(UIInterface *ifc, CreateInterface *c, ShaderType *m)
        : PreviewViewport(ifc),
          createInterface(c),
          shader(m)
      {
      auto myPath = m->relativePath();
      auto modes = shaderModes();
      modes << myPath;
      setShaderModes(modes);
      setCurrentShaderMode(myPath);
      }

    const VertexDescription::Layout *vertexLayout() const X_OVERRIDE
      {
      auto comp = shader->_vert.pointed<ShaderVertexComponent>();
      if (!comp)
        {
        return nullptr;
        }

      layout = const_cast<VertexDescription*>(comp->layoutDescription());
      if (!layout)
        {
        return nullptr;
        }

      return &layout->layout();
      }

    void bindShader(Eks::Renderer *r, const QString &name) X_OVERRIDE
      {
      auto a = shader->asset(createInterface);
      if (a && name == shader->relativePath())
        {
        Nomad::Shader *m = a->castTo<Nomad::Shader>();
        r->setShader(&m->shader(), &m->vertexComponent()->layout());
        return;
        }

      return PreviewViewport::bindShader(r, name);
      }

    mutable Shift::EntityWeakPointer<VertexDescription> layout;
    CreateInterface *createInterface;
    ShaderType *shader;
    };

  return new ShaderViewport(ifc, c, this);
  }


}

}
