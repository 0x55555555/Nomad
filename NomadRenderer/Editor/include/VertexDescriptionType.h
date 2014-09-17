#pragma once
#include "RendererEditor.h"
#include "Assets/ExternalSourceAsset.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_RENDERER_EDITOR_EXPORT VertexDescriptionType : public ExternalSourceAsset
  {
  S_ENTITY(VertexDescriptionType, ExternalSourceAsset)

public:
  const char *extension() X_OVERRIDE;
  QByteArray defaultSource() const X_OVERRIDE;
  Asset *processSource(const QByteArray &source, CreateInterface *c) X_OVERRIDE;
  };

}

}
