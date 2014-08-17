#include "Assets/AssetType.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Properties/sarray.h"
#include "shift/Serialisation/sloader.h"
#include "shift/Serialisation/sjsonio.h"
#include "QFile"
#include "QDebug"

namespace Nomad
{

namespace Editor
{

S_IMPLEMENT_PROPERTY(AssetType, Nomad)

void AssetType::createTypeInformation(
    Shift::PropertyInformationTyped<AssetType> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  childBlock.add(&AssetType::_uuid, "uuid");
  }

void AssetType::save(const QString &location)
  {
  QFile file(location);
  file.open(QFile::WriteOnly);

  Shift::SaveBuilder builder;
  Shift::JSONSaver writer;
  writer.setAutoWhitespace(true);

  Eks::String fileStr;
    {
    auto block = writer.beginWriting(&fileStr);

    builder.save(this, true, &writer);
    }

  file.write(fileStr.data());
  }

AssetType *AssetType::create(
    const Shift::PropertyInformation *info,
    Shift::Array *handleParent,
    Shift::Array *assetParent,
    const QString &location)
  {
  AssetType *a = handleParent->add(info)->uncheckedCastTo<AssetType>();
  if (!a)
    {
    return nullptr;
    }

  a->initialise(assetParent, location);

  a->save(location);
  return a;
  }

AssetType *AssetType::load(Shift::Array *parent, const QString &location)
  {
  QFile toLoad(location);
  if(!toLoad.open(QFile::ReadOnly))
    {
    xAssertFail(location.toLocal8Bit().data());
    return nullptr;
    }

  qDebug() << "loading file" << location;

  Eks::String toLoadStr = toLoad.readAll().data();

  Shift::LoadBuilder builder;
  Eks::TemporaryAllocator alloc(parent->temporaryAllocator());
  auto loading = builder.beginLoading(parent, &alloc);

  Shift::JSONLoader loader;
  loader.load(&toLoadStr, &builder);

  xAssert(loading->loadedData().size() == 1);
  if (loading->loadedData().size() != 1)
    {
    return nullptr;
    }

  auto asset = loading->loadedData().front()->castTo<Nomad::Editor::AssetType>();
  return asset;
  }

}

}
