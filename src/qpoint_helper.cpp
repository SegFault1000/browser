#include "qpoint_helper.h"
#include <cmath>
const QPointF QPoint_right(1,0);
const QPointF QPoint_up(0,1);
float QPointF_magnitude(const QPointF& point)
{
  float result = sqrt(pow(point.x(),2) + pow(point.y(), 2));
  return result;
}

QPointF QPointF_normalized(const QPointF& point)
{
  float mag = QPointF_magnitude(point);
  QPointF result(point);
  if(result.x() != 0)
    result.setX(point.x() / mag);
  if(result.y() != 0)
    result.setY(point.y() / mag);
  return result;

}
