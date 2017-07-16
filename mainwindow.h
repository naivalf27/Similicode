#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qformatter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_button_add_clicked();

    void on_button_minus_clicked();

private:
    QFormatter formater;
    QStringList listGroup = QStringList();
    QMap<QString,int> mapRef;
    QMap<QString,int> mapOther;
    QString dataRef;
    QString dataOther;
    void generateCharTableView();
    void generateGroupTableView();
    void generateTableView();
    void initCharMapFor(QString const& data, QMap<QString, int>& map);
    void initValues();
    void initFormater();

    Ui::MainWindow *ui;

    QString pathFileRef;
    QString pathFileOther;

    void generate();
};

#endif // MAINWINDOW_H
