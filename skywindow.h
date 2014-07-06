#ifndef SKYWINDOW_H
#define SKYWINDOW_H

#include <QMainWindow>

namespace Ui {
class SkyWindow;
}

class SkyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SkyWindow(QWidget *parent = 0);
    ~SkyWindow();

private:
    Ui::SkyWindow *ui;
};

#endif // SKYWINDOW_H
