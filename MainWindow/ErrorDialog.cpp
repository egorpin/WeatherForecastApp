#include "ErrorDialog.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>

ErrorDialog::ErrorDialog(const QString& errorMessage, QWidget* parent)
    : QDialog(parent)
{
    setupUi(errorMessage);
}

void ErrorDialog::setupUi(const QString& errorMessage)
{
    setWindowTitle(tr("Ошибка"));
    //setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
    setModal(true);
    setFixedSize(200, 150);

    // Создание элементов интерфейса
    QLabel* iconLabel = new QLabel(this);
    iconLabel->setPixmap(QIcon(":/MainWindow/icons/error.png").pixmap(48, 48));

    QLabel* messageLabel = new QLabel(errorMessage, this);
    messageLabel->setWordWrap(true);
    messageLabel->setAlignment(Qt::AlignCenter);

    QPushButton* okButton = new QPushButton(tr("OK"), this);
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    // Разметка
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(iconLabel);
    topLayout->addStretch();

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(messageLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}
