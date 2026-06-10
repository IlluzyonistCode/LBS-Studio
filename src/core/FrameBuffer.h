#pragma once

#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <queue>
#include <optional>
#include <cstddef>

/**
 * @class FrameBuffer
 * @brief Thread-safe bounded queue for inter-thread frame passing.
 *
 * Producers block when the buffer is full; consumers block when empty.
 * The capacity cap prevents unbounded memory growth under encoder lag.
 */
class FrameBuffer
{
public:
    explicit FrameBuffer(std::size_t capacity = 8);

    /** @brief Push a frame, blocking if at capacity. */
    void push(QImage frame);

    /** @brief Pop the oldest frame, blocking until one is available. */
    QImage pop();

    /** @brief Non-blocking pop. Returns nullopt if empty. */
    std::optional<QImage> tryPop();

    /** @brief Wake all blocked threads and mark buffer as closed. */
    void close();

    bool        isClosed() const;
    std::size_t size()     const;

private:
    std::size_t        m_capacity;
    bool               m_closed { false };
    std::queue<QImage> m_queue;
    mutable QMutex     m_mutex;
    QWaitCondition     m_notEmpty;
    QWaitCondition     m_notFull;
};
