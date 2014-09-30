#pragma once
#include "Containers/XUnorderedMap.h"
class QObject;

namespace Shift
{
class Array;
class Set;
}

namespace Nomad
{
class Project;
class AssetManager;

namespace Editor
{

class ProjectUserData;
class AssetType;
class AssetEditor;

class AssetEditorInterface
  {
public:
  virtual void setAsset(const QUuid &t) = 0;
  };

class ProjectInterface
  {
public:
  virtual Nomad::Project *getCurrentProject() = 0;
  virtual Nomad::Editor::ProjectUserData *getCurrentProjectUserData() = 0;

  virtual AssetManager *getAssetManager() = 0;
  virtual Shift::Set *getAssetHandleParent() = 0;
  virtual AssetType *getAssetHandle(const QUuid &h) = 0;

  virtual const Eks::UnorderedMap<AssetType *, AssetEditor *>& openEditors() = 0;

  virtual Shift::Array *getScratchParent() = 0;

  virtual void addProjectChanged(QObject *obj, const char *slot) = 0;
  virtual void addProjectAboutToChange(QObject *obj, const char *slot) = 0;

  virtual void openAssetEditor(AssetType *a) = 0;
  virtual AssetEditorInterface *createAssetEditor(const QUuid &a, QWidget *w, QWidget **out) = 0;

  virtual void reloadLibraries() = 0;
  };

}
}
