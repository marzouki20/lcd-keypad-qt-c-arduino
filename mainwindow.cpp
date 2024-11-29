#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTimer>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer


}

MainWindow::~MainWindow()
{
    delete ui;
}



//-------------------------------------------------------------------------------------------------------------------------------**
void MainWindow::update_label() {
    QByteArray data = A.getserial()->readAll();

    if (!data.isEmpty()) {
        QString jei_ml_arduino = QString::fromUtf8(data).trimmed();

        if (jei_ml_arduino.length() > 2) {
            qDebug() << "Received ID and password: " << jei_ml_arduino;

            // hneee 3malt combain bin id w pass binethomm : 5ater ki nog3ed na9ra bl ka3ba yod5el b3thou
            QStringList idwlpass = jei_ml_arduino.split(":");

            if (idwlpass.size() == 2) {  // hnee bch ngsmou 3la thnin
                QString id = idwlpass.at(0).trimmed();   //iddddddddd
                QString password = idwlpass.at(1).trimmed();  // passss

                qDebug() << "ID: " << id;
                qDebug() << "Password: " << password;
                  ui->label->setText(id);
                    ui->passwordLabel->setText(password);

                // 3adinehom ll fonction check f database  donccc bch traj3lna entier (1 ou 2 ou 3)
                int result = lawej_fl_base(id, password);

                if (result == 1) {  // ken 1 kol chy s7i7
                    ui->label_2->setText("id et le mot de pass sont correcttt");
                    A.write_to_arduino("true"); // nb3thou true ll arduino bch yafichinla bonjourrrrrr admin
                } else if (result == 2) {  // ID s7i777
                    ui->label_2->setText("innncorrect ID");
                    A.write_to_arduino("ID"); // id 8alet ab3th klmt id ll arduino
                } else if (result == 3) {  // mod pass 8alt
                    ui->label_2->setText("innncorrect Password");
                    A.write_to_arduino("Pass"); //
                } else {
                    ui->label_2->setText("Error fl kol ");
                   A.write_to_arduino("Error");
                }
            } else {
                ui->label_2->setText("Invalid data format (ID:pass expected)");
                A.write_to_arduino("Error"); // Send 'Error' to Arduino if the data format is incorrect
            }
        }
    }
}



int MainWindow::lawej_fl_base(const QString &id, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT id, mot_de_pass FROM gafsa WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError();
        return 0;
    }
    if (query.next()) {
        QString passwordmawjoud = query.value(1).toString();
        if (passwordmawjoud == password) {
            return 1;
        } else {
            return 3;
        }
    } else {
        return 2;
    }
}
