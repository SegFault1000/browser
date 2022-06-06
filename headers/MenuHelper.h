#pragma once
#include <QMenu>
class QString;
class QWidget;
class MenuHelper
{
  QMenu* menu = nullptr;
public:
  MenuHelper(QWidget* parent, const QString& title);
  template<class T, class Slot>
  MenuHelper& AddAction(const QString& actionName, T* receiver, Slot slot)
  {
    QAction* action = menu->addAction(actionName);
    QObject::connect(action, &QAction::triggered, receiver, slot);
    return *this;
  }
  template<class Callback>
  MenuHelper& AddAction(const QString& actionName, Callback callback)
  {
    QAction* action = menu->addAction(actionName);
    QObject::connect(action, &QAction::triggered, callback);
    return *this;
  }

  QMenu* get() const { return menu; }
};

template<class Callback>
void MenuHelper_AddAction(QMenu* menu, const QString& actionName, Callback callback)
{
  QAction* action = menu->addAction(actionName);
  QObject::connect(action, &QAction::triggered, callback);  
}
template<class T, class Slot>
void MenuHelper_AddAction(QMenu* menu, const QString & actionName, T * receiver, Slot slot)
{
  QAction* action = menu->addAction(actionName);
  QObject::connect(action, &QAction::triggered, receiver, slot);  
}
