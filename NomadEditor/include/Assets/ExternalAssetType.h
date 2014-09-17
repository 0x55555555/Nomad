#pragma once
#include "Assets/AssetType.h"


namespace Nomad
{

namespace Editor
{

class NOMAD_EXPORT ExternalAssetType : public AssetType
  {
  S_ABSTRACT_ENTITY(ExternalAssetType, AssetType)

public:
  ExternalAssetType();

  virtual void createNewAsset(CreateInterface *c) X_OVERRIDE;
  virtual Asset *initialiseFromSource(const QByteArray &source, CreateInterface *c);
  virtual Asset *asset(CreateInterface *c) X_OVERRIDE;
  virtual bool save() X_OVERRIDE;
  virtual bool needsSave() X_OVERRIDE;

  virtual void clear() = 0;
  virtual const char *extension() = 0;
  virtual Asset *process(const QByteArray &source, CreateInterface *c) = 0;
  virtual QByteArray unprocess(Asset *a) = 0;
  virtual Asset *defaultCreate(CreateInterface *c) = 0;

protected:
  Asset *cachedAsset();
  void setNeedsSave();

private:
  bool saveContents(const QString &assFile);
  QString contentsPath(const QString &assFile);

  Shift::TypedPointer<Asset> _asset;
  bool _needsSave;
  };

}

}
