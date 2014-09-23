#pragma once
#include "sexternalpointer.h"
#include "NGlobal.h"

namespace Nomad
{
class AssetManager;

class NOMAD_EXPORT Asset : public Shift::UuidEntity
  {
  S_ENTITY(Asset, UuidEntity)

XProperties:
  XROProperty(AssetManager *, manager);

public:
  Asset();

  static const Property *resolve(
       const Shift::ExternalPointer *ptr,
       const Shift::ExternalPointerInstanceInformation *inst,
       Shift::ExternalPointerInstanceInformation::ResolveResult *res);

private:
  friend class AssetManager;
  };

class AssetPointer : public Shift::ExternalUuidPointer
  {
public:
  Asset *pointed()
    {
    auto ptd = resolve();
    if(!ptd)
      {
      return nullptr;
      }
    return ptd->uncheckedCastTo<Asset>();
    }
  const Asset *pointed() const
    {
    auto ptd = resolve();
    if(!ptd)
      {
      return nullptr;
      }
    return ptd->uncheckedCastTo<Asset>();
    }
  Asset *operator()() { return pointed(); }
  const Asset *operator()() const { return pointed(); }

  template <typename T>const T *pointed() const
    {
    const Asset *p = pointed();
    if(p)
      {
      return p->castTo<T>();
      }
    return 0;
    }

  template <typename T> T *pointed()
    {
    const Asset *p = pointed();
    if(p)
      {
      return p->castTo<T>();
      }
    return 0;
    }
  };

}
