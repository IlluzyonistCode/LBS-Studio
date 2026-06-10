#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include <atomic>
#include "IPlugin.h"

class ICaptureSource;
class IEncoder;
class FrameBuffer;
class QThread;

/**
 * @class Pipeline
 * @brief Orchestrates capture -> filter -> encode data flow.
 *
 * Owns the capture source, plugin chain, encoder, and frame buffer.
 * Runs the encode loop in a dedicated thread separate from the
 * capture thread to maximize throughput.
 */
class Pipeline : public QObject
{
    Q_OBJECT

public:
    explicit Pipeline(QObject* parent = nullptr);
    ~Pipeline() override;

    void setCaptureSource(std::unique_ptr<ICaptureSource> source);
    void setEncoder(std::unique_ptr<IEncoder> encoder);

    void addPlugin(std::shared_ptr<IPlugin> plugin);
    void removePlugin(const QString& name);

    bool start();
    void stop();

    bool isActive() const;

signals:
    void frameReady(QImage frame);
    void error(QString message);

private:
    void   onFrame(const QImage& raw);
    QImage applyPlugins(const QImage& frame);
    void   encodeLoop();

    std::unique_ptr<ICaptureSource> m_source;
    std::unique_ptr<IEncoder>       m_encoder;
    std::unique_ptr<FrameBuffer>    m_buffer;
    std::unique_ptr<QThread>        m_encodeThread;

    std::vector<std::shared_ptr<IPlugin>> m_plugins;

    std::atomic<bool> m_active { false };
};
