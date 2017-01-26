#ifndef SKYWINDOW_H
#define SKYWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>
//#include <QWidget>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QUrl>
//#include <QMenuBar>

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
    void doXmlTablePopUpCopy(); //
    void metaDataChanged();
    void readyRead();
    void finished(QNetworkReply *reply);
    void error(QNetworkReply::NetworkError);
    void debugLog(QString logText);
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_pbQuitButton_clicked(); // Quit Button

    void on_pbExecuteButton_clicked();

    void setConnectButtonUpdateState(); // determine if both user and password fields are not empty

    //void on_cbAPIUrl_currentIndexChanged(const QString &arg1);

private:
     Ui::SkyWindow *ui;
     void doRest(const QString &restCommand, const QUrl &url);

     QNetworkAccessManager manager;
     QNetworkReply *currentReply;

     QXmlStreamReader xml;

     void parseXml();


     // void loadTextFile(); // see saved link
};

#endif // SKYWINDOW_H
