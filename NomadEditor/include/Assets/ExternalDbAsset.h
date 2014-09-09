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
  QByteArray unprocess(Asset *a) X_OVERRIDE;
  QWidget *createEditor(CreateInterface *c) X_OVERRIDE;

protected:
  QByteArray source();
  };

}

}
