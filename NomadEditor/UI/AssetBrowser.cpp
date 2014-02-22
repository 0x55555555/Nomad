#include "AssetBrowser.h"
#include "ui_AssetBrowser.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "Application.h"
#include "shift/Properties/sdata.inl"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(AssetLocation, NomadEditor)

void AssetLocation::createTypeInformation(
    Shift::PropertyInformationTyped<AssetLocation> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  if (data.registerAttributes)
    {
    auto block = info->createChildrenBlock(data);

    block.add(&AssetLocation::path, "path");
    }
  }


S_IMPLEMENT_PROPERTY(AssetBrowserData, NomadEditor)

void AssetBrowserData::createTypeInformation(
    Shift::PropertyInformationTyped<AssetBrowserData> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  if (data.registerAttributes)
    {
    auto block = info->createChildrenBlock(data);
    }
  }

AssetBrowser::AssetBrowser(ProjectInterface *ifc, QWidget *parent) :
  QDockWidget(parent),
  _project(ifc),
  ui(new Ui::AssetBrowser)
  {
  ui->setupUi(this);

  _project->addProjectChanged(this, SLOT(setupProject()));

  auto scratch = _project->getScratchParent();
  _browser = scratch->add<AssetBrowserData>();
  }

AssetBrowser::~AssetBrowser()
  {
  delete ui;
  }

void AssetBrowser::setupProject()
  {
  auto currentProject = _project->getCurrentProject();
  if(!currentProject)
    {
    return;
    }

  auto location = _browser->addChild<AssetLocation>();

  location->path = currentProject->assets();

  rebuildUI();
  }

void AssetBrowser::rebuildUI()
  {
  }

}
}
