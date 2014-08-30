#pragma once
#include "shift/sentity.h"
#include "sexternalpointer.h"
#include "NGlobal.h"
#include "NFile.h"

class QWidget;

namespace Shift
{
class Array;
}

namespace Nomad
{

class Asset;

namespace Editor
{

class ProjectInterface;
class AssetBrowser;

class NOMAD_EXPORT AssetType : public File
  {
  S_ABSTRACT_ENTITY(AssetType, File)

XProperties:
  XROProperty(ProjectInterface *, project)

public:
  const QUuid &uuid() const { return _uuid(); }
  QString relativePath() const;

  void setPath(const Eks::String &s, ProjectInterface *ifc);

  virtual QWidget *createEditor() = 0;
  virtual QWidget *createPreview();

  virtual void createNewAsset(Shift::Set *assetParent, const QString &fileLocation) = 0;
  virtual Asset *asset(Shift::Set *assetParent, const QString &fileLocation) = 0;
  virtual void save();

  static AssetType *create(const Shift::PropertyInformation *info,
    Shift::Set *handleParent,
    Shift::Set *assetParent,
    const QString &fileLocation,
    ProjectInterface *ifc);
  static AssetType *load(const QString &location, Shift::Set *parent);

protected:
  Shift::Data<QUuid> _uuid;

  friend class AssetBrowser;
  };

}

}
