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
  virtual void createNewAsset(Shift::Set *assetParent, const QString &fileLocation) X_OVERRIDE;
  virtual Asset *initialiseFromSource(Shift::Set *assetParent, const QByteArray &source);
  virtual Asset *asset(Shift::Set *assetParent, const QString &fileLocation) X_OVERRIDE;
  virtual void save() X_OVERRIDE;

  virtual void clear() = 0;
  virtual const char *extension() = 0;
  virtual Asset *process(Shift::Set *parent, const QByteArray &source) = 0;
  virtual QByteArray unprocess(Asset *a) = 0;
  virtual Asset *defaultCreate(Shift::Set *parent) = 0;

protected:
  Asset *cachedAsset();

private:
  void saveContents(const QString &assFile);
  QString contentsPath(const QString &assFile);

  Shift::TypedPointer<Asset> _asset;
  };

}

}
