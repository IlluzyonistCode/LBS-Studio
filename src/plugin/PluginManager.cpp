#include "PluginManager.h"
#include "IPlugin.h"
#include "Logger.h"

#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <algorithm>

/* Factory function signature expected from each plugin library */
using PluginFactory = IPlugin* (*)();

PluginManager::PluginManager(QObject* parent)
    : QObject(parent)
{
}

PluginManager::~PluginManager()
{
    unloadAll();
}

void PluginManager::discover(const QString& pluginDir)
{
    const QDir dir(pluginDir);

    if (!dir.exists())
    {
        LOG_WARN("PluginManager",
                 "Plugin directory not found: " + pluginDir);
        return;
    }

    const QStringList filters = {
#ifdef Q_OS_WIN
        "*.dll"
#else
        "*.so"
#endif
    };

    const QFileInfoList entries = dir.entryInfoList(
        filters, QDir::Files);

    for (const QFileInfo& entry : entries)
        loadPlugin(entry.absoluteFilePath());
}

bool PluginManager::loadPlugin(const QString& filePath)
{
    QLibrary lib(filePath);

    if (!lib.load())
    {
        const QString reason = lib.errorString();

        LOG_WARN("PluginManager",
                 "Failed to load: " + filePath + " -- " + reason);

        emit pluginFailed(filePath, reason);
        return false;
    }

    auto* factory = reinterpret_cast<PluginFactory>(
        lib.resolve("createPlugin"));

    if (!factory)
    {
        const QString reason = "Missing createPlugin() export.";

        LOG_WARN("PluginManager",
                 filePath + ": " + reason);

        emit pluginFailed(filePath, reason);
        lib.unload();
        return false;
    }

    IPlugin* raw = factory();

    if (!raw)
    {
        LOG_WARN("PluginManager",
                 filePath + ": createPlugin() returned nullptr.");
        return false;
    }

    std::shared_ptr<IPlugin> plugin(raw);

    if (!plugin->initialize())
    {
        LOG_WARN("PluginManager",
                 "Plugin initialization failed: " + plugin->name());
        return false;
    }

    m_plugins.append(plugin);

    LOG_INFO("PluginManager",
             "Loaded plugin: " + plugin->name() +
             " [" + plugin->category() + "]");

    emit pluginLoaded(plugin->name());
    return true;
}

QVector<std::shared_ptr<IPlugin>> PluginManager::plugins() const
{
    return m_plugins;
}

std::shared_ptr<IPlugin> PluginManager::find(const QString& name) const
{
    auto it = std::find_if(m_plugins.begin(), m_plugins.end(),
        [&name](const auto& p) { return p->name() == name; });

    return (it != m_plugins.end()) ? *it : nullptr;
}

void PluginManager::unloadAll()
{
    for (auto& plugin : m_plugins)
        plugin->shutdown();

    m_plugins.clear();

    LOG_INFO("PluginManager", "All plugins unloaded.");
}

int PluginManager::count() const
{
    return m_plugins.size();
}
