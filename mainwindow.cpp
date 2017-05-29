#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formatter.h"
#include "mapmodel.h"
#include <QFileDialog>

#include <QMessageBox>

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

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                "TextFile (*.cpp)"
                );

    Formatter formatter(filename.toStdString());

    map<string,int> map = formatter.getMap();

    mapmodel.setMap(&map);
    ui->tableView->setModel(&mapmodel);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/",
                "TextFile (*.cpp)"
                );

    Formatter formatter(filename.toStdString());

    map<string,int> map = formatter.getMap();

    string content = to_string(map["if"]);

    QString str = QString::fromUtf8(content.c_str(), content.size());

    QMessageBox::information(this, tr("File Name"), str);
}
