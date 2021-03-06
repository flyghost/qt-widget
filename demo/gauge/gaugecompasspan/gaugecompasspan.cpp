#pragma execution_character_set("utf-8")

#include "gaugecompasspan.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "qmath.h"
#include "qtimer.h"
#include "qdebug.h"

GaugeCompassPan::GaugeCompassPan(QWidget *parent) : QWidget(parent)
{
    value = 0;
    precision = 0;

    bgColor = QColor(40, 45, 48);
    textColor = QColor(187, 191, 194);
    borderColor = QColor(34, 163, 169);
    borderWidth = 5;

    northDotColor = QColor(214, 77, 84);
    otherDotColor = QColor(37, 40, 49);
    pointerColor = QColor(214, 77, 84);
}

GaugeCompassPan::~GaugeCompassPan()
{

}

void GaugeCompassPan::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //绘制背景
    drawBg(&painter);
    //绘制四个方向点
    drawFourDots(&painter);
    //绘制四个方向文字
    drawFourText(&painter);
    //绘制指针
    drawPointer(&painter);
    //绘制当前值
    drawValue(&painter);
}

void GaugeCompassPan::drawBg(QPainter *painter)
{
    int radius = 85;
    painter->save();

    QRectF rect(-radius + borderWidth / 2, -radius + borderWidth / 2, radius * 2 - borderWidth, radius * 2 - borderWidth);
    //可以自行修改画笔的后三个参数,形成各种各样的效果,例如Qt::FlatCap改为Qt::RoundCap可以产生圆角效果
    QPen pen(borderColor, borderWidth, Qt::SolidLine, Qt::FlatCap, Qt::MPenJoinStyle);

    //绘制圆
    painter->setPen(pen);
    painter->setBrush(bgColor);
    painter->drawEllipse(rect);
    painter->restore();
}

void GaugeCompassPan::drawFourDots(QPainter *painter)
{
    int radius = 85;
    int offset = 5;
    painter->save();

    QRectF rect(-radius + borderWidth / 2, -radius + borderWidth / 2, radius * 2 - borderWidth, radius * 2 - borderWidth);

    //绘制正北点
    painter->setPen(Qt::NoPen);
    painter->setBrush(northDotColor);
    painter->drawEllipse(QPointF(0, -rect.height() / 2 - 10), offset, offset);

    //绘制其他方向点
    painter->setBrush(otherDotColor);
    painter->drawEllipse(QPointF(0, rect.height() / 2 + 10), offset, offset);
    painter->drawEllipse(QPointF(-rect.width() / 2 - 10, 0), offset, offset);
    painter->drawEllipse(QPointF(rect.width() / 2 + 10, 0), offset, offset);

    painter->restore();
}

void GaugeCompassPan::drawFourText(QPainter *painter)
{
    int radius = 85;
    int offset = 5;
    painter->save();

    QRectF rect(-radius + borderWidth / 2, -radius + borderWidth / 2, radius * 2 - borderWidth, radius * 2 - borderWidth);

    QFont font;
    font.setPixelSize(15);
    painter->setFont(font);
    painter->setPen(northDotColor);
    QFontMetrics fm = painter->fontMetrics();
    double textHeight = fm.height();

    QStringList texts;
    texts << "北" << "西" << "南" << "东";
    for (int i = 0; i < texts.count(); i++) {
        QString text = texts.at(i);
#if (QT_VERSION >= QT_VERSION_CHECK(5,11,0))
        double textWidth = fm.horizontalAdvance(text);
#else
        double textWidth = fm.width(text);
#endif
        if (i > 0) {
            painter->setPen(textColor);
            painter->rotate(-90);
        }

        QRectF titleRect(-textWidth / 2, -rect.height() / 2 + offset, textWidth, textHeight);
        painter->drawText(titleRect, Qt::AlignCenter, text);
    }

    painter->restore();
}

void GaugeCompassPan::drawPointer(QPainter *painter)
{
    int radius = 45;
    painter->save();

    QPolygon polygon;
    polygon.append(QPoint(radius, 0));
    polygon.append(QPoint(-radius, 0));
    polygon.append(QPoint(0, -radius - 10));
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPainterPath painterPath;
    painterPath.addPolygon(polygon);

    QPainterPath painterPath2;
    painterPath2.addEllipse(QPoint(0, 0), radius, radius);

    QPainterPath painterPath3;
    painterPath3.addEllipse(QPoint(0, 0), radius - 6, radius - 6);

    painter->rotate(value);
    painter->drawPath(painterPath2 - painterPath3);
    painter->fillPath(painterPath + painterPath2 - painterPath3, QBrush(pointerColor));
    painter->restore();
}

void GaugeCompassPan::drawValue(QPainter *painter)
{
    int radius = 100;
    painter->save();

    QFont font;
    font.setPixelSize(precision == 0 ? 26 : 23);
    painter->setFont(font);
    painter->setPen(textColor);

    //画数值
    QString text = QString("%1°").arg(QString::number(value, 'f', precision));
    QRectF rectValue(-radius, -radius, radius * 2, radius * 2);
    painter->drawText(rectValue, Qt::AlignCenter, text);

    //画方向
    QString strPosition;
    if (value >= 0 && value < 90) {
        if (value == 0) {
            strPosition = "正北";
        } else {
            strPosition = "东北";
        }
    } else if (value >= 90 && value < 180) {
        if (value == 90) {
            strPosition = "正东";
        } else {
            strPosition = "东南";
        }
    } else if (value >= 180 && value < 270) {
        if (value == 180) {
            strPosition = "正南";
        } else {
            strPosition = "西南";
        }
    } else if (value >= 270 && value <= 360) {
        if (value == 270) {
            strPosition = "正西";
        } else {
            strPosition = "西北";
        }
    }

    font.setPixelSize(10);
    painter->setFont(font);
    painter->setPen(Qt::gray);
    QRectF rectPosition(-radius, 12, radius * 2, radius * 2);
    painter->drawText(rectPosition, Qt::AlignHCenter | Qt::AlignTop, strPosition);
    painter->restore();
}

double GaugeCompassPan::getValue() const
{
    return this->value;
}

int GaugeCompassPan::getPrecision() const
{
    return this->precision;
}

QColor GaugeCompassPan::getBgColor() const
{
    return this->bgColor;
}

QColor GaugeCompassPan::getTextColor() const
{
    return this->textColor;
}

QColor GaugeCompassPan::getBorderColor() const
{
    return this->borderColor;
}

int GaugeCompassPan::getBorderWidth() const
{
    return this->borderWidth;
}

QColor GaugeCompassPan::getNorthDotColor() const
{
    return this->northDotColor;
}

QColor GaugeCompassPan::getOtherDotColor() const
{
    return this->otherDotColor;
}

QColor GaugeCompassPan::getPointerColor() const
{
    return this->pointerColor;
}

QSize GaugeCompassPan::sizeHint() const
{
    return QSize(200, 200);
}

QSize GaugeCompassPan::minimumSizeHint() const
{
    return QSize(50, 50);
}

void GaugeCompassPan::setValue(double value)
{
    if (this->value != value) {
        this->value = value;
        this->update();
    }
}

void GaugeCompassPan::setPrecision(int precision)
{
    //限定精确度不能太多,不然太长了不好显示
    if (this->precision != precision && precision >= 0 && precision <= 2) {
        this->precision = precision;
        this->update();
    }
}

void GaugeCompassPan::setBgColor(const QColor &bgColor)
{
    if (this->bgColor != bgColor) {
        this->bgColor = bgColor;
        this->update();
    }
}

void GaugeCompassPan::setTextColor(const QColor &textColor)
{
    if (this->textColor != textColor) {
        this->textColor = textColor;
        this->update();
    }
}

void GaugeCompassPan::setBorderColor(const QColor &borderColor)
{
    if (this->borderColor != borderColor) {
        this->borderColor = borderColor;
        this->update();
    }
}

void GaugeCompassPan::setBorderWidth(int borderWidth)
{
    if (this->borderWidth != borderWidth) {
        this->borderWidth = borderWidth;
        this->update();
    }
}

void GaugeCompassPan::setNorthDotColor(const QColor &northDotColor)
{
    if (this->northDotColor != northDotColor) {
        this->northDotColor = northDotColor;
        this->update();
    }
}

void GaugeCompassPan::setOtherDotColor(const QColor &otherDotColor)
{
    if (this->otherDotColor != otherDotColor) {
        this->otherDotColor = otherDotColor;
        this->update();
    }
}

void GaugeCompassPan::setPointerColor(const QColor &pointerColor)
{
    if (this->pointerColor != pointerColor) {
        this->pointerColor = pointerColor;
        this->update();
    }
}
