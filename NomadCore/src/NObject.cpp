#include "NObject.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"

namespace Nomad
{

S_IMPLEMENT_ABSTRACT_PROPERTY(Interface, Nomad)

void Interface::createTypeInformation(
    Shift::PropertyInformationTyped<Interface> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

S_IMPLEMENT_PROPERTY(Object, Nomad)

void Object::createTypeInformation(
    Shift::PropertyInformationTyped<Object> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

Interface *Object::addInterface(const Shift::PropertyInformation *info)
  {
  auto i = addAttribute(info);
  return i->castTo<Interface>();
  }
}
