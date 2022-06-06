#ifndef QHBOXWIDGET_H
#define QHBOXWIDGET_H

#include <QObject>
#include <QHBoxLayout>
class QHBoxWidget
{
public:
  QHBoxWidget(QWidget* parent = nullptr);
  template<class... Ts>
  static QHBoxWidget Create(Ts*... args)
  {
    QHBoxWidget w;
    QHBoxLayout* lay = new QHBoxLayout();
    (lay->addWidget(std::forward<Ts>(args)), ...);
    return w;
  }
};

#endif // QHBOXWIDGET_H
