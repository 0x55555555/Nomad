#pragma once
#include "RendererEditor.h"
#include "Assets/ExternalSourceAsset.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_RENDERER_EDITOR_EXPORT ShaderComponentType : public ExternalSourceAsset
  {
  S_ENTITY(ShaderComponentType, ExternalSourceAsset)

public:
  const char *extension() const X_OVERRIDE;
  QByteArray defaultSource() const X_OVERRIDE;
  Asset *processSource(const QByteArray &source, CreateInterface *c) X_OVERRIDE;

  QWidget *createEditor(ProjectInterface *ifc, CreateInterface *c) X_OVERRIDE;

  Shift::EnumProperty type;
  };

}

}
