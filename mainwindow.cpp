#include "math.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

// Raspberry Pi version of the get weather app for PC

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_downloadButton_clicked()
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("http://192.168.0.196/wthrdata.dat") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll(); // strReply holds the full data stream from weather station

        //qDebug() << strReply;

        ui->incomingtxtlbl->setText(strReply); // Places all of the weather station text into a large label.

        //qDebug() << strReply.data()[20]; // 20th char is outdoor temperature

        QString ODtemp = ""; //Out Door Temperature
        for (int i = 20; i <=23; ++i) {
            ODtemp.append(strReply.data()[i]);
        }
        double tempNum = ODtemp.toDouble();  //Convert  string to double
        tempNum = (tempNum * 1.8) + 32; //Conver 'C' to 'F'
        QString nODtemp = QString::number(tempNum);  //Convert double to string to display in label
        ui->idoortemplbl->setText(nODtemp);

        QString humid = "";
        for (int i = 25; i <=28; ++i) {
            humid.append(strReply.data()[i]);
        }
        ui->humidlbl->setText(humid);

        QString BXtemp = ""; //Internal Box temperature Temperature
        for (int i = 30; i <=33; ++i) {
            BXtemp.append(strReply.data()[i]);
        }
        //qDebug() << BXtemp;
        double btempNum = BXtemp.toDouble();  //Convert  string to double
        btempNum = (btempNum * 1.8) + 32; //Conver 'C' to 'F'
        QString nBXtemp = QString::number(btempNum);  //Convert double to string to display in label
        ui->itemplbl->setText(nBXtemp); // Write temperature to label

        QString press = ""; // Barometric pressure in milibars
        for (int i = 35; i <=42; ++i) {
            press.append(strReply.data()[i]);
        }
        //qDebug() << press;
        double bpress = press.toDouble();  //Convert  string to double
        bpress = (bpress * 0.029530) / 100; //Convert milibar to inches of mercury
        QString npress = QString::number(bpress,'f',2);  //Convert double to string to display in label the "(num,'f',2)" formats for 2 decimal places only.

        ui->presslbl->setText(npress);

        QString speed = ""; // Current wind speed
        for (int i = 50; i <=53; ++i) {
            speed.append(strReply.data()[i]);
        }
        double bspeed = speed.toDouble();  //Convert  string to double
        bspeed = bspeed * 0.62137119223733; //Convert kph to mph
        QString nspeed = QString::number(bspeed,'f',2);  //Convert double to string to display in label the "(num,'f',2)" formats for 2 decimal places only.

        ui->speedlbl->setText(nspeed);

        QString gust = ""; // Current wind gust
        for (int i = 55; i <=58; ++i) {
            gust.append(strReply.data()[i]);
        }
        double bgust = gust.toDouble();  //Convert  string to double
        bgust = bgust * 0.62137119223733; //Convert kph to mph and round out the number
        QString ngust = QString::number(bgust,'f',2);  //Convert double to string to display in label the "(num,'f',2)" formats for 2 decimal places only.

        ui->gustlbl->setText(ngust);


        QString dir = ""; // Current wind direction
        for (int i = 60; i <=64; ++i) {
            dir.append(strReply.data()[i]);
        }
        //qDebug() << dir;

        double bdir = dir.toDouble();
        // bdir = 68;
        //qDebug() << bdir;

        if (bdir >= 0 && bdir < 22){
            ui->dirlbl->setText("North");
        }
        else if (bdir >= 22 && bdir < 45){
            ui->dirlbl->setText("North NE");
        }
        else if (bdir >= 45 && bdir < 67){
            ui->dirlbl->setText("North East");
        }
        else if (bdir >= 67 && bdir < 90){
            ui->dirlbl->setText("East NE");
        }
        else if (bdir >= 90 && bdir < 112){
            ui->dirlbl->setText("East");
        }
        else if (bdir >= 112 && bdir < 135){
            ui->dirlbl->setText("East SE");
        }
        else if (bdir >= 135 && bdir < 157){
            ui->dirlbl->setText("South East");
        }
        else if (bdir >= 157 && bdir < 180){
            ui->dirlbl->setText("South SE");
        }
        else if (bdir >= 180 && bdir < 202){
            ui->dirlbl->setText("South");
        }
        else if (bdir >= 202 && bdir < 225){
            ui->dirlbl->setText("South SW");
        }
        else if (bdir >= 225 && bdir < 247){
            ui->dirlbl->setText("South West");
        }
        else if (bdir >= 247 && bdir < 270){
            ui->dirlbl->setText("West SW");
        }
        else if (bdir >= 270 && bdir < 292){
            ui->dirlbl->setText("West");
        }
        else if (bdir >= 292 && bdir < 315){
            ui->dirlbl->setText("West NW");
        }
        else if (bdir >= 315 && bdir < 337){
            ui->dirlbl->setText("North West");
        }
        else if (bdir >= 337 && bdir < 359){
            ui->dirlbl->setText("North NW");
        }


     delete reply;

}
}

void MainWindow::on_quitButton_clicked()
{
    this -> close();
}
