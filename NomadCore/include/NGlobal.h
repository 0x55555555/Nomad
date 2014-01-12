#include "XGlobal.h"
#include "shift/sglobal.h"

#if defined(NOMADCORE_BUILD)
#  define NOMAD_EXPORT X_DECL_EXPORT
#else
#  define NOMAD_EXPORT X_DECL_IMPORT
#endif

namespace Shift
{
class PropertyGroup;
class Module;
}

S_MODULE(NOMAD_EXPORT, Nomad)
