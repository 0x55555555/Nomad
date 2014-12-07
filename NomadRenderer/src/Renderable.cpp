#include "Renderable.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "Renderer.h"
#include "NAssetManager.h"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(Renderable, NomadRenderer)

void Renderable::createTypeInformation(
    Shift::PropertyInformationTyped<Renderable> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  auto mesh = childBlock.add(&Renderable::mesh, "mesh");
  mesh->setResolveFunction(AssetManager::resolveAssetPointer);

  auto shader = childBlock.add(&Renderable::shader, "shader");
  shader->setResolveFunction(AssetManager::resolveAssetPointer);
  }

Renderable::Renderable()
  {
  }
}
