#include "NewInterface.h"
#include "ui_NewInterface.h"
#include "shift/TypeInformation/spropertyinformation.h"
#include "NObject.h"
#include "QFileDialog"

NewInterface::NewInterface(QWidget *parent) :
  QDialog(parent),
  _ui(new Ui::NewInterface),
  _alloc(Eks::Core::temporaryAllocator())
  {
  _ui->setupUi(this);

  xForeach(const Shift::PropertyInformation *t, Shift::TypeRegistry::types())
    {
    if (t->inheritsFromType(Nomad::Interface::staticTypeInformation()) && !t->isAbstract())
      {
      _ui->type->addItem(t->typeName().data(), QVariant((qulonglong)t));
      }
    }
  }

NewInterface::~NewInterface()
  {
  delete _ui;
  }

int NewInterface::show()
  {
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

const Shift::PropertyInformation *NewInterface::type()
  {
  return (const Shift::PropertyInformation *)_ui->type->itemData(_ui->type->currentIndex()).toULongLong();
  }
