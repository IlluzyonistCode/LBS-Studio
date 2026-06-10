#include "SourcePanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>

SourcePanel::SourcePanel(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
}

void SourcePanel::setupUi()
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(6);

    auto* label = new QLabel("SOURCES", this);
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

void SourcePanel::setupConnections()
{
    connect(m_addBtn,    &QPushButton::clicked,
            this,        &SourcePanel::addSource);

    connect(m_removeBtn, &QPushButton::clicked,
            this,        &SourcePanel::removeSource);
}

void SourcePanel::onSceneChanged(const QString& sceneName)
{
    m_list->clear();

    if (!sceneName.isEmpty())
        m_list->addItem("Display Capture");
}

void SourcePanel::addSource()
{
    auto* dlg = new QDialog(this);
    dlg->setWindowTitle("Add Source");
    dlg->setFixedSize(300, 130);

    auto* combo = new QComboBox(dlg);
    combo->addItems({
        "Display Capture",
        "Window Capture",
        "Image",
        "Color Source",
        "Text"
    });

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dlg);

    auto* layout = new QVBoxLayout(dlg);
    layout->addWidget(new QLabel("Source type:", dlg));
    layout->addWidget(combo);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, dlg, &QDialog::reject);

    if (dlg->exec() == QDialog::Accepted)
        m_list->addItem(combo->currentText());
}

void SourcePanel::removeSource()
{
    const int row = m_list->currentRow();

    if (row < 0)
        return;

    delete m_list->takeItem(row);
}
