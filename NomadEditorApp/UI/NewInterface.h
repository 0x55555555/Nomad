#pragma once
#include <QDialog>
#include "Memory/XTemporaryAllocator.h"

namespace Shift
{
class PropertyInformation;
}

namespace Ui {
class NewInterface;
}

class NewInterface : public QDialog
  {
  Q_OBJECT

public:
  explicit NewInterface(QWidget *parent = 0);
  ~NewInterface();

  int show();

  const QString &location() const { return _location; }

  const Shift::PropertyInformation *type();

private:
  Ui::NewInterface *_ui;
  Eks::TemporaryAllocator _alloc;
  QString _location;
  };
