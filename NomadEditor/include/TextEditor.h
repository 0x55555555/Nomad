#pragma once
#include "NomadEditor.h"
#include "QtWidgets/QTextEdit"


class NOMAD_EDITOR_EXPORT TextEditor : public QTextEdit
  {
public:
  TextEditor(const QString &s);
  };
