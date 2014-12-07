#pragma once
#include "NGlobal.h"
#include "NAsset.h"
#include "shift/Properties/scontaineriterators.h"

namespace Nomad
{

class NOMAD_EXPORT Interface : public Shift::Container
  {
  S_ABSTRACT_PROPERTY(Interface, Container)
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
  };

}
