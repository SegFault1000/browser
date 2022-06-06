#ifndef UTIL_H
#define UTIL_H

#include <type_traits>
//container manipulation
namespace util
{
  template<class IT, class T>
  T* find(IT begin, IT end, const T& val)
  {
    for(;begin != end; ++begin)
      if(*begin == val)
        return (T*)&*begin;

    return nullptr;
  }
  template<class IT, class Func>
  auto find_if(IT begin, IT end, Func func)
  {
    for(;begin != end; ++begin)
      if(func(*begin))
        return &*begin;
    return nullptr;
  }

  template<class IT, class T>
  bool contains(IT begin, IT end, const T& val)
  {
    for(;begin != end; ++begin)
      if(*begin == val)
        return true;

    return false;
  }
  template<class IT, class Func>
  bool contains_if(IT begin, IT end, Func func)
  {
    for(;begin != end; ++begin)
      if(func(*begin))
        return true;
    return false;
  }

  template<class T>
  T* binary_find(T* arr, int l, int r, const T& x)
  {
    if (r >= l) {
      int mid = l + (r - l) / 2;

      if (arr[mid] == x)
         return arr + mid;

      if (arr[mid] > x)
         return binarySearch(arr, l, mid - 1, x);

      return binarySearch(arr, mid + 1, r, x);
    }

    return nullptr;
  }

  template<class T, class Func>
  T* binary_find_if(T* arr, int l, int r, const T& x, Func func)
  {
    static_assert(std::is_invocable_v<Func, T>, "std::is_invocable_v<Func, T>");
    if (r >= l) {
        int mid = l + (r - l) / 2;

        if(func(arr[mid]))
            return arr + mid;

        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);

        return binarySearch(arr, mid + 1, r, x);
    }

    return nullptr;
  }


}

class QString;
#include <QMessageBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
namespace util
{
  void ShowMessageBox(const QString& str);
  void ShowMessageBox(int);
  void ShowMessageBox(double);
  template<class Func>
  QPushButton* CreateButton(QWidget* parent, Func func)
  {
    QPushButton* btn = new QPushButton(parent);
    QObject::connect(btn, &QPushButton::clicked, func);
    return btn;
  }
  template<class T>
  QPushButton* CreateButton(QWidget* parent, T* reciever, void(T::*func)())
  {
    static_assert(std::is_base_of_v<QWidget, T>, "Must derive from QWidget");
    QPushButton* btn = new QPushButton(parent);
    QObject::connect(btn, &QPushButton::clicked, reciever, func);
    return btn;
  }


  template<class T>
  constexpr inline bool is_number_v = std::is_arithmetic_v<T> || std::is_floating_point_v<T>;

 

  template<class... Ts>
  void CreateHBoxWidget(QWidget* source, Ts*... args)
  {
    QHBoxLayout* lay = new QHBoxLayout(source);
    (lay->addWidget(args), ...);
    source->setLayout(lay);
  }
  template<class... Ts>
  void CreateVBoxWidget(QWidget* source, Ts*... args)
  {
    QVBoxLayout* lay = new QVBoxLayout(source);
    (lay->addWidget(args), ...);
    source->setLayout(lay);
  }

  template<class... Ts>
  QAction* CreateAction(const QString& text, QWidget* parent, Ts&&... args)
  {
    QAction* act = new QAction(text, parent);
    QObject::connect(act, &QAction::triggered, std::forward<Ts>(args)...);
    return act;
  }
}


//QWidget helpers
class QWebEngineView;
class QWebEnginePage;
namespace util
{
  QWidget* QWidget_setSize(QWidget* w, int width, int height);
  QSize QWidget_getSize(QWidget* w);

  void SanitizeUrl(QString& str);
  void LoadUrl(QWebEngineView* view, const QString& str);
  void LoadUrl(QWebEnginePage* page, const QString& str);
  bool UrlIsValid(const QString& url);
}


#include <QWidget>
#include <QHBoxLayout>
class QHBoxWidget : public QWidget
{
public:
  QHBoxWidget(QWidget* parent) : QWidget(parent) {
    setLayout(new QHBoxLayout);
  }
  
  QHBoxWidget* addWidget(QWidget* w, int stretch = 0, Qt::Alignment alignment = { 0 })
  {
    QHBoxLayout* lay = (QHBoxLayout*)layout();
    lay->addWidget(w, stretch, alignment);        
    return this;
  }
};





#endif // UTIL_H
