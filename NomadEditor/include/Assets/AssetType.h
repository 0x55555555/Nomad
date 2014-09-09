#pragma once
#include "shift/sentity.h"
#include "sexternalpointer.h"
#include "shift/Properties/sbasepointerproperties.h"
#include "NGlobal.h"
#include "NFile.h"

class QWidget;

namespace Eks
{
class Renderer;
}

namespace Shift
{
class Array;
}

namespace Nomad
{

class Asset;

namespace Editor
{

class UIInterface;
class ProjectInterface;
class AssetBrowser;

class NOMAD_EXPORT AssetType : public File
  {
  S_ABSTRACT_ENTITY(AssetType, File)

XProperties:
  XROProperty(ProjectInterface *, project)

public:
  class CreateInterface
    {
  public:
    virtual Eks::Renderer *renderer() = 0;
    };

  const QUuid &uuid() const { return _uuid(); }
  QString relativePath() const;

  void setPath(const Eks::String &s, ProjectInterface *ifc);

  virtual QWidget *createEditor(CreateInterface *c) = 0;
  virtual QWidget *createPreview(UIInterface *ctx);

  virtual void createNewAsset(const QString &fileLocation, CreateInterface *c) = 0;
  virtual Asset *asset(const QString &fileLocation, CreateInterface *c) = 0;
  virtual void save();

  static AssetType *create(
    const Shift::PropertyInformation *info,
    const QString &fileLocation,
    Shift::Set *handleParent,
    Shift::Set *assetParent,
    ProjectInterface *ifc,
    CreateInterface *c);
  static AssetType *load(
    const QString &location,
    Shift::Set *parent,
    Shift::Set *assetParent,
    ProjectInterface *ifc,
    CreateInterface *c);

  Shift::Set *assetParent() { return _assetParent(); }
  const Shift::Set *assetParent() const { return _assetParent(); }

protected:
  Shift::Data<QUuid> _uuid;
  Shift::TypedPointer<Shift::Set> _assetParent;

  friend class AssetBrowser;
  };

}

}
