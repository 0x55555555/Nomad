#include "NProject.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sbaseproperties.inl"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(File, Nomad)

void File::createTypeInformation(
    Shift::PropertyInformationTyped<File> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto childBlock = info->createChildrenBlock(data);
    }
  }

S_IMPLEMENT_PROPERTY(Project, Nomad)

void Project::createTypeInformation(
    Shift::PropertyInformationTyped<Project> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto childBlock = info->createChildrenBlock(data);

    childBlock.add(&Project::libraries, "libraries");
    childBlock.add(&Project::assets, "assets");
    }
  }

}
