#include "NProject.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sdata.inl"

namespace Nomad
{
S_IMPLEMENT_PROPERTY(Project, Nomad)

void Project::createTypeInformation(
    Shift::PropertyInformationTyped<Project> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  childBlock.add(&Project::libraries, "libraries");
  childBlock.add(&Project::binFolder, "binFolder");
  }

}
