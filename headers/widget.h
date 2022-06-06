#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUrl>
#include <QShortcut>
#include "settings.h"
namespace Ui {
  class widget;
}
#include <QMainWindow>
#include "interceptorsettings.h"
#include "BrowserModule.h"
class Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Widget(QWidget* parent = nullptr);
  virtual ~Widget();
  class Header* header = nullptr;
  class QWebEngineView* view = nullptr;
  class QTabBar* tabBar = nullptr;
  class QTextEdit* console = nullptr;
  class Interceptor* interceptor = nullptr;
  InterceptorSettings icpSettings;
  QVector<class QWebEnginePage*> pages;
  Settings settings;
  const QUrl& GetUrl() const;
  void SetupSlotsAndSignals();
  void LoadUrl(const QString&);
  void SetInterceptor(class Interceptor*);


  void SetupShortcuts();

  template<class... Ts>
  void AddShortcut(const QKeySequence& seq, Ts&&... args)
  {
    QShortcut* shortcut = new QShortcut(seq, this);
    connect(shortcut, &QShortcut::activated, std::forward<Ts>(args)...);
  }

  template<class T>
  T* getLayoutAs() {
    return static_cast<T*>(layout());
  }
  QMenuBar* menuBar = nullptr;
#pragma region MODULES
  bool modulesLoaded = false;
  std::vector<class BrowserModule*> browserModules;
  template<class... Ts>
  void LoadModules()
  {
    if (modulesLoaded) return;
    modulesLoaded = true;
    static_assert((std::is_base_of_v<BrowserModule, Ts> && ...), "Each template argument must inherit from BrowserModule.");
    auto lambda = [&](auto* ptr){
      ptr->Init(this);
      browserModules.push_back(ptr);
    };
    (lambda(new Ts()), ...);
  }
#pragma endregion
private:
  Ui::widget* ui;
  class QWebEngineHistory* history;
  QUrl url;
private: //preparation functions
  void PrepareLayout();
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
public slots:
  void OnUrlChanged(const QUrl&);
  void SetTabNameFromHtmlTitle();
  void OpenSettings();
  void TabBarCurrentChanged(int index);


#pragma region events
private: 
  enum class MouseButton;
  MouseButton lastMouseButtonPressed;
public: 
  enum class MouseButton {
    None = 0, Left = 1, Middle = 2, Right = 3
  };
  MouseButton LastMouseButtonPressed() const { lastMouseButtonPressed; }
  virtual void mousePressEvent(class QMouseEvent* event);
#pragma endregion
};

#endif // WIDGET_H