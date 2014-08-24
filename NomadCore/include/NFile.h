#pragma once
#include "shift/sentity.h"
#include "shift/Properties/sarray.h"
#include "shift/Properties/sbaseproperties.h"
#include "shift/Changes/shandler.h"
#include "NGlobal.h"

namespace Nomad
{

class NOMAD_EXPORT File : public Shift::Entity, public Shift::Handler
  {
  S_ENTITY(File, Entity)

XProperties:
  XByRefProperty(Eks::String, path, setPath)
  XROProperty(xsize, savedRevision)

public:
  File();

  bool hasChangedFromFile() const;
  void setSaved();
  };

}
