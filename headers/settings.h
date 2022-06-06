#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QLineEdit>
class Settings : public QWidget
{
  Q_OBJECT
public:
  Settings(QWidget* parent = nullptr);
  void PrepareLayout();
  class QGridLayout* grid = nullptr;
  class ClickableLineEdit* LEIcpPath = nullptr;
  class QLabel* icpLabel = nullptr;
  QString icpPath = "";

private:
  void ChooseIcpPath();
  void SetIcpPath(const QString& path);
  class Interceptor* interceptor = nullptr;
};

#endif // SETTINGS_H
