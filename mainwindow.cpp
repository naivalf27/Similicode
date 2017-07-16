#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMap>
#include <QDir>
#include <QDebug>

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
    setFixedSize(QSize(400,630));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initFormater()
{
    formater.removeCmt = ui->commentIsRemoved->isChecked();
    formater.useAlphaNumeriqueSeparator = ui->alphaNumeriqueSeparator->isChecked();
    formater.includeInclude = ui->includeIsInclude->isChecked();
    formater.includeString = ui->stringIsInclude->isChecked();
}

void MainWindow::initValues()
{
    dataRef = formater.executeInFile(pathFileRef);
    dataOther = formater.executeInFile(pathFileOther);

    mapRef = formater.getMap(dataRef);
    mapOther = formater.getMap(dataOther);
}

void MainWindow::generate()
{
    if (pathFileRef.isEmpty() || pathFileOther.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Les deux fichiers doivent être séléctionnés");
        msgBox.exec();
        return;
    }

    initFormater();
    initValues();

    generateTableView();
    generateGroupTableView();
    generateCharTableView();
}

// MARK: Generate TableView

void MainWindow::generateGroupTableView()
{
    QStandardItemModel *model = new QStandardItemModel(listGroup.size(),4,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Group")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Ref")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Other")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("%")));

    double prct(0.0);

    for(int i(0); i < listGroup.size() ; i++){

        QString keysString = listGroup.at(i);

        QStringList keysList = keysString.split(";",QString::SkipEmptyParts);


        int valueRef = 0;
        for (int j(0); j < keysList.size(); j++)
        {
            valueRef += mapRef[keysList.at(j)];
        }

        int valueOther = 0;
        for (int j(0); j < keysList.size(); j++)
        {
            valueOther += mapOther[keysList.at(j)];
        }

        double prc = valueOther * 100 / valueRef;

        if (prc >= 200) {
            prc = 0;
        } else if (prc > 100){
            prc = 200 - prc;
        }

        prct += prc;

        QStandardItem *row = new QStandardItem(QString(keysString));
        model->setItem(i,0,row);
        QStandardItem *row2 = new QStandardItem(QString::number(valueRef));
        row2->setTextAlignment(Qt::AlignHCenter);
        model->setItem(i,1,row2);
        QStandardItem *row3 = new QStandardItem(QString::number(valueOther));
        row3->setTextAlignment(Qt::AlignHCenter);
        model->setItem(i,2,row3);
        QStandardItem *row4 = new QStandardItem(QString::number(prc)+"%");
        row4->setTextAlignment(Qt::AlignHCenter);
        model->setItem(i,3,row4);
    }

    if (listGroup.size() == 0){
        prct = 0.0;
    } else {
        prct /= (listGroup.size());
    }

    ui->progressBarGroup->setValue(prct);
    ui->label_6->setText(QString::number(prct)+"%");
    ui->tableViewGroupe->setModel(model);
    ui->tableViewGroupe->horizontalHeader()->resizeSection(0, 140);
    ui->tableViewGroupe->horizontalHeader()->resizeSection(1, 40);
    ui->tableViewGroupe->horizontalHeader()->resizeSection(2, 40);
    ui->tableViewGroupe->horizontalHeader()->resizeSection(3, 50);
}

void MainWindow::generateTableView()
{
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
            prc = 200 - prc;
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
}

void MainWindow::generateCharTableView()
{
    QMap<QString, int> charsRef;
    QMap<QString, int> charsOther;

    initCharMapFor(dataRef, charsRef);
    initCharMapFor(dataOther, charsOther);

    QStandardItemModel *model = new QStandardItemModel(charsRef.keys().size(),4,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Char")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Ref")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Other")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("%")));

    double prct(0.0);

    for (int i(0); i < charsRef.keys().size(); i++)
    {
        QString key = charsRef.keys().at(i);

        QStandardItem *row = new QStandardItem(key);
        model->setItem(i,0,row);

        int valueRef = charsRef[key];
        int valueOther = charsOther[key];

        double prc = valueOther * 100 / valueRef;

        if (prc >= 200) {
            prc = 0;
        } else if (prc > 100){
            prc = 200 - prc;
        }

        prct += prc;

        QStandardItem *row2 = new QStandardItem(QString::number(valueRef));
        row2->setTextAlignment(Qt::AlignHCenter);
        model->setItem(i,1,row2);
        QStandardItem *row3 = new QStandardItem(QString::number(valueOther));
        row3->setTextAlignment(Qt::AlignHCenter);
        model->setItem(i,2,row3);
        QStandardItem *row4 = new QStandardItem(QString::number(prc)+"%");
        row4->setTextAlignment(Qt::AlignHCenter);
        model->setItem(i,3,row4);
    }

    prct /= (charsRef.keys().size());

    ui->progressBarChar->setValue(prct);
    ui->label_5->setText(QString::number(prct)+"%");
    ui->tableViewChar->setModel(model);
    ui->tableViewChar->horizontalHeader()->resizeSection(0, 160);
    ui->tableViewChar->horizontalHeader()->resizeSection(1, 50);
    ui->tableViewChar->horizontalHeader()->resizeSection(2, 50);
    ui->tableViewChar->horizontalHeader()->resizeSection(3, 50);
}

void MainWindow::initCharMapFor(const QString &data, QMap<QString, int> &map){
    for (int i(0); i< data.size(); i++){
        map[QString(data.at(i))] += 1;
    }
}

// MARK: Button clicked

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                QDir::homePath()+"/Prog/ESGI/Cpp/SililiCode_xcode/SililiCode_xcode/SimiliCode",
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
                QDir::homePath()+"/Prog/ESGI/Cpp/SililiCode_xcode/SililiCode_xcode/SimiliCode",
                "TextFile (*.cpp)"
                );

    pathFileOther = filename;
    ui->nameOther->setText(".../"+pathFileOther.split("/").last());
}

void MainWindow::on_pushButton_3_clicked()
{
    dataRef = "";
    dataOther = "";
    mapRef.clear();
    mapOther.clear();
    listGroup.clear();

    generate();
    ui->pushButton_4->setEnabled(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    setFixedSize(QSize(800,630));
}

void MainWindow::on_button_add_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (!select->hasSelection()){
        return;
    }

    QModelIndexList selection = select->selectedIndexes();

    QString newGroup = "";
    for(int i(0); i<selection.count();i++)
    {

        QModelIndex index = selection.at(i);
        QString key = mapRef.keys().at(index.row()+1);

        if (newGroup.size() == 0)
        {
            newGroup.append(key);
        } else
        {
            newGroup.append(";"+key);
        }
    }

    listGroup << newGroup;
    generateGroupTableView();

    ui->tableView->clearSelection();
}

void MainWindow::on_button_minus_clicked()
{
    QItemSelectionModel *select = ui->tableViewGroupe->selectionModel();
    if (!select->hasSelection()){
        return;
    }

    QModelIndexList selection = select->selectedIndexes();

    for(int i(selection.count()-1); i>=0 ;i--)
    {
        QModelIndex index = selection.at(i);
        listGroup.removeAt(index.row());
    }
    generateGroupTableView();
}
