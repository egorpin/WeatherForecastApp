#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QJsonObject> 

class WeatherView : public QWidget {
public:
    explicit WeatherView(QWidget *parent = nullptr);
    
    std::string getSelectedCity() const;
    void displayWeather(const QJsonObject &data);
    void showErrorMessage(const QString &message);
    void showLoadingIndicator();
    
signals:
    void citySearchRequested(const QString& city);

private:
    void setupUI();
  //  void setupConnections();
    

    QLineEdit *cityInput;
    QPushButton *searchButton;
    QLabel *weatherIconLabel;
    QLabel *weatherLabel;
};