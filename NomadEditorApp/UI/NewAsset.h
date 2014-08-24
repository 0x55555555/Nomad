#ifndef NEWASSET_H
#define NEWASSET_H

#include <QDialog>
#include "Memory/XTemporaryAllocator.h"

namespace Shift
{
class PropertyInformation;
}

namespace Ui {
class NewAsset;
}

class NewAsset : public QDialog
  {
  Q_OBJECT

public:
  explicit NewAsset(QWidget *parent = 0);
  ~NewAsset();

  int show(const QString &location);

  const QString &location() const { return _location; }

  const Shift::PropertyInformation *type();

private:
  Ui::NewAsset *_ui;
  Eks::TemporaryAllocator _alloc;
  QString _location;
  };

#endif // NEWASSET_H
