#pragma once
#include "RendererEditor.h"
#include "Assets/ExternalSourceAsset.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_RENDERER_EDITOR_EXPORT ShaderType : public ExternalSourceAsset
  {
  S_ENTITY(ShaderType, ExternalSourceAsset)

public:
  virtual const char *extension() const X_OVERRIDE;
  QByteArray defaultSource() const X_OVERRIDE;
  Asset *processSource(const QByteArray &source, CreateInterface *c) X_OVERRIDE;

  QWidget *createEditor(ProjectInterface *ifc, CreateInterface *c) X_OVERRIDE;
  QWidget *createPreview(UIInterface *ifc, CreateInterface *c) X_OVERRIDE;
private:
  Shift::ExternalUuidPointer _frag;
  Shift::ExternalUuidPointer _vert;
  };

}

}
