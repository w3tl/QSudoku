#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include <QPoint>
#include <QDebug>

class Cell
{
public:
    // Конструктор Cell(зачение, видимость (по умолчанию true),
    //                  введенное значение (по умолчанию 0 - ничего не введено))
    Cell(int _number, bool _visible = true, int _value = 0) : number(_number),
                                                              value(_value),
                                                              visible(_visible) {}
    int number; // хранит число (значение клетки)
    int value; // введенное значение
    bool visible; // флаг (видна/не видна клетка для пользователя)
};

// Перечисление (Сложность = кол-во убираемых клеток)
enum Complexity {Easy = 50, Medium = 55, Hard = 60};

class Sudoku
{
public:
    // Конструктор Sudoku(Сложность)
    Sudoku(Complexity comp);

    // вводит value в ячейку с координатами (x, y)
    void enterValue(int x, int y, int value);
    // возвращает значение ячейки (если для ввода, то 0)
    int getNumber(int x, int y);
    // возвращает значение, которое ввел пользователь
    int getValue(int x, int y);

    // проверяет верно ли введены значения в скрытые ячейки
        // возвращает координаты первой ошибочной ячейки
    QPoint checkCells();
        // возвращает, true, если нигде нет ошибок и все ячейки заполнены
    bool fullCheckCells();

    void printSelf();
    void printSelfFull();
    static const int size = 9; // размер поля (клеток х клеток)
private:
    // Запрещаем конструктор по-умолчанию (без аргументов)
    // чтобы экземпляр класса создавался только с указанием
    // сложности
    Sudoku();
    // генерирует поле (со сдвигом, см. в комментарии внутри)
    void generate();
    // перемешивает поле с помощью функций (ниже)
    void shuffle();

    void transposing(); // Транспонирование всей таблицы
    void swap_rows_small(); // Обмен двух строк в пределах одного района
    void swap_colums_small(); // Обмен двух столбцов в пределах одного района
    void swap_rows_area(); // Обмен двух районов по горизонтали
    void swap_colums_area(); // Обмен двух районов по вертикали

    // скрывает ячейки случайным образом, в зависимости от сложности
    void hideCells();

    // если все правильно: (-1; -1)
    QPoint checkRows(); // проверка строк
    QPoint checkColumns(); // проверка столбцов
    QPoint checkSquare(); // проверка района

    bool allFill(); // возвращает, все ли ячейки заполненыы полльзователем

    static const int shuffle_count = 43; // кол-во перемешиваний
    static const int shuffle_func_count = 5; // кол-во функций сортировки
    Complexity complexity; // сложность
    std::vector< std::vector<Cell> > area; // массив клеток поля

};

#endif // SUDOKU_H
