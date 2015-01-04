#pragma once
#include "NGlobal.h"
#include "NAsset.h"
#include "shift/Properties/scontaineriterators.h"

namespace Nomad
{

class ObjectInstance;

class NOMAD_EXPORT Interface : public Shift::Container
  {
  S_ABSTRACT_PROPERTY(Interface, Container)

public:
  virtual void onInstance(ObjectInstance *) { }
  };

class NOMAD_EXPORT Object : public Asset
  {
  S_ENTITY(Object, Asset)

public:
  Shift::ContainerTypedIteratorWrapperFrom<const Interface, const Container, Shift::ContainerIterator<const Interface, const Container> > interfaces() const
    {
    return walker<Interface>();
    }

  Shift::ContainerTypedIteratorWrapperFrom<Interface, Container, Shift::ContainerIterator<Interface, Container> > interfaces()
    {
    return walker<Interface>();
    }

  Interface *addInterface(const Shift::PropertyInformation *info);

  /// Add a new object instance as a child of this object. To be duplicated on any instances of this object.
  ObjectInstance *addInstance(Object *obj);

  /// Instance this object under a new object
  ObjectInstance *instance(Shift::Set *parent);
  };

}
