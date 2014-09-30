#include "ShaderVertexComponentType.h"
#include "ShaderVertexComponent.h"
#include "VertexDescription.h"
#include "VertexDescriptionType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"
#include "NAssetManager.h"
#include "AssetSelector.h"
#include "Utilities/XParseException.h"
#include "QVBoxLayout"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ShaderVertexComponentType, NomadRendererEditor)

void ShaderVertexComponentType::createTypeInformation(
    Shift::PropertyInformationTyped<ShaderVertexComponentType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  auto layout = childBlock.add(&ShaderVertexComponentType::_layout, "layout");
  layout->setResolveFunction(AssetManager::resolveAssetPointer);
  }

const char *ShaderVertexComponentType::extension() const
  {
  return "shav";
  }

QByteArray ShaderVertexComponentType::defaultSource() const
  {
  return
"layout (std140) uniform cb0 { mat4 model; mat4 modelView; mat4 modelViewProj; };\n"
"layout (std140) uniform cb1 { mat4 view; mat4 proj; };\n"
"\n"
"in vec3 position;\n"
"\n"
"void main()\n"
"  {\n"
"  gl_Position = modelViewProj * vec4(position, 1.0);\n"
"  }\n";
  }

Asset *ShaderVertexComponentType::processSource(const QByteArray &source, CreateInterface *c)
  {
  auto comp = assetParent()->add<ShaderVertexComponent>();

  auto layout = _layout.pointed<VertexDescription>();
  if(!layout)
    {
    addError(X_CURRENT_CODE_LOCATION_DETAILED, "Missing vertex layout");
    return 0;
    }

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

    ShaderVertexComponentType *asset;
    } ifc;
  ifc.asset = this;

  if(!Eks::ShaderVertexComponent::delayedCreate(
      comp->component(),
      c->renderer(),
      source.data(),
      source.length(),
      layout->layout().data(),
      layout->layout().size(),
      &comp->layout(),
      &ifc))
    {
    return comp;
    }

  comp->setLayoutDescription(layout);

  return comp;
  }

QWidget *ShaderVertexComponentType::createEditor(ProjectInterface *ifc, CreateInterface *c)
  {
  QWidget *w = new QWidget();
  QVBoxLayout *l = new QVBoxLayout();
  l->setContentsMargins(0, 0, 0, 0);
  w->setLayout(l);

  auto box = new AssetSelector(ifc, c, &_layout, VertexDescriptionType::staticTypeInformation(), w);
  QObject::connect(box, &AssetSelector::assetChanged, [this, c]()
    {
    reinitialise(source(), c);
    });
  l->addWidget(box);

  auto editor = ExternalSourceAsset::createEditor(ifc, c);
  editor->setParent(w);
  l->addWidget(editor);

  return w;
  }

}

}
