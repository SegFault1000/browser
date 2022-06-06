#include "settings.h"
#include "util.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include "clickablelineedit.h"
#include "interceptor.h"
#include <QStandardPaths>
#define DEBUGGING_APP
Settings::Settings(QWidget* parent) : QWidget(parent){
  PrepareLayout();
  resize(600, 400);
}
static constexpr const char* const ICP_LABEL_NONE = "No interceptor";
static constexpr const char* const ICP_LABEL_FOUND = "Interceptor located";
void Settings::PrepareLayout()
{
  QVBoxLayout* lay = new QVBoxLayout(this);
  setLayout(lay);
  lay->setMargin(0);
  lay->setSpacing(0);

  QLabel* label = new QLabel("Interceptor path:", this);
  QFont labelFont;
  labelFont.setBold(true);
  label->setFont(labelFont);
  lay->addWidget(label,0);

  QWidget* w = new QWidget(this);
  icpPath = QDir::currentPath() + "/interceptor.json";
  LEIcpPath = new ClickableLineEdit(this);
  LEIcpPath->OnClick = [&]{this->ChooseIcpPath();};
  icpLabel = new QLabel(ICP_LABEL_NONE, this);
  util::CreateHBoxWidget(w,LEIcpPath, icpLabel);
  lay->addWidget(w,0);
  lay->insertStretch(-1,1);

#ifdef DEBUGGING_APP
  {
    QString documentsDir = QStandardPaths::locate(
      QStandardPaths::DocumentsLocation, "",
      QStandardPaths::LocateOption::LocateDirectory);
    SetIcpPath(QStringLiteral("%0interceptor.json").arg(documentsDir));
  }
#else
  SetIcpPath(icpPath);
#endif
}

void Settings::ChooseIcpPath()
{
  QString name = QFileDialog::getOpenFileName(this, "choose interceptor", "", "*.json");
  if(name.isEmpty()) return;
  SetIcpPath(name);
}

void Settings::SetIcpPath(const QString& path)
{
  bool exists = path.endsWith("interceptor.json") && QFile::exists(path);
  bool valid = !path.isEmpty() && exists;
  icpPath = path;
  if(LEIcpPath)
  {
    LEIcpPath->setText(path);
    QString style = valid ? "border: 1px solid green;" : "border: 1px solid red;";
    LEIcpPath->setStyleSheet(style);
    if(icpLabel)
      icpLabel->setText(valid ? ICP_LABEL_FOUND : ICP_LABEL_NONE);
  }

  if(valid && interceptor)
  {
    interceptor->SetDataFromFile(path);
  }
}
