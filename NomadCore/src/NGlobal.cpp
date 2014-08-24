#include "NGlobal.h"
#include "shift/TypeInformation/smodule.h"
#include "shift/Properties/sattribute.inl"
#include "NProject.h"

S_IMPLEMENT_MODULE_WITH_INTERFACES(Nomad)
  {
  using namespace Shift;
  module.addInheritedInterface<Nomad::File, Handler>();
  }
