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
  if (auto parent = asset->parent()->castTo<Shift::Set>())
    {
    parent->remove(asset);
    }
  }

QByteArray ExternalSourceAsset::unprocess(Asset *)
  {
  return _source;
  }

Asset *ExternalSourceAsset::defaultCreate(Shift::Set *parent)
  {
  return process(parent, defaultSource());
  }

QWidget *ExternalSourceAsset::createEditor()
  {
  class Editor : public TextEditor
    {
  public:
    Editor(const QString &s, ExternalSourceAsset *ass)
        : TextEditor(s),
          _asset(ass)
      {
      connect(
        this,
        &QTextEdit::textChanged,
        [this]()
          {
          auto asset = _asset->cachedAsset();
          if (!asset)
            {
            return;
            }

          if (auto parent = asset->parent()->castTo<Shift::Set>())
            {
            _asset->clear();
            _asset->initialiseFromSource(parent, toPlainText().toUtf8());
            }
          }
        );
      }

  private:
    ExternalSourceAsset *_asset;
    };

  return new Editor(_source, this);
  }

void ExternalSourceAsset::setSource(const QByteArray &src)
  {
  _source = src;
  }
}

}
