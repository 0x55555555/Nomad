#pragma once
#include "shift/TypeInformation/sinterface.h"
#include "Assets/AssetType.h"

S_DEFINE_INTERFACE_TYPE(UICreatorInterface, 5001)

namespace Eks
{
class AbstractCanvas;
}

class QWidget;

namespace Nomad
{

class Interface;

namespace Editor
{

class ProjectInterface;

class UIInterface
  {
public:
  virtual QWidget *createViewport(QWidget *parent, Eks::AbstractCanvas** can = nullptr) = 0;
  };

class UICreator : public Shift::StaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(UICreator, UICreatorInterface)

public:
  virtual QWidget *createEditor(Interface *i, AssetType::CreateInterface *c, ProjectInterface *p) = 0;
  };

}
}
