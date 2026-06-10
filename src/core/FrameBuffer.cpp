#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(std::size_t capacity)
    : m_capacity(capacity)
{
}

void FrameBuffer::push(QImage frame)
{
    QMutexLocker lock(&m_mutex);

    while (m_queue.size() >= m_capacity && !m_closed)
        m_notFull.wait(&m_mutex);

    if (m_closed)
        return;

    m_queue.push(std::move(frame));
    m_notEmpty.wakeOne();
}

QImage FrameBuffer::pop()
{
    QMutexLocker lock(&m_mutex);

    while (m_queue.empty() && !m_closed)
        m_notEmpty.wait(&m_mutex);

    if (m_queue.empty())
        return {};

    QImage frame = std::move(m_queue.front());
    m_queue.pop();
    m_notFull.wakeOne();

    return frame;
}

std::optional<QImage> FrameBuffer::tryPop()
{
    QMutexLocker lock(&m_mutex);

    if (m_queue.empty())
        return std::nullopt;

    QImage frame = std::move(m_queue.front());
    m_queue.pop();
    m_notFull.wakeOne();

    return frame;
}

void FrameBuffer::close()
{
    QMutexLocker lock(&m_mutex);
    m_closed = true;
    m_notEmpty.wakeAll();
    m_notFull.wakeAll();
}

bool FrameBuffer::isClosed() const
{
    QMutexLocker lock(&m_mutex);
    return m_closed;
}

std::size_t FrameBuffer::size() const
{
    QMutexLocker lock(&m_mutex);
    return m_queue.size();
}
