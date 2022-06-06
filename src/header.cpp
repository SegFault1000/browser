#include "header.h"
#include "ui_header.h"
#include "widget.h"
#include "util.h"
#include <QWebEngineView>
#include <QWebEngineHistory>
Header::Header(Widget *parent) :
  QWidget(parent),
  ui(new Ui::Header)
{
  ui->setupUi(this);  
  mainWidget = parent;
  setLayout(ui->gridLayout);

  mainWidget->AddShortcut(QKeySequence("Alt+Left"), this, &Header::on_BTNPrevious_clicked);
  mainWidget->AddShortcut(QKeySequence("Alt+Right"), this, &Header::on_BTNNext_clicked);
}

Header::~Header()
{
  delete ui;
}

void Header::on_BTNPrevious_clicked()
{  
  auto view = (QWebEngineView*)mainWidget->view;
  if(view->history()->canGoBack())
    view->back();
}

void Header::on_BTNNext_clicked()
{
  auto view = (QWebEngineView*)mainWidget->view;
  if (view->history()->canGoForward())
    view->forward();
}

void Header::on_LEUrl_returnPressed()
{
    mainWidget->LoadUrl(ui->LEUrl->text());
}

void Header::on_BTNSubmit_clicked()
{
    mainWidget->LoadUrl(ui->LEUrl->text());
}

void Header::on_BTNRefresh_clicked()
{
  auto view = (QWebEngineView*)mainWidget->view;
  view->reload();
}
