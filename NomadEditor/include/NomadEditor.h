#pragma once
#include "XGlobal.h"

#if defined(NOMADEDITOR_BUILD)
#  define NOMAD_EDITOR_EXPORT X_DECL_EXPORT
#else
#  define NOMAD_EDITOR_EXPORT X_DECL_IMPORT
#endif
