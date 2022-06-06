#include "interceptorsettings.h"
#include <QTextEdit>
#include <interceptor.h>
#include <widget.h>
#include <QRegularExpression>
#include <QGridLayout>

InterceptorSettings::InterceptorSettings(QWidget* parent) : QWidget(parent){
  this->parent = (Widget*)parent;
  grid = new QGridLayout(this);
  setLayout(grid);
  textEdit = new QTextEdit(this);
  textEdit->setReadOnly(true);
  grid->addWidget(textEdit,0,0);  
}

void InterceptorSettings::RefreshInfo()
{
  auto wte = textEdit;
  Interceptor* interceptor = parent->interceptor;
  wte->clear();
  const auto Append = [&](const QString& txt){
    wte->append(txt);
  };
  if(parent->view == nullptr)
  {
    wte->setText("There is no view yet.");
    return;
  }

  //Global Block List
  Append("Global block list:\n ");
  for(const QString& target : interceptor->GetGlobalBlockList())
  {
    Append(QStringLiteral("[%0] ").arg(target));
  }
  Append("\n-------\n");
  //host map:
  Append("Host map:");
  auto it = interceptor->GetHostMap().begin();
  for(;it != interceptor->GetHostMap().end(); ++it)
  {
    const QString& key = it.key();
    const HostInfo& info = it.value();
    Append(QStringLiteral("Name: %0\nList:").arg(key));
    for(const QString& target : info.BlockList)
    {
      Append(QStringLiteral("[%0] ").arg(target));
    }
    Append("Regex List: ");
    for(const QRegularExpression& target : info.RegexBlockList)
    {
      Append(QStringLiteral("[%0] ").arg(target.pattern()));
    }
    Append("\n------\n");
  }
}

