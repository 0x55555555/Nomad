#pragma once
#include "shift/sentity.h"
#include "shift/Properties/sarray.h"
#include "shift/Properties/sbaseproperties.h"
#include "NGlobal.h"

namespace Nomad
{

class NOMAD_EXPORT File : public Shift::Entity
  {
  S_ENTITY(File, Entity)

XProperties:
  XProperty(Eks::String, path, setPath)
  };

class NOMAD_EXPORT Project : public File
  {
  S_ENTITY(Project, File)

public:
  Shift::Array libraries;

  Shift::StringProperty assets;
  };

}
