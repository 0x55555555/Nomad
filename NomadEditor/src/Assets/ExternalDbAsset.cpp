	#include "Assets/ExternalDbAsset.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"
#include "Application.h"
#include "TextEditor.h"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ExternalDbAsset, Nomad)

void ExternalDbAsset::createTypeInformation(
    Shift::PropertyInformationTyped<ExternalDbAsset> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

void ExternalDbAsset::clear()
  {
  ExternalAssetType::clear();
  auto asset = cachedAsset();
  if (auto parent = asset->parent()->castTo<Shift::Set>())
    {
    parent->remove(asset);
    }
  }

Asset *ExternalDbAsset::process(const QByteArray &src, CreateInterface *)
  {
  auto asset = Application::loadSource(src, assetParent());
  if(!asset)
    {
    return nullptr;
    }

  return asset->castTo<Asset>();
  }

QByteArray ExternalDbAsset::unprocess()
  {
  return source();
  }

QWidget *ExternalDbAsset::createEditor(ProjectInterface *, CreateInterface *c)
  {
  class Editor : public TextEditor
    {
  public:
    Editor(const QString &s, ExternalDbAsset *ass, CreateInterface *c)
        : TextEditor(s),
          _asset(ass),
          _ctx(c)
      {
      connect(
        this,
        &QTextEdit::textChanged,
        [this]()
          {
          _asset->clear();
          _asset->initialiseFromSource(toPlainText().toUtf8(), _ctx);
          _asset->setNeedsSave();
          }
        );
      }

  private:
    ExternalDbAsset *_asset;
    CreateInterface *_ctx;
    };

  return new Editor(source(), this, c);
  }

QByteArray ExternalDbAsset::source() const
  {
  auto ass = cachedAsset();
  if(!ass)
    {
    return getDiskSource();
    }

  return Application::toSource(ass);
  }

bool ExternalDbAsset::save()
  {
  auto ass = cachedAsset();
  if (ass)
    {
    auto obj = ass->castTo<File>();
    if (obj)
      {
      obj->setSaved();
      }
    }

  return ExternalAssetType::save();
  }

bool ExternalDbAsset::needsSave()
  {
  if (AssetType::needsSave())
    {
    return true;
    }

  auto ass = cachedAsset();
  if (!ass)
    {
    return false;
    }

  auto obj = ass->castTo<File>();
  if (!obj)
    {
    xAssertFail();
    return false;
    }

  return obj->hasChangedFromFile();
  }
}

}
