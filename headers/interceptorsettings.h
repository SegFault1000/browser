#ifndef INTERCEPTORSETTINGS_H
#define INTERCEPTORSETTINGS_H
#include <QWidget>
class Widget;
class InterceptorSettings : public QWidget
{
public:
  InterceptorSettings(QWidget* parent = nullptr);
  class QTextEdit* textEdit = nullptr;
  class QGridLayout* grid = nullptr;
  Widget* parent = nullptr;

  void RefreshInfo();  
};

#endif // INTERCEPTORSETTINGS_H
