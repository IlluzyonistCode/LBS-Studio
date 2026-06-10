#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>

class IPlugin;

/**
 * @class PluginManager
 * @brief Discovers, loads, and manages IPlugin instances.
 *
 * Scans a designated plugin directory for shared libraries
 * implementing the IPlugin interface and exposes them to
 * the Pipeline via a clean registration API.
 *
 * All plugin lifecycle operations are guarded; a faulty plugin
 * cannot crash the host process.
 */
class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject* parent = nullptr);
    ~PluginManager() override;

    /** @brief Scan directory and load all valid plugin libraries. */
    void discover(const QString& pluginDir);

    /** @brief Retrieve all successfully loaded plugins. */
    QVector<std::shared_ptr<IPlugin>> plugins() const;

    /** @brief Find a plugin by name. Returns nullptr if not found. */
    std::shared_ptr<IPlugin> find(const QString& name) const;

    /** @brief Unload all plugins and release library handles. */
    void unloadAll();

    int count() const;

signals:
    void pluginLoaded(const QString& name);
    void pluginFailed(const QString& path, const QString& reason);

private:
    bool loadPlugin(const QString& filePath);

    QVector<std::shared_ptr<IPlugin>> m_plugins;
};
