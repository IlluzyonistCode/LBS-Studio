#include "PreviewWidget.h"

#include <QPainter>
#include <QPaintEvent>

PreviewWidget::PreviewWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(320, 180);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void PreviewWidget::setFrame(const QImage& frame)
{
    {
        QMutexLocker lock(&m_frameMutex);
        m_frame = frame;
    }

    recalcDrawRect();
    update();
}

void PreviewWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(rect(), QColor(0x0D, 0x0E, 0x10));

    QMutexLocker lock(&m_frameMutex);

    if (!m_frame.isNull())
    {
        painter.drawImage(m_drawRect, m_frame);
    }
    else
    {
        painter.setPen(QColor(0x30, 0x31, 0x35));
        painter.setFont(QFont("Segoe UI", 14));
        painter.drawText(rect(), Qt::AlignCenter, "No Signal");
    }
}

void PreviewWidget::resizeEvent(QResizeEvent*)
{
    recalcDrawRect();
}

void PreviewWidget::recalcDrawRect()
{
    QMutexLocker lock(&m_frameMutex);

    if (m_frame.isNull())
        return;

    const QSize scaled = m_frame.size().scaled(size(), Qt::KeepAspectRatio);

    const int x = (width()  - scaled.width())  / 2;
    const int y = (height() - scaled.height()) / 2;

    m_drawRect = QRect(QPoint(x, y), scaled);
}
