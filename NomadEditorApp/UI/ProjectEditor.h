#pragma once
#include <QDockWidget>
#include "Utilities/XProperty.h"
#include "shift/Changes/sobserver.h"
#include "shift/Properties/sattribute.inl"

class QListWidget;

namespace Nomad
{
namespace Editor
{

class ProjectInterface;

class ProjectEditor : public QDockWidget, Shift::TreeObserver
  {
  Q_OBJECT

XProperties:
  XROProperty(ProjectInterface *, project);

public:
  explicit ProjectEditor(ProjectInterface *ifc, QWidget *parent = 0);
  ~ProjectEditor();

private slots:
  void setupProject();
  void addNewLibrary();
  void removeLibraries();

private:
  void actOnChanges() X_OVERRIDE;
  void onTreeChange(const Shift::Change *, bool backwards) X_OVERRIDE;
  void updateLibraries();

  QWidget *_contents;
  QListWidget *_libraries;
  };

}
}

