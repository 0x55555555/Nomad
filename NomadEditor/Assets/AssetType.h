#pragma once
#include "shift/sentity.h"
#include "shift/QtExtensions/sexternalpointer.h"
#include "NGlobal.h"

namespace Shift
{
class Array;
}

namespace Nomad
{

class Asset;

namespace Editor
{

class NOMAD_EXPORT AssetType : public Shift::Entity
  {
  S_ABSTRACT_ENTITY(AssetType, Entity)

public:
  const QUuid &uuid() const { return _uuid(); }

  virtual void initialise(Shift::Array *assetParent, const QString &fileLocation) = 0;
  virtual Asset *asset(Shift::Array *assetParent, const QString &fileLocation) = 0;
  virtual void save(const QString &location);

  static AssetType *create(const Shift::PropertyInformation *info,
    Shift::Array *handleParent,
    Shift::Array *assetParent,
    const QString &fileLocation);
  static AssetType *load(Shift::Array *parent, const QString &location);

protected:
  Shift::Data<QUuid> _uuid;
  };

}

}
