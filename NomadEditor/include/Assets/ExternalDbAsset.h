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
  Asset *process(Shift::Set *parent, const QByteArray &source) X_OVERRIDE;
  QByteArray unprocess(Asset *a) X_OVERRIDE;
  QWidget *createEditor() X_OVERRIDE;

protected:
  QByteArray source();
  };

}

}
