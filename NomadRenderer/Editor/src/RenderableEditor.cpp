#include "RenderableEditor.h"
#include "Renderable.h"
#include "AssetSelector.h"
#include "ShaderType.h"
#include "ObjMeshType.h"
#include "QFormLayout"
#include "QWidget"

namespace Nomad
{

namespace Editor
{

QWidget *RenderableEditor::createEditor(Interface *i, AssetType::CreateInterface *c, ProjectInterface *ifc)
  {
  QWidget *w = new QWidget();
  auto l = new QFormLayout();
  l->setContentsMargins(0, 0, 0, 0);
  w->setLayout(l);

  Renderable *r = i->castTo<Renderable>();
  if (!r)
    {
    return nullptr;
    }

  auto meshEditor = new AssetSelector(ifc, c, &r->mesh, ObjMeshType::staticTypeInformation(), w);
  l->addRow("Mesh", meshEditor);

  auto shaderEditor = new AssetSelector(ifc, c, &r->shader, ShaderType::staticTypeInformation(), w);
  l->addRow("Shader", shaderEditor);

  return w;
  }

}

}
