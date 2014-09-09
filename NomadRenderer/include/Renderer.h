#pragma once
#include "shift/sglobal.h"

#if defined(NOMADRENDERER_BUILD)
#  define NOMAD_RENDERER_EXPORT X_DECL_EXPORT
#else
#  define NOMAD_RENDERER_EXPORT X_DECL_IMPORT
#endif

S_MODULE(NOMAD_RENDERER_EXPORT, NomadRenderer)
