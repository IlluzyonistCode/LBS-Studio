#pragma once
#include "ICaptureSource.h"
#include <QObject>
#include <QMutex>
#include <memory>
#include <atomic>
class QThread;
/**
 * @class DesktopCapture
 * @brief DXGI Desktop Duplication capture source.
 *
 * Runs a dedicated worker thread that pulls frames from the
 * DXGI output duplicator and converts them to QImage for
 * downstream pipeline consumption.
 */
class DesktopCapture final : public QObject, public ICaptureSource
{
    Q_OBJECT
public:
    explicit DesktopCapture(int adapterIndex = 0, int outputIndex = 0,
                            QObject* parent = nullptr);
    ~DesktopCapture() override;
    QString name() const override;
    void setFrameCallback(FrameCallback callback) override;
    bool start() override;
    void stop()  override;
    bool isRunning() const override;
private:
    int m_adapterIndex;
    int m_outputIndex;
    FrameCallback            m_callback;
    QMutex                   m_callbackMutex;
    std::atomic<bool>        m_running { false };
    std::unique_ptr<QThread> m_thread;
};