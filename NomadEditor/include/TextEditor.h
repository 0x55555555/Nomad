#pragma once
#include "NomadEditor.h"
#include "QtWidgets/QTextEdit"

namespace Nomad
{
namespace Editor
{

class NOMAD_EDITOR_EXPORT TextEditor : public QTextEdit
  {
public:
  TextEditor(const QString &s);
  };

}

}
