#include "Assets/ExternalSourceAsset.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"
#include "Application.h"
#include "TextEditor.h"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ExternalSourceAsset, Nomad)

void ExternalSourceAsset::createTypeInformation(
    Shift::PropertyInformationTyped<ExternalSourceAsset> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

ExternalSourceAsset::ExternalSourceAsset()
    : _hasSource(false)
  {
  }

void ExternalSourceAsset::clear()
  {
  ExternalAssetType::clear();
  auto asset = cachedAsset();
  if (!asset)
    {
    return;
    }

  _hasSource = false;
  if (auto parent = asset->parent()->castTo<Shift::Set>())
    {
    parent->remove(asset);
    }
  }

Asset *ExternalSourceAsset::process(const QByteArray &source, CreateInterface *c)
  {
  setSource(source);
  auto s = processSource(source, c);
  if (s)
    {
    s->setUuid(uuid());
    }
  return s;
  }

QByteArray ExternalSourceAsset::unprocess()
  {
  return _source;
  }

Asset *ExternalSourceAsset::defaultCreate(CreateInterface *c)
  {
  auto src = defaultSource();
  return process(src, c);
  }

QWidget *ExternalSourceAsset::createEditor(ProjectInterface *, CreateInterface *c)
  {
  class Editor : public TextEditor
    {
  public:
    Editor(const QString &s, ExternalSourceAsset *ass, CreateInterface *c)
        : TextEditor(s),
          _asset(ass),
          _ctx(c)
      {
      connect(
        this,
        &QTextEdit::textChanged,
        [this]()
          {
          _asset->reinitialise(toPlainText().toUtf8(), _ctx);
          }
        );
      }

  private:
    ExternalSourceAsset *_asset;
    CreateInterface *_ctx;
    };

  // ensure the asset is loaded.
  auto ass = asset(c);
  (void)ass;
  return new Editor(_source, this, c);
  }

void ExternalSourceAsset::setSource(const QByteArray &src)
  {
  _hasSource = true;
  _source = src;
  }

QByteArray ExternalSourceAsset::source() const
  {
  if (!_hasSource)
    {
    return getDiskSource();
    }

  return _source;
  }
}

}
