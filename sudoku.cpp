#include "sudoku.h"

Sudoku::Sudoku(Complexity comp)
{
    complexity = comp; // запоминаем переданную в конструктор сложность
    generate(); // генерация поля
    shuffle();  // сортировка поля
    hideCells(); // скрываем ячейки в зависимости от сложности
}

void Sudoku::printSelf()
{
    std::cout << std::endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (area[i][j].visible)
                std::cout << area[i][j].number;
            else
                std::cout << " ";
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}
void Sudoku::printSelfFull()
{
    std::cout << std::endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (area[i][j].visible)
                std::cout << area[i][j].number;
            else
                std::cout << area[i][j].value;
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void Sudoku::generate()
{
    /*
     * Размещаем в первую строку 1..9, в строках ниже смещаем
     * на 3 позиции влево, т.е. 4,5..2,3 и 7,8..5,6.
     * Далее переходя в следующий район по вертикали смещаем
     * на 1 позицию влево предыдущий район.
    */
    int n = 3;
    area.resize(size); // задаем количество строк в двумерном массиве
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            area[i].push_back(Cell((i * n + i / n + j) % // помещаем в массив
                                    size + 1));          // новую ячейку с вычисленным знасчением
        }
    }
}

void Sudoku::hideCells()
{
    std::srand(std::time(0));
    int hides = complexity;
    while (hides > 0) {
        // если ячейка со случайными координатами не скрыта
        int i = rand() % 9, j = rand() % 9;
        if (area[i][j].visible) {
            area[i][j].visible = false; // скрываем ее
            hides--;
        }
    }
}

QPoint Sudoku::checkCells()
{
    QPoint row = checkRows(),
    col = checkColumns();
    if (row.x() > -1) // если в строке есть ошибка (>-1)
        return row;
    if (col.y() > -1) // если в столбце есть ошибка
        return col;
    return checkSquare(); // если в строках и столбцах нет ошибок, возвращаем результат проверки регионов
}

bool Sudoku::fullCheckCells()
{
    QPoint res = checkCells();
    return (res.x() + res.y() < -1) && allFill();
}

bool Sudoku::allFill()
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // если ячейка для ввода (!.visible) и значение == 0
            if (!area[i][j].visible && area[i][j].value == 0)
                return false; // то поле не заполнено
        }
    }
    return true; // если выход из цикла не произошел, то значит все поле заполненно
}

QPoint Sudoku::checkSquare()
{
    QString square; // строка для чисел региона
    for (int sq = 0; sq < size; sq++) { // всего 9 районов
        for (int i = 0; i < 3; i++) { // в каждом 3 строки и 3 столбца
            int x = (sq / 3) * 3 + i; // глобальная координата строки
            for (int j = 0; j < 3; j++) {
                int y = (sq % 3) * 3 + j; // глобальная координата столбца
                if (area[x][y].visible) { // ячейка видима (не для ввода)
                    if (square.indexOf(QString::number(area[x][y].number)) >= 0) { // число уже есть в регионе
                        return QPoint(x - i, y - j); // возвращаем координаты начала региона
                    } else { // числа еще нет в регионе
                        square.append(QString::number(area[x][y].number)); // добавляем в регион
                    }
                } else if (area[x][y].value != 0) { // ячейка для ввода пользователя и там уже введено значение (!=0)
                    if (square.indexOf(QString::number(area[x][y].value)) >= 0) { // число уже есть в регионе
                        return QPoint(x - i, y - j);
                    } else {
                        square.append(QString::number(area[x][y].value));
                    }
                }
            }
        }
        square.clear(); // очищаем строку с накопленными значениями
    }
    return QPoint(-1, -1);
}

QPoint Sudoku::checkRows()
{
    /*
     * если ячейка не скрыта, то проверяем, есть она уже в строке (area.number),
     *     если есть, то ошибка (indexOf >= 0)
     *     если нет, то добавляем ее значение к строке
     * если ячейка скрыта, то смотрим что ввел пользователь (area.value)
     *     если еще ничего не ввел, пропускаем ее идем дальше
     *     если ввел, то ищем ее значние в строке,
     *         если нет в строке, то добавляем
     *         если есть, то ошибка
     */
    QString row; // для строки, куда будут добавляться
                     // неповторяющиеся значения в строке

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (area[i][j].visible) {
                if (row.indexOf(QString::number(area[i][j].number)) >= 0) {
                    return QPoint(i, -1); // передаю строку, в которой ошибка
                } else {
                    row.append(QString::number(area[i][j].number));
                }
            } else if (area[i][j].value != 0) {
                if (row.indexOf(QString::number(area[i][j].value)) >= 0) {
                    return QPoint(i, -1);
                } else {
                    row.append(QString::number(area[i][j].value));
                }
            }
        }
        row.clear(); // очищаем строку
    }
    return QPoint(-1, -1); // если все совпадает
}

QPoint Sudoku::checkColumns()
{
    QString col; // для строки, куда будут добавляться
                     // неповторяющиеся значения в столбце

    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            if (area[i][j].visible) {
                if (col.indexOf(QString::number(area[i][j].number)) >= 0) {
                    return QPoint(-1, j); // передаю столбец, в котором ошибка
                } else {
                    col.append(QString::number(area[i][j].number));
                }
            } else if (area[i][j].value != 0) {
                if (col.indexOf(QString::number(area[i][j].value)) >= 0) {
                    return QPoint(-1, j);
                } else {
                    col.append(QString::number(area[i][j].value));
                }
            }
        }
        col.clear(); // очищаем строку
    }
    return QPoint(-1, -1); // если все совпадает
}

void Sudoku::enterValue(int x, int y, int value)
{
    if (!area[x][y].visible) // если ячейка скрыта (т.е. пользователь может вводить в нее значения)
        area[x][y].value = value;
}

int Sudoku::getNumber(int x, int y)
{
    return area[x][y].visible ? area[x][y].number : 0;
}

int Sudoku::getValue(int x, int y)
{
    return  area[x][y].visible ? 0 : area[x][y].value;
}

void Sudoku::shuffle()
{
    // применяю сортировки в случайном порядке shuffle_count раз
    int i = 0;
    srand( time(0) ); // генерирую новую псевдо-случайную последовательность
    while (i < shuffle_count) {
        int func_num = rand() % shuffle_func_count; // 0..shuffle_func_count - 1
        switch (func_num) {
        case 0:
            transposing();
            break;
        case 1:
            swap_rows_small();
            break;
        case 2:
            swap_colums_small();
            break;
        case 3:
            swap_rows_area();
            break;
        case 4:
            swap_colums_area();
            break;
        }
        i++;
    }
}

void Sudoku::transposing()
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j < i) {
                Cell temp = area[i][j];
                area[i][j] = area[j][i];
                area[j][i] = temp;
            }
        }
    }
}

void Sudoku::swap_rows_small()
{
    int row = (std::rand() % 3) * 3;  // выбираем "большую" строку
    int row1 = std::rand() % 3; // выбираем "маленькие" строки
    int row2 = std::rand() % 3; // внутри "большой" строки
    while (row1 == row2) {     // если числа совпали, то генерируем новые
        row1 = std::rand() % 3;      // пока не получатся разные
        row2 = std::rand() % 3;
    }
    std::vector<Cell> temp = area[row + row1];
    area[row + row1] = area[row + row2];
    area[row + row2] = temp;
}

void Sudoku::swap_colums_small()
{
    int col = (std::rand() % 3) * 3;  // выбираем "большой" столбец
    int col1 = std::rand() % 3; // выбираем "маленькие" столбцы
    int col2 = std::rand() % 3; // внутри "большого" столбца
    while (col1 == col2) {     // если числа совпали, то генерируем новые
        col1 = std::rand() % 3;      // пока не получатся разные
        col2 = std::rand() % 3;
    }
    for (int i = 0; i < size; i++) {
        Cell temp = area[i][col + col1];
        area[i][col + col1] = area[i][col + col2];
        area[i][col + col2] = temp;
    }
}

void Sudoku::swap_rows_area()
{
    int row1 = (std::rand() % 3) * 3; // случайные числа
    int row2 = (std::rand() % 3) * 3; // для выбора "больших" строк (0..2) * 3
    while (row1 == row2) {     // если числа совпали, то генерируем новые
        row1 = (std::rand() % 3) * 3;      // пока не получатся разные
        row2 = (std::rand() % 3) * 3;
    }
    for (int i = 0; i < size / 3; i++) {
        std::vector<Cell> temp = area[row1 + i];
        area[row1 + i] = area[row2 + i];
        area[row2 + i] = temp;
    }
}

void Sudoku::swap_colums_area()
{
    int col1 = (std::rand() % 3) * 3; // случайные числа
    int col2 = (std::rand() % 3) * 3; // для выбора "больших" столбцов (0..2) * 3
    while (col1 == col2) {     // если числа совпали, то генерируем новые
        col1 = (std::rand() % 3) * 3;      // пока не получатся разные
        col2 = (std::rand() % 3) * 3;
    }
    for (int j = 0; j < size / 3; j++) {
        for (int i = 0; i < size; i++) {
            Cell temp = area[i][col1 + j];
            area[i][col1 + j] = area[i][col2 + j];
            area[i][col2 + j] = temp;
        }
    }
}
