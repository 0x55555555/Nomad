#pragma once
#include "NomadEditor.h"
#include "QtWidgets/QWidget"
#include "QtWidgets/QListWidget"
#include "Utilities/XCodeLocation.h"
#include "Containers/XStringSimple.h"


namespace Nomad
{
namespace Editor
{

class NOMAD_EDITOR_EXPORT MessageList : public QWidget
  {
public:
  enum Type
    {
    Error,
    Warning
    };

  MessageList(QWidget *parent);

  void clear();
  void add(Type, const Eks::DetailedCodeLocation &l, const Eks::String &context, const Eks::String &msg);

private:
  QListWidget *_list;
  };

}

}
