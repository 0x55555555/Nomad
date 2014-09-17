#include "ObjMeshType.h"
#include "Mesh.h"
#include "VertexDescription.h"
#include "VertexDescriptionType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "NAsset.h"
#include "NAssetManager.h"
#include "XObjLoader.h"
#include "PreviewViewport.h"
#include "AssetSelector.h"
#include "QVBoxLayout"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(ObjMeshType, NomadRendererEditor)

void ObjMeshType::createTypeInformation(
    Shift::PropertyInformationTyped<ObjMeshType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  auto layout = childBlock.add(&ObjMeshType::_layout, "layout");
  layout->setResolveFunction(AssetManager::resolveAssetPointer);
  }

const char *ObjMeshType::extension()
  {
  return "obj";
  }

QByteArray ObjMeshType::defaultSource() const
  {
  return
"g cube\n"
"\n"
"v  0.0  0.0  0.0\n"
"v  0.0  0.0  1.0\n"
"v  0.0  1.0  0.0\n"
"v  0.0  1.0  1.0\n"
"v  1.0  0.0  0.0\n"
"v  1.0  0.0  1.0\n"
"v  1.0  1.0  0.0\n"
"v  1.0  1.0  1.0\n"
"\n"
"vn  0.0  0.0  1.0\n"
"vn  0.0  0.0 -1.0\n"
"vn  0.0  1.0  0.0\n"
"vn  0.0 -1.0  0.0\n"
"vn  1.0  0.0  0.0\n"
"vn -1.0  0.0  0.0\n"
"\n"
"f  1//2  7//2  5//2\n"
"f  1//2  3//2  7//2\n"
"f  1//6  4//6  3//6\n"
"f  1//6  2//6  4//6\n"
"f  3//3  8//3  7//3\n"
"f  3//3  4//3  8//3\n"
"f  5//5  7//5  8//5\n"
"f  5//5  8//5  6//5\n"
"f  1//4  5//4  6//4\n"
"f  1//4  6//4  2//4\n"
"f  2//1  6//1  8//1\n"
"f  2//1  8//1  4//1\n";
  }

Asset *ObjMeshType::processSource(const QByteArray &source, CreateInterface *c)
  {
  auto mesh = assetParent()->add<Mesh>();

  auto layout = _layout.pointed<VertexDescription>();
  if(!layout)
    {
    qWarning() << "invalid vertex layout";
    return mesh;
    }

  Eks::TemporaryAllocator alloc(Attribute::temporaryAllocator());

  auto itemCount = layout->layout().size();

  Eks::Vector<Eks::VectorI3D> triangles(&alloc);
  xsize vertexSize;
  Eks::Vector<Eks::ObjLoader::ElementData> elements(&alloc);
  elements.resize(itemCount);

  Eks::Vector<Eks::ShaderVertexLayoutDescription::Semantic> semantics(&alloc);
  layout->bakeSemantics(&semantics);

  Eks::ObjLoader loader(&alloc);
  loader.load(
    source.data(),
    source.size(),
    semantics.data(),
    itemCount,
    &triangles,
    &vertexSize,
    elements.data());

  //loader.computeUnusedElements(elements.data(), itemCount, &triangles);

  Eks::Vector<xuint8> dataOut(&alloc);
  bool result = loader.bake(triangles, elements.data(), itemCount, &dataOut);
  if (!result)
    {
    qWarning() << "invalid obj";
    return mesh;
    }

  xsize vertCount = dataOut.size() / vertexSize;

  if(!Eks::Geometry::delayedCreate(mesh->geometry(), c->renderer(), dataOut.data(), dataOut.size(), vertCount))
    {
    return mesh;
    }

  return mesh;
  }

QWidget *ObjMeshType::createEditor(ProjectInterface *ifc, CreateInterface *c)
  {
  QWidget *w = new QWidget();
  QVBoxLayout *l = new QVBoxLayout();
  l->setContentsMargins(2, 2, 2, 2);
  w->setLayout(l);

  auto box = new AssetSelector(ifc, c, &_layout, VertexDescriptionType::staticTypeInformation(), w);
  QObject::connect(box, &AssetSelector::assetChanged, [this, c]()
    {
    clear();
    initialiseFromSource(source(), c);
    setNeedsSave();
    });
  l->addWidget(box);

  auto editor = ExternalSourceAsset::createEditor(ifc, c);
  editor->setParent(w);
  l->addWidget(editor);

  return w;
  }

QWidget *ObjMeshType::createPreview(UIInterface *ifc)
  {
  struct MeshViewport : public PreviewViewport
    {
    MeshViewport(UIInterface *ifc, ObjMeshType *)
        : PreviewViewport(ifc)
      {
      }
    };

  return new MeshViewport(ifc, this);
  }

}

}
