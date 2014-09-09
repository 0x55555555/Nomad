#pragma once
#include "NomadEditor.h"
#include "QtWidgets/QWidget"
#include "UIInterface.h"

namespace Nomad
{
namespace Editor
{

class NOMAD_EDITOR_EXPORT PreviewViewport : public QWidget
  {
public:
  PreviewViewport(UIInterface *r);
  };

}

}
