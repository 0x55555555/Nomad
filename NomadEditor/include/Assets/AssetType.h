#pragma once
#include "shift/sentity.h"
#include "sexternalpointer.h"
#include "shift/Properties/sbasepointerproperties.h"
#include "NGlobal.h"
#include "NFile.h"
#include "Application.h"

class QWidget;

namespace Eks
{
class ParseError;
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

class NOMAD_EXPORT AssetType : public QObject, public File
  {
  Q_OBJECT
  S_ABSTRACT_ENTITY(AssetType, File)

XProperties:
  XROProperty(ProjectInterface *, project)
  XROProperty(bool, requiresReload)

public:
  AssetType();

  class CreateInterface
    {
  public:
    virtual Eks::Renderer *renderer() = 0;
    };

  const QUuid &assetUuid() const { return _assetUuid(); }
  QString relativePath() const;

  void setPath(const Eks::String &s, ProjectInterface *ifc);

  virtual QWidget *createEditor(ProjectInterface *ifc, CreateInterface *c) = 0;
  virtual QWidget *createPreview(UIInterface *ctx, CreateInterface *c);

  virtual void createNewAsset(CreateInterface *c) = 0;
  virtual void reload(CreateInterface *c) = 0;
  virtual Asset *asset(CreateInterface *c=nullptr) = 0;
  virtual bool save();
  virtual bool needsSave();
  virtual void clear();
  Application::FileResult offerToSave();

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

  void setRequiresReload(bool r);

  struct Message
    {
    enum Type
      {
      Error,
      Warning
      };

    Type type;
    Eks::DetailedCodeLocation location;
    Eks::String context;
    Eks::String message;
    };

  const Eks::Vector<Message> &messages() const { return _messages; }

signals:
  void requiresReloadChanged();
  void messagesChanged();

protected:
  void markDependantsForReload();

  void addError(const Eks::DetailedCodeLocation& loc, const Eks::String &s);
  void addError(const Eks::ParseError &s);
  void addWarning(const Eks::ParseError &s);

  Shift::Data<QUuid> _assetUuid;
  Shift::TypedPointer<Shift::Set> _assetParent;

private:
  Eks::Vector<Message> _messages;

  friend class AssetBrowser;
  };

}

}
