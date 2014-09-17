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

void ExternalSourceAsset::clear()
  {
  auto asset = cachedAsset();
  if (!asset)
    {
    return;
    }

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

QByteArray ExternalSourceAsset::unprocess(Asset *)
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
          _asset->clear();
          _asset->initialiseFromSource(toPlainText().toUtf8(), _ctx);
          _asset->setNeedsSave();
          }
        );
      }

  private:
    ExternalSourceAsset *_asset;
    CreateInterface *_ctx;
    };

  // ensure the asset is loaded.
  auto ass = asset(c);
  xAssert(ass);
  (void)ass;

  return new Editor(_source, this, c);
  }

void ExternalSourceAsset::setSource(const QByteArray &src)
  {
  _source = src;
  }
}

}
