#pragma once
#include "Containers/XUnorderedMap.h"
class QObject;

namespace Shift
{
class Array;
}

namespace Nomad
{
class Project;

namespace Editor
{

class ProjectUserData;
class AssetType;
class AssetEditor;

class ProjectInterface
  {
public:
  virtual Nomad::Project *getCurrentProject() = 0;
  virtual Nomad::Editor::ProjectUserData *getCurrentProjectUserData() = 0;

  virtual const Eks::UnorderedMap<AssetType *, AssetEditor *>& openEditors() = 0;

  virtual Shift::Array *getScratchParent() = 0;

  virtual void addProjectChanged(QObject *obj, const char *slot) = 0;
  virtual void addProjectAboutToChange(QObject *obj, const char *slot) = 0;

  virtual void openAssetEditor(AssetType *a) = 0;
  };

}
}
