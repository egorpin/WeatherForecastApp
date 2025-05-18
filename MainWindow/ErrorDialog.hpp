#pragma once

#include <QDialog>

class ErrorDialog : public QDialog
{
    Q_OBJECT

public:
    ErrorDialog(const QString& errorMessage, QWidget* parent = nullptr);
    ~ErrorDialog() = default;

private:
    void setupUi(const QString& errorMessage);
};
