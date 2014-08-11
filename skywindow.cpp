// commit local
// push remote
// 08-11-2014
// adding integration with Jenkins

#include <QtNetwork>
#include "skywindow.h"
#include "ui_skywindow.h"

SkyWindow::SkyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SkyWindow)
{

    ui->setupUi(this);

    ui->cbAPIUrl->addItem("https://cloud.skytap.com/users");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/configurations");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/templates");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/vpns");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/ips");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/assets");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/projects");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/groups");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/departments");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/notifications");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/imports");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/schedules");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/configurations/1329704");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/configurations/1329704?runstate=running");

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

SkyWindow::~SkyWindow()
{
    delete ui;
}

/////////////////////////////////////////
// Handle events of UI objects
/////////////////////////////////////////

void SkyWindow::on_pbConnectButton_clicked()
{

}

// value of combobox changed
void SkyWindow::on_cbAPIUrl_currentIndexChanged(int index)
{
   // index not used
   debugLog(ui->cbAPIUrl->currentText());
   ui->treeWidget->clear();
   xml.clear();
   doRest(ui->cbRestAction->currentText(), ui->cbAPIUrl->currentText());
}

/////////////////////////////////////////
// networking stuff
/////////////////////////////////////////
/*
    Starts the network GET request and connects the needed signals
*/
void SkyWindow::doRest(const QString &restCommand, const QUrl &url)
{

    debugLog("rest Command:"+ restCommand);

    QNetworkRequest request(url);
    if (currentReply) {
        currentReply->disconnect(this);
        currentReply->deleteLater();
    }

    // encode user and api key from form
    // HTTP Basic authentication header value: base64(username:password)
    QString username = ui->leUserName->text();
    QString password = ui->leAPIKey->text();
    QString concatenated = username + ":" + password;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    //request.setRawHeader("Authorization","Basic b3JlZ29uX3RvbnlAeWFob28uY29tOjJjNjQ1ZjM3MWZmYmNiZDA4OTFmNzg5NGEyZjM5MzkzYmE0MGQwNTM=");
    request.setRawHeader("Accept","application/xml");
    request.setRawHeader("Content-Type","application/xml");

    // command can either be GET, POST, PUT, etc

    // set up a list of command so we can get an index for the switch command
    QStringList restList;
    restList << "GET" << "POST" << "PUT"; // get = 0, post =1, put =2

    QByteArray postData; // final data of param string converted to text
    QUrl params;
    QUrlQuery query; // set up parameters from grid


    // do steps for the current command use "restCommand" to find index in restList for switch statement
    switch(restList.indexOf(restCommand))
    {
        case 0:
            debugLog("doing GET");
            currentReply = manager.get(request);
            break;

        case 1:
            debugLog("doing POST");
            /////////////////
            //ui->twPostValues->setRowCount(1);
            //ui->twPostValues->setColumnCount(1);
            //ui->twPostValues->setItem(0, 0, new QTableWidgetItem("Hello World!"));
            ///
            // get values from grid
            //debugLog(ui->twPostValues->itemAt(0, 0)->text());
            // do post
            //params.addQueryItem("runstate", "halted");
           // params.addQueryItem("another_key", "äöutf8-value");

            //finalParamData.append(params.toString());

            //currentReply = manager.post(request, finalParamData);
            break;

        case 2:
            debugLog("doing PUT");
            /////////////////
            //ui->twPostValues->setRowCount(1);
            //ui->twPostValues->setColumnCount(1);
            //ui->twPostValues->setItem(0, 0, new QTableWidgetItem("Hello World!"));
            ///
            // get values from grid
            //debugLog(ui->twPostValues->itemAt(0, 0)->text());
            // do post
            //query.addQueryItem("", "");
            //params.setQuery(query);

            //postData = params.toEncoded(QUrl::RemoveFragment);

            //finalParamData.append(params.toString(QUrl::FullyEncoded).toUtf8());
            debugLog("Post Data:" + postData);

            currentReply = manager.put(request,postData);
            break;
    }


    // common to all commands
    connect(currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged())); // pass command as parameter ??????
    connect(currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

/*
    Starts fetching data from a news source specified in the line
    edit widget.

    The line edit is made read only to prevent the user from modifying its
    contents during the fetch; this is only for cosmetic purposes.
    The fetch button is disabled, the list view is cleared, and we
    define the last list view item to be 0, meaning that there are no
    existing items in the list.

    A URL is created with the raw contents of the line edit and
    a get is initiated.
*/

void SkyWindow::fetch()
{
    //lineEdit->setReadOnly(true);
    //fetchButton->setEnabled(false);
    //treeWidget->clear();

    //xml.clear();

//    QUrl url(lineEdit->text());
//    get(url);
}

void SkyWindow::metaDataChanged()
{
    QUrl redirectionTarget = currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirectionTarget.isValid()) {

// how to reference current command other than grabbgin UI object value again?
        doRest(ui->cbRestAction->currentText(), redirectionTarget);
    }
}

/*
    Reads data received from the RDF source.

    We read all the available data, and pass it to the XML
    stream reader. Then we call the XML parsing function.
*/

void SkyWindow::readyRead()
{
    int statusCode = currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        QByteArray data = currentReply->readAll();
        xml.addData(data);
        parseXml();
        //qDebug() << "RAW DATA:\n" << data;
    }
}

/*
    Finishes processing an HTTP request.

    The default behavior is to keep the text edit read only.

    If an error has occurred, the user interface is made available
    to the user for further input, allowing a new fetch to be
    started.

    If the HTTP get request has finished, we make the
    user interface available to the user for further input.
*/

void SkyWindow::finished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    //lineEdit->setReadOnly(false);
    //fetchButton->setEnabled(true);
}


/*
    Parses the XML data and creates treeWidget items accordingly.
*/
void SkyWindow::parseXml()
{
/*
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "item")
                linkString = xml.attributes().value("rss:about").toString();
            currentTag = xml.name().toString();
        } else if (xml.isEndElement()) {
            if (xml.name() == "item") {

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, titleString);
                item->setText(1, linkString);
                treeWidget->addTopLevelItem(item);

                titleString.clear();
                linkString.clear();
            }

        } else if (xml.isCharacters() && !xml.isWhitespace()) {
            if (currentTag == "title")
                titleString += xml.text().toString();
            else if (currentTag == "link")
                linkString += xml.text().toString();
        }
    }
    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
    }
 */
    ////////////////////////////////////////////////////
    QXmlStreamReader::TokenType token;
    while(!xml.atEnd() && !xml.hasError())
    {
        /* Read next element.*/
        token = xml.readNext();
         //qDebug() << "XML Token:" << token << "\n";
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
            continue;

        if(token == QXmlStreamReader::StartElement) {
            QTreeWidgetItem *item = new QTreeWidgetItem;

            //qDebug() << "XML Name:" << xml.name().toString() << "\n";
            item->setText(0, xml.name().toString());

            xml.readNext(); // advance to the value of the element

            //qDebug() << "XML Value:" << xml.text().toString() << "\n\n";
            item->setText(1, xml.text().toString());
            //QMessage::information(this,"all text", xml.text().toString());

            ui->treeWidget->addTopLevelItem(item);

         } // start element
    } //while
    ////////////////////////////////////////////////////

}

/*
    Open the link in the browser
*/
//void SkyWindow::itemActivated(QTreeWidgetItem * item)
//{
//    QDesktopServices::openUrl(QUrl(item->text(1)));
//}

void SkyWindow::error(QNetworkReply::NetworkError)
{
    qWarning("Network returned error.");

    QString reason = currentReply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
     debugLog("Error reason:" + reason);

    currentReply->disconnect(this);
    currentReply->deleteLater();
    currentReply = 0;
}

/////////////////////////////////////
// UI Events
/////////////////////////////////////

// Combo Box item activated
//void SkyWindow::selectUrlComboBoxItem(int my_index)
//{
//    qDebug() << "INDEX:" << my_index;
//    qDebug() << "SELECTION:" << (urlDropDown->currentText()) << "\n";
    //combobox->itemData(combobox->currentIndex()
    //(urlDropDown->itemData(my_index))
//    treeWidget->clear();
//    xml.clear();
//    get(urlDropDown->currentText());
//}

/////////////////////////////////////
// Utility
/////////////////////////////////////

void SkyWindow::debugLog(QString logText)
{
     ui->teDebugLog->append(logText);

}

void SkyWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    debugLog("Right Click Menu");

    QMenu* xmlTableRightClick = new QMenu();

    QAction* xmlTablePopUpCopy = new QAction("Copy", this);
    connect(xmlTablePopUpCopy, SIGNAL(triggered()), this, SLOT(doXmlTablePopUpCopy()));

    QTreeWidgetItem *clickedItem = new QTreeWidgetItem;

    clickedItem = ui->treeWidget->itemAt(pos);                              // get the item at the current cursor position
    debugLog(clickedItem->text(0));                                                 // left item
    debugLog(clickedItem->text(1));                                                 // right item

    xmlTableRightClick->addAction(xmlTablePopUpCopy);
    xmlTableRightClick->exec(QCursor::pos());

}

void SkyWindow::doXmlTablePopUpCopy()
{
    debugLog("COPY OPTION PICKED");
}

