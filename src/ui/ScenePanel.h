#pragma once

#include <QWidget>

class QListWidget;
class QPushButton;

/**
 * @class ScenePanel
 * @brief Scene list manager with add/remove/reorder controls.
 *
 * Emits sceneSelected() whenever the active scene changes,
 * allowing the SourcePanel to refresh its content accordingly.
 */
class ScenePanel : public QWidget
{
    Q_OBJECT

public:
    explicit ScenePanel(QWidget* parent = nullptr);

signals:
    void sceneSelected(const QString& name);

private slots:
    void addScene();
    void removeScene();

private:
    void setupUi();
    void setupConnections();

    QListWidget* m_list;
    QPushButton* m_addBtn;
    QPushButton* m_removeBtn;
};
