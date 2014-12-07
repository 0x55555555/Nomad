#pragma once
#include "Assets/ExternalDbAsset.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_EXPORT ObjectType : public ExternalDbAsset
  {
  S_ENTITY(ObjectType, ExternalDbAsset)

public:
  const char *extension() const X_OVERRIDE;
  Asset *defaultCreate(CreateInterface *c) X_OVERRIDE;

  QWidget *createEditor(ProjectInterface *ifc, CreateInterface *c) X_OVERRIDE;
  QWidget *createPreview(UIInterface *ctx, CreateInterface *c) X_OVERRIDE;

private:
  void updateWidget(QTabWidget *w, ProjectInterface *p, CreateInterface *c);
  };

}

}
