#pragma once

class QWidget;

namespace Nomad
{
namespace Editor
{

class UIInterface
  {
public:
  virtual QWidget *createViewport(QWidget *parent) = 0;
  };

}
}
