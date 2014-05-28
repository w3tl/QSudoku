#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // помещаем цвета для ячеек регионов в массив
    colors.push_back("#31b9f5");
    colors.push_back("#eaf033");
    colors.push_back("#42d95f");
    colors.push_back("#fb9ed2");
    colors.push_back("#c385e2");
    colors.push_back("#2CE4E7");
    colors.push_back("#2C82E7");
    colors.push_back("#30FFC7");
    colors.push_back("#3059FF");

    game = NULL;
    newGame();
}

void MainWindow::newGame()
{
    Complexity comp;
    switch (ui->comboBox->currentIndex()) { // проверяем выбранный элемент ComboBox
    case 0:
        comp = ::Easy;
        break;
    case 1:
        comp = ::Medium;
        break;
    case 2:
        comp = ::Hard;
        break;
    }
    if (game != NULL) // если под переменную уже выделена память
        delete game;  // удаляем ее
    game = new Sudoku(comp); // новый экземпляр игры
    error = false;
    showGame(); // отобразить поле
}

void MainWindow::showGame()
{
    /* QLabel - визуальный элемеент, отображающий текст
     * QLineEdit - элемент для ввода текста
     * QGridLayout - "сетка", которая автоматически выравнивает помещяемые в нее виджеты
     *
     * проходим все ячейки поля и если ячейка не скрыта (getNumber > 0), то
     * помещаем в GridLayout Label со значением ячейки
     * иначе помещаем в GridLayout LineEdit, в который можно вводить числа
     *
     * все помещаемые в GridLayout объекты создаются динамически и храняться
     * в массиве this->area
     */
    std::random_shuffle(colors.begin(), colors.end()); // перемешиваем цвета для регионов
    clearArea(); // очищаем поле на форме
    area.resize(game->size); // устанавливаю кол-во строк в массиве
    for (int i = 0; i < game->size; i++) {
        for (int j = 0; j < game->size; j++) {
            if (game->getNumber(i, j) > 0) { // если ячейка отображаемая
                // создаем QLabel со значением ячейки Sudoku
                QLabel *label = new QLabel(QString::number(game->getNumber(i, j)));
                // задаем выравнивание текста по центру
                label->setAlignment(Qt::AlignCenter);
                // задаем стили для ячеек
                label->setStyleSheet(QString("border: 1px solid grey; ") + // граница: 1пиксель сплошная серая
                                     QString("background-color: " + colors[(i / 3) * 3 + j / 3])); // цвет-фона: цвет
                // помещаем в массив новый QLabel
                area[i].push_back(label);
                // добавляем QLabel в GridLayout в i-ю строчку и j-й столбец
                ui->gridLayout->addWidget(area[i][j], i, j);
            } else {
                // помещаем в массив новый QLineEdit
                QLineEdit *edit = new QLineEdit();
                // связываем событие (SIGNAL) edit'а изменение текста с функцией remove_error(QString)
                // для того, чтобы при вводе нового значения подчеркивание строки/столбца об ошибке
                // красным цветом убиралось в этой функции
                QObject::connect(edit, SIGNAL(textChanged(QString)), this, SLOT(remove_error(QString)));
                edit->setAlignment(Qt::AlignCenter);
                edit->setStyleSheet(QString("border: 1px dotted black; color: white; ") +
                                    QString("background-color: " + colors[(i / 3) * 3 + j / 3]));
                // задаем максимальную длину текста внутри QLineEdit
                edit->setMaxLength(1);
                area[i].push_back(edit);
                ui->gridLayout->addWidget(area[i][j], i, j);
            }
        }
    }
}

void MainWindow::clearArea()
{
    for (int i = 0; i < area.size(); i++) {
        for (int j = 0; j < area[i].size(); j++)
            delete area[i][j];
        area[i].clear();
    }
}

MainWindow::~MainWindow()
{
    clearArea();
    delete game;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    newGame();
}

void MainWindow::on_pushButton_2_clicked()
{
    for (int i = 0; i < game->size; i++) {
        for (int j = 0; j < game->size; j++) {
            if (game->getNumber(i, j) == 0 &&
                    static_cast<QLineEdit *>(area[i][j])->text().toInt() > 0) { // если ячейка для ввода пользователю
                // записываем введенное значение в game
                game->enterValue(i, j, static_cast<QLineEdit *>(area[i][j])->text().toInt());
            }
        }
    }
    if (game->fullCheckCells()) {
        int res = QMessageBox::information(this, "Поздравляем!",
                                           "Вы прошли игру! Начать заново?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::Yes) {
            delete game;
            newGame();
        }
    } else {
        QPoint res = game->checkCells();
        error = true;
        if (res.x() > -1 && res.y() > -1) { // если ошибка в регионе (т.е и строка и столбец > -1)
            for (int i = res.x(); i < res.x() + 3; i++) {
                for (int j = res.y(); j < res.y() + 3; j++) {
                    area[i][j]->setStyleSheet("background-color: red");
                }
            }
        } else { // если строка/столбец содержит ошибку
            if (res.x() > -1) { // строка содержит ошибку
                for (int j = 0; j < game->size; j++) {
                    area[res.x()][j]->setStyleSheet("background-color: red");
                }
            } else if (res.y() > -1) {
                for (int i = 0; i < game->size; i++) {
                    area[i][res.y()]->setStyleSheet("background-color: red");
                }
            } else {
                error = false;
            }

        }
    }

}

void MainWindow::remove_error(QString text)
{
    if (error) {
        for (int i = 0; i < game->size; i++) {
            for (int j = 0; j < game->size; j++) {
                if (game->getNumber(i, j) > 0) { // если ячейка отображаемая
                    area[i][j]->setStyleSheet(QString("border: 1px solid grey; ") + // граница: 1пиксель сплошная серая
                                         QString("background-color: " + colors[(i / 3) * 3 + j / 3])); // цвет-фона: цвет
                } else {
                    area[i][j]->setStyleSheet(QString("border: 1px dotted black; color: white; ") +
                                        QString("background-color: " + colors[(i / 3) * 3 + j / 3]));
                }
            }
        }
        error = false;
    }
}
