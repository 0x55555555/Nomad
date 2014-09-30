#pragma once
#include "QWidget"
#include "Utilities/XProperty.h"
#include "Assets/AssetType.h"
#include "ProjectInterface.h"

class QMainWindow;

namespace Nomad
{

namespace Editor
{

class MessageList;

class AssetEditor : public QWidget
  {
  Q_OBJECT

XProperties:
  XROProperty(AssetType *, asset);

public:
  static AssetEditor *build(
      AssetType *t,
      ProjectInterface *ifc,
      AssetType::CreateInterface *c,
      UIInterface *ui);

  void makeDockable(QMainWindow *mw);
  void showEditor();

protected slots:
  void updateMessages();
  void reloadableChanged();

protected:
  AssetEditor(AssetType *t, ProjectInterface *ifc, AssetType::CreateInterface *c, UIInterface *ui);

  QWidget *getTopLevel();

  MessageList *_messages;

  QAction *_reload;
  };

class DynamicAssetEditor : public QWidget, public AssetEditorInterface
  {
public:
  DynamicAssetEditor(
      const QUuid &t,
      ProjectInterface *ifc,
      AssetType::CreateInterface *c,
      UIInterface *ui,
      QWidget *w);

  void setAsset(const QUuid &t) X_OVERRIDE;

private:
  AssetEditor *_contents;
  ProjectInterface *_ifc;
  AssetType::CreateInterface *_cre;
  UIInterface *_ui;
  };

}

}
