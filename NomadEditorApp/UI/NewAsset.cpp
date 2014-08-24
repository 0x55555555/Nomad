#include "NewAsset.h"
#include "AssetBrowser.h"
#include "ui_NewAsset.h"
#include "shift/TypeInformation/spropertyinformation.h"
#include "Assets/AssetType.h"
#include "QFileDialog"

NewAsset::NewAsset(QWidget *parent) :
  QDialog(parent),
  _ui(new Ui::NewAsset),
  _alloc(Eks::Core::temporaryAllocator())
  {
  _ui->setupUi(this);

  xForeach(const Shift::PropertyInformation *t, Shift::TypeRegistry::types())
    {
    if (t->inheritsFromType(Nomad::Editor::AssetType::staticTypeInformation()) && !t->isAbstract())
      {
      _ui->type->addItem(t->typeName().data(), QVariant((qulonglong)t));
      }
    }
  }

NewAsset::~NewAsset()
  {
  delete _ui;
  }

int NewAsset::show(const QString &location)
  {
  _location = QFileDialog::getSaveFileName(this, "New asset location", location, ASSET_FILTER);
  if (_location.isEmpty())
    {
    return QDialog::Rejected;
    }

  int result = QDialog::exec();
  if (result != QDialog::Accepted)
    {
    return QDialog::Rejected;
    }

  if (type() == nullptr)
    {
    return QDialog::Rejected;
    }

  return QDialog::Accepted;
  }

const Shift::PropertyInformation *NewAsset::type()
  {
  return (const Shift::PropertyInformation *)_ui->type->itemData(_ui->type->currentIndex()).toULongLong();
  }
