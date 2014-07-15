// commit local
// push remote

#include <QtNetwork>
#include "skywindow.h"
#include "ui_skywindow.h"

SkyWindow::SkyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SkyWindow)
{

    ui->setupUi(this);

    ui->cbAPIUrl->addItem("https://cloud.skytap.com/users/");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/configurations/");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/templates/");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/vpns/");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/ips/");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/assets/");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/projects/");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/groups/");
    ui->cbAPIUrl->addItem("https://cloud.skytap.com/departments/");
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
   debugLog(ui->cbAPIUrl->currentText());
   ui->treeWidget->clear();
   xml.clear();
   get(ui->cbAPIUrl->currentText());
}

/////////////////////////////////////////
// networking stuff
/////////////////////////////////////////
/*
    Starts the network request and connects the needed signals
*/
void SkyWindow::get(const QUrl &url)
{
    QNetworkRequest request(url);
    if (currentReply) {
        currentReply->disconnect(this);
        currentReply->deleteLater();
    }


    //doc.setRequestHeader("Authorization", "Basic b3JlZ29uX3RvbnlAeWFob28uY29tOjJjNjQ1ZjM3MWZmYmNiZDA4OTFmNzg5NGEyZjM5MzkzYmE0MGQwNTM=");
    //doc.setRequestHeader("Accept", "application/xml");
    //request.setHeader();

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


    currentReply = manager.get(request);
    connect(currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
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
        get(redirectionTarget);
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
    qWarning("error retrieving RSS feed");
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
