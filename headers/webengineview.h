#ifndef WEBENGINEVIEW_H
#define WEBENGINEVIEW_H

#include <QWebEngineView>
class WebEngineView : public QWebEngineView
{
  Q_OBJECT
private:
  void load(const QUrl&){}
  Qt::MouseButton lastMouseButtonPressed = Qt::MouseButton::LeftButton;
public:
  using Base = QWebEngineView;
  class Interceptor* interceptor = nullptr;
  WebEngineView(QWidget* parent = nullptr);
  void contextMenuEvent(QContextMenuEvent *) override;
  void LoadUrl(const QString&);
  void mousePressEvent(class QMouseEvent* ev) override;
  Qt::MouseButton LastMouseButtonPressed() const { return lastMouseButtonPressed;  }
};

#endif // WEBENGINEVIEW_H
