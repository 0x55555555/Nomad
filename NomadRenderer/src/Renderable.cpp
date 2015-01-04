#include "Renderable.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "Renderer.h"
#include "NAssetManager.h"
#include "NObjectInstance.h"
#include "shift/Properties/sdata.inl"

namespace Nomad
{

S_IMPLEMENT_PROPERTY(RenderableInstance, NomadRenderer)

void RenderableInstance::createTypeInformation(
    Shift::PropertyInformationTyped<RenderableInstance> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  childBlock.add(&RenderableInstance::transform, "transform");
  }

void RenderableInstance::render(Eks::Renderer *r, const RenderState &state) const
  {
  auto inst = parent()->uncheckedCastTo<ObjectInstance>();
  auto object = inst->type.pointed<Object>();

  auto rend = object->firstChild<Renderable>();
  xAssert(rend);

  auto mesh = rend->mesh.pointed<Mesh>();
  auto shader = rend->shader.pointed<Shader>();
  if (!mesh || !shader)
    {
    return;
    }

  r->setTransform(state.transform * transform());

  r->setShader(&shader->shader(), &shader->vertexComponent()->layout());
  r->drawTriangles(&mesh->geometry());
  }

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

void Renderable::onInstance(ObjectInstance *obj)
  {
  obj->addAttribute<RenderableInstance>();
  }
}
