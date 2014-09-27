#pragma once
#include "ExternalAssetType.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_EXPORT ExternalDbAsset : public ExternalAssetType
  {
  S_ABSTRACT_ENTITY(ExternalDbAsset, ExternalAssetType)

public:
  void clear() X_OVERRIDE;
  Asset *process(const QByteArray &source, CreateInterface *c) X_OVERRIDE;
  QByteArray unprocess() X_OVERRIDE;
  QWidget *createEditor(ProjectInterface *ifc, CreateInterface *c) X_OVERRIDE;

protected:
  QByteArray source() const;
  };

}

}
