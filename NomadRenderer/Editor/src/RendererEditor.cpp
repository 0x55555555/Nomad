#include "RendererEditor.h"
#include "Renderable.h"
#include "shift/TypeInformation/smodule.h"
#include "shift/Properties/sattribute.inl"
#include "RenderableEditor.h"

S_IMPLEMENT_MODULE_WITH_INTERFACES(NomadRendererEditor)
  {
  module.addStaticInterface<Nomad::Renderable, Nomad::Editor::RenderableEditor>();
  }
