#ifndef CLICKABLELINEEDIT_H
#define CLICKABLELINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <functional>
class ClickableLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  std::function<void()> OnClick = []{};
  ClickableLineEdit(QWidget* parent = nullptr);
  void mousePressEvent(QMouseEvent *) override;
};

#endif // CLICKABLELINEEDIT_H
