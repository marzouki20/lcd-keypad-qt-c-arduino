#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "arduino.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


   void update_label();
   int lawej_fl_base(const QString &id, const QString &password);


private:
    Ui::MainWindow *ui;

    QByteArray data; // variable contenant les données reçues

    Arduino A; // objet temporaire
};

#endif // MAINWINDOW_H
