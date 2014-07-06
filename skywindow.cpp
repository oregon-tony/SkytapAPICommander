#include "skywindow.h"
#include "ui_skywindow.h"

SkyWindow::SkyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SkyWindow)
{
    ui->setupUi(this);
}

SkyWindow::~SkyWindow()
{
    delete ui;
}
