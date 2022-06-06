#ifndef QPOINT_HELPER_H
#define QPOINT_HELPER_H
#include <QPoint>
extern const QPointF QPoint_right;
extern const QPointF QPoint_up;
float QPointF_magnitude(const QPointF& point);
QPointF QPointF_normalized(const QPointF& point);
#endif // QPOINT_HELPER_H
