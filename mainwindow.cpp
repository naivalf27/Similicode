#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMap>

#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>

#include <string>
#include <map>

using namespace std;

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

void MainWindow::generate()
{
    if (!pathFileRef.isEmpty() && !pathFileOther.isEmpty()){
        QFormatter ref(pathFileRef);
        QFormatter other(pathFileOther);

        QMap<QString,int> mapRef = ref.getMap();
        QMap<QString,int> mapOther = other.getMap();

        QStandardItemModel *model = new QStandardItemModel(mapRef.size()-1,4,this);
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Key")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Ref")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Other")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("%")));

        double prct(0.0);

        for(int i(1); i < mapRef.size() ; i++){
            QStandardItem *row = new QStandardItem(QString(mapRef.keys().at(i)));
            model->setItem(i-1,0,row);

            int valueRef = mapRef.values().at(i);
            int valueOther = mapOther[mapRef.keys().at(i)];

            double prc = valueOther * 100 / valueRef;

            if (prc >= 200) {
                prc = 0;
            } else if (prc > 100){
                prc = ((prc - 200) * -1);
            }

            prct += prc;

            QStandardItem *row2 = new QStandardItem(QString::number(valueRef));
            row2->setTextAlignment(Qt::AlignHCenter);
            model->setItem(i-1,1,row2);
            QStandardItem *row3 = new QStandardItem(QString::number(valueOther));
            row3->setTextAlignment(Qt::AlignHCenter);
            model->setItem(i-1,2,row3);
            QStandardItem *row4 = new QStandardItem(QString::number(prc)+"%");
            row4->setTextAlignment(Qt::AlignHCenter);
            model->setItem(i-1,3,row4);
        }

        prct /= (mapRef.size()-1);

        ui->label_3->setText(QString::number(prct)+"%");
        ui->progressBar->setValue(prct);
        ui->tableView->setModel(model);
        ui->tableView->horizontalHeader()->resizeSection(0, 200);
        ui->tableView->horizontalHeader()->resizeSection(1, 50);
        ui->tableView->horizontalHeader()->resizeSection(2, 50);
        ui->tableView->horizontalHeader()->resizeSection(3, 50);
    } else {
        QMessageBox msgBox;
        msgBox.setText("Les deux fichiers doivent être séléctionnés");
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                "TextFile (*.cpp)"
                );

    pathFileRef = filename;
    ui->nameRef->setText(".../"+pathFileRef.split("/").last());
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                "TextFile (*.cpp)"
                );

    pathFileOther = filename;
    ui->nameOther->setText(".../"+pathFileOther.split("/").last());
}

void MainWindow::on_pushButton_3_clicked()
{
    generate();
}
