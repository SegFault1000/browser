#include "clickablelineedit.h"
#include <QMouseEvent>

ClickableLineEdit::ClickableLineEdit(QWidget* parent) : QLineEdit(parent)
{
  this->setReadOnly(true);
}

void ClickableLineEdit::mousePressEvent(QMouseEvent*)
{
  OnClick();
}
