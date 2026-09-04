#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем центральный виджет и сетку
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(centralWidget);

    // Ограничиваем ввод: только одна цифра от 1 до 9
    QRegularExpression regExp("[1-9]");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, this);

    // Циклом создаем 81 ячейку для Судоку
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QLineEdit *cell = new QLineEdit(this);

            // Настраиваем внешний вид ячейки
            cell->setFixedSize(40, 40);             // Размер квадратика
            cell->setAlignment(Qt::AlignCenter);     // Цифра строго по центру
            cell->setValidator(validator);           // Запрещаем вводить буквы и нули
            cell->setFont(QFont("Arial", 16, QFont::Bold)); // Делаем шрифт крупным

            // Добавляем ячейку в сетку (строка, столбец)
            gridLayout->addWidget(cell, row, col);
        }
    }
    setCentralWidget(centralWidget);
    setWindowTitle("Судоку");
}

MainWindow::~MainWindow()
{
    delete ui;
}
