#include "PreviewViewport.h"
#include "X3DCanvas.h"
#include "QVBoxLayout"

namespace Nomad
{

namespace Editor
{

PreviewViewport::PreviewViewport(UIInterface *r)
  {
  QVBoxLayout *l = new QVBoxLayout();
  l->setContentsMargins(2, 2, 2, 2);
  setLayout(l);

  auto widg = r->createViewport(this);
  l->addWidget(widg);
  }

}

}
