#include "ProjectEditor.h"
#include "ProjectInterface.h"
#include "QFormLayout"
#include "QListWidget"
#include "QLabel"
#include "QToolButton"
#include "UI/spropertydefaultui.h"
#include "UI/sentityui.h"
#include "NProject.h"
#include "ProjectInterface.h"
#include "shift/Properties/scontaineriterators.h"

#ifdef Q_OS_DARWIN
# define LIBRARY_EXT "dylib"
#else
# error library not defined
#endif

#define LIBRARY_FILTER "*." LIBRARY_EXT

namespace Nomad
{

namespace Editor
{

ProjectEditor::ProjectEditor(ProjectInterface *ifc, QWidget *parent) :
  QDockWidget(parent),
  _project(ifc),
  _contents(nullptr)
  {
  setObjectName("ProjectEditor");
  setWindowTitle("Project Editor");
  _project->addProjectChanged(this, SLOT(setupProject()));
  }

ProjectEditor::~ProjectEditor()
  {
  }

void ProjectEditor::setupProject()
  {
  delete _contents;
  _contents = new QWidget(this);
  setWidget(_contents);

  auto currentProject = _project->getCurrentProject();
  if (!currentProject)
    {	
    return;
    }

  QVBoxLayout *l = new QVBoxLayout;
  l->setContentsMargins(4, 4, 4, 4);
  l->setSpacing(4);
  _contents->setLayout(l);

  QHBoxLayout *l3 = new QHBoxLayout;
  l->addLayout(l3);

  l3->addWidget(new QLabel("Binary Folder"));
  auto editor = new Shift::PropertyDefaultUI::Directory(&currentProject->binFolder, false, _contents);
  l3->addWidget(editor);

  l3->addStretch();
  auto add = new QToolButton();
  add->setText("+");
  auto remove = new QToolButton();
  remove->setText("-");
  l3->addWidget(add);
  l3->addWidget(remove);

  _libraries = new QListWidget;
  _libraries->setSelectionMode(QListWidget::ContiguousSelection);
  l->addWidget(_libraries);

  connect(add, SIGNAL(clicked()), this, SLOT(addNewLibrary()));
  connect(remove, SIGNAL(clicked()), this, SLOT(removeLibraries()));

  updateLibraries();
  currentProject->addTreeObserver(this);
  }

void ProjectEditor::addNewLibrary()
  {
  auto currentProject = _project->getCurrentProject();
  if (!currentProject)
    {
    return;
    }

  QString value = QFileDialog::getOpenFileName(
    this,
    tr("Select Library"),
    QString(),
    LIBRARY_FILTER);

  QDir s(currentProject->binFolder().data());
  Eks::String str = s.relativeFilePath(value);

  xForeach(auto w, currentProject->libraries.walker<Shift::StringProperty>())
    {
    if (w->value() == str)
      {
      return;
      }
    }
    
  Shift::Block b(currentProject->handler());
  auto newVal = currentProject->libraries.add<Shift::StringProperty>();
  newVal->assign(str);
  }

void ProjectEditor::removeLibraries()
  {
  auto currentProject = _project->getCurrentProject();
  if (!currentProject)
    {
    return;
    }

  Shift::Block b(currentProject->handler());

  xForeach(auto item, _libraries->selectedItems())
    {
    Eks::String str = item->text().toUtf8().data();

    xForeach(auto w, currentProject->libraries.walker<Shift::StringProperty>())
      {
      if (w->value() == str)
        {
        currentProject->libraries.remove(w);
        break;
        }
      }
    }
  }

void ProjectEditor::actOnChanges()
  {
  updateLibraries();
  }

void ProjectEditor::onTreeChange(const Shift::Change *, bool)
  {
  }

void ProjectEditor::updateLibraries()
  {
  _libraries->clear();

  auto currentProject = _project->getCurrentProject();
  if (!currentProject)
    {
    return;
    }

  xForeach(auto w, currentProject->libraries.walker<Shift::StringProperty>())
    {
    auto &str = w->value();
    _libraries->addItem(str.data());
    }
  }
}

}
