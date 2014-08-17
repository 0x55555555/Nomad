#pragma once
#include "Assets/AssetType.h"
#include "shift/Properties/sbasepointerproperties.h"


namespace Nomad
{

namespace Editor
{

class NOMAD_EXPORT ExternalAssetType : public AssetType
  {
  S_ABSTRACT_ENTITY(ExternalAssetType, AssetType)

public:
  virtual void initialise(Shift::Array *assetParent, const QString &fileLocation) X_OVERRIDE;
  virtual Asset *asset(Shift::Array *assetParent, const QString &fileLocation) X_OVERRIDE;
  virtual void save(const QString &location);

  virtual const char *extension() = 0;
  virtual Asset *process(Shift::Array *parent, const QString &source) = 0;
  virtual QString unprocess(Asset *a) = 0;
  virtual Asset *initialise(Shift::Array *parent) = 0;

private:
  void saveContents(const QString &assFile);
  QString contentsPath(const QString &assFile);

  Shift::TypedPointer<Asset> _asset;
  };

}

}
