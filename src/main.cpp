#include <QApplication>
#include <QWebEngineView>
#include <QWebEnginePage>
#include "widget.h"
#include <QDebug>
#include <QWebEngineUrlRequestInterceptor>
#include <QDebug>
#include <QProcess>
#include <interceptor.h>
#include <QStandardPaths>
#include "util.h"
constexpr const char* const INTERCEPTOR_FILE = "interceptor.json";
#include <windows.h>
#include "BookmarkModule.h"


int main(int argc, char** argv)
{
  QApplication a(argc, argv);
  Widget w;			
  w.LoadModules<BookmarkModule>();
  w.resize(800,600);
  w.SetInterceptor(new Interceptor(INTERCEPTOR_FILE, &w));
  w.show();
  return a.exec();
}

