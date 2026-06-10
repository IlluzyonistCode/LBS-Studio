#pragma once

#include <QWidget>

class QListWidget;
class QPushButton;

/**
 * @class SourcePanel
 * @brief Source list for the currently active scene.
 *
 * Refreshes its content whenever the active scene changes.
 * Sources can be reordered via drag-and-drop.
 */
class SourcePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SourcePanel(QWidget* parent = nullptr);

public slots:
    void onSceneChanged(const QString& sceneName);

private slots:
    void addSource();
    void removeSource();

private:
    void setupUi();
    void setupConnections();

    QListWidget* m_list;
    QPushButton* m_addBtn;
    QPushButton* m_removeBtn;
};
