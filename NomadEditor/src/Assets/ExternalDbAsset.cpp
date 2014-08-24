#include "Assets/ExternalDbAsset.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"
#include "Application.h"
#include "QTextEdit"

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
  auto asset = cachedAsset();
  if (auto parent = asset->parent()->castTo<Shift::Set>())
    {
    parent->remove(asset);
    }
  }

Asset *ExternalDbAsset::process(Shift::Set *s, const QByteArray &src)
  {
  auto asset = Application::loadSource(src, s);
  if(!asset)
    {
    return nullptr;
    }

  return asset->castTo<Asset>();
  }

QByteArray ExternalDbAsset::unprocess(Asset *)
  {
  return source();
  }

QWidget *ExternalDbAsset::createEditor()
  {
  class Editor : public QTextEdit
    {
  public:
    Editor(const QString &s, ExternalDbAsset *ass)
        : _asset(ass)
      {
      setPlainText(s);

      QFont font;
      font.setFamily("Courier");
      font.setStyleHint(QFont::Monospace);
      font.setFixedPitch(true);
      font.setPointSize(10);
      setFont(font);

      QFontMetrics metrics(font);
      setTabStopWidth(2 * metrics.width(' '));

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
    ExternalDbAsset *_asset;
    };

  return new Editor(source(), this);
  }

QByteArray ExternalDbAsset::source()
  {
  auto ass = cachedAsset();
  if(!ass)
    {
    return QByteArray();
    }

  return Application::toSource(ass);
  }
}

}
