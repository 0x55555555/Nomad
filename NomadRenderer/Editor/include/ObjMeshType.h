#pragma once
#include "RendererEditor.h"
#include "Assets/ExternalSourceAsset.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_RENDERER_EDITOR_EXPORT ObjMeshType : public ExternalSourceAsset
  {
  S_ENTITY(ObjMeshType, ExternalSourceAsset)

public:
  const char *extension() X_OVERRIDE;
  QByteArray defaultSource() const X_OVERRIDE;
  Asset *process(const QByteArray &source, CreateInterface *c) X_OVERRIDE;

  QWidget *createEditor(ProjectInterface *ifc, CreateInterface *c) X_OVERRIDE;
  QWidget *createPreview(UIInterface *ifc) X_OVERRIDE;

  Shift::ExternalUuidPointer _layout;
  };

}

}
