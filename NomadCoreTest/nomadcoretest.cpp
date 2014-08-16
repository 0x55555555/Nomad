#include "nomadcoretest.h"
#include "Utilities/XProperty.h"
#include "shift/TypeInformation/smodule.h"
#include "shift/TypeInformation/spropertyinformationhelpers.h"
#include "shift/Serialisation/ssaver.h"
#include "shift/Serialisation/sjsonio.h"
#include "NAsset.h"
#include "NAssetManager.h"
#include "NAssetHelpers.h"
#include "NReloadableAsset.h"
#include <set>

class TestReloadableAsset : public Nomad::ReloadableAsset
  {
  S_ENTITY(TestReloadableAsset, ReloadableAsset)

public:
  };

class TestAsset : public Nomad::Asset
  {
  S_ENTITY(TestAsset, Asset)

public:
  };

class TestData : public Nomad::Asset
  {
  S_ENTITY(TestData, Asset)

public:
  Nomad::AssetPointer asset;
  Nomad::AssetPointer reloadableAsset;
  };


S_IMPLEMENT_PROPERTY(TestReloadableAsset, Test)

void TestReloadableAsset::createTypeInformation(
    Shift::PropertyInformationTyped<TestReloadableAsset> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

S_IMPLEMENT_PROPERTY(TestAsset, Test)

void TestAsset::createTypeInformation(
    Shift::PropertyInformationTyped<TestAsset> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);
  }

S_IMPLEMENT_PROPERTY(TestData, Test)

void TestData::createTypeInformation(
    Shift::PropertyInformationTyped<TestData> *info,
    const Shift::PropertyInformationCreateData &data)
  {
  auto childBlock = info->createChildrenBlock(data);

  Nomad::detail::addAssetPointer(childBlock, &TestData::asset, "asset");
  Nomad::detail::addAssetPointer(childBlock, &TestData::reloadableAsset, "reloadableAsset");
  }

template <typename T> void save(T *data, const char *name)
  {
  QFile file(name);
  file.open(QFile::WriteOnly);

  Shift::SaveBuilder builder;
  Shift::JSONSaver writer;
  writer.setAutoWhitespace(true);

  Eks::String fileStr;
    {
    auto block = writer.beginWriting(&fileStr);

    builder.save(data, true, &writer);
    }

  file.write(fileStr.data());
  }

void generateData(Shift::Entity *dataRoot, Nomad::AssetManager *manager)
  {
  auto reloadable = manager->createAsset<TestReloadableAsset>();
  auto asset = manager->createAsset<TestAsset>();

  auto data = dataRoot->addChild<TestData>();
  data->asset.setPointed(asset);
  data->reloadableAsset.setPointed(reloadable);

  save(data, "testData.json");

  save(reloadable, "reloadable.json");
  save(asset, "asset.json");
  }

class TestAssetInterface : public Nomad::AssetManager::Interface
  {
public:
  TestAssetInterface()
      : _loadCount(0)
    {
    }

  template <typename T> T *loadPrimaryAsset(const char *str)
    {
    auto ass = loadPrimaryAsset(str);
    xAssert(ass);
    return ass ? ass->castTo<T>() : nullptr;
    }

  Nomad::Asset *loadPrimaryAsset(const char *str)
    {
    xAssert(manager());
    return load(str, manager()->assetContainer());
    }

  Nomad::Asset *load(const char *name, Shift::Array *parent)
    {
    QFile toLoad(name);
    if(!toLoad.open(QFile::ReadOnly))
      {
      xAssertFail(name);
      return nullptr;
      }

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

    auto asset = loading->loadedData().front()->castTo<Nomad::Asset>();
    manager()->registerAsset(asset);

    ++_loadCount;
    return asset;
    }

  void triggerReload(const QUuid &uuid)
    {
    _toReload.insert(uuid);
    }

  xsize loadCount() const
    {
    return _loadCount;
    }

private:
  Nomad::Asset *load(const QUuid &name, Shift::Array *parent) X_OVERRIDE
    {
    _toReload.erase(name);

    QString path = name.toString();

    path = ":/resources/" + path + ".json";

    return load(path.toUtf8().data(), parent);
    }

  bool requiresReload(const QUuid &id) X_OVERRIDE
    {
    return _toReload.find(id) != _toReload.end();
    }

  std::set<QUuid> _toReload;
  xsize _loadCount;
  };

QUuid asId("{ccaa494e-a258-49f3-98b9-909310c52928}");
QUuid relId("{d2cd09cf-1e0b-4b1c-b991-fbcd8276b0f5}");


void NomadCoreTest::basicResource()
  {
  TestAssetInterface testInterface;

  TestDatabase db;
  auto manager = db.addChild<Nomad::AssetManager>();
  manager->reset(&testInterface);

#define GENERATE_DATAx
#ifdef GENERATE_DATA
  generateData(&db, manager);
#endif

  auto data = testInterface.loadPrimaryAsset<TestData>(":/data/testData.json");
  QVERIFY(data);

  QCOMPARE(testInterface.loadCount(), 1U);

  auto as = data->asset();
  auto rel = data->reloadableAsset();
  QVERIFY(as);
  QVERIFY(rel);

  QCOMPARE(testInterface.loadCount(), 3U);

  QCOMPARE(as->uuid(), asId);
  QCOMPARE(rel->uuid(), relId);

  QVERIFY(as->castTo<TestAsset>());
  QVERIFY(rel->castTo<TestReloadableAsset>());
  }

void NomadCoreTest::reloadResource()
  {
  TestAssetInterface testInterface;

  TestDatabase db;
  auto manager = db.addChild<Nomad::AssetManager>();
  manager->reset(&testInterface);

  auto data = testInterface.loadPrimaryAsset<TestData>(":/data/testData.json");

  QCOMPARE(testInterface.loadCount(), 1U);

  auto as = data->asset();
  auto rel = data->reloadableAsset();
  QVERIFY(as);
  QVERIFY(rel);

  QCOMPARE(testInterface.loadCount(), 3U);

  testInterface.triggerReload(asId);
  testInterface.triggerReload(relId);

  QCOMPARE(testInterface.loadCount(), 3U);

  manager->reloadRequiredAssets();

  QCOMPARE(testInterface.loadCount(), 4U);
  }

S_IMPLEMENT_TEST
QTEST_APPLESS_MAIN(NomadCoreTest)
