#include "NObject.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NObjectInstance.h"

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

ObjectInstance *Object::addInstance(Object *obj)
  {
  return obj->instance(&children);
  }

ObjectInstance *Object::instance(Shift::Set *parent)
  {
  auto inst = parent->add<ObjectInstance>();

  inst->type.setPointed(this);

  xForeach(auto ifc, interfaces())
    {
    ifc->onInstance(inst);
    }

  return inst;
  }
}
