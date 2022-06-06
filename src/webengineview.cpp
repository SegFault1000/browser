#include "webengineview.h"
#include <QContextMenuEvent>
#include <QMenu>
#include "util.h"
#include <QWebEngineContextMenuData>
#include <QClipboard>
#include <QApplication>
#include <unordered_map>
#include <QVector>
#include "interceptor.h"
using MediaType = QWebEngineContextMenuData::MediaType;
std::unordered_map<MediaType, QVector<QAction*>> contextActionMap;
const QWebEngineContextMenuData* conDataPtr = nullptr;

WebEngineView::WebEngineView(QWidget* parent) : Base(parent)
{
  using util::CreateAction;
  contextActionMap[MediaType::MediaTypeImage] = QVector<QAction*>{
    CreateAction("Copy image url", this, [&]{
         QClipboard* clip = QApplication::clipboard();
         clip->setText(conDataPtr->mediaUrl().toString());
    }),
    CreateAction("Block image", this, [&]{

    })
  };
}


void WebEngineView::contextMenuEvent(QContextMenuEvent* ev)
{


  QMenu menu(this);
  conDataPtr = &page()->contextMenuData();
  auto it = contextActionMap.find(conDataPtr->mediaType());
  if(it != contextActionMap.end())
  {
    for(QAction*& ptr : it->second)
    {
      menu.addAction(ptr);
    }
  }
  menu.addAction(new QAction("nothing", &menu));

  menu.exec(ev->globalPos());

}
void WebEngineView::LoadUrl(const QString& str)
{
  using util::UrlIsValid;
  QUrl url(str);
  if (UrlIsValid(str))
    load(url);
  else
    load(QStringLiteral("https://%0").arg(str));
}

void WebEngineView::mousePressEvent(QMouseEvent* ev)
{
  lastMouseButtonPressed = ev->button();
}
