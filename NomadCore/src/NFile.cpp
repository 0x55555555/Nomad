#include "NFile.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sdata.inl"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(File, Nomad)

void File::createTypeInformation(
    Shift::PropertyInformationTyped<File> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

File::File()
  : XInitProperty(savedRevision, 0)
  {
  }

bool File::hasChangedFromFile() const
  {
  return savedRevision() != revision();
  }

void File::setSaved()
  {
  _savedRevision = revision();
  }

}
