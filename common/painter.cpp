#include "painter.h"
#include <QPainter>

namespace Start {

void Painter::paintBackground(QWidget *widget)
{
    if(!widget)
        return;

    QPainter p(widget);

    p.save();

    p.setPen(Qt::NoPen);

    QColor colorTop(183,28,44);
    QColor colorMiddle(42,5,16);
    QColor colorBottom(77,12,28);

    int y_offset = 0;

    p.setBrush(QBrush(colorTop));
    p.drawRect(widget->rect().x(), widget->rect().y() + y_offset, widget->rect().width(),60);

    y_offset += 60;
    p.setBrush(QBrush(colorMiddle));
    p.drawRect(widget->rect().x(), widget->rect().y() + y_offset, widget->rect().width(), 500);

    y_offset += 500;
    p.setBrush(QBrush(colorBottom));
    p.drawRect(widget->rect().x(), widget->rect().y() + y_offset, widget->rect().width(), widget->rect().height() - y_offset);

    p.restore();
}

} // namespace Symlex
