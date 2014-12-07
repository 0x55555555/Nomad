#pragma once
#include "RendererEditor.h"
#include "UIInterface.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_RENDERER_EDITOR_EXPORT RenderableEditor : public UICreator
  {
  QWidget *createEditor(Interface *i, AssetType::CreateInterface *c, ProjectInterface *p) X_OVERRIDE;
  };

}

}
