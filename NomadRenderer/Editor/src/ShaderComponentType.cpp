#include "ShaderComponentType.h"
#include "ShaderComponent.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"
#include "NAssetManager.h"
#include "AssetSelector.h"
#include "Utilities/XParseException.h"
#include "QVBoxLayout"
#include "QComboBox"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ShaderComponentType, NomadRendererEditor)

void ShaderComponentType::createTypeInformation(
    Shift::PropertyInformationTyped<ShaderComponentType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  auto t = childBlock.add(&ShaderComponentType::type, "type");
  t->setDefault(Eks::ShaderComponent::Fragment);
  }

const char *ShaderComponentType::extension() const
  {
  return "shac";
  }

QByteArray ShaderComponentType::defaultSource() const
  {
  return
"precision mediump float;"
"\n"
"out vec4 outColour;\n"
"\n"
"void main()\n"
"{\n"
"  outColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";
  }

Asset *ShaderComponentType::processSource(const QByteArray &source, CreateInterface *c)
  {
  auto comp = assetParent()->add<ShaderComponent>();

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

    ShaderComponentType *asset;
    } ifc;
  ifc.asset = this;

  if(!Eks::ShaderComponent::delayedCreate(
      comp->component(),
      c->renderer(),
      (Eks::ShaderComponent::ShaderType)type(),
      source.data(),
      source.length(),
      &ifc))
    {
    return comp;
    }

  return comp;
  }

QWidget *ShaderComponentType::createEditor(ProjectInterface *ifc, CreateInterface *c)
  {
  QWidget *w = new QWidget();
  QVBoxLayout *l = new QVBoxLayout();
  l->setContentsMargins(0, 0, 0, 0);
  w->setLayout(l);

  auto box = new QComboBox();
  xCompileTimeAssert(Eks::ShaderComponent::ShaderComponentCount == 5);
  box->addItems(
    QStringList() <<
        "TesselationControl" <<
        "TesselationEvaluator" <<
        "Fragment" <<
        "Geometry");
  enum { Offset = Eks::ShaderComponent::TesselationControl };
  box->setCurrentIndex(type()-Offset);
  QObject::connect(
    box,
    (void (QComboBox::*)(int))&QComboBox::currentIndexChanged,
    [this, c, box](int i)
    {
    type = i+Offset;
    reload(c);
    });
  l->addWidget(box);

  auto editor = ExternalSourceAsset::createEditor(ifc, c);
  editor->setParent(w);
  l->addWidget(editor);

  return w;
  }

}

}
