#pragma once
#include "ExternalAssetType.h"

namespace Nomad
{

namespace Editor
{

class NOMAD_EXPORT ExternalSourceAsset : public ExternalAssetType
  {
  S_ABSTRACT_ENTITY(ExternalSourceAsset, ExternalAssetType)

public:
  void clear() X_OVERRIDE;
  QByteArray unprocess(Asset *a) X_OVERRIDE;
  Asset *defaultCreate(CreateInterface *c) X_OVERRIDE;
  QWidget *createEditor(ProjectInterface *ifc, CreateInterface *c) X_OVERRIDE;

  virtual QByteArray defaultSource() const = 0;

protected:
  void setSource(const QByteArray &src);

  QByteArray _source;
  };

}

}
