#include "VertexDescriptionType.h"
#include "VertexDescription.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(VertexDescriptionType, NomadRendererEditor)

void VertexDescriptionType::createTypeInformation(
    Shift::PropertyInformationTyped<VertexDescriptionType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

const char *VertexDescriptionType::extension()
  {
  return "vtxd";
  }

QByteArray VertexDescriptionType::defaultSource() const
  {
  return QByteArray();
  }

Asset *VertexDescriptionType::process(const QByteArray &s, CreateInterface *)
  {
  setSource(s);

  auto desc = assetParent()->add<VertexDescription>();

  const char *names[] = {
    "position",
    "colour",
    "texturecoordinate",
    "normal",
    "binormal",
  };
  xCompileTimeAssert(Eks::ShaderVertexLayoutDescription::SemanticCount == X_ARRAY_COUNT(names));

  QJsonParseError err;
  auto doc = QJsonDocument::fromJson(s, &err);
  if(err.error != QJsonParseError::NoError)
    {
    qWarning() << "invalid document: " << err.errorString();
    return desc;
    }

  auto root = doc.array();
  xForeach(auto el, root)
    {
    auto obj = el.toObject();

    QString name = obj["name"].toString();
    Eks::ShaderVertexLayoutDescription::Format f = (Eks::ShaderVertexLayoutDescription::Format)obj["count"].toInt();
    Eks::ShaderVertexLayoutDescription::Semantic semantic = Eks::ShaderVertexLayoutDescription::Position;

    if (f < 0 || f >= Eks::ShaderVertexLayoutDescription::FormatCount)
      {
      qWarning() << "invalid format" << obj["count"].toString();
      continue;
      }
    if (semantic < 0 || semantic >= Eks::ShaderVertexLayoutDescription::SemanticCount)
      {
      qWarning() << "invalid semantic" << obj["name"].toString();
      continue;
      }

    for (xsize i = 0; i < X_ARRAY_COUNT(names); ++i)
      {
      if(names[i] == name)
        {
        semantic = (Eks::ShaderVertexLayoutDescription::Semantic)i;
        break;
        }
      ++i;
      }

    desc->addElement(semantic, f);
    }


  if (desc->layout().isEmpty())
    {
    qWarning() << "empty description";
    }

  return desc;
  }

}

}
