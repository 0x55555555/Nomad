#include "UI/ProjectUserData.h"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ProjectUserData, Nomad)

void ProjectUserData::createTypeInformation(
    Shift::PropertyInformationTyped<ProjectUserData> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  childBlock.add(&ProjectUserData::layout, "layout");
  childBlock.add(&ProjectUserData::openFiles, "openFiles");
  }

}

}
