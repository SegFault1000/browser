#include <MenuHelper.h>
MenuHelper::MenuHelper(QWidget* parent, const QString& title)
{
  menu = new QMenu(parent);
  menu->setTitle(title);
}
