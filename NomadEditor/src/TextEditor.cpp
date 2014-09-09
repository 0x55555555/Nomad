#include "TextEditor.h"

namespace Nomad
{

namespace Editor
{

TextEditor::TextEditor(const QString &s)
  {
  setPlainText(s);

  QFont font;
  font.setFamily("Courier");
  font.setStyleHint(QFont::Monospace);
  font.setFixedPitch(true);
  font.setPointSize(10);
  setFont(font);

  QFontMetrics metrics(font);
  setTabStopWidth(2 * metrics.width(' '));
  }

}

}
