#pragma once

#include "NGlobal.h"
#include "sexternalpointer.h"

namespace Nomad
{

class NOMAD_EXPORT ObjectInstance : public Shift::UuidEntity
  {
  S_ENTITY(ObjectInstance, Shift::UuidEntity)

public:
  Shift::ExternalUuidPointer type;
  };

}
