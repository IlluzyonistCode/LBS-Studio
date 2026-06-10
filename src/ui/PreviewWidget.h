#pragma once

#include <QWidget>
#include <QImage>
#include <QMutex>
#include <QRect>

/**
 * @class PreviewWidget
 * @brief Live preview canvas with aspect-ratio-correct rendering.
 *
 * Accepts frames via setFrame() from any thread and schedules
 * a repaint on the UI thread. Uses QPainter with smooth scaling.
 */
class PreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget* parent = nullptr);

public slots:
    void setFrame(const QImage& frame);

protected:
    void paintEvent(QPaintEvent* event)   override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void recalcDrawRect();

    QImage m_frame;
    QMutex m_frameMutex;
    QRect  m_drawRect;
};
