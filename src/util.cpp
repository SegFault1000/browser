#include "util.h"
#include <QMessageBox>
#include <QString>
#include <qwebengineview.h>
#include <QMenu>

void util::ShowMessageBox(const QString& str)
{
  static QMessageBox box;
  box.setText(str);
  box.exec();
}

void util::ShowMessageBox(int num)
{
  static QMessageBox box;
  box.setText(QString::number(num));
  box.exec();
}

void util::ShowMessageBox(double num)
{
  static QMessageBox box;
  box.setText(QString::number(num));
  box.exec();
}


void util::SanitizeUrl(QString& str)
{

}

void util::LoadUrl(QWebEngineView* view, const QString& str)
{
    QUrl url(str);
    if (UrlIsValid(str))
        view->load(url);
    else
        view->load(QStringLiteral("https://%0").arg(str));
}

void util::LoadUrl(QWebEnginePage* page, const QString& str)
{
    QUrl url(str);
    if (UrlIsValid(str))
        page->load(url);
    else
        page->load(QStringLiteral("https://%0").arg(str));    
}

bool util::UrlIsValid(const QString& url)
{
  return url.startsWith("http://") || url.startsWith("https://");
}

#include <QSize>
QWidget* util::QWidget_setSize(QWidget* w, int width, int height)
{
  auto geo = w->geometry();
  auto size = geo.size();
  size.setWidth(width);
  size.setHeight(height);
  geo.setSize(size);
  w->setGeometry(geo);
  return w;
}

QSize util::QWidget_getSize(QWidget* w)
{
  return w->geometry().size();
}

