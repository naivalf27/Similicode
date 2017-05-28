#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include <QMessageBox>

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
                "/",
                "TextFile (*.cpp)"
                );

    QMessageBox::information(this, tr("File Name"), filename);
}
