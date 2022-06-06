#ifndef HEADER_H
#define HEADER_H

#include <QWidget>
#include <QVector>
namespace Ui {
class Header;
}

class Widget;
class Header : public QWidget
{
  Q_OBJECT

public:
  explicit Header(Widget* parent);
  ~Header();
  Ui::Header* ui;
private:
  Widget* mainWidget = nullptr;
public:
private slots:
  void on_BTNPrevious_clicked();
  void on_BTNNext_clicked();
  friend class Widget;
  void on_LEUrl_returnPressed();
  void on_BTNSubmit_clicked();
  void on_BTNRefresh_clicked();
};

#endif // HEADER_H
