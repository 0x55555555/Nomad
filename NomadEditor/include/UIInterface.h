#pragma once

namespace Eks
{
class AbstractCanvas;
}

class QWidget;

namespace Nomad
{
namespace Editor
{

class UIInterface
  {
public:
  virtual QWidget *createViewport(QWidget *parent, Eks::AbstractCanvas** can = nullptr) = 0;
  };

}
}
