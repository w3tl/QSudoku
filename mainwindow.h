#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QVector>
#include <QMessageBox>


#include <algorithm>
#include <cstddef>

#include "sudoku.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    QVector< QVector<QWidget *> > area;

    Sudoku *game;
    bool error;

    void newGame(); // создает новую игру в зависимости от сложности
    void showGame(); // заполянет форму игровым полем
    void clearArea(); // очищает массив area от динамических объектов

    QVector<QString> colors; // массив для хранения цвета регионов
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void remove_error(QString text);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
