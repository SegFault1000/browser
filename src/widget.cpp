#include "widget.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWebEngineView>
#include "header.h"
#include "interceptor.h"
#include "ui_widget.h"
#include "ui_header.h"
#include <QWebEngineHistory>
#include <QDockWidget>
#include <interceptorsettings.h>
#include <QNetworkReply>
#include "webengineview.h"
#include <qtabbar.h>

#include <QMenuBar>
#include <QMenu>
#include <QDir>
#include <QDesktopServices>
#include <QRegularExpression>

#include <MenuHelper.h>

#include <QMouseEvent>
using util::UrlIsValid;
static constexpr const char* DEFAULT_PAGE_URL = "https://google.com";
Widget::Widget(QWidget* parent)
{
  setParent(parent);
  PrepareLayout();
  SetupSlotsAndSignals();
  SetupShortcuts();
  LoadUrl("https://www.google.com");

}

Widget::~Widget()
{
  view->setPage(nullptr);
  for (auto page : pages)
    delete page;
  for (auto mod : browserModules)
    delete mod;
}

const QUrl& Widget::GetUrl() const
{
  return url;
}

void Widget::SetupSlotsAndSignals()
{
  connect(view, &QWebEngineView::urlChanged, this, &Widget::OnUrlChanged);
  connect(tabBar, &QTabBar::currentChanged, this, &Widget::TabBarCurrentChanged);
}

void Widget::LoadUrl(const QString& str)
{
  QUrl url(str);
  if (UrlIsValid(str))
  {
    view->load(url);
  }
  else
  {
    view->load(QStringLiteral("https://%0").arg(str));
  }
  SetTabNameFromHtmlTitle();
}

void Widget::SetInterceptor(Interceptor* arg)
{
  if (!view)
  {
    qDebug() << "Warning. Tried to set interceptor of null QWebEngineView.\n";
    return;
  }
  view->page()->setUrlRequestInterceptor(arg);
 
  auto webView = (WebEngineView*)view;
  
  webView->interceptor = arg;
  interceptor = arg;
}


void Widget::SetupShortcuts()
{
  AddShortcut(QKeySequence("Ctrl+L"), [&] {
    if (!view) return;
    auto page = view->page();
    page->setInspectedPage(page);
    });
  AddShortcut(QKeySequence("Ctrl+O"), &settings, &Settings::show);
  AddShortcut(QKeySequence("Ctrl+Tab"), [&]
  {
    if (tabBar->currentIndex() < tabBar->count() - 2)
      tabBar->setCurrentIndex(tabBar->currentIndex() + 1);          
  });
  AddShortcut(QKeySequence("Ctrl+Shift+Tab"), [&]
  {
    if (tabBar->currentIndex() > 0)
      tabBar->setCurrentIndex(tabBar->currentIndex() - 1);          
  });
  
}

void Widget::PrepareLayout()
{
  #pragma region MENU BAR STUFF
  menuBar = new QMenuBar(this);
  QMenu* menu = MenuHelper(this, "Menu" )
    .AddAction(QString("Settings"), &settings, &Settings::show)
    .AddAction(QString("Working directory"), []
    {
      QDesktopServices::openUrl(QDir::currentPath());
    })
    .AddAction("edit interceptor.json", []
    {
      auto path = QStringLiteral("%0/%1")
        .arg(QDir::currentPath())
        .arg("interceptor.json");
      QDesktopServices::openUrl(path);
    })
    .get();
    
      
     
  QAction* action = menu->addAction("interceptor settings");
  #pragma endregion

  this->icpSettings.parent = this;
  this->icpSettings.hide();
  QObject::connect(action, &QAction::triggered, [this] {
    icpSettings.RefreshInfo();
    icpSettings.show();
  });

  
  //....

  menuBar->addMenu(menu);

  header = new Header(this);
  header->resize(size().width(), header->size().height());

  view = new WebEngineView(this);
  history = view->history();  

  tabBar = new QTabBar(this);
  tabBar->addTab("tab 1");
  tabBar->addTab("+");

  
  pages.push_back(new QWebEnginePage());
  view->setPage(pages.back());  
  util::LoadUrl(pages.back(), DEFAULT_PAGE_URL);   
  SetTabNameFromHtmlTitle();

  
  setLayout(new QVBoxLayout);
  QVBoxLayout* vbox = static_cast<QVBoxLayout*>(layout());
	vbox->setSpacing(0);
  vbox->setMargin(0);
  vbox->addWidget(menuBar, 0);
  
  QHBoxWidget* tabBarHbox = new QHBoxWidget(this);
  tabBarHbox->addWidget(tabBar, 2, Qt::AlignmentFlag::AlignLeading);
  vbox->addWidget(tabBarHbox, 0);
  
  vbox->addWidget(header, 0);
  vbox->addWidget(view, 7);  

  setFocus();
}

void Widget::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
}
#include "qpoint_helper.h"
void Widget::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  /*
  QPainter painter(this);
  painter.setPen(Qt::blue);
  QPoint pos = view->pos();
  painter.drawLine(QPointF(0,0), QPointF_normalized(QPointF(1,1)) * view->height());
*/
}

#include <string_view>
void Widget::OnUrlChanged(const QUrl& newUrl)
{
  if (!newUrl.isValid())
    return;


  QPushButton* prev = header->ui->BTNPrevious;
  QPushButton* next = header->ui->BTNNext;
  auto history = view->page()->history();
  prev->setEnabled(history->canGoBack());
  next->setEnabled(history->canGoForward());

  QLineEdit* lineEdit = header->ui->LEUrl;
  QString newUrlStr = newUrl.toString();
  if (newUrlStr != lineEdit->text())
    lineEdit->setText(newUrl.toString());

  SetTabNameFromHtmlTitle();
    
}

void Widget::SetTabNameFromHtmlTitle()
{
  if (!tabBar || !view)
    return;
  QString title = view->page()->title();
  if (title.size() >= 23)
    title = title.left(23);
  else if (title.size() == 0)
    title = QStringLiteral("tab %0").arg(tabBar->currentIndex() + 1);
  tabBar->setTabText(tabBar->currentIndex(), title);
  return;  
}

void Widget::OpenSettings()
{
  settings.show();
}

void Widget::TabBarCurrentChanged(int index)
{  
  QWebEnginePage* currentPage = nullptr;  
  if (pages.size() < index + 1)
  {
    pages.resize(index + 1);
    currentPage = pages[index] = new QWebEnginePage();
    util::LoadUrl(currentPage, DEFAULT_PAGE_URL);
  }
  else if(pages[index] == nullptr)
  {
    currentPage = pages[index] = new QWebEnginePage();
    util::LoadUrl(currentPage, DEFAULT_PAGE_URL);    
  }
  else
  {
    currentPage = pages[index];
  }

  const int tabBarCount = tabBar->count();
  const bool isPlusTab = tabBarCount == index + 1;
  if (isPlusTab)
  {
    const QString newTabName = QStringLiteral("tab %0").arg(tabBarCount);
    tabBar->setTabText(index, newTabName);
    tabBar->addTab("+");           
  }  
  using WebAction = QWebEnginePage::WebAction;  
  
  view->setEnabled(false);
  view->setPage(pages[index]);    
  view->setEnabled(true); 
  
  //The setEnabled calls are to ensure the selected tab gains focus 
}


void Widget::mousePressEvent(QMouseEvent* event)
{
  Qt::MouseButton mb = event->button();
  if (mb == Qt::MouseButton::LeftButton)
    lastMouseButtonPressed == MouseButton::Left;
  else if (mb == Qt::MouseButton::RightButton)
    lastMouseButtonPressed == MouseButton::Right;
  else if (mb == Qt::MouseButton::MiddleButton)
    lastMouseButtonPressed == MouseButton::Middle;
  event->accept();
}
