#pragma once
#include "NFile.h"
#include "shift/Properties/sarray.h"
#include "shift/Properties/sbaseproperties.h"
#include "shift/Changes/shandler.h"
#include "NGlobal.h"

namespace Nomad
{

class NOMAD_EXPORT Project : public File
  {
  S_ENTITY(Project, File)

public:

  Shift::Array libraries;
  Shift::StringProperty binFolder;
  };
}
