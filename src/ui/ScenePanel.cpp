#include "ScenePanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>

ScenePanel::ScenePanel(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();

    m_list->addItem("Scene 1");
    m_list->setCurrentRow(0);
}

void ScenePanel::setupUi()
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(6);

    auto* label = new QLabel("SCENES", this);
    label->setObjectName("sectionLabel");

    m_list = new QListWidget(this);
    m_list->setDragDropMode(QAbstractItemView::InternalMove);

    m_addBtn    = new QPushButton("+", this);
    m_removeBtn = new QPushButton("-", this);
    m_addBtn->setFixedWidth(32);
    m_removeBtn->setFixedWidth(32);

    auto* btnRow = new QHBoxLayout;
    btnRow->addStretch();
    btnRow->addWidget(m_addBtn);
    btnRow->addWidget(m_removeBtn);

    root->addWidget(label);
    root->addWidget(m_list);
    root->addLayout(btnRow);
}

void ScenePanel::setupConnections()
{
    connect(m_addBtn,    &QPushButton::clicked,
            this,        &ScenePanel::addScene);

    connect(m_removeBtn, &QPushButton::clicked,
            this,        &ScenePanel::removeScene);

    connect(m_list, &QListWidget::currentTextChanged,
            this,   &ScenePanel::sceneSelected);
}

void ScenePanel::addScene()
{
    bool ok = false;

    const QString name = QInputDialog::getText(
        this, "New Scene", "Scene name:",
        QLineEdit::Normal, QString(), &ok);

    if (ok && !name.trimmed().isEmpty())
    {
        m_list->addItem(name.trimmed());
        m_list->setCurrentRow(m_list->count() - 1);
    }
}

void ScenePanel::removeScene()
{
    const int row = m_list->currentRow();

    if (row < 0)
        return;

    delete m_list->takeItem(row);
}
