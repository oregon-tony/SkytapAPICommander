#ifndef SKYWINDOW_H
#define SKYWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>
//#include <QWidget>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QUrl>

namespace Ui {
    class SkyWindow;
}

class SkyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SkyWindow(QWidget *parent = 0);
    ~SkyWindow();

private slots:
    void on_pbConnectButton_clicked();
    void on_cbAPIUrl_currentIndexChanged(int index);
    void fetch();
    void metaDataChanged();
    void readyRead();
    void finished(QNetworkReply *reply);
    void error(QNetworkReply::NetworkError);
    void debugLog(QString logText);

private:
     Ui::SkyWindow *ui;
     void get(const QUrl &url);

     QNetworkAccessManager manager;
     QNetworkReply *currentReply;

     QXmlStreamReader xml;

     void parseXml();

     // void loadTextFile(); // see saved link
};

#endif // SKYWINDOW_H
